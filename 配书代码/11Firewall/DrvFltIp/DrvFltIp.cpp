// DrvFltIp.cpp文件


extern "C"
{
	#include <ntddk.h>
	#include <ntddndis.h>
	#include <pfhook.h>
}

#include "DrvFltIp.h"
#include "internal.h"

// 自定义函数的声明
NTSTATUS DispatchCreateClose(PDEVICE_OBJECT pDevObj, PIRP pIrp);
void DriverUnload(PDRIVER_OBJECT pDriverObj);
NTSTATUS DispatchIoctl(PDEVICE_OBJECT pDevObj, PIRP pIrp);
NTSTATUS AddFilterToList(CIPFilter* pFilter);
void ClearFilterList();
NTSTATUS SetFilterFunction(PacketFilterExtensionPtr filterFun);
PF_FORWARD_ACTION FilterPackets(unsigned char*, unsigned char*, unsigned int, unsigned int, unsigned int, IPAddr, IPAddr);

// 过滤列表首地址
struct CFilterList* g_pHeader = NULL;


// 驱动内部名称和符号连接名称
#define DEVICE_NAME L"\\Device\\devDrvFltIp"
#define LINK_NAME L"\\??\\DrvFltIp"


NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObj, PUNICODE_STRING pRegistryString)
{
	NTSTATUS status = STATUS_SUCCESS;

	// 初始化各个派遣例程
	pDriverObj->MajorFunction[IRP_MJ_CREATE] = DispatchCreateClose;
	pDriverObj->MajorFunction[IRP_MJ_CLOSE] = DispatchCreateClose;
	pDriverObj->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchIoctl;
	pDriverObj->DriverUnload = DriverUnload;

		// 创建、初始化设备对象
	// 设备名称
	UNICODE_STRING ustrDevName;
	RtlInitUnicodeString(&ustrDevName, DEVICE_NAME);
	// 创建设备对象
	PDEVICE_OBJECT pDevObj;
	status = IoCreateDevice(pDriverObj, 
				0,
				&ustrDevName, 
				FILE_DEVICE_DRVFLTIP,
				0,
				FALSE,
				&pDevObj);
	if(!NT_SUCCESS(status))
	{
		return status;
	}

		// 创建符号连接名称
	// 符号连接名称
	UNICODE_STRING ustrLinkName;
	RtlInitUnicodeString(&ustrLinkName, LINK_NAME);
	// 创建关联
	status = IoCreateSymbolicLink(&ustrLinkName, &ustrDevName);
	if(!NT_SUCCESS(status))
	{
		IoDeleteDevice(pDevObj);
		return status;
	}
	
	return STATUS_SUCCESS;
}


void DriverUnload(PDRIVER_OBJECT pDriverObj)
{	
	// 卸载过滤函数
	SetFilterFunction(NULL);
	// 释放所有资源
	ClearFilterList();

	// 删除符号连接名称
	UNICODE_STRING strLink;
	RtlInitUnicodeString(&strLink, LINK_NAME);
	IoDeleteSymbolicLink(&strLink);

	// 删除设备对象
	IoDeleteDevice(pDriverObj->DeviceObject);
}

// 处理IRP_MJ_CREATE、IRP_MJ_CLOSE功能代码
NTSTATUS DispatchCreateClose(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	pIrp->IoStatus.Status = STATUS_SUCCESS;
//	pIrp->IoStatus.Information = 0;
	// 完成此请求
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}


// I/O控制派遣例程
NTSTATUS DispatchIoctl(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	NTSTATUS status = STATUS_SUCCESS;

	// 取得此IRP（pIrp）的I/O堆栈指针
	PIO_STACK_LOCATION pIrpStack = IoGetCurrentIrpStackLocation(pIrp);

	// 取得I/O控制代码
	ULONG uIoControlCode = pIrpStack->Parameters.DeviceIoControl.IoControlCode;
	// 取得I/O缓冲区指针和它的长度
	PVOID pIoBuffer = pIrp->AssociatedIrp.SystemBuffer;
	ULONG uInSize = pIrpStack->Parameters.DeviceIoControl.InputBufferLength;
	
	// 响应用户的命令
	switch(uIoControlCode)
	{
	case START_IP_HOOK:	// 开始过滤
		status = SetFilterFunction(FilterPackets);
		break;

	case STOP_IP_HOOK:	// 停止过滤
		status = SetFilterFunction(NULL);
		break;

	case ADD_FILTER:	// 添加一个过滤规则
		if(uInSize == sizeof(CIPFilter))
			status = AddFilterToList((CIPFilter*)pIoBuffer);
		else 
			status = STATUS_INVALID_DEVICE_REQUEST;
		break;

	case CLEAR_FILTER:	// 释放过滤规则列表
		ClearFilterList();
		break;

	default:
		status = STATUS_INVALID_DEVICE_REQUEST;
		break;
	}
	
	// 完成请求
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = 0;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return status;
}



///////////////////////////////////////////////////////////////////
//过滤列表

// 向过滤列表中添加一个过滤规则
NTSTATUS AddFilterToList(CIPFilter* pFilter)
{
	// 为新的过滤规则申请内存空间
	CFilterList* pNew = (CFilterList*)ExAllocatePool(NonPagedPool, sizeof(CFilterList));
	if(pNew == NULL)
		return STATUS_INSUFFICIENT_RESOURCES;

	// 填充这块内存
	RtlCopyMemory(&pNew->ipf, pFilter, sizeof(CIPFilter));
	
	// 连接到过滤列表中
	pNew->pNext = g_pHeader;
	g_pHeader = pNew;

	return STATUS_SUCCESS;
}

// 清除过滤列表
void ClearFilterList()
{
	CFilterList* pNext;
	// 释放过滤列表占用的所有内存
	while(g_pHeader != NULL)
	{
		pNext = g_pHeader->pNext;
		// 释放内存
		ExFreePool(g_pHeader);
		g_pHeader = pNext;
	}
}

////////////////////////////////////////////////////
//过滤函数

// 过滤钩子回调函数
PF_FORWARD_ACTION FilterPackets(
        unsigned char   *PacketHeader,
        unsigned char   *Packet,
        unsigned int    PacketLength,
        unsigned int    RecvInterfaceIndex,
        unsigned int    SendInterfaceIndex,
        IPAddr          RecvLinkNextHop,
        IPAddr          SendLinkNextHop)
{
	// 提取IP头
	IPHeader* pIPHdr = (IPHeader*)PacketHeader;

	if(pIPHdr->ipProtocol == 6)  // 是TCP协议？
	{
		// 提取TCP头
		TCPHeader* pTCPHdr = (TCPHeader*)Packet;
		// 我们接受所有已经建立连接的TCP封包
		if(!(pTCPHdr->flags & 0x02))
		{
			return PF_FORWARD;
		}
	}

	// 与过滤规则相比较，决定采取的行动
	CFilterList* pList = g_pHeader;
	while(pList != NULL)
	{
		// 比较协议
		if(pList->ipf.protocol == 0 || pList->ipf.protocol == pIPHdr->ipProtocol)
		{
			// 查看源IP地址
			if(pList->ipf.sourceIP != 0 && 
				(pList->ipf.sourceIP & pList->ipf.sourceMask) != pIPHdr->ipSource)
			{
				pList = pList->pNext;
				continue;
			}

			// 查看目标IP地址
			if(pList->ipf.destinationIP != 0 && 
				(pList->ipf.destinationIP & pList->ipf.destinationMask) != pIPHdr->ipDestination)
			{
				pList = pList->pNext;
				continue;
			}

			// 如果是TCP封包，查看端口号
			if(pIPHdr->ipProtocol == 6)
			{
				TCPHeader* pTCPHdr = (TCPHeader*)Packet;
				if(pList->ipf.sourcePort == 0 || pList->ipf.sourcePort == pTCPHdr->sourcePort)
				{
					if(pList->ipf.destinationPort == 0 
						|| pList->ipf.destinationPort == pTCPHdr->destinationPort)
					{
						// 现在决定如何处理这个封包
						if(pList->ipf.bDrop)
							return PF_DROP;
						else
							return PF_FORWARD;
					}
				}
			}

			// 如果是UDP封包，查看端口号
			else if(pIPHdr->ipProtocol == 17)
			{
				UDPHeader* pUDPHdr = (UDPHeader*)Packet;
				if(pList->ipf.sourcePort == 0 || pList->ipf.sourcePort == pUDPHdr->sourcePort)
				{
					if(pList->ipf.destinationPort == 0 
						|| pList->ipf.destinationPort == pUDPHdr->destinationPort)
					{
						// 现在决定如何处理这个封包
						if(pList->ipf.bDrop)
							return PF_DROP;
						else
							return PF_FORWARD;
					}
				}
			}
			else 
			{
				// 对于其它封包，我们直接处理
				if(pList->ipf.bDrop)
						return PF_DROP;
					else
						return PF_FORWARD;
			}
		}

		// 比较下一个规则
		pList = pList->pNext;
	}

	// 我们接受所有没有注册的封包
	return PF_FORWARD;
}

// 注册钩子回调函数
NTSTATUS SetFilterFunction(PacketFilterExtensionPtr filterFun)
{	
	NTSTATUS status = STATUS_SUCCESS;

		// 取得IP过滤驱动设备对象。下面代码执行后，pDeviceObj变量将指向IP过滤驱动设备对象
	PDEVICE_OBJECT pDeviceObj;
	PFILE_OBJECT pFileObj;
	// 初始化IP过滤驱动的名称
	UNICODE_STRING ustrFilterDriver;
	RtlInitUnicodeString(&ustrFilterDriver, L"\\Device\\IPFILTERDRIVER");
	// 取得设备对象指针	
	status = IoGetDeviceObjectPointer(&ustrFilterDriver, FILE_ALL_ACCESS, &pFileObj, &pDeviceObj);
	if(!NT_SUCCESS(status))
	{
		return status;
	}

		// 使用到IP过滤驱动中设备对象的指针创建一个IRP	
	// 填充PF_SET_EXTENSION_HOOK_INFO结构
	PF_SET_EXTENSION_HOOK_INFO filterData;
	filterData.ExtensionPointer = filterFun;

	// 我们需要初始化一个事件对象。
	// 构建IRP时需要使用这个事件内核对象，当IP过滤取得接受到此IRP，完成工作以后会将它置位
	KEVENT event;
	KeInitializeEvent(&event, NotificationEvent, FALSE);

	// 为设备控制请求申请和构建一个IRP
	PIRP pIrp;
	IO_STATUS_BLOCK ioStatus;
	pIrp = IoBuildDeviceIoControlRequest(IOCTL_PF_SET_EXTENSION_POINTER,  // io control code
		pDeviceObj, 
		(PVOID) &filterData,
		sizeof(PF_SET_EXTENSION_HOOK_INFO),
		NULL,
		0,
		FALSE,
		&event,
		&ioStatus);
	if(pIrp == NULL)
	{
		// 如果不能申请空间，返回对应的错误代码
		return STATUS_INSUFFICIENT_RESOURCES;
	}
	
		// 请求安装钩子回调函数
	// 发送此IRP到IP过滤驱动
	status = IoCallDriver(pDeviceObj, pIrp);
	// 等待IP过滤驱动的通知
	if(status == STATUS_PENDING) 
	{
		KeWaitForSingleObject(&event, Executive, KernelMode, FALSE, NULL);
	}
	status = ioStatus.Status;

	// 清除资源
	if(pFileObj != NULL)
		ObDereferenceObject(pFileObj);
	return status;
}
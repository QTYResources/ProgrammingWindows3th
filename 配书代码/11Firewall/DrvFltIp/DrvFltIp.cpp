// DrvFltIp.cpp�ļ�


extern "C"
{
	#include <ntddk.h>
	#include <ntddndis.h>
	#include <pfhook.h>
}

#include "DrvFltIp.h"
#include "internal.h"

// �Զ��庯��������
NTSTATUS DispatchCreateClose(PDEVICE_OBJECT pDevObj, PIRP pIrp);
void DriverUnload(PDRIVER_OBJECT pDriverObj);
NTSTATUS DispatchIoctl(PDEVICE_OBJECT pDevObj, PIRP pIrp);
NTSTATUS AddFilterToList(CIPFilter* pFilter);
void ClearFilterList();
NTSTATUS SetFilterFunction(PacketFilterExtensionPtr filterFun);
PF_FORWARD_ACTION FilterPackets(unsigned char*, unsigned char*, unsigned int, unsigned int, unsigned int, IPAddr, IPAddr);

// �����б��׵�ַ
struct CFilterList* g_pHeader = NULL;


// �����ڲ����ƺͷ�����������
#define DEVICE_NAME L"\\Device\\devDrvFltIp"
#define LINK_NAME L"\\??\\DrvFltIp"


NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObj, PUNICODE_STRING pRegistryString)
{
	NTSTATUS status = STATUS_SUCCESS;

	// ��ʼ��������ǲ����
	pDriverObj->MajorFunction[IRP_MJ_CREATE] = DispatchCreateClose;
	pDriverObj->MajorFunction[IRP_MJ_CLOSE] = DispatchCreateClose;
	pDriverObj->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchIoctl;
	pDriverObj->DriverUnload = DriverUnload;

		// ��������ʼ���豸����
	// �豸����
	UNICODE_STRING ustrDevName;
	RtlInitUnicodeString(&ustrDevName, DEVICE_NAME);
	// �����豸����
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

		// ����������������
	// ������������
	UNICODE_STRING ustrLinkName;
	RtlInitUnicodeString(&ustrLinkName, LINK_NAME);
	// ��������
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
	// ж�ع��˺���
	SetFilterFunction(NULL);
	// �ͷ�������Դ
	ClearFilterList();

	// ɾ��������������
	UNICODE_STRING strLink;
	RtlInitUnicodeString(&strLink, LINK_NAME);
	IoDeleteSymbolicLink(&strLink);

	// ɾ���豸����
	IoDeleteDevice(pDriverObj->DeviceObject);
}

// ����IRP_MJ_CREATE��IRP_MJ_CLOSE���ܴ���
NTSTATUS DispatchCreateClose(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	pIrp->IoStatus.Status = STATUS_SUCCESS;
//	pIrp->IoStatus.Information = 0;
	// ��ɴ�����
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}


// I/O������ǲ����
NTSTATUS DispatchIoctl(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	NTSTATUS status = STATUS_SUCCESS;

	// ȡ�ô�IRP��pIrp����I/O��ջָ��
	PIO_STACK_LOCATION pIrpStack = IoGetCurrentIrpStackLocation(pIrp);

	// ȡ��I/O���ƴ���
	ULONG uIoControlCode = pIrpStack->Parameters.DeviceIoControl.IoControlCode;
	// ȡ��I/O������ָ������ĳ���
	PVOID pIoBuffer = pIrp->AssociatedIrp.SystemBuffer;
	ULONG uInSize = pIrpStack->Parameters.DeviceIoControl.InputBufferLength;
	
	// ��Ӧ�û�������
	switch(uIoControlCode)
	{
	case START_IP_HOOK:	// ��ʼ����
		status = SetFilterFunction(FilterPackets);
		break;

	case STOP_IP_HOOK:	// ֹͣ����
		status = SetFilterFunction(NULL);
		break;

	case ADD_FILTER:	// ���һ�����˹���
		if(uInSize == sizeof(CIPFilter))
			status = AddFilterToList((CIPFilter*)pIoBuffer);
		else 
			status = STATUS_INVALID_DEVICE_REQUEST;
		break;

	case CLEAR_FILTER:	// �ͷŹ��˹����б�
		ClearFilterList();
		break;

	default:
		status = STATUS_INVALID_DEVICE_REQUEST;
		break;
	}
	
	// �������
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = 0;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return status;
}



///////////////////////////////////////////////////////////////////
//�����б�

// ������б������һ�����˹���
NTSTATUS AddFilterToList(CIPFilter* pFilter)
{
	// Ϊ�µĹ��˹��������ڴ�ռ�
	CFilterList* pNew = (CFilterList*)ExAllocatePool(NonPagedPool, sizeof(CFilterList));
	if(pNew == NULL)
		return STATUS_INSUFFICIENT_RESOURCES;

	// �������ڴ�
	RtlCopyMemory(&pNew->ipf, pFilter, sizeof(CIPFilter));
	
	// ���ӵ������б���
	pNew->pNext = g_pHeader;
	g_pHeader = pNew;

	return STATUS_SUCCESS;
}

// ��������б�
void ClearFilterList()
{
	CFilterList* pNext;
	// �ͷŹ����б�ռ�õ������ڴ�
	while(g_pHeader != NULL)
	{
		pNext = g_pHeader->pNext;
		// �ͷ��ڴ�
		ExFreePool(g_pHeader);
		g_pHeader = pNext;
	}
}

////////////////////////////////////////////////////
//���˺���

// ���˹��ӻص�����
PF_FORWARD_ACTION FilterPackets(
        unsigned char   *PacketHeader,
        unsigned char   *Packet,
        unsigned int    PacketLength,
        unsigned int    RecvInterfaceIndex,
        unsigned int    SendInterfaceIndex,
        IPAddr          RecvLinkNextHop,
        IPAddr          SendLinkNextHop)
{
	// ��ȡIPͷ
	IPHeader* pIPHdr = (IPHeader*)PacketHeader;

	if(pIPHdr->ipProtocol == 6)  // ��TCPЭ�飿
	{
		// ��ȡTCPͷ
		TCPHeader* pTCPHdr = (TCPHeader*)Packet;
		// ���ǽ��������Ѿ��������ӵ�TCP���
		if(!(pTCPHdr->flags & 0x02))
		{
			return PF_FORWARD;
		}
	}

	// ����˹�����Ƚϣ�������ȡ���ж�
	CFilterList* pList = g_pHeader;
	while(pList != NULL)
	{
		// �Ƚ�Э��
		if(pList->ipf.protocol == 0 || pList->ipf.protocol == pIPHdr->ipProtocol)
		{
			// �鿴ԴIP��ַ
			if(pList->ipf.sourceIP != 0 && 
				(pList->ipf.sourceIP & pList->ipf.sourceMask) != pIPHdr->ipSource)
			{
				pList = pList->pNext;
				continue;
			}

			// �鿴Ŀ��IP��ַ
			if(pList->ipf.destinationIP != 0 && 
				(pList->ipf.destinationIP & pList->ipf.destinationMask) != pIPHdr->ipDestination)
			{
				pList = pList->pNext;
				continue;
			}

			// �����TCP������鿴�˿ں�
			if(pIPHdr->ipProtocol == 6)
			{
				TCPHeader* pTCPHdr = (TCPHeader*)Packet;
				if(pList->ipf.sourcePort == 0 || pList->ipf.sourcePort == pTCPHdr->sourcePort)
				{
					if(pList->ipf.destinationPort == 0 
						|| pList->ipf.destinationPort == pTCPHdr->destinationPort)
					{
						// ���ھ�����δ���������
						if(pList->ipf.bDrop)
							return PF_DROP;
						else
							return PF_FORWARD;
					}
				}
			}

			// �����UDP������鿴�˿ں�
			else if(pIPHdr->ipProtocol == 17)
			{
				UDPHeader* pUDPHdr = (UDPHeader*)Packet;
				if(pList->ipf.sourcePort == 0 || pList->ipf.sourcePort == pUDPHdr->sourcePort)
				{
					if(pList->ipf.destinationPort == 0 
						|| pList->ipf.destinationPort == pUDPHdr->destinationPort)
					{
						// ���ھ�����δ���������
						if(pList->ipf.bDrop)
							return PF_DROP;
						else
							return PF_FORWARD;
					}
				}
			}
			else 
			{
				// �����������������ֱ�Ӵ���
				if(pList->ipf.bDrop)
						return PF_DROP;
					else
						return PF_FORWARD;
			}
		}

		// �Ƚ���һ������
		pList = pList->pNext;
	}

	// ���ǽ�������û��ע��ķ��
	return PF_FORWARD;
}

// ע�ṳ�ӻص�����
NTSTATUS SetFilterFunction(PacketFilterExtensionPtr filterFun)
{	
	NTSTATUS status = STATUS_SUCCESS;

		// ȡ��IP���������豸�����������ִ�к�pDeviceObj������ָ��IP���������豸����
	PDEVICE_OBJECT pDeviceObj;
	PFILE_OBJECT pFileObj;
	// ��ʼ��IP��������������
	UNICODE_STRING ustrFilterDriver;
	RtlInitUnicodeString(&ustrFilterDriver, L"\\Device\\IPFILTERDRIVER");
	// ȡ���豸����ָ��	
	status = IoGetDeviceObjectPointer(&ustrFilterDriver, FILE_ALL_ACCESS, &pFileObj, &pDeviceObj);
	if(!NT_SUCCESS(status))
	{
		return status;
	}

		// ʹ�õ�IP�����������豸�����ָ�봴��һ��IRP	
	// ���PF_SET_EXTENSION_HOOK_INFO�ṹ
	PF_SET_EXTENSION_HOOK_INFO filterData;
	filterData.ExtensionPointer = filterFun;

	// ������Ҫ��ʼ��һ���¼�����
	// ����IRPʱ��Ҫʹ������¼��ں˶��󣬵�IP����ȡ�ý��ܵ���IRP����ɹ����Ժ�Ὣ����λ
	KEVENT event;
	KeInitializeEvent(&event, NotificationEvent, FALSE);

	// Ϊ�豸������������͹���һ��IRP
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
		// �����������ռ䣬���ض�Ӧ�Ĵ������
		return STATUS_INSUFFICIENT_RESOURCES;
	}
	
		// ����װ���ӻص�����
	// ���ʹ�IRP��IP��������
	status = IoCallDriver(pDeviceObj, pIrp);
	// �ȴ�IP����������֪ͨ
	if(status == STATUS_PENDING) 
	{
		KeWaitForSingleObject(&event, Executive, KernelMode, FALSE, NULL);
	}
	status = ioStatus.Status;

	// �����Դ
	if(pFileObj != NULL)
		ObDereferenceObject(pFileObj);
	return status;
}
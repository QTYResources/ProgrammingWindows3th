//////////////////////////////////////////////////
// CharConvert.cpp文件


extern "C"
{
	#include <ntddk.h>
}

#include <devioctl.h>
#include "CharConvert.h"

// 自定义函数的声明
NTSTATUS DispatchCreateClose(PDEVICE_OBJECT pDevObj, PIRP pIrp);
void DriverUnload(PDRIVER_OBJECT pDriverObj);
NTSTATUS DispatchIoctl(PDEVICE_OBJECT pDevObj, PIRP pIrp);

// 驱动内部名称和符号连接名称
#define DEVICE_NAME L"\\Device\\devCharConvert"
#define LINK_NAME L"\\DosDevices\\slCharConvert"

// 驱动程序加载时调用DriverEntry例程
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
				FILE_DEVICE_UNKNOWN,
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
	// 完成此请求
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}


// I/O控制派遣例程
NTSTATUS DispatchIoctl(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	// 假设失败
	NTSTATUS status = STATUS_INVALID_DEVICE_REQUEST;

	// 取得此IRP（pIrp）的I/O堆栈指针
	PIO_STACK_LOCATION pIrpStack = IoGetCurrentIrpStackLocation(pIrp);

	// 取得I/O控制代码
	ULONG uIoControlCode = pIrpStack->Parameters.DeviceIoControl.IoControlCode;
	// 取得I/O缓冲区指针和它的长度
	PVOID pIoBuffer = pIrp->AssociatedIrp.SystemBuffer;
	ULONG uInSize = pIrpStack->Parameters.DeviceIoControl.InputBufferLength;
	ULONG uOutSize = pIrpStack->Parameters.DeviceIoControl.OutputBufferLength;

	switch(uIoControlCode)
	{
	case CHAR_CONVERT:
		{
			char str[] = "零一二三四五六七八九";
			if(uInSize >= 1 && uOutSize >=2)
			{
				char c = ((char*)pIoBuffer)[0];
				if(c >= '0' && c <= '9')
				{
					// 进行转换
					c -= '0';
					RtlCopyMemory(pIoBuffer, &str[c*2], 2);
					status = STATUS_SUCCESS;
				}
			}
		}
		break;
	}

	if(status == STATUS_SUCCESS)
		pIrp->IoStatus.Information = uOutSize;
	else
		pIrp->IoStatus.Information = 0;


	// 完成请求
	pIrp->IoStatus.Status = status;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return status;
}
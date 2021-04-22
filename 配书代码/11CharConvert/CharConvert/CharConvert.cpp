//////////////////////////////////////////////////
// CharConvert.cpp�ļ�


extern "C"
{
	#include <ntddk.h>
}

#include <devioctl.h>
#include "CharConvert.h"

// �Զ��庯��������
NTSTATUS DispatchCreateClose(PDEVICE_OBJECT pDevObj, PIRP pIrp);
void DriverUnload(PDRIVER_OBJECT pDriverObj);
NTSTATUS DispatchIoctl(PDEVICE_OBJECT pDevObj, PIRP pIrp);

// �����ڲ����ƺͷ�����������
#define DEVICE_NAME L"\\Device\\devCharConvert"
#define LINK_NAME L"\\DosDevices\\slCharConvert"

// �����������ʱ����DriverEntry����
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
				FILE_DEVICE_UNKNOWN,
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
	// ��ɴ�����
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}


// I/O������ǲ����
NTSTATUS DispatchIoctl(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	// ����ʧ��
	NTSTATUS status = STATUS_INVALID_DEVICE_REQUEST;

	// ȡ�ô�IRP��pIrp����I/O��ջָ��
	PIO_STACK_LOCATION pIrpStack = IoGetCurrentIrpStackLocation(pIrp);

	// ȡ��I/O���ƴ���
	ULONG uIoControlCode = pIrpStack->Parameters.DeviceIoControl.IoControlCode;
	// ȡ��I/O������ָ������ĳ���
	PVOID pIoBuffer = pIrp->AssociatedIrp.SystemBuffer;
	ULONG uInSize = pIrpStack->Parameters.DeviceIoControl.InputBufferLength;
	ULONG uOutSize = pIrpStack->Parameters.DeviceIoControl.OutputBufferLength;

	switch(uIoControlCode)
	{
	case CHAR_CONVERT:
		{
			char str[] = "��һ�����������߰˾�";
			if(uInSize >= 1 && uOutSize >=2)
			{
				char c = ((char*)pIoBuffer)[0];
				if(c >= '0' && c <= '9')
				{
					// ����ת��
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


	// �������
	pIrp->IoStatus.Status = status;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return status;
}
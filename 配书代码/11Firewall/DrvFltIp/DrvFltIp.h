/////////////////////////////////////////////////////
// DrvFltIp.h�ļ�

#ifndef __DRVFLTIP_H__
#define __DRVFLTIP_H__

// �Զ����豸���ͣ��ڴ����豸����ʱʹ��
// ע�⣬�Զ���ֵ�ķ�Χ��32768-65535
#define FILE_DEVICE_DRVFLTIP  0x00654322

// �Զ����IO���ƴ��룬�������ֲ�ͬ���豸��������
// ע�⣬�Զ���ֵ�ķ�Χ��2048-4095
#define DRVFLTIP_IOCTL_INDEX  0x830

//
// ��������豸���ƴ��롣�ֱ��ǿ�ʼ���ˡ�ֹͣ���ˡ���ӹ��˹���������˹���
// 
#define START_IP_HOOK	CTL_CODE(FILE_DEVICE_DRVFLTIP, \
				DRVFLTIP_IOCTL_INDEX, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define STOP_IP_HOOK	CTL_CODE(FILE_DEVICE_DRVFLTIP, \
				DRVFLTIP_IOCTL_INDEX+1, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define ADD_FILTER	CTL_CODE(FILE_DEVICE_DRVFLTIP, \
				DRVFLTIP_IOCTL_INDEX+2, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define CLEAR_FILTER	CTL_CODE(FILE_DEVICE_DRVFLTIP, \
				DRVFLTIP_IOCTL_INDEX+3, METHOD_BUFFERED, FILE_ANY_ACCESS)

// ������˹���Ľṹ
struct CIPFilter
{
	USHORT protocol;		// ʹ�õ�Э��

	ULONG sourceIP;			// ԴIP��ַ
	ULONG destinationIP;		// Ŀ��IP��ַ

	ULONG sourceMask;		// Դ��ַ������
	ULONG destinationMask;		// Ŀ�ĵ�ַ������

	USHORT sourcePort;		// Դ�˿ں�
	USHORT destinationPort;		// Ŀ�Ķ˿ں�
	
	BOOLEAN bDrop;			// �Ƿ����˷��
};

#endif // __DRVFLTIP_H__
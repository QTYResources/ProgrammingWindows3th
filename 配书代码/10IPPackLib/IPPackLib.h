///////////////////////////////////////////
// IPPackLib.h�ļ�


#ifndef __IPPACKLIB_H__
#define __IPPACKLIB_H__

struct CMessageData
{
	DWORD dwThreadId;	// ��ǰ���߳�ID��
	SOCKET socket;		// ��������������׽��־��
	int nDataLength;	// ��ʵ���ݵĳ���
	// TCHAR[nDataLength]

	TCHAR* data()           // ��������������
	{ return (TCHAR*)(this+1); }
};

enum HOOK_MESSAGE		// �����Զ�����Ϣ
{
	HM_RECEIVE = WM_USER + 100,	// ˵��Ŀ����̽��ܵ�����
	HM_SEND				// ˵��Ŀ�����Ҫ��������
};


#endif // __IPPACKLIB_H__

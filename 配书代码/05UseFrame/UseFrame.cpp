/////////////////////////////////////////
// UseFrame.cpp�ļ�

#include "UseFrame.h"


CMyApp theApp;		// Ӧ�ó���ʵ������

///////////////////////////////////////
// CMyApp��

BOOL CMyApp::InitInstance()
{
	::MessageBox(NULL, "���߳̿�ʼִ�У�", "CMyApp::InitInstance", 0);

	return FALSE;	// ��Ҫ������Ϣѭ��
}

int CMyApp::ExitInstance()
{
	::MessageBox(NULL, "���߳̽�Ҫ�˳���", "CMyApp::ExitInstance", MB_OK);
	return 0;
}

/////////////////////////////////////////
// UseFrame.cpp文件

#include "UseFrame.h"


CMyApp theApp;		// 应用程序实例对象

///////////////////////////////////////
// CMyApp类

BOOL CMyApp::InitInstance()
{
	::MessageBox(NULL, "主线程开始执行！", "CMyApp::InitInstance", 0);

	return FALSE;	// 不要进入消息循环
}

int CMyApp::ExitInstance()
{
	::MessageBox(NULL, "主线程将要退出！", "CMyApp::ExitInstance", MB_OK);
	return 0;
}

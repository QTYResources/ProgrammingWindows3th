////////////////////////////////////////////////
// WINMAIN.CPP文件

#include "_afxwin.h"

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPTSTR lpCmdLine, int nCmdShow)
{
	ASSERT(hPrevInstance == NULL);

	int nReturnCode = -1;
	CWinThread* pThread = AfxGetThread();
	CWinApp* pApp = AfxGetApp();

	// 类库框架内部的初始化
	if(!AfxWinInit(hInstance, hPrevInstance, lpCmdLine, nCmdShow))
		goto InitFailure;

	// 应用程序的全局初始化
	if(pApp != NULL && !pApp->InitApplication())
		goto InitFailure;

	// 主线程的初始化
	if(!pThread->InitInstance())
	{
		nReturnCode = pThread->ExitInstance();
		goto InitFailure;
	}

	// 开始与用户交互
	nReturnCode = pThread->Run();

InitFailure:
	return nReturnCode;
}
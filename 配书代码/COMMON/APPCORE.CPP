///////////////////////////////////////////////////////
// APPCORE.CPP文件

#include "_afxwin.h"

CWinApp::CWinApp()
{
	// 初始化CWinThread状态
	AFX_MODULE_STATE* pModuleState = AfxGetModuleState();
	AFX_MODULE_THREAD_STATE* pThreadState = pModuleState->m_thread;
	ASSERT(AfxGetThread() == NULL);
	pThreadState->m_pCurrentWinThread = this;
	ASSERT(AfxGetThread() == this);
	m_hThread = ::GetCurrentThread();
	m_nThreadID = ::GetCurrentThreadId();

	// 初始化CWinApp状态
	ASSERT(pModuleState->m_pCurrentWinApp == NULL);
	pModuleState->m_pCurrentWinApp = this;
	ASSERT(AfxGetApp() == this);

	// 直到进入WinMain函数之后再设置为运行状态
	m_hInstance = NULL;
}

CWinApp::~CWinApp()
{
	AFX_MODULE_STATE* pModuleState = AfxGetModuleState();
	if(pModuleState->m_pCurrentWinApp == this)
		pModuleState->m_pCurrentWinApp = NULL;
}

BOOL CWinApp::InitApplication()
{
	return TRUE;
}

BOOL CWinApp::InitInstance()
{
	return TRUE;
}

int CWinApp::Run()
{
	return CWinThread::Run();
}

int CWinApp::ExitInstance()
{
	return m_msgCur.wParam;
}

IMPLEMENT_DYNCREATE(CWinApp, CWinThread)
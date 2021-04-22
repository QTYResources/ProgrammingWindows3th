////////////////////////////////////////////////
// HookTermProApp.cpp文件

#include "resource.h"
#include "HookTermProApp.h"

CMyApp theApp;

// DLL导出函数
BOOL WINAPI SetSysHook(BOOL bInstall, DWORD dwThreadId = 0)
{
	typedef (WINAPI *PFNSETSYSHOOK)(BOOL, DWORD);

	// 调试的时候可以这样设置szDll[] = "..//09HookTermProLib//debug//09HookTermProLib.dll";
	char szDll[] = "09HookTermProLib.dll";

	// 加载09HookTermProLib.dll模块
	BOOL bNeedFree = FALSE;
	HMODULE hModule = ::GetModuleHandle(szDll);
	if(hModule == NULL)
	{
		hModule = ::LoadLibrary(szDll);
		bNeedFree = TRUE;
	}

	// 获取SetSysHook函数的地址
	PFNSETSYSHOOK mSetSysHook = (PFNSETSYSHOOK)::GetProcAddress(hModule, "SetSysHook");
	if(mSetSysHook == NULL) // 文件不正确?
	{
		if(bNeedFree)
			::FreeLibrary(hModule);
		return FALSE;
	}

	// 调用SetSysHook函数
	BOOL bRet = mSetSysHook(bInstall, dwThreadId);

	// 如果有必要，释放上面加载的模块
	if(bNeedFree)
		::FreeLibrary(hModule);

	return bRet;
}

BOOL CMyApp::InitInstance()
{
	// 安装钩子
	if(!SetSysHook(TRUE, 0))
		::MessageBox(NULL, "安装钩子出错！", "09HookTermProApp", 0);
	// 显示对话框
	CMainDialog dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	// 卸载钩子
	SetSysHook(FALSE);

	return FALSE;
}

CMainDialog::CMainDialog(CWnd* pParentWnd):CDialog(IDD_MAINDLG, pParentWnd)
{
}

BEGIN_MESSAGE_MAP(CMainDialog, CDialog)
ON_WM_COPYDATA()
END_MESSAGE_MAP()

BOOL CMainDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(theApp.LoadIcon(IDI_MAIN), FALSE);
	::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 
		0, 0, SWP_NOSIZE|SWP_NOREDRAW|SWP_NOMOVE);

	return TRUE;
}

BOOL CMainDialog::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	GetDlgItem(IDC_HOOKINFO)->SetWindowText((char*)pCopyDataStruct->lpData);
	// 检查是否禁止执行
	BOOL bForbid = ((CButton*)GetDlgItem(IDC_FORBIDEXE))->GetCheck();
	if(bForbid)
		return -1;
	return TRUE;
}



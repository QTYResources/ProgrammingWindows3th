////////////////////////////////////////////////
// HookTermProApp.cpp�ļ�

#include "resource.h"
#include "HookTermProApp.h"

CMyApp theApp;

// DLL��������
BOOL WINAPI SetSysHook(BOOL bInstall, DWORD dwThreadId = 0)
{
	typedef (WINAPI *PFNSETSYSHOOK)(BOOL, DWORD);

	// ���Ե�ʱ�������������szDll[] = "..//09HookTermProLib//debug//09HookTermProLib.dll";
	char szDll[] = "09HookTermProLib.dll";

	// ����09HookTermProLib.dllģ��
	BOOL bNeedFree = FALSE;
	HMODULE hModule = ::GetModuleHandle(szDll);
	if(hModule == NULL)
	{
		hModule = ::LoadLibrary(szDll);
		bNeedFree = TRUE;
	}

	// ��ȡSetSysHook�����ĵ�ַ
	PFNSETSYSHOOK mSetSysHook = (PFNSETSYSHOOK)::GetProcAddress(hModule, "SetSysHook");
	if(mSetSysHook == NULL) // �ļ�����ȷ?
	{
		if(bNeedFree)
			::FreeLibrary(hModule);
		return FALSE;
	}

	// ����SetSysHook����
	BOOL bRet = mSetSysHook(bInstall, dwThreadId);

	// ����б�Ҫ���ͷ�������ص�ģ��
	if(bNeedFree)
		::FreeLibrary(hModule);

	return bRet;
}

BOOL CMyApp::InitInstance()
{
	// ��װ����
	if(!SetSysHook(TRUE, 0))
		::MessageBox(NULL, "��װ���ӳ���", "09HookTermProApp", 0);
	// ��ʾ�Ի���
	CMainDialog dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	// ж�ع���
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
	// ����Ƿ��ִֹ��
	BOOL bForbid = ((CButton*)GetDlgItem(IDC_FORBIDEXE))->GetCheck();
	if(bForbid)
		return -1;
	return TRUE;
}



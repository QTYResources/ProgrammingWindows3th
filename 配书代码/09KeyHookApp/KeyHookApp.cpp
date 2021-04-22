////////////////////////////////////////////////
// KeyHookApp.cpp�ļ�

#include "resource.h"
#include "KeyHookApp.h"
#include "../09KeyHookLib/KeyHookLib.h"

#pragma comment(lib, "09KeyHookLib")

CMyApp theApp;

BOOL CMyApp::InitInstance()
{
	CMainDialog dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	return FALSE;
}

CMainDialog::CMainDialog(CWnd* pParentWnd):CDialog(IDD_MAIN, pParentWnd)
{
}

BEGIN_MESSAGE_MAP(CMainDialog, CDialog)
ON_MESSAGE(HM_KEY, OnHookKey)
END_MESSAGE_MAP()

BOOL CMainDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(theApp.LoadIcon(IDI_MAIN), FALSE);
	::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 
		0, 0, SWP_NOSIZE|SWP_NOREDRAW|SWP_NOMOVE);

	// ��װ����
	if(!SetKeyHook(TRUE, 0, m_hWnd))
		MessageBox("��װ����ʧ�ܣ�");

	return TRUE;
}


void CMainDialog::OnCancel()
{
	// ж�ع���
	SetKeyHook(FALSE);
	CDialog::OnCancel();
	return;
}

long CMainDialog::OnHookKey(WPARAM wParam, LPARAM lParam)
{
	// ��ʱ����wParamΪ�û�������������룬
	// lParam���������������ظ�������ɨ���롢ǰһ������״̬����Ϣ

	char szKey[80];
	::GetKeyNameText(lParam, szKey, 80);

	CString strItem;
	strItem.Format(" �û�������%s \r\n", szKey);
	// ��ӵ��༭����
	CString strEdit;
	GetDlgItem(IDC_KEYMSG)->GetWindowText(strEdit);
	GetDlgItem(IDC_KEYMSG)->SetWindowText(strItem + strEdit);

	::MessageBeep(MB_OK);
	return 0;
}



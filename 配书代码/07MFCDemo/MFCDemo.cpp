////////////////////////////////////////////
// MFCDemo.cpp�ļ�

#include "resource.h"
#include "MFCDemo.h"

// ȫ��Ӧ�ó���ʵ��
CMyApp theApp;

//-----------------------CMyApp��----------------------//

BOOL CMyApp::InitInstance()
{
	m_pMainWnd = new CMainWindow;
	m_pMainWnd->ShowWindow(m_nCmdShow);
	return TRUE;
}

//-----------------------CMainWindow��---------------------------//

#define IDC_MODEL		10
#define IDC_MODELESS		11

CMainWindow::CMainWindow()
{
	LPCTSTR lpszClassName = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW, 
		::LoadCursor(NULL, IDC_ARROW), (HBRUSH)(COLOR_3DFACE+1));

	CreateEx(WS_EX_CLIENTEDGE, lpszClassName, 
		"��ܳ��򴴽��Ĵ���", WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL);
}

BEGIN_MESSAGE_MAP(CMainWindow, CWnd)
ON_WM_CREATE()
ON_COMMAND(IDC_MODEL, OnModel)
ON_COMMAND(IDC_MODELESS, OnModeless)
END_MESSAGE_MAP()

BOOL CMainWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_btnModel.Create("Model Dialog", 
		WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, CRect(50, 30, 180, 70), this, IDC_MODEL);

	m_btnModeless.Create("Modeless Dialog", 
		WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, CRect(220, 30, 350, 70), this, IDC_MODELESS);

	return TRUE;
}

void CMainWindow::OnModel()	// �û������Model Dialog����ť
{
	CMyDialog dlg(this);
	// ��ʾģʽ�Ի���
	dlg.DoModal();
}

void CMainWindow::OnModeless()	// �û������Modeless Dialo����ť
{
	CMyDialog* pDlg = new CMyDialog(this);
	pDlg->m_bModeless = TRUE;

	// ������ģʽ�Ի���
	pDlg->Create(IDD_MYDIALOG);
	// �ƶ����ڵ������ڵ�����
	pDlg->CenterWindow();
	// ��ʾ���´���
	pDlg->ShowWindow(SW_NORMAL);
	pDlg->UpdateWindow();
}

void CMainWindow::PostNcDestroy()
{
	delete this;
}


//-----------------------CMyDialog��------------------------//

CMyDialog::CMyDialog(CWnd* pParentWnd) : CDialog(IDD_MYDIALOG, pParentWnd)
{
	m_bModeless = FALSE;
}

BEGIN_MESSAGE_MAP(CMyDialog, CDialog)
ON_BN_CLICKED(IDC_START, OnStart)	// Ҳ������ON_COMMAND(IDC_START, OnStart)
END_MESSAGE_MAP()

BOOL CMyDialog::OnInitDialog()		// ��ʼ���Ի���
{
	CDialog::OnInitDialog();

	SetWindowText("��ܳ��򴰿ڵĶԻ���");
	return TRUE;
}

void CMyDialog::OnCancel()		// �û��رնԻ���
{
	if(m_bModeless)
		DestroyWindow();
	else
		CDialog::OnCancel(); 
}

void CMyDialog::OnStart()		// �û��������ʼ����ť
{
	MessageBox("��ʼ��");
}

void CMyDialog::PostNcDestroy()
{
	if(m_bModeless)
		delete this;
}


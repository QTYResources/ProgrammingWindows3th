////////////////////////////////////////////
// MFCDemo.cpp文件

#include "resource.h"
#include "MFCDemo.h"

// 全局应用程序实例
CMyApp theApp;

//-----------------------CMyApp类----------------------//

BOOL CMyApp::InitInstance()
{
	m_pMainWnd = new CMainWindow;
	m_pMainWnd->ShowWindow(m_nCmdShow);
	return TRUE;
}

//-----------------------CMainWindow类---------------------------//

#define IDC_MODEL		10
#define IDC_MODELESS		11

CMainWindow::CMainWindow()
{
	LPCTSTR lpszClassName = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW, 
		::LoadCursor(NULL, IDC_ARROW), (HBRUSH)(COLOR_3DFACE+1));

	CreateEx(WS_EX_CLIENTEDGE, lpszClassName, 
		"框架程序创建的窗口", WS_OVERLAPPEDWINDOW, 
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

void CMainWindow::OnModel()	// 用户点击“Model Dialog”按钮
{
	CMyDialog dlg(this);
	// 显示模式对话框
	dlg.DoModal();
}

void CMainWindow::OnModeless()	// 用户点击“Modeless Dialo”按钮
{
	CMyDialog* pDlg = new CMyDialog(this);
	pDlg->m_bModeless = TRUE;

	// 创建无模式对话框
	pDlg->Create(IDD_MYDIALOG);
	// 移动窗口到主窗口的中央
	pDlg->CenterWindow();
	// 显示更新窗口
	pDlg->ShowWindow(SW_NORMAL);
	pDlg->UpdateWindow();
}

void CMainWindow::PostNcDestroy()
{
	delete this;
}


//-----------------------CMyDialog类------------------------//

CMyDialog::CMyDialog(CWnd* pParentWnd) : CDialog(IDD_MYDIALOG, pParentWnd)
{
	m_bModeless = FALSE;
}

BEGIN_MESSAGE_MAP(CMyDialog, CDialog)
ON_BN_CLICKED(IDC_START, OnStart)	// 也可以是ON_COMMAND(IDC_START, OnStart)
END_MESSAGE_MAP()

BOOL CMyDialog::OnInitDialog()		// 初始化对话框
{
	CDialog::OnInitDialog();

	SetWindowText("框架程序窗口的对话框");
	return TRUE;
}

void CMyDialog::OnCancel()		// 用户关闭对话框
{
	if(m_bModeless)
		DestroyWindow();
	else
		CDialog::OnCancel(); 
}

void CMyDialog::OnStart()		// 用户点击“开始”按钮
{
	MessageBox("开始！");
}

void CMyDialog::PostNcDestroy()
{
	if(m_bModeless)
		delete this;
}


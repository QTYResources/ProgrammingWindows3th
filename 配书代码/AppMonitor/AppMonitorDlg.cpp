// AppMonitorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AppMonitor.h"
#include "AppMonitorDlg.h"

#include <tlhelp32.h> // 声明快照函数的头文件

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAppMonitorDlg dialog

CAppMonitorDlg::CAppMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAppMonitorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAppMonitorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAppMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAppMonitorDlg)
	DDX_Control(pDX, IDC_BTNBROWSE, m_Browse);
	DDX_Control(pDX, IDOK, m_Start);
	DDX_Control(pDX, IDC_EDITAPP, m_Name);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAppMonitorDlg, CDialog)
	//{{AFX_MSG_MAP(CAppMonitorDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTNBROWSE, OnBtnbrowse)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAppMonitorDlg message handlers

BOOL CAppMonitorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	m_bStart = FALSE;

	SetTimer(2, 1000, NULL);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAppMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAppMonitorDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAppMonitorDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CAppMonitorDlg::OnBtnbrowse() 
{
	CFileDialog openfile(TRUE);
	if(openfile.DoModal() != IDOK)
		return;

	m_Name.SetWindowText(openfile.GetPathName());

	
}

void CAppMonitorDlg::OnOK() 
{
	if(m_bStart)
	{
		m_Start.SetWindowText("启动");
		m_bStart = FALSE;
		m_Browse.EnableWindow(TRUE);
	}
	else
	{
		CString str;
		m_Name.GetWindowText(str);
		
		if(str.IsEmpty())
		{
			MessageBox("empty");
			return;
		}


		m_Start.SetWindowText("停止");
		m_bStart = TRUE;
		m_Browse.EnableWindow(FALSE);
	}
}


void CAppMonitorDlg::OnCancel() 
{
	
	CDialog::OnCancel();
}



int CAppMonitorDlg::Proc()
{
	
	CString str;
	m_Name.GetWindowText(str);
	
	if(str.IsEmpty())
		return 1;

	CString strName = str;

	int nn = strName.ReverseFind('\\');
	if(nn <= 0)
		return 1;

	strName = str.Right(str.GetLength() - nn - 1);




	PROCESSENTRY32 pe32;
	// 在使用这个结构之前，先设置它的大小
	pe32.dwSize = sizeof(pe32); 
	
	// 给系统内的所有进程拍一个快照
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hProcessSnap == INVALID_HANDLE_VALUE)
	{
		printf(" CreateToolhelp32Snapshot调用失败！ \n");
		return -1;
	}

	BOOL bNeedStart = TRUE;
	
	// 遍历进程快照，轮流显示每个进程的信息
	BOOL bMore = ::Process32First(hProcessSnap, &pe32);
	while(bMore)
	{
		if(strcmp(pe32.szExeFile, strName) == 0)
		{
			bNeedStart = FALSE;
		}

		bMore = ::Process32Next(hProcessSnap, &pe32);
	}

	// 不要忘记清除掉snapshot对象
	::CloseHandle(hProcessSnap);
	
	if(bNeedStart)
	{	
		::Sleep(1000);
		
		STARTUPINFO si = { sizeof(si) };
		PROCESS_INFORMATION pi;
		
		si.dwFlags = STARTF_USESHOWWINDOW;	// 指定wShowWindow成员有效
		si.wShowWindow = TRUE;			// 此成员设为TRUE的话则显示新建进程的主窗口，
		// 为FALSE的话则不显示
		BOOL bRet = ::CreateProcess (
			NULL,			// 不在此指定可执行文件的文件名
			str.GetBuffer(str.GetLength()),		// 命令行参数
			NULL,			// 默认进程安全性
			NULL,			// 默认线程安全性
			FALSE,			// 指定当前进程内的句柄不可以被子进程继承
			CREATE_NEW_CONSOLE,	// 为新进程创建一个新的控制台窗口
			NULL,			// 使用本进程的环境变量
			NULL,			// 使用本进程的驱动器和目录
			&si,
			&pi);
		
		
		
		if(bRet)
		{
			// 既然我们不使用两个句柄，最好是立刻将它们关闭
			::CloseHandle (pi.hThread);
			::CloseHandle (pi.hProcess);
		}
		//
	}
	
	return 1;
}

void CAppMonitorDlg::OnTimer(UINT nIDEvent) 
{
	if(m_bStart)
	{
		Proc();
	}
	
	CDialog::OnTimer(nIDEvent);
}

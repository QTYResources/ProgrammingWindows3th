// AppMonitorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AppMonitor.h"
#include "AppMonitorDlg.h"

#include <tlhelp32.h> // �������պ�����ͷ�ļ�

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
		m_Start.SetWindowText("����");
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


		m_Start.SetWindowText("ֹͣ");
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
	// ��ʹ������ṹ֮ǰ�����������Ĵ�С
	pe32.dwSize = sizeof(pe32); 
	
	// ��ϵͳ�ڵ����н�����һ������
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hProcessSnap == INVALID_HANDLE_VALUE)
	{
		printf(" CreateToolhelp32Snapshot����ʧ�ܣ� \n");
		return -1;
	}

	BOOL bNeedStart = TRUE;
	
	// �������̿��գ�������ʾÿ�����̵���Ϣ
	BOOL bMore = ::Process32First(hProcessSnap, &pe32);
	while(bMore)
	{
		if(strcmp(pe32.szExeFile, strName) == 0)
		{
			bNeedStart = FALSE;
		}

		bMore = ::Process32Next(hProcessSnap, &pe32);
	}

	// ��Ҫ���������snapshot����
	::CloseHandle(hProcessSnap);
	
	if(bNeedStart)
	{	
		::Sleep(1000);
		
		STARTUPINFO si = { sizeof(si) };
		PROCESS_INFORMATION pi;
		
		si.dwFlags = STARTF_USESHOWWINDOW;	// ָ��wShowWindow��Ա��Ч
		si.wShowWindow = TRUE;			// �˳�Ա��ΪTRUE�Ļ�����ʾ�½����̵������ڣ�
		// ΪFALSE�Ļ�����ʾ
		BOOL bRet = ::CreateProcess (
			NULL,			// ���ڴ�ָ����ִ���ļ����ļ���
			str.GetBuffer(str.GetLength()),		// �����в���
			NULL,			// Ĭ�Ͻ��̰�ȫ��
			NULL,			// Ĭ���̰߳�ȫ��
			FALSE,			// ָ����ǰ�����ڵľ�������Ա��ӽ��̼̳�
			CREATE_NEW_CONSOLE,	// Ϊ�½��̴���һ���µĿ���̨����
			NULL,			// ʹ�ñ����̵Ļ�������
			NULL,			// ʹ�ñ����̵���������Ŀ¼
			&si,
			&pi);
		
		
		
		if(bRet)
		{
			// ��Ȼ���ǲ�ʹ�������������������̽����ǹر�
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

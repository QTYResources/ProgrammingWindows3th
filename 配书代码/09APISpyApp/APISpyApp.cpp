////////////////////////////////////////////////
// APISpyApp.cpp文件

#include <afxdlgs.h> // 为了使用CFileDialog类
#include "resource.h"
#include "APISpyApp.h"

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
	char szDllPath[MAX_PATH];
	LPSTR p;

	::GetFullPathName("09APISpyLib.dll", MAX_PATH, szDllPath, &p);
	// 调试时可以直接用“E:\\MyWork\\book_code\\09APISpyLib\\Debug\\09APISpyLib.dll”
	m_pInjector = new CRemThreadInjector(szDllPath); 
	m_pShareMem = NULL;
}

CMainDialog::~CMainDialog()
{
	delete m_pInjector;
}

BEGIN_MESSAGE_MAP(CMainDialog, CDialog)
ON_BN_CLICKED(IDC_BROWSER, OnBrowser)
ON_BN_CLICKED(IDC_START, OnStart)
ON_BN_CLICKED(IDC_CLEAR, OnClear)
ON_MESSAGE(HM_SPYACALL, OnSpyACall)
END_MESSAGE_MAP()

BOOL CMainDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(theApp.LoadIcon(IDI_MAIN), FALSE);

	return TRUE;
}

void CMainDialog::OnCancel()
{
	CDialog::OnCancel();
}

void CMainDialog::OnBrowser()
{
	CFileDialog file(TRUE);

	// 显示选择文件对话框
	if(file.DoModal() == IDOK)
	{
		GetDlgItem(IDC_TARGETAPP)->SetWindowText(file.GetPathName());
	}
}

void CMainDialog::OnStart()
{
	// 如果m_pShareMem不为NULL，则证明用户单击“停止”按钮
	if(m_pShareMem != NULL)
	{
		// 取消注入
		m_pInjector->EjectModuleFrom(m_dwProcessId);
		// 删除对象
		delete m_pShareMem;
		m_pShareMem = NULL;
		// 设置UI界面
		GetDlgItem(IDC_START)->SetWindowText("开始");
		return;
	}

		// 取得用户输入
	// 取得目标程序名称
	CString strTargetApp;
	GetDlgItem(IDC_TARGETAPP)->GetWindowText(strTargetApp);
	if(strTargetApp.IsEmpty())
	{
		MessageBox("请选择目标程序！");
		return; 
	}
	// 取得API函数和所在模块名称
	CString strAPIName, strDllName;
	GetDlgItem(IDC_APINAME)->GetWindowText(strAPIName);
	GetDlgItem(IDC_DLLNAME)->GetWindowText(strDllName);
	if(strAPIName.IsEmpty() || strDllName.IsEmpty())
	{
		MessageBox("请输入您要侦测的函数或模块名称！");
		return;
	}
	// 检查用户输入的函数是否在指定模块中
	HMODULE h = ::LoadLibrary(strDllName);
	if(::GetProcAddress(h, strAPIName) == NULL)
	{
		MessageBox("您输入的模块中不包含要侦测的函数！");
		if(h != NULL)
			::FreeLibrary(h);
		return;
	}
	::FreeLibrary(h);

		// 注入DLL
	// 创建共享内存，写入参数信息
	m_pShareMem = new CMyShareMem(TRUE);
	m_pShareMem->GetData()->hWndCaller = m_hWnd;
	strncpy(m_pShareMem->GetData()->szFuncName, strAPIName, 56);
	strncpy(m_pShareMem->GetData()->szModName, strDllName, 56);

	// 创建目标进程
	BOOL bOK;
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	bOK = ::CreateProcess(NULL, strTargetApp.GetBuffer(0), 
				NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if(bOK)
	{	
		m_dwProcessId = pi.dwProcessId;
		// 注入DLL
		bOK = m_pInjector->InjectModuleInto(m_dwProcessId);
		if(!bOK)
		{
			MessageBox("注入DLL出错！");
		}
		::CloseHandle(pi.hThread);
		::CloseHandle(pi.hProcess);
	}
	else
	{
		MessageBox("启动目标程序失败！");
	}

	// 如果没有成功，删除共享内存
	if(!bOK)
	{
		delete m_pShareMem;
		m_pShareMem = NULL;
		return;
	}
	// 设置UI界面
	OnClear();
	GetDlgItem(IDC_START)->SetWindowText("停止");
}

long CMainDialog::OnSpyACall(WPARAM wParam, LPARAM lParam)
{
	BOOL bPause = ((CButton*)GetDlgItem(IDC_CHECKPAUSE))->GetCheck();
	if(bPause)
		return 0;

	CString strItem ;
	strItem.Format(" 第%d次调用； \r\n", m_nCount++);

	// 添加到编辑框中
	CString strEdit;
	GetDlgItem(IDC_SPYMSG)->GetWindowText(strEdit);
	GetDlgItem(IDC_SPYMSG)->SetWindowText(strItem + strEdit);
	return 0;
}

void CMainDialog::OnClear()
{
	m_nCount = 0;
	GetDlgItem(IDC_SPYMSG)->SetWindowText("");
}
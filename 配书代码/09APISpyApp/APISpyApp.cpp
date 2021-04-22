////////////////////////////////////////////////
// APISpyApp.cpp�ļ�

#include <afxdlgs.h> // Ϊ��ʹ��CFileDialog��
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
	// ����ʱ����ֱ���á�E:\\MyWork\\book_code\\09APISpyLib\\Debug\\09APISpyLib.dll��
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

	// ��ʾѡ���ļ��Ի���
	if(file.DoModal() == IDOK)
	{
		GetDlgItem(IDC_TARGETAPP)->SetWindowText(file.GetPathName());
	}
}

void CMainDialog::OnStart()
{
	// ���m_pShareMem��ΪNULL����֤���û�������ֹͣ����ť
	if(m_pShareMem != NULL)
	{
		// ȡ��ע��
		m_pInjector->EjectModuleFrom(m_dwProcessId);
		// ɾ������
		delete m_pShareMem;
		m_pShareMem = NULL;
		// ����UI����
		GetDlgItem(IDC_START)->SetWindowText("��ʼ");
		return;
	}

		// ȡ���û�����
	// ȡ��Ŀ���������
	CString strTargetApp;
	GetDlgItem(IDC_TARGETAPP)->GetWindowText(strTargetApp);
	if(strTargetApp.IsEmpty())
	{
		MessageBox("��ѡ��Ŀ�����");
		return; 
	}
	// ȡ��API����������ģ������
	CString strAPIName, strDllName;
	GetDlgItem(IDC_APINAME)->GetWindowText(strAPIName);
	GetDlgItem(IDC_DLLNAME)->GetWindowText(strDllName);
	if(strAPIName.IsEmpty() || strDllName.IsEmpty())
	{
		MessageBox("��������Ҫ���ĺ�����ģ�����ƣ�");
		return;
	}
	// ����û�����ĺ����Ƿ���ָ��ģ����
	HMODULE h = ::LoadLibrary(strDllName);
	if(::GetProcAddress(h, strAPIName) == NULL)
	{
		MessageBox("�������ģ���в�����Ҫ���ĺ�����");
		if(h != NULL)
			::FreeLibrary(h);
		return;
	}
	::FreeLibrary(h);

		// ע��DLL
	// ���������ڴ棬д�������Ϣ
	m_pShareMem = new CMyShareMem(TRUE);
	m_pShareMem->GetData()->hWndCaller = m_hWnd;
	strncpy(m_pShareMem->GetData()->szFuncName, strAPIName, 56);
	strncpy(m_pShareMem->GetData()->szModName, strDllName, 56);

	// ����Ŀ�����
	BOOL bOK;
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	bOK = ::CreateProcess(NULL, strTargetApp.GetBuffer(0), 
				NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if(bOK)
	{	
		m_dwProcessId = pi.dwProcessId;
		// ע��DLL
		bOK = m_pInjector->InjectModuleInto(m_dwProcessId);
		if(!bOK)
		{
			MessageBox("ע��DLL����");
		}
		::CloseHandle(pi.hThread);
		::CloseHandle(pi.hProcess);
	}
	else
	{
		MessageBox("����Ŀ�����ʧ�ܣ�");
	}

	// ���û�гɹ���ɾ�������ڴ�
	if(!bOK)
	{
		delete m_pShareMem;
		m_pShareMem = NULL;
		return;
	}
	// ����UI����
	OnClear();
	GetDlgItem(IDC_START)->SetWindowText("ֹͣ");
}

long CMainDialog::OnSpyACall(WPARAM wParam, LPARAM lParam)
{
	BOOL bPause = ((CButton*)GetDlgItem(IDC_CHECKPAUSE))->GetCheck();
	if(bPause)
		return 0;

	CString strItem ;
	strItem.Format(" ��%d�ε��ã� \r\n", m_nCount++);

	// ��ӵ��༭����
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
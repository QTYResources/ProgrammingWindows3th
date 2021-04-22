////////////////////////////////////////////////
// MonitorDir.cpp�ļ�

#include "resource.h"
#include "MonitorDir.h"
#include "DirDialog.h"

#include "SkinMagicLib.h"
// ע�⣬���MFC�Ƕ�̬���ӵ������еģ���Ӧ��ѡ��SkinMagicLibMD6Trial.lib��
#pragma comment(lib, "SkinMagicLibMT6Trial")

CMyApp theApp;

BOOL CMyApp::InitInstance()
{
	//===SkinMagic===

	// ��ʼ��SkinMagic��
	VERIFY(InitSkinMagicLib(AfxGetInstanceHandle(), "MonitorDir", NULL, NULL));
	// ����Դ�м���Ƥ���ļ���Ҳ�����ô��롰LoadSkinFile("corona.smf")��ֱ�Ӵ��ļ��м���
	if(LoadSkinFromResource(AfxGetInstanceHandle(), (LPCTSTR)IDR_SKINMAGIC1, "SKINMAGIC"))
	{
		// ���öԻ���Ĭ��Ƥ��
		SetDialogSkin("Dialog"); 
	}

	//===SkinMagic===

	CMonitorDialog dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	return FALSE;	// ����FALSE��ֹ���������Ϣѭ��
}

int CMyApp::ExitInstance()
{
	//===SkinMagic===

	// �ͷ�SkinMagic��������ڴ�
	ExitSkinMagicLib();

	//===SkinMagic===

	return CWinApp::ExitInstance();
}

CMonitorDialog::CMonitorDialog(CWnd* pParentWnd):CDialog(IDD_MAINDIALOG, pParentWnd)
{
	m_hEvent = ::CreateEvent(NULL, FALSE, 0, NULL);
}

CMonitorDialog::~CMonitorDialog()
{
	::CloseHandle(m_hEvent);
}

BEGIN_MESSAGE_MAP(CMonitorDialog, CDialog)
ON_BN_CLICKED(IDC_START, OnStart)
ON_BN_CLICKED(IDC_STOP, OnStop)
ON_BN_CLICKED(IDC_BROWSER, OnBrowser)
ON_BN_CLICKED(IDC_CLEAR, OnClear)
END_MESSAGE_MAP()

BOOL CMonitorDialog::OnInitDialog()
{
	// �ø�������ڲ���ʼ��
	CDialog::OnInitDialog();

	// ����ͼ��
	SetIcon(theApp.LoadIcon(IDI_MAIN), FALSE);

	// ����״̬���������������ԣ�CStatusBarCtrl���װ�˶�״̬���ؼ��Ĳ�����
	m_bar.Create(WS_CHILD|WS_VISIBLE|SBS_SIZEGRIP, CRect(0, 0, 0, 0), this, 101);
	m_bar.SetBkColor(RGB(0xa6, 0xca, 0xf0));		// ����ɫ
	int arWidth[] = { 250, -1 };
	m_bar.SetParts(2, arWidth);				// ����
	m_bar.SetText(" Windows����������ŵ����룡", 1, 0);	// �ڶ��������ı�
	m_bar.SetText(" ����", 0, 0);				// ��һ�������ı�

	// ��Чֹͣ��ť
	GetDlgItem(IDC_STOP)->EnableWindow(FALSE);
	// ���ø�����ѡ��Ϊѡ��״̬
	((CButton*)GetDlgItem(IDC_SUBDIR))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_FILENAME_CHANGE))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_FILESIZE_CHANGE))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_DIRNAME_CHANGE))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_LASTWRITE_CHANGE))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_ATTRIBUTE_CHANGE))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_SECURITY_CHANGE))->SetCheck(1);

	return TRUE;
}

void CMonitorDialog::OnBrowser()	// �û���������ť
{
	// ����ѡ��Ŀ¼�Ի���
	CDirDialog dir;
	if(dir.DoBrowse(m_hWnd, "��ѡ����Ҫ���ӵ�Ŀ¼��"))
	{
		GetDlgItem(IDC_TARGETDIR)->SetWindowText(dir.GetPath());
	}
}

void CMonitorDialog::OnStart()		// �û������ʼ��ť
{
	CString strDir;
	// ȡ��Ŀ¼����
	GetDlgItem(IDC_TARGETDIR)->GetWindowText(strDir);
	if(strDir.IsEmpty())
	{
		MessageBox("��ѡ��һ��Ҫ���ӵ�Ŀ¼��");
		return;
	}

	
	// ���¼���������ʼ���������
	for(int i=0; i<6; i++)
		m_arhChange[i] = m_hEvent;
	m_bExit = FALSE;

	// �Ƿ�Ҫ������Ŀ¼
	BOOL bSubDir = ((CButton*)GetDlgItem(IDC_SUBDIR))->GetCheck();
	BOOL bNeedExecute = FALSE;

	// ����Ŀ¼���Ƶĸı�		arhChange[0]
	if(((CButton*)GetDlgItem(IDC_DIRNAME_CHANGE))->GetCheck())
	{
		m_arhChange[0] = 
			::FindFirstChangeNotification(strDir, bSubDir, FILE_NOTIFY_CHANGE_DIR_NAME);
		bNeedExecute = TRUE;
	}
	// �����ļ����Ƶĸı�		arhChange[1]
	if(((CButton*)GetDlgItem(IDC_FILENAME_CHANGE))->GetCheck())
	{
		m_arhChange[1] = 
			::FindFirstChangeNotification(strDir, bSubDir, FILE_NOTIFY_CHANGE_FILE_NAME);
		bNeedExecute = TRUE;
	}
	// �������Եĸı�		arhChange[2]
	if(((CButton*)GetDlgItem(IDC_ATTRIBUTE_CHANGE))->GetCheck())
	{
		m_arhChange[2] = 
			::FindFirstChangeNotification(strDir, bSubDir, FILE_NOTIFY_CHANGE_ATTRIBUTES);
		bNeedExecute = TRUE;
	}
	// �����ļ���С�ĸı�		arhChange[3]
	if(((CButton*)GetDlgItem(IDC_FILESIZE_CHANGE))->GetCheck())
	{
		m_arhChange[3] = 
			::FindFirstChangeNotification(strDir, bSubDir, FILE_NOTIFY_CHANGE_SIZE);
		bNeedExecute = TRUE;
	}
	// �������д��ʱ��ĸı�	arhChange[4]
	if(((CButton*)GetDlgItem(IDC_LASTWRITE_CHANGE))->GetCheck())
	{
		m_arhChange[4] = 
			::FindFirstChangeNotification(strDir, bSubDir, FILE_NOTIFY_CHANGE_LAST_WRITE);
		bNeedExecute = TRUE;
	}
	// ���Ӱ�ȫ���Եĸı�		arhChange[5]
	if(((CButton*)GetDlgItem(IDC_SECURITY_CHANGE))->GetCheck())
	{
		m_arhChange[5] = 
			::FindFirstChangeNotification(strDir, bSubDir, FILE_NOTIFY_CHANGE_SECURITY);
		bNeedExecute = TRUE;
	}

	if(!bNeedExecute)
	{
		MessageBox("��ѡ��һ���������ͣ�");
		return;
	}

	// ���������߳�
	AfxBeginThread(MonitorThread, this);
	// ���½���
	GetDlgItem(IDC_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOP)->EnableWindow(TRUE);
	m_bar.SetText(" ���ڼ���...", 0, 0);
}

void CMonitorDialog::OnStop()		// �û����ֹͣ��ť
{
	if(!m_bExit)
	{
		// �����˳���־
		m_bExit = TRUE;
		for(int i=0; i<6; i++)
		{
			if(m_arhChange[i] != m_hEvent)
				::FindCloseChangeNotification(m_arhChange[i]);
		}
	}

	GetDlgItem(IDC_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_STOP)->EnableWindow(FALSE);
	m_bar.SetText(" ����", 0, 0);
}

void CMonitorDialog::OnClear()		// �û������հ�ť
{
	GetDlgItem(IDC_EDITCHANGES)->SetWindowText("");
}

void CMonitorDialog::OnCancel()
{
	OnStop();
	CDialog::OnCancel();
}

void CMonitorDialog::AddStringToList(LPCTSTR lpszString)
{
	// ��"�ı�"����������ı�
	CString strEdit;
	GetDlgItem(IDC_EDITCHANGES)->GetWindowText(strEdit);
	strEdit += lpszString;
	GetDlgItem(IDC_EDITCHANGES)->SetWindowText(strEdit);
}

UINT MonitorThread(LPVOID lpParam)
{
	CMonitorDialog* pDlg = (CMonitorDialog*)lpParam;
	while(TRUE)
	{
		// �ڶ���ı�֪ͨ�¼��ϵȴ�
		DWORD nObjectWait = ::WaitForMultipleObjects(
					6, pDlg->m_arhChange, FALSE, INFINITE);

		if(pDlg->m_bExit)	// �û�Ҫ���˳�
			break;
		// ���Ҵ�ʹ�ȴ��������صľ����֪ͨ�û�
		int nIndex = nObjectWait - WAIT_OBJECT_0;
		switch(nIndex)
		{
		case 0:
			pDlg->AddStringToList(" Directory name changed \r\n");
			break;
		case 1:
			pDlg->AddStringToList(" File name changed \r\n");
			break;
		case 2:
			pDlg->AddStringToList(" File attribute changed \r\n");
			break;
		case 3:
			pDlg->AddStringToList(" File size changed \r\n");
			break;
		case 4:
			pDlg->AddStringToList(" Last write changed \r\n");
			break;
		case 5:
			pDlg->AddStringToList(" Security changed \r\n");
			break;
		}
		// ��������
		::FindNextChangeNotification(pDlg->m_arhChange[nObjectWait]);
	}
	return 0;
}





/*

	InitSkinMagicLib(AfxGetInstanceHandle(), "MonitorDir", NULL, NULL);

	if(LoadSkinFile("Devior.smf"))
	{
		SetDialogSkin("Dialog");
	}

  */

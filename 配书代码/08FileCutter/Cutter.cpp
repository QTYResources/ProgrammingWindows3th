////////////////////////////////////////////////
// MainDir.cpp�ļ�

#include <afxdlgs.h> // Ϊ��ʹ��CFileDialog��
#include "DirDialog.h"
#include "resource.h"
#include "Cutter.h"

CMyApp theApp;

BOOL CMyApp::InitInstance()
{
	CMainDialog dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	return FALSE;
}

CMainDialog::CMainDialog(CWnd* pParentWnd):CDialog(IDD_FILECUTTER_DIALOG, pParentWnd)
{
}

BEGIN_MESSAGE_MAP(CMainDialog, CDialog)
ON_BN_CLICKED(IDC_SOURCEBROWSER, OnSourceBrowser)	// ѡ��Դ�ļ��ġ�ѡ�񡱰�ť
ON_BN_CLICKED(IDC_DESTBROWSER, OnDestBrowser)		// ѡ��Ŀ���ļ��ġ�ѡ�񡱰�ť
ON_BN_CLICKED(IDC_START, OnStart)			// ��ʼ���ָ��ť
ON_BN_CLICKED(IDC_STOP, OnStop)				// ����ֹ���ָť
ON_BN_CLICKED(IDC_SELECTSPLIT, OnSelect)		// �ָѡ��ť
ON_BN_CLICKED(IDC_SELECTMERGE, OnSelect)		// �ϲ���ѡ��ť
// ������3��CFileCutter�෢������Ϣ
ON_MESSAGE(WM_CUTTERSTART, OnCutterStart)		
ON_MESSAGE(WM_CUTTERSTATUS, OnCutterStatus)
ON_MESSAGE(WM_CUTTERSTOP, OnCutterStop)
END_MESSAGE_MAP()

BOOL CMainDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(theApp.LoadIcon(IDI_MAIN), FALSE);

	// ����CFileCutter����
	m_pCutter = new CFileCutter(m_hWnd);

	// Ĭ��ѡ�зָ���
	((CButton*)GetDlgItem(IDC_SELECTSPLIT))->SetCheck(1);
	// ��ʼ����λѡ����Ͽ򡣿���������������������
	((CComboBox*)GetDlgItem(IDC_UNIT))->AddString("1");
	((CComboBox*)GetDlgItem(IDC_UNIT))->AddString("30");
	((CComboBox*)GetDlgItem(IDC_UNIT))->AddString("60");
	((CComboBox*)GetDlgItem(IDC_UNIT))->SetCurSel(0);

	// ���໯�������ؼ���Ҳ������m_Progress����ȡ�ý������ؼ��Ŀ���Ȩ
	m_Progress.SubclassWindow(*GetDlgItem(IDC_PROGRESS));

	UIControl();
	return TRUE;
}

void CMainDialog::UIControl()
{
	BOOL bIsWorking = m_pCutter->IsRunning();
	
	// ����ѡ��������3���ؼ���״̬
	GetDlgItem(IDC_SELECTSPLIT)->EnableWindow(!bIsWorking);
	GetDlgItem(IDC_SELECTMERGE)->EnableWindow(!bIsWorking);
	GetDlgItem(IDC_UNIT)->EnableWindow(!bIsWorking);
	// ���÷ָ��ֹ������ť��״̬
	GetDlgItem(IDC_START)->EnableWindow(!bIsWorking);
	GetDlgItem(IDC_STOP)->EnableWindow(bIsWorking);

	if(bIsWorking)
	{
		return;
	}
	
	// �����û���ѡ�����ò�ͬ���ı�
	BOOL bSplit = ((CButton*)GetDlgItem(IDC_SELECTSPLIT))->GetCheck();
	if(bSplit)	// ����ָ�	
	{
		GetDlgItem(IDC_START)->SetWindowText("�ָ�");
		GetDlgItem(IDC_SOURCETITLE)->SetWindowText("��ѡ��Ҫ�ָ���ļ���");
		GetDlgItem(IDC_DESTTITLE)->SetWindowText("��ѡ��ָ�󱣴浽���ļ��У�");
	
		GetDlgItem(IDC_UNIT)->EnableWindow(TRUE);
	}
	else		// ����ϲ�
	{
		GetDlgItem(IDC_START)->SetWindowText("�ϲ�");
		GetDlgItem(IDC_SOURCETITLE)->SetWindowText("��ѡ����ϲ��ļ����ļ��У�");
		GetDlgItem(IDC_DESTTITLE)->SetWindowText("��ѡ��ϲ��󱣴浽���ļ��У�");
		
		GetDlgItem(IDC_UNIT)->EnableWindow(FALSE);
	}

	// ��ʼ��״̬��Ϣ
	GetDlgItem(IDC_STATUSTEXT)->SetWindowText("    ״̬��ʾ��");
	m_Progress.SetPos(0);

}

void CMainDialog::OnCancel()
{
	// �Ƿ�����˳���
	BOOL bExit = TRUE;
	if(m_pCutter->IsRunning())
	{
		if(MessageBox("������δ��ɣ�ȷʵҪ�˳���", NULL, MB_YESNO) == IDNO)
		{
			bExit = FALSE;
		}
	}

	if(bExit)
	{
		delete m_pCutter;
		CDialog::OnCancel();
	}
}

void CMainDialog::OnSelect()
{
	UIControl();
}

void CMainDialog::OnSourceBrowser()
{
	BOOL bSplit = ((CButton*)GetDlgItem(IDC_SELECTSPLIT))->GetCheck();
	if(bSplit)	// ����ָ�
	{
		CFileDialog sourceFile(TRUE);
		// ��ʾѡ���ļ��Ի���
		if(sourceFile.DoModal() == IDOK)
		{
			GetDlgItem(IDC_EDITSOURCE)->SetWindowText(sourceFile.GetPathName());
			
			// ����Ĭ��Ŀ¼
			// ���磬����û�ѡ���ļ���D:\cd\��ѡ����.iso������ô��D:\cd\��ѡ���������ᱻ����ΪĬ��Ŀ¼
			CString strDef = sourceFile.GetPathName();
			strDef = strDef.Left(strDef.ReverseFind('.'));
			GetDlgItem(IDC_EDITDEST)->SetWindowText(strDef);
		}
	}
	else		// ����ϲ�
	{
		CDirDialog sourceFolder;
		// ��ʾѡ��Ŀ¼�Ի���
		if(sourceFolder.DoBrowse(*this) == IDOK)
		{
			GetDlgItem(IDC_EDITSOURCE)->SetWindowText(sourceFolder.GetPath());

			// ����Ĭ��Ŀ¼
			// ���磬����û�ѡ��Ŀ¼��D:\cd������ô��D:\cd\cd�����ᱻ����ΪĬ��Ŀ¼
			CString strDef = sourceFolder.GetPath();
			strDef.TrimRight('\\');
			strDef = strDef + '\\' + strDef.Mid(strDef.ReverseFind('\\') + 1);
			// ��ֹ�û�ѡ���Ŀ¼
			strDef.TrimRight(':');
			GetDlgItem(IDC_EDITDEST)->SetWindowText(strDef);
		}
	}
}

void CMainDialog::OnDestBrowser()
{
	CDirDialog destFolder;
	// ��ʾѡ��Ŀ¼�Ի���
	if(destFolder.DoBrowse(*this) == IDOK)
	{
		GetDlgItem(IDC_EDITDEST)->SetWindowText(destFolder.GetPath());
	}
}

void CMainDialog::OnStart()
{
	CString strSource, strDest;

	// �������
	GetDlgItem(IDC_EDITSOURCE)->GetWindowText(strSource);
	GetDlgItem(IDC_EDITDEST)->GetWindowText(strDest);
	if(strSource.IsEmpty() || strDest.IsEmpty())
	{
		MessageBox("�ļ���·�����Ʋ���Ϊ��");
		return;
	}

	BOOL bSplit = ((CButton*)GetDlgItem(IDC_SELECTSPLIT))->GetCheck();
	if(bSplit)	// ����ָ�
	{
		CString str;
		GetDlgItem(IDC_UNIT)->GetWindowText(str);
		m_pCutter->StartSplit(strDest, strSource, atoi(str)*1024*1024);
	}
	else		// ����ϲ�
	{
		m_pCutter->StartMerge(strDest, strSource);
	}
}

void CMainDialog::OnStop()
{
	m_pCutter->SuspendCutter();
	if(MessageBox("ȷʵҪ��ֹ��", NULL, MB_YESNO) == IDYES)
	{
		m_pCutter->StopCutter();
	}
	else
	{
		m_pCutter->ResumeCutter();
	}
}
 
////////////////////////////////////////////////////
// ����Ĵ��봦��CFileCutter�෢������Ϣ


long CMainDialog::OnCutterStart(WPARAM wParam, LPARAM)		// WM_CUTTERSTART ��ʼ����
{
	// ���ý�������Χ
	int nTotalFiles = wParam;	// ���ļ�����
	m_Progress.SetRange(0, nTotalFiles);
	
	UIControl();
	return 0;
}

long CMainDialog::OnCutterStatus(WPARAM wParam, LPARAM lParam)	// WM_CUTTERSTATUS ��������
{
	// ���ý���������
	int nCompleted = (int)lParam;
	m_Progress.SetPos(nCompleted);
	// ��ʾ״̬
	CString s;
	s.Format(" ���%d���ļ�", nCompleted);
	GetDlgItem(IDC_STATUSTEXT)->SetWindowText(s);
	return 0;
}

long CMainDialog::OnCutterStop(WPARAM wParam, LPARAM lParam)	// WM_CUTTERSTOP ֹͣ����
{
	int nErrorCode = wParam;
	switch(nErrorCode)
	{
	case CFileCutter::exitSuccess:
		MessageBox("�����ɹ����", "�ɹ�");
		break;
	case CFileCutter::exitSourceErr:
		MessageBox("Դ�ļ�����", "ʧ��");
		break;
	case CFileCutter::exitDestErr:
		MessageBox("Ŀ���ļ�����", "ʧ��");
		break;
	case CFileCutter::exitUserForce:
		MessageBox("�û���ֹ", "ʧ��");
		break;
	}
	UIControl();
	return 0;
}



//////////////////////////////////////
// MemRepair.cpp�ļ�

#include "resource.h"
#include "MemRepair.h"
#include "afxdlgs.h"
#include <tlhelp32.h>


CMyApp theApp;

BOOL CMyApp::InitInstance()
{
	CMainDialog dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	return 1;
}


CMainDialog::CMainDialog(CWnd* pWndParent) : CDialog(IDD_MAIN, pWndParent)
{
}

BEGIN_MESSAGE_MAP(CMainDialog, CDialog)
ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
ON_BN_CLICKED(IDC_SEARCH, OnSearch)
ON_BN_CLICKED(IDC_RESTART, OnRestart)
ON_BN_CLICKED(IDC_MODIFY, OnModify)
ON_LBN_SELCHANGE(IDC_ADDRLIST, OnSelectChange)
END_MESSAGE_MAP()

BOOL CMainDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ����ͼ��
	SetIcon(theApp.LoadIcon(IDI_MAIN), FALSE);

	m_comboList.SubclassWindow(::GetDlgItem(m_hWnd, IDC_PROLIST));
	m_btnUpdate.SubclassWindow(*GetDlgItem(IDC_UPDATE));
	m_listAddr.SubclassWindow(*GetDlgItem(IDC_ADDRLIST));

	m_pFinder = NULL;

	// ����״̬���������������ԣ�CStatusBarCtrl���װ�˶�״̬���ؼ��Ĳ�����
	m_bar.Create(WS_CHILD|WS_VISIBLE|SBS_SIZEGRIP, CRect(0, 0, 0, 0), this, 10);
	m_bar.SetBkColor(RGB(0xa6, 0xca, 0xf0));		// ����ɫ
	m_bar.SetText(" ����", 0, 0);
	
	// ���½����б�
	OnUpdate();
	// ���½���
	UIControl();

	return FALSE;
}

void CMainDialog::OnSearch()
{
	// �������Ҷ���
	if(m_pFinder == NULL)
	{
		DWORD dwId = m_comboList.GetItemData(m_comboList.GetCurSel());
		m_pFinder = new CMsgMemFinder(dwId);
		UIControl();
	}

	if(m_pFinder->IsWorking())
		return;

	CString sText;
	GetDlgItem(IDC_EDITSEARCH)->GetWindowText(sText);
	if(!sText.IsEmpty())
	{
		DWORD dwSearch = atoi(sText);
		if(m_pFinder->IsFirst())
		{
			m_bar.SetText(" ���������������ĵȴ�...", 0, 0);	
			
			// ��ʱ�����
			m_pFinder->FindFirst(dwSearch);
		}
		else
		{
			m_pFinder->FindNext(dwSearch);	
		}
		sText.Format(" ������%d�����", m_pFinder->GetListCount());
		m_bar.SetText(sText, 0, 0);
	}
	else
	{
		if(m_pFinder->IsValid())
		{
			m_bar.SetText(" ��Ŀ����̳ɹ���", 0, 0);
		}
		else
		{
			m_bar.SetText(" ��Ŀ�����ʧ�ܣ�", 0, 0);
		}
	}

	UIControl();
}

void CMainDialog::OnRestart()
{
	if(m_pFinder != NULL)
	{
		if(m_pFinder->IsWorking())
			return;

		delete m_pFinder;
		m_pFinder = NULL;
	}

	m_bar.SetText(" ����", 0, 0);
	UIControl();
}

void CMainDialog::UIControl()
{
	m_comboList.EnableWindow(m_pFinder == NULL);
	m_btnUpdate.EnableWindow(m_pFinder == NULL);
	GetDlgItem(IDC_MODIFY)->EnableWindow(m_pFinder != NULL);
	
	// �г��������
	if(m_pFinder != NULL)
	{
		
		CString s;
		m_listAddr.ResetContent();
		for(int i=0; i<m_pFinder->GetListCount(); i++)
		{
			s.Format("%08lX", (*m_pFinder)[i]);
			m_listAddr.AddString(s);
		}
	}
}


void CMainDialog::OnUpdate()
{
	// ɾ�����е���
	m_comboList.ResetContent();


	int nItem = 0;	// �����

	PROCESSENTRY32 pe32 = { sizeof(PROCESSENTRY32) }; 
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 
	if(hProcessSnap == INVALID_HANDLE_VALUE) 
		return; 
	if(Process32First(hProcessSnap, &pe32)) 
	{ 
		do 
		{ 	
			CString szText;
			szText.Format("%s  (%d)", pe32.szExeFile, pe32.th32ProcessID);

			m_comboList.InsertString(nItem, szText);
			m_comboList.SetItemData(nItem, pe32.th32ProcessID);

			nItem++;
		} 
		while(Process32Next(hProcessSnap, &pe32)); 
	}
	::CloseHandle(hProcessSnap);

	m_comboList.SetCurSel(nItem-1);
}

void CMainDialog::OnModify()
{
	CString sText;
	DWORD dwValue;
	GetDlgItem(IDC_EDITMODIFY)->GetWindowText(sText);

	if(sText.IsEmpty())
	{
		MessageBox("������һ������Ҫ��ֵ��");
		return;
	}
	dwValue = (DWORD)atoi(sText);


	GetDlgItem(IDC_SELADDR)->GetWindowText(sText);
	if(sText.IsEmpty())
	{
		MessageBox("��������Ҫ���ĸ���ַ��ֵ��Ϊ��%s", sText);
		return;
	}
	DWORD dwAddr;
	sscanf(sText, "%x", &dwAddr);

	if(m_pFinder != NULL)
	{
		if(m_pFinder->WriteMemory(dwAddr, dwValue))
			m_bar.SetText("�޸ĳɹ���", 0, 0);
		else
			m_bar.SetText("�޸�ʧ�ܣ�", 0, 0);
	}
}

void CMainDialog::OnSelectChange()
{
	CString sText;
	int nSel = m_listAddr.GetCurSel();
	m_listAddr.GetText(nSel, sText);

	GetDlgItem(IDC_SELADDR)->SetWindowText(sText);
}

void CMainDialog::OnCancel()
{
	OnRestart();
	CDialog::OnCancel();
}
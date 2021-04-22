////////////////////////////////////////////
// EnumProcessDlg.cpp�ļ�

#include "EnumProcessDlg.h"
#include "resource.h"
#include <tlhelp32.h>


CEnumProcessDlg::CEnumProcessDlg(CWnd* pParentWnd):CDialog(IDD_ENUMDIALOG, pParentWnd)
{
}

BOOL CEnumProcessDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ȡ���б���ͼ�Ӵ��ڵĿ���Ȩ
	m_listPro.SubclassWindow(::GetDlgItem(m_hWnd, IDC_PROLIST));

	// ��������
	m_listPro.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_listPro.InsertColumn(0, "����", LVCFMT_LEFT, 120);
	m_listPro.InsertColumn(1, "Pid", LVCFMT_LEFT, 70);

	// ���½����б�
	UpdateProcess();

	return FALSE;
}

void CEnumProcessDlg::OnOK()
{
	// ȡ�õ�ǰѡ����Ŀ������
	int nCur = m_listPro.GetNextItem(-1, LVNI_SELECTED);
	if(nCur == -1)
	{
		MessageBox("��ѡ��Ҫ�򿪵Ľ���");
	}
	else
	{
		// �����߳�ID
		m_dwThreadId = (DWORD)m_listPro.GetItemData(nCur);
		// ���ý���ID
		char sz[32] = "";
		m_listPro.GetItemText(nCur, 1, sz, 31);
		m_dwProcessId = (DWORD)atoi(sz);
		// �رնԻ��򣬷���IDOK
		CDialog::OnOK();
	}
}

void CEnumProcessDlg::UpdateProcess()
{
	// ɾ�����е���
	m_listPro.DeleteAllItems();

	int nItem = 0;	// �����

	PROCESSENTRY32 pe32 = { sizeof(PROCESSENTRY32) }; 
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 
	if(hProcessSnap == INVALID_HANDLE_VALUE) 
		return; 
	if(::Process32First(hProcessSnap, &pe32)) 
	{ 
		do 
		{ 
			// ��������
			m_listPro.InsertItem(nItem, pe32.szExeFile, 0);
	
			// ȡ�ý���ID�����ô�����ı�
			char szID[56];
			wsprintf(szID, "%u", pe32.th32ProcessID);
			m_listPro.SetItemText(nItem, 1, szID);

			// ����Ĵ��뽫����ϵͳ�ڵ������̣߳��Ա��ҵ��˽��̵����߳�
			HANDLE hThreadSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
			THREADENTRY32 te32 = { sizeof(te32) };
			if(::Thread32First(hThreadSnap, &te32))
			{
				do
				{
					if(te32.th32OwnerProcessID == pe32.th32ProcessID)
					{
						// �ҵ����߳�ID�ţ�����������ǰ��
						m_listPro.SetItemData(nItem, te32.th32ThreadID);
						break;
					}
				}
				while(::Thread32Next(hThreadSnap, &te32));
			}
			::CloseHandle(hThreadSnap);
	
			nItem++;
		} 
		while(::Process32Next(hProcessSnap, &pe32)); 
	}
	::CloseHandle(hProcessSnap);
}
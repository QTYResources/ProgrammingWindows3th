////////////////////////////////////////////
// EnumProcessDlg.cpp文件

#include "EnumProcessDlg.h"
#include "resource.h"
#include <tlhelp32.h>


CEnumProcessDlg::CEnumProcessDlg(CWnd* pParentWnd):CDialog(IDD_ENUMDIALOG, pParentWnd)
{
}

BOOL CEnumProcessDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 取得列表视图子窗口的控制权
	m_listPro.SubclassWindow(::GetDlgItem(m_hWnd, IDC_PROLIST));

	// 设置属性
	m_listPro.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_listPro.InsertColumn(0, "进程", LVCFMT_LEFT, 120);
	m_listPro.InsertColumn(1, "Pid", LVCFMT_LEFT, 70);

	// 更新进程列表
	UpdateProcess();

	return FALSE;
}

void CEnumProcessDlg::OnOK()
{
	// 取得当前选中项目的索引
	int nCur = m_listPro.GetNextItem(-1, LVNI_SELECTED);
	if(nCur == -1)
	{
		MessageBox("请选择要打开的进程");
	}
	else
	{
		// 设置线程ID
		m_dwThreadId = (DWORD)m_listPro.GetItemData(nCur);
		// 设置进程ID
		char sz[32] = "";
		m_listPro.GetItemText(nCur, 1, sz, 31);
		m_dwProcessId = (DWORD)atoi(sz);
		// 关闭对话框，返回IDOK
		CDialog::OnOK();
	}
}

void CEnumProcessDlg::UpdateProcess()
{
	// 删除所有的项
	m_listPro.DeleteAllItems();

	int nItem = 0;	// 项计数

	PROCESSENTRY32 pe32 = { sizeof(PROCESSENTRY32) }; 
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 
	if(hProcessSnap == INVALID_HANDLE_VALUE) 
		return; 
	if(::Process32First(hProcessSnap, &pe32)) 
	{ 
		do 
		{ 
			// 插入新项
			m_listPro.InsertItem(nItem, pe32.szExeFile, 0);
	
			// 取得进程ID，设置此项的文本
			char szID[56];
			wsprintf(szID, "%u", pe32.th32ProcessID);
			m_listPro.SetItemText(nItem, 1, szID);

			// 下面的代码将遍历系统内的所有线程，以便找到此进程的主线程
			HANDLE hThreadSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
			THREADENTRY32 te32 = { sizeof(te32) };
			if(::Thread32First(hThreadSnap, &te32))
			{
				do
				{
					if(te32.th32OwnerProcessID == pe32.th32ProcessID)
					{
						// 找到主线程ID号，关联它到当前项
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
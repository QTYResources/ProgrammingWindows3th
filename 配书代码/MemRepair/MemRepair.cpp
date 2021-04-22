//////////////////////////////////////
// MemRepair.cpp文件

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

	// 设置图标
	SetIcon(theApp.LoadIcon(IDI_MAIN), FALSE);

	m_comboList.SubclassWindow(::GetDlgItem(m_hWnd, IDC_PROLIST));
	m_btnUpdate.SubclassWindow(*GetDlgItem(IDC_UPDATE));
	m_listAddr.SubclassWindow(*GetDlgItem(IDC_ADDRLIST));

	m_pFinder = NULL;

	// 创建状态栏，设置它的属性（CStatusBarCtrl类封装了对状态栏控件的操作）
	m_bar.Create(WS_CHILD|WS_VISIBLE|SBS_SIZEGRIP, CRect(0, 0, 0, 0), this, 10);
	m_bar.SetBkColor(RGB(0xa6, 0xca, 0xf0));		// 背景色
	m_bar.SetText(" 空闲", 0, 0);
	
	// 更新进程列表
	OnUpdate();
	// 更新界面
	UIControl();

	return FALSE;
}

void CMainDialog::OnSearch()
{
	// 创建查找对象
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
			m_bar.SetText(" 正在搜索，请耐心等待...", 0, 0);	
			
			// 长时间操作
			m_pFinder->FindFirst(dwSearch);
		}
		else
		{
			m_pFinder->FindNext(dwSearch);	
		}
		sText.Format(" 搜索到%d个结果", m_pFinder->GetListCount());
		m_bar.SetText(sText, 0, 0);
	}
	else
	{
		if(m_pFinder->IsValid())
		{
			m_bar.SetText(" 打开目标进程成功！", 0, 0);
		}
		else
		{
			m_bar.SetText(" 打开目标进程失败！", 0, 0);
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

	m_bar.SetText(" 空闲", 0, 0);
	UIControl();
}

void CMainDialog::UIControl()
{
	m_comboList.EnableWindow(m_pFinder == NULL);
	m_btnUpdate.EnableWindow(m_pFinder == NULL);
	GetDlgItem(IDC_MODIFY)->EnableWindow(m_pFinder != NULL);
	
	// 列出搜索结果
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
	// 删除所有的项
	m_comboList.ResetContent();


	int nItem = 0;	// 项计数

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
		MessageBox("请输入一个你想要的值！");
		return;
	}
	dwValue = (DWORD)atoi(sText);


	GetDlgItem(IDC_SELADDR)->GetWindowText(sText);
	if(sText.IsEmpty())
	{
		MessageBox("请输入你要将哪个地址的值改为：%s", sText);
		return;
	}
	DWORD dwAddr;
	sscanf(sText, "%x", &dwAddr);

	if(m_pFinder != NULL)
	{
		if(m_pFinder->WriteMemory(dwAddr, dwValue))
			m_bar.SetText("修改成功！", 0, 0);
		else
			m_bar.SetText("修改失败！", 0, 0);
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
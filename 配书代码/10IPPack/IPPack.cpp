/////////////////////////////////////////////
// IPPack.cpp文件


#include "IPPack.h"
#include "resource.h"
#include "EnumProcessDlg.h"

#include "IPPackLib.h"
#include "ShareMemory.h"

CMyApp theApp;


// dll导出函数
BOOL SetHook(BOOL bInstall, DWORD dwThreadId = 0, HWND hWndCaller = NULL)
{
	// 定义导出函数的类型
	typedef (WINAPI *PFNSETHOOK)(BOOL, DWORD, HWND);
	// 导出函数的DLL文件名。调试时可设置为../10IPPackLib/debug/10IPPackLib.dll
	char szDll[] = "10IPPackLib.dll";

	// 加载DLL模块
	BOOL bNeedFree = FALSE;
	HMODULE hModule = ::GetModuleHandle(szDll);
	if(hModule == NULL)
	{
		hModule = ::LoadLibrary(szDll);
		bNeedFree = TRUE;
	}

	// 获取SetHook函数的地址
	PFNSETHOOK mSetHook = (PFNSETHOOK)::GetProcAddress(hModule, "SetHook");
	if(mSetHook == NULL) // 文件不正确?
	{
		if(bNeedFree)
			::FreeLibrary(hModule);
		return FALSE;
	}

	// 调用SetHook函数
	BOOL bRet = mSetHook(bInstall, dwThreadId, hWndCaller);

	// 如果卸载，释放上面加载的模块
	if(!bInstall)
		::FreeLibrary(hModule);

	return bRet;
}

BOOL CMyApp::InitInstance()
{
	CMainDialog dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	return FALSE;	// 返回FALSE阻止程序进入消息循环
}

CMainDialog::CMainDialog(CWnd* pParentWnd):CDialog(IDD_MAINDIALOG, pParentWnd)
{
}

BEGIN_MESSAGE_MAP(CMainDialog, CDialog)
// 按钮的单击事件
ON_BN_CLICKED(IDC_TARGET, OnTarget)
ON_BN_CLICKED(IDC_PAUSE, OnPause)
ON_BN_CLICKED(IDC_CLEAR, OnClear)
ON_BN_CLICKED(IDC_TOPMOST, OnTopMost)
ON_BN_CLICKED(IDC_CLOSE, OnClose)
// 列表视图的单击和删除事件
ON_NOTIFY(NM_CLICK, IDC_LISTDATA, OnClickListData)
ON_NOTIFY(LVN_DELETEITEM, IDC_LISTDATA, OnDeleteItemList)  
// 两个自定义消息
ON_MESSAGE(HM_RECEIVE, OnReceive)
ON_MESSAGE(HM_SEND, OnSend)
END_MESSAGE_MAP()

BOOL CMainDialog::OnInitDialog()
{
	// 让父类进行内部初始化
	CDialog::OnInitDialog();

	// 设置图标
	SetIcon(theApp.LoadIcon(IDI_MAIN), FALSE);

	// 创建状态栏，设置它的属性（CStatusBarCtrl类封装了对状态栏控件的操作）
	m_bar.Create(WS_CHILD|WS_VISIBLE|SBS_SIZEGRIP, CRect(0, 0, 0, 0), this, 101);
	m_bar.SetBkColor(RGB(0xa6, 0xca, 0xf0));		// 背景色
	int arWidth[] = { 250, -1 };
	m_bar.SetParts(2, arWidth);				// 分栏
	m_bar.SetText(" Windows程序设计进阶之路！", 1, 0);	// 第二个栏的文本

	// 取得列表视图窗口的控制权，设置它的分栏
	m_listData.SubclassWindow(::GetDlgItem(m_hWnd, IDC_LISTDATA));
	m_listData.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listData.InsertColumn(0, "编号", LVCFMT_LEFT, 38);
	m_listData.InsertColumn(2, "类型", LVCFMT_LEFT, 80);
	m_listData.InsertColumn(3, "数据", LVCFMT_LEFT, 180);

	// 注册热键CTRL＋F9。当用户按此热键时，主窗口显示或者隐藏
	if(!::RegisterHotKey(m_hWnd, 11, MOD_CONTROL, VK_F9))
	{
		MessageBox("注册热键CTRL＋F9失败！");
	}

	// 初始化状态数据
	InitData();
	// 更新用户界面
	UIControl();

	return TRUE;
}

void CMainDialog::InitData()
{
	m_bOpen = FALSE;	// 没有打开
	m_bPause = FALSE;	// 没有暂停
}

void CMainDialog::UIControl()
{
	if(m_bOpen)
	{
		GetDlgItem(IDC_PAUSE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CLOSE)->EnableWindow(TRUE);

		if(!m_bPause)
			m_bar.SetText(" 正在截拦网络数据...", 0, 0);
		else
			m_bar.SetText(" 暂停截拦网络数据", 0, 0);
	}
	else
	{
		GetDlgItem(IDC_PAUSE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CLOSE)->EnableWindow(FALSE);

		m_bar.SetText(" 您还没有打开目标程序", 0, 0);
	}

	if(m_bPause)
	{
		GetDlgItem(IDC_PAUSE)->SetWindowText("恢复");
	}
	else
	{
		GetDlgItem(IDC_PAUSE)->SetWindowText("暂停");
	}
}


void CMainDialog::OnTarget()
{
	CEnumProcessDlg dlg(this);
	// 弹出选择进程对话框
	if(dlg.DoModal() == IDOK)
	{
		// 如果为其它进程安装了钩子，先卸载
		if(m_bOpen)	
		{
			SetHook(FALSE);
			InitData();
		}
		// 为用户选择进程的主线程安装钩子
		if(SetHook(TRUE, dlg.m_dwThreadId, m_hWnd))
		{
			m_bOpen = TRUE;
			m_bPause = FALSE;
		}
		else
		{
			MessageBox(" 安装钩子出错！");
		}
	}
	// 更新用户界面
	UIControl();
}

long CMainDialog::OnReceive(WPARAM wParam, LPARAM lParam)
{
	CShareMemory sm("IPPACK_RECEIVE");
	CMessageData *pData = (CMessageData*)sm.GetBuffer();

	// 如果暂停截获，则返回
	if(m_bPause)
		return 0;

	// 可以在这里过滤数据，比如
	// DoCopyFilter(pData);

	// 如果没有关闭发送，则显示截获到的数据
	BOOL bNoSend = IsDlgButtonChecked(IDC_CLOSERECV);
	if(!bNoSend)
	{
		// 得到已有的项目数量，以便添加一个新项
		int nIndex = m_listData.GetItemCount();
		if(nIndex == 100)
			return 0; 
		
			// 添加新项
		char sz[32] = "";
		// 转化整型数据为字符串。第三个参数10指的是以十进制方式显示的字符串
		itoa(nIndex+1, sz, 10);
		m_listData.InsertItem(nIndex, sz, 0);
		
			// 设置新项文本
		// 设置要显示的数据（只显示一部分即可，从共享内存中取出）
		char szText[128] = "";
		int nCopy = min(pData->nDataLength, 127);
		strncpy(szText, pData->data(), nCopy);
		// 设置文本
		m_listData.SetItemText(nIndex, 1, "接受");
		m_listData.SetItemText(nIndex, 2, szText);
		
			// 保存共享内存中的数据到进程堆中，以便用户查询
		// 取得数据长度，最大保存500个字节（可以根据需要自己设置）
		int nTotal = sizeof(CMessageData) + pData->nDataLength;
		nTotal = min(nTotal, 500);
		// 在进程堆中申请空间，复制共享内存中的数据
		BYTE* pByte = new BYTE[nTotal];
		memcpy(pByte, pData, nTotal);
		// 将首地址关联到新项中
		m_listData.SetItemData(nIndex, (DWORD)pByte);
	}
	
	return 0;
}

long CMainDialog::OnSend(WPARAM wParam, LPARAM lParam)
{
	CShareMemory sm("IPPACK_SEND");
	CMessageData *pData = (CMessageData*)sm.GetBuffer();

	if(m_bPause)
		return 0;

	// ... 可以在这里过滤数据

	BOOL bNoSend = IsDlgButtonChecked(IDC_CLOSESEND);
	if(!bNoSend)
	{
		int nIndex = m_listData.GetItemCount();
		if(nIndex == 100)
			return 0; 
		
			// 添加新项
		char sz[32] = "";
		itoa(nIndex+1, sz, 10);
		m_listData.InsertItem(nIndex, sz, 0);
		
			// 设置新项文本
		char szText[128] = "";
		int nCopy = min(pData->nDataLength, 127);
		strncpy(szText, pData->data(), nCopy);
		m_listData.SetItemText(nIndex, 1, "发送");
		m_listData.SetItemText(nIndex, 2, szText);
		
		
			// 保存共享内存中的数据到进程堆中，以便用户查询
		int nTotal = sizeof(CMessageData) + pData->nDataLength;
		nTotal = min(nTotal, 500);
		BYTE* pByte = new BYTE[nTotal];
		memcpy(pByte, pData, nTotal);
		m_listData.SetItemData(nIndex, (DWORD)pByte);
	}
	return 0;
}

void CMainDialog::OnClickListData(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	// pNMListView->iItem的值为用户单击项目的索引
	int nIndex = pNMListView->iItem;	
	if (nIndex >= 0)
	{
			// 以16进制的形式显示数据到编辑框
		CMessageData* pData = (CMessageData*)m_listData.GetItemData(nIndex);

		// 为字符串申请空间
		// 一个字节要占用3个字符（2个16进制数和1个空格），如“16 12 b2 c7”。所以要乘以3
		char* pBuf = new char[pData->nDataLength*3 + 1];

		// 转化截获的数据为16进制字符串的形式
		char* pTemp = pBuf;
		char* psz = pData->data();
		for(int i=0; i<pData->nDataLength; i++, psz++)
		{
			wsprintf(pTemp, "%02x ", (BYTE)(*psz));
			pTemp += 3;
		}
		*pTemp = '\0';
	
		// 显示数据到编辑框中
		GetDlgItem(IDC_SPECIFICDATA)->SetWindowText(pBuf);

		// 释放内存空间
		delete[] pBuf;
	}

	*pResult = 0;
}

void CMainDialog::OnClear()
{
	// 删除列表试图中的所有项，清空编辑框
	m_listData.DeleteAllItems();
	GetDlgItem(IDC_SPECIFICDATA)->SetWindowText("");
}

void CMainDialog::OnDeleteItemList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	// 取得要删除项目的关联数据，也可以用下面的代码
	// 	int nIndex = pNMListView->iItem;
	//	pByte = (PBYTE)m_listData.GetItemData(nIndex);
	BYTE* pByte = (PBYTE)pNMListView->lParam;

	// 释放内存空间
	delete[] pByte;

	*pResult = 0;
}


void CMainDialog::OnCancel()
{
	// 确保钩子被卸载
	if(m_bOpen)
		SetHook(FALSE);
	CDialog::OnCancel();
}

void CMainDialog::OnClose()	
{
	// 用户单击关闭按钮时本函数被调用
	// 重置所有状态
	if(m_bOpen)
	{
		SetHook(FALSE);
		InitData();
		OnClear();
	}
	UIControl();
}

void CMainDialog::OnPause()
{
	m_bPause = !m_bPause;
	UIControl();
}

void CMainDialog::OnTopMost()
{
	BOOL bTopMost = ::IsDlgButtonChecked(m_hWnd, IDC_TOPMOST);
	if(bTopMost)
	{
		::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, 
					SWP_NOMOVE|SWP_NOSIZE|SWP_NOREDRAW);
	}
	else
	{
		::SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, 
					SWP_NOMOVE|SWP_NOSIZE|SWP_NOREDRAW);
	}
}

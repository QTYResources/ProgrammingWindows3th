////////////////////////////////////////////////
// MonitorDir.cpp文件

#include "resource.h"
#include "MonitorDir.h"
#include "DirDialog.h"

#include "SkinMagicLib.h"
// 注意，如果MFC是动态链接到工程中的，则应该选择SkinMagicLibMD6Trial.lib库
#pragma comment(lib, "SkinMagicLibMT6Trial")

CMyApp theApp;

BOOL CMyApp::InitInstance()
{
	//===SkinMagic===

	// 初始化SkinMagic库
	VERIFY(InitSkinMagicLib(AfxGetInstanceHandle(), "MonitorDir", NULL, NULL));
	// 从资源中加载皮肤文件。也可以用代码“LoadSkinFile("corona.smf")”直接从文件中加载
	if(LoadSkinFromResource(AfxGetInstanceHandle(), (LPCTSTR)IDR_SKINMAGIC1, "SKINMAGIC"))
	{
		// 设置对话框默认皮肤
		SetDialogSkin("Dialog"); 
	}

	//===SkinMagic===

	CMonitorDialog dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	return FALSE;	// 返回FALSE阻止程序进入消息循环
}

int CMyApp::ExitInstance()
{
	//===SkinMagic===

	// 释放SkinMagic库申请的内存
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
	// 让父类进行内部初始化
	CDialog::OnInitDialog();

	// 设置图标
	SetIcon(theApp.LoadIcon(IDI_MAIN), FALSE);

	// 创建状态栏，设置它的属性（CStatusBarCtrl类封装了对状态栏控件的操作）
	m_bar.Create(WS_CHILD|WS_VISIBLE|SBS_SIZEGRIP, CRect(0, 0, 0, 0), this, 101);
	m_bar.SetBkColor(RGB(0xa6, 0xca, 0xf0));		// 背景色
	int arWidth[] = { 250, -1 };
	m_bar.SetParts(2, arWidth);				// 分栏
	m_bar.SetText(" Windows程序设计入门到深入！", 1, 0);	// 第二个栏的文本
	m_bar.SetText(" 空闲", 0, 0);				// 第一个栏的文本

	// 无效停止按钮
	GetDlgItem(IDC_STOP)->EnableWindow(FALSE);
	// 设置各个复选框为选中状态
	((CButton*)GetDlgItem(IDC_SUBDIR))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_FILENAME_CHANGE))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_FILESIZE_CHANGE))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_DIRNAME_CHANGE))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_LASTWRITE_CHANGE))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_ATTRIBUTE_CHANGE))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_SECURITY_CHANGE))->SetCheck(1);

	return TRUE;
}

void CMonitorDialog::OnBrowser()	// 用户点击浏览按钮
{
	// 弹出选择目录对话框
	CDirDialog dir;
	if(dir.DoBrowse(m_hWnd, "请选择您要监视的目录："))
	{
		GetDlgItem(IDC_TARGETDIR)->SetWindowText(dir.GetPath());
	}
}

void CMonitorDialog::OnStart()		// 用户点击开始按钮
{
	CString strDir;
	// 取得目录名称
	GetDlgItem(IDC_TARGETDIR)->GetWindowText(strDir);
	if(strDir.IsEmpty())
	{
		MessageBox("请选择一个要监视的目录！");
		return;
	}

	
	// 用事件对象句柄初始化句柄数组
	for(int i=0; i<6; i++)
		m_arhChange[i] = m_hEvent;
	m_bExit = FALSE;

	// 是否要监视子目录
	BOOL bSubDir = ((CButton*)GetDlgItem(IDC_SUBDIR))->GetCheck();
	BOOL bNeedExecute = FALSE;

	// 监视目录名称的改变		arhChange[0]
	if(((CButton*)GetDlgItem(IDC_DIRNAME_CHANGE))->GetCheck())
	{
		m_arhChange[0] = 
			::FindFirstChangeNotification(strDir, bSubDir, FILE_NOTIFY_CHANGE_DIR_NAME);
		bNeedExecute = TRUE;
	}
	// 监视文件名称的改变		arhChange[1]
	if(((CButton*)GetDlgItem(IDC_FILENAME_CHANGE))->GetCheck())
	{
		m_arhChange[1] = 
			::FindFirstChangeNotification(strDir, bSubDir, FILE_NOTIFY_CHANGE_FILE_NAME);
		bNeedExecute = TRUE;
	}
	// 监视属性的改变		arhChange[2]
	if(((CButton*)GetDlgItem(IDC_ATTRIBUTE_CHANGE))->GetCheck())
	{
		m_arhChange[2] = 
			::FindFirstChangeNotification(strDir, bSubDir, FILE_NOTIFY_CHANGE_ATTRIBUTES);
		bNeedExecute = TRUE;
	}
	// 监视文件大小的改变		arhChange[3]
	if(((CButton*)GetDlgItem(IDC_FILESIZE_CHANGE))->GetCheck())
	{
		m_arhChange[3] = 
			::FindFirstChangeNotification(strDir, bSubDir, FILE_NOTIFY_CHANGE_SIZE);
		bNeedExecute = TRUE;
	}
	// 监视最后写入时间的改变	arhChange[4]
	if(((CButton*)GetDlgItem(IDC_LASTWRITE_CHANGE))->GetCheck())
	{
		m_arhChange[4] = 
			::FindFirstChangeNotification(strDir, bSubDir, FILE_NOTIFY_CHANGE_LAST_WRITE);
		bNeedExecute = TRUE;
	}
	// 监视安全属性的改变		arhChange[5]
	if(((CButton*)GetDlgItem(IDC_SECURITY_CHANGE))->GetCheck())
	{
		m_arhChange[5] = 
			::FindFirstChangeNotification(strDir, bSubDir, FILE_NOTIFY_CHANGE_SECURITY);
		bNeedExecute = TRUE;
	}

	if(!bNeedExecute)
	{
		MessageBox("请选择一个监视类型！");
		return;
	}

	// 启动监视线程
	AfxBeginThread(MonitorThread, this);
	// 更新界面
	GetDlgItem(IDC_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOP)->EnableWindow(TRUE);
	m_bar.SetText(" 正在监视...", 0, 0);
}

void CMonitorDialog::OnStop()		// 用户点击停止按钮
{
	if(!m_bExit)
	{
		// 设置退出标志
		m_bExit = TRUE;
		for(int i=0; i<6; i++)
		{
			if(m_arhChange[i] != m_hEvent)
				::FindCloseChangeNotification(m_arhChange[i]);
		}
	}

	GetDlgItem(IDC_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_STOP)->EnableWindow(FALSE);
	m_bar.SetText(" 空闲", 0, 0);
}

void CMonitorDialog::OnClear()		// 用户点击清空按钮
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
	// 向"改变"窗口中添加文本
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
		// 在多个改变通知事件上等待
		DWORD nObjectWait = ::WaitForMultipleObjects(
					6, pDlg->m_arhChange, FALSE, INFINITE);

		if(pDlg->m_bExit)	// 用户要求退出
			break;
		// 查找促使等待函数返回的句柄，通知用户
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
		// 继续监视
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

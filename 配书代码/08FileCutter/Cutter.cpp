////////////////////////////////////////////////
// MainDir.cpp文件

#include <afxdlgs.h> // 为了使用CFileDialog类
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
ON_BN_CLICKED(IDC_SOURCEBROWSER, OnSourceBrowser)	// 选择源文件的“选择”按钮
ON_BN_CLICKED(IDC_DESTBROWSER, OnDestBrowser)		// 选择目标文件的“选择”按钮
ON_BN_CLICKED(IDC_START, OnStart)			// 开始“分割”按钮
ON_BN_CLICKED(IDC_STOP, OnStop)				// “终止”分割按钮
ON_BN_CLICKED(IDC_SELECTSPLIT, OnSelect)		// 分割单选框按钮
ON_BN_CLICKED(IDC_SELECTMERGE, OnSelect)		// 合并单选框按钮
// 下面是3各CFileCutter类发来的消息
ON_MESSAGE(WM_CUTTERSTART, OnCutterStart)		
ON_MESSAGE(WM_CUTTERSTATUS, OnCutterStatus)
ON_MESSAGE(WM_CUTTERSTOP, OnCutterStop)
END_MESSAGE_MAP()

BOOL CMainDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(theApp.LoadIcon(IDI_MAIN), FALSE);

	// 创建CFileCutter对象
	m_pCutter = new CFileCutter(m_hWnd);

	// 默认选中分割单项框
	((CButton*)GetDlgItem(IDC_SELECTSPLIT))->SetCheck(1);
	// 初始化单位选择组合框。可以在这里继续添加其它项
	((CComboBox*)GetDlgItem(IDC_UNIT))->AddString("1");
	((CComboBox*)GetDlgItem(IDC_UNIT))->AddString("30");
	((CComboBox*)GetDlgItem(IDC_UNIT))->AddString("60");
	((CComboBox*)GetDlgItem(IDC_UNIT))->SetCurSel(0);

	// 子类化进度条控件。也就是让m_Progress对象取得进度条控件的控制权
	m_Progress.SubclassWindow(*GetDlgItem(IDC_PROGRESS));

	UIControl();
	return TRUE;
}

void CMainDialog::UIControl()
{
	BOOL bIsWorking = m_pCutter->IsRunning();
	
	// 设置选项分组框中3个控件的状态
	GetDlgItem(IDC_SELECTSPLIT)->EnableWindow(!bIsWorking);
	GetDlgItem(IDC_SELECTMERGE)->EnableWindow(!bIsWorking);
	GetDlgItem(IDC_UNIT)->EnableWindow(!bIsWorking);
	// 设置分割、终止两个按钮的状态
	GetDlgItem(IDC_START)->EnableWindow(!bIsWorking);
	GetDlgItem(IDC_STOP)->EnableWindow(bIsWorking);

	if(bIsWorking)
	{
		return;
	}
	
	// 根据用户的选择设置不同的文本
	BOOL bSplit = ((CButton*)GetDlgItem(IDC_SELECTSPLIT))->GetCheck();
	if(bSplit)	// 请求分割	
	{
		GetDlgItem(IDC_START)->SetWindowText("分割");
		GetDlgItem(IDC_SOURCETITLE)->SetWindowText("请选择要分割的文件：");
		GetDlgItem(IDC_DESTTITLE)->SetWindowText("请选择分割后保存到的文件夹：");
	
		GetDlgItem(IDC_UNIT)->EnableWindow(TRUE);
	}
	else		// 请求合并
	{
		GetDlgItem(IDC_START)->SetWindowText("合并");
		GetDlgItem(IDC_SOURCETITLE)->SetWindowText("请选择待合并文件的文件夹：");
		GetDlgItem(IDC_DESTTITLE)->SetWindowText("请选择合并后保存到的文件夹：");
		
		GetDlgItem(IDC_UNIT)->EnableWindow(FALSE);
	}

	// 初始化状态信息
	GetDlgItem(IDC_STATUSTEXT)->SetWindowText("    状态显示：");
	m_Progress.SetPos(0);

}

void CMainDialog::OnCancel()
{
	// 是否真的退出？
	BOOL bExit = TRUE;
	if(m_pCutter->IsRunning())
	{
		if(MessageBox("工作还未完成，确实要退出吗？", NULL, MB_YESNO) == IDNO)
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
	if(bSplit)	// 请求分割
	{
		CFileDialog sourceFile(TRUE);
		// 显示选择文件对话框
		if(sourceFile.DoModal() == IDOK)
		{
			GetDlgItem(IDC_EDITSOURCE)->SetWindowText(sourceFile.GetPathName());
			
			// 设置默认目录
			// 例如，如果用户选择文件“D:\cd\精选歌曲.iso”，那么“D:\cd\精选歌曲”将会被设置为默认目录
			CString strDef = sourceFile.GetPathName();
			strDef = strDef.Left(strDef.ReverseFind('.'));
			GetDlgItem(IDC_EDITDEST)->SetWindowText(strDef);
		}
	}
	else		// 请求合并
	{
		CDirDialog sourceFolder;
		// 显示选择目录对话框
		if(sourceFolder.DoBrowse(*this) == IDOK)
		{
			GetDlgItem(IDC_EDITSOURCE)->SetWindowText(sourceFolder.GetPath());

			// 设置默认目录
			// 例如，如果用户选择目录“D:\cd”，那么“D:\cd\cd”将会被设置为默认目录
			CString strDef = sourceFolder.GetPath();
			strDef.TrimRight('\\');
			strDef = strDef + '\\' + strDef.Mid(strDef.ReverseFind('\\') + 1);
			// 防止用户选择根目录
			strDef.TrimRight(':');
			GetDlgItem(IDC_EDITDEST)->SetWindowText(strDef);
		}
	}
}

void CMainDialog::OnDestBrowser()
{
	CDirDialog destFolder;
	// 显示选择目录对话框
	if(destFolder.DoBrowse(*this) == IDOK)
	{
		GetDlgItem(IDC_EDITDEST)->SetWindowText(destFolder.GetPath());
	}
}

void CMainDialog::OnStart()
{
	CString strSource, strDest;

	// 检查输入
	GetDlgItem(IDC_EDITSOURCE)->GetWindowText(strSource);
	GetDlgItem(IDC_EDITDEST)->GetWindowText(strDest);
	if(strSource.IsEmpty() || strDest.IsEmpty())
	{
		MessageBox("文件或路径名称不能为空");
		return;
	}

	BOOL bSplit = ((CButton*)GetDlgItem(IDC_SELECTSPLIT))->GetCheck();
	if(bSplit)	// 请求分割
	{
		CString str;
		GetDlgItem(IDC_UNIT)->GetWindowText(str);
		m_pCutter->StartSplit(strDest, strSource, atoi(str)*1024*1024);
	}
	else		// 请求合并
	{
		m_pCutter->StartMerge(strDest, strSource);
	}
}

void CMainDialog::OnStop()
{
	m_pCutter->SuspendCutter();
	if(MessageBox("确实要终止吗？", NULL, MB_YESNO) == IDYES)
	{
		m_pCutter->StopCutter();
	}
	else
	{
		m_pCutter->ResumeCutter();
	}
}
 
////////////////////////////////////////////////////
// 下面的代码处理CFileCutter类发来的消息


long CMainDialog::OnCutterStart(WPARAM wParam, LPARAM)		// WM_CUTTERSTART 开始工作
{
	// 设置进度条范围
	int nTotalFiles = wParam;	// 总文件数量
	m_Progress.SetRange(0, nTotalFiles);
	
	UIControl();
	return 0;
}

long CMainDialog::OnCutterStatus(WPARAM wParam, LPARAM lParam)	// WM_CUTTERSTATUS 工作进度
{
	// 设置进度条进度
	int nCompleted = (int)lParam;
	m_Progress.SetPos(nCompleted);
	// 显示状态
	CString s;
	s.Format(" 完成%d个文件", nCompleted);
	GetDlgItem(IDC_STATUSTEXT)->SetWindowText(s);
	return 0;
}

long CMainDialog::OnCutterStop(WPARAM wParam, LPARAM lParam)	// WM_CUTTERSTOP 停止工作
{
	int nErrorCode = wParam;
	switch(nErrorCode)
	{
	case CFileCutter::exitSuccess:
		MessageBox("操作成功完成", "成功");
		break;
	case CFileCutter::exitSourceErr:
		MessageBox("源文件出错", "失败");
		break;
	case CFileCutter::exitDestErr:
		MessageBox("目标文件出错", "失败");
		break;
	case CFileCutter::exitUserForce:
		MessageBox("用户终止", "失败");
		break;
	}
	UIControl();
	return 0;
}



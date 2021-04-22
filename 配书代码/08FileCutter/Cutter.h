///////////////////////////////////////////
// MainDir.h文件	// 07MainDir

#include <afxwin.h>	
#include <afxcmn.h>	// 为了使用CStatusBarCtrl类

#include "FileCutter.h"

class CMyApp : public CWinApp
{
public:
	BOOL InitInstance();
};

class CMainDialog : public CDialog
{
public:
	CMainDialog(CWnd* pParentWnd = NULL);

protected:
	// 进度条控件对象
	CProgressCtrl m_Progress;

	CFileCutter* m_pCutter;

	// 动态控制程序界面的函数
	void UIControl();
protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();

	afx_msg void OnSourceBrowser();
	afx_msg void OnDestBrowser();
	afx_msg void OnStart();
	afx_msg void OnStop();
	afx_msg void OnSelect();
	// 处理CFileCutter类发来的消息
	afx_msg long OnCutterStart(WPARAM wParam, LPARAM);
	afx_msg long OnCutterStatus(WPARAM wParam, LPARAM lParam);
	afx_msg long OnCutterStop(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
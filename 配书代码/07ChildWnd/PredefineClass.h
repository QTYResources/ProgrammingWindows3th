/////////////////////////////////////////////////
// PredefineClass.h文件

#define IDC_BUTTON	10	// Button按钮
#define IDC_RADIO	11	// 单选框
#define IDC_CHECKBOX	12	// 复选框
#define IDC_STATIC	13	// 静态文本
#define IDC_EDITTEXT	14	// 文本框


#include "afxwin.h"

class CMyApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
};

class CMainWindow : public CWnd
{
public:
	CMainWindow();
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void PostNcDestroy();
	afx_msg BOOL OnCreate(LPCREATESTRUCT);
	DECLARE_MESSAGE_MAP()
};
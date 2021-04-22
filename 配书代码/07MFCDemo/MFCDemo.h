//////////////////////////////////////////////////////
// MFCDemo.h文件


#include <afxwin.h>

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
	// 两个子窗口控件
	CButton m_btnModel;
	CButton m_btnModeless;

	virtual void PostNcDestroy();
	afx_msg BOOL OnCreate(LPCREATESTRUCT);
	afx_msg void OnModel();
	afx_msg void OnModeless();
	DECLARE_MESSAGE_MAP()
};

class CMyDialog : public CDialog
{
public:
	CMyDialog(CWnd* pParentWnd = NULL);	
	BOOL m_bModeless;

protected:

	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void PostNcDestroy();

	afx_msg void OnStart();
	DECLARE_MESSAGE_MAP()
};
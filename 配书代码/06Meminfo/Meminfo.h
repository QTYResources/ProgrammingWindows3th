////////////////////////////////////////////
// Meminfo.h文件

#include "../common/_afxwin.h"

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
	char m_szText[1024];	// 客户区文本缓冲区
	RECT m_rcInfo;		// 文本所在方框的大小

protected:
	virtual void PostNcDestroy();
	afx_msg BOOL OnCreate(LPCREATESTRUCT);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()
};
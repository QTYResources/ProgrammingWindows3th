////////////////////////////////////////////
// Meminfo.h�ļ�

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
	char m_szText[1024];	// �ͻ����ı�������
	RECT m_rcInfo;		// �ı����ڷ���Ĵ�С

protected:
	virtual void PostNcDestroy();
	afx_msg BOOL OnCreate(LPCREATESTRUCT);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()
};
///////////////////////////////////////////
// HookTermProApp.hÎÄ¼þ

#include <afxwin.h>	


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
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT *pCopyDataStruct);
	DECLARE_MESSAGE_MAP()
};
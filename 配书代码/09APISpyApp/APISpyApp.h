///////////////////////////////////////////
//APISpyApp.hÎÄ¼þ

#include <afxwin.h>	

#include "RemThreadInjector.h"

#include "../09APISpyLib/APISpyLib.h"

class CMyApp : public CWinApp
{
public:
	BOOL InitInstance();
};

class CMainDialog : public CDialog
{
public:
	CMainDialog(CWnd* pParentWnd = NULL);
	~CMainDialog();


protected:

	CRemThreadInjector* m_pInjector;
	CMyShareMem* m_pShareMem;
	DWORD m_dwProcessId;

	int m_nCount;


	virtual BOOL OnInitDialog();
	virtual void OnCancel();

	afx_msg void OnBrowser();
	afx_msg void OnStart();
	afx_msg void OnClear();
	afx_msg long OnSpyACall(WPARAM wParam, LPARAM lParam);



	DECLARE_MESSAGE_MAP()
};
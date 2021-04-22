///////////////////////////////////
// MemRepair.h�ļ�

#include "afxwin.h"
#include <afxcmn.h>	// Ϊ��ʹ��CStatusBarCtrl��

#include "MemFinder.h"


class CMyApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
};


class CMainDialog : public CDialog
{
public:
	CMainDialog(CWnd* pWndParent = NULL);

	CComboBox m_comboList;
	CButton m_btnUpdate;
	CListBox m_listAddr;

	CStatusBarCtrl m_bar;

	CMsgMemFinder *m_pFinder;
	
	void OnUpdate();
	void OnSearch();
	void OnRestart();
	void OnModify();
	void OnSelectChange();


	void UIControl();
	
	virtual BOOL OnInitDialog();
	virtual void OnCancel();


	

	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////
// PredefineClass.h�ļ�

#define IDC_BUTTON	10	// Button��ť
#define IDC_RADIO	11	// ��ѡ��
#define IDC_CHECKBOX	12	// ��ѡ��
#define IDC_STATIC	13	// ��̬�ı�
#define IDC_EDITTEXT	14	// �ı���


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
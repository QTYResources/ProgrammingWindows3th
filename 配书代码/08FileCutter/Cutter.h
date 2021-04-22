///////////////////////////////////////////
// MainDir.h�ļ�	// 07MainDir

#include <afxwin.h>	
#include <afxcmn.h>	// Ϊ��ʹ��CStatusBarCtrl��

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
	// �������ؼ�����
	CProgressCtrl m_Progress;

	CFileCutter* m_pCutter;

	// ��̬���Ƴ������ĺ���
	void UIControl();
protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();

	afx_msg void OnSourceBrowser();
	afx_msg void OnDestBrowser();
	afx_msg void OnStart();
	afx_msg void OnStop();
	afx_msg void OnSelect();
	// ����CFileCutter�෢������Ϣ
	afx_msg long OnCutterStart(WPARAM wParam, LPARAM);
	afx_msg long OnCutterStatus(WPARAM wParam, LPARAM lParam);
	afx_msg long OnCutterStop(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
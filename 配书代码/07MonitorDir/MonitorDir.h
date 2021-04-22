///////////////////////////////////////////
// MonitorDir.h文件

#include <afxwin.h>	
#include <afxcmn.h>	// 为了使用CStatusBarCtrl类


class CMyApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
};

class CMonitorDialog : public CDialog
{
public:
	CMonitorDialog(CWnd* pParentWnd = NULL);
	~CMonitorDialog();
protected:
	// 向输出窗口添加文本
	void AddStringToList(LPCTSTR lpszString);
	// 监视线程
	friend UINT MonitorThread(LPVOID lpParam);

	CStatusBarCtrl m_bar;		// 一个状态栏对象
	HANDLE m_hEvent;		// 用于占位的事件对象句柄

	HANDLE m_arhChange[6];		// 改变通知事件的6个句柄
	BOOL m_bExit;			// 指示监视线程是否退出
protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();

	afx_msg void OnBrowser();
	afx_msg void OnStart();
	afx_msg void OnStop();
	afx_msg void OnClear();
	DECLARE_MESSAGE_MAP()
};
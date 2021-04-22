///////////////////////////////////////////
// MonitorDir.h�ļ�

#include <afxwin.h>	
#include <afxcmn.h>	// Ϊ��ʹ��CStatusBarCtrl��


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
	// �������������ı�
	void AddStringToList(LPCTSTR lpszString);
	// �����߳�
	friend UINT MonitorThread(LPVOID lpParam);

	CStatusBarCtrl m_bar;		// һ��״̬������
	HANDLE m_hEvent;		// ����ռλ���¼�������

	HANDLE m_arhChange[6];		// �ı�֪ͨ�¼���6�����
	BOOL m_bExit;			// ָʾ�����߳��Ƿ��˳�
protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();

	afx_msg void OnBrowser();
	afx_msg void OnStart();
	afx_msg void OnStop();
	afx_msg void OnClear();
	DECLARE_MESSAGE_MAP()
};
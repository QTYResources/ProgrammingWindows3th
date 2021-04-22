///////////////////////////////////////////
// TCPClient.h�ļ�

#include <afxwin.h>	
#include <afxcmn.h>
#include <winsock2.h> 

// ������������WS2_32������
#pragma comment(lib,"WS2_32.lib")

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
	// ���ӷ�����
	BOOL Connect(LPCTSTR pszRemoteAddr, u_short nPort);
	// ���ı���������ı�
	void AddStringToList(LPCTSTR pszText, BOOL bRecv = TRUE);

protected:
	// ״̬���Ӵ��ڿؼ�
	CStatusBarCtrl m_bar;	
	// �����������ȡ�����ӵ��׽��־��
	SOCKET m_socket;

protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	// ȡ�û�Ͽ�����
	afx_msg void OnButtonConnect();
	// ��������
	afx_msg void OnButtonSend();
	// ��ձ༭��
	afx_msg void OnButtonClear();
	// �׽���֪ͨ�¼�
	afx_msg long OnSocket(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
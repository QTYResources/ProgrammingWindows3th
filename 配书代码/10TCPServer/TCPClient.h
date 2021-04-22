///////////////////////////////////////////
// TCPServer.h�ļ�

#include <afxwin.h>		// 10TCPServer	
#include <afxcmn.h>
#include <winsock2.h> 

// ������������WS2_32������
#pragma comment(lib,"WS2_32.lib")

#define MAX_SOCKET 56	// ����˷��������ܽ��ܵ����ͻ���

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
	// �����׽��֣�������Ϊ����״̬��׼�����ܿͻ�������
	BOOL CreateAndListen(int nPort);
	// �ر������׽��֣����������׽��ֺ�����accept�������ص��׽���
	void CloseAllSocket();
	// ��ͻ������б������һ���ͻ�
	BOOL AddClient(SOCKET s);
	// �ӿͻ������б����ƴ�һ���ͻ�
	void RemoveClient(SOCKET s);

protected:
	// �����Ӵ��ڿؼ���һ����״̬����һ�����б��
	CStatusBarCtrl m_bar;
	CListBox m_listInfo;
	
	// �����׽��־��
	SOCKET m_socket;

	// �ͻ������б�
	SOCKET m_arClient[MAX_SOCKET];	// �׽�������
	int m_nClient;			// ��������Ĵ�С
	
protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	// ������ֹͣ����
	afx_msg void OnStart();
	// �����Ϣ
	afx_msg void OnClear();
	// �׽���֪ͨ�¼�
	afx_msg long OnSocket(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
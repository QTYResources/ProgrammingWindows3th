///////////////////////////////////////////
// TCPServer.h文件

#include <afxwin.h>		// 10TCPServer	
#include <afxcmn.h>
#include <winsock2.h> 

// 告诉连接器与WS2_32库连接
#pragma comment(lib,"WS2_32.lib")

#define MAX_SOCKET 56	// 定义此服务器所能接受的最大客户量

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
	// 创建套节字，并设置为监听状态，准备接受客户的连接
	BOOL CreateAndListen(int nPort);
	// 关闭所有套节字，包括监听套节字和所有accept函数返回的套节字
	void CloseAllSocket();
	// 向客户连接列表中添加一个客户
	BOOL AddClient(SOCKET s);
	// 从客户连接列表中移处一个客户
	void RemoveClient(SOCKET s);

protected:
	// 两个子窗口控件，一个是状态栏，一个是列表框
	CStatusBarCtrl m_bar;
	CListBox m_listInfo;
	
	// 监听套节字句柄
	SOCKET m_socket;

	// 客户连接列表
	SOCKET m_arClient[MAX_SOCKET];	// 套节字数组
	int m_nClient;			// 上述数组的大小
	
protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	// 开启或停止服务
	afx_msg void OnStart();
	// 清空信息
	afx_msg void OnClear();
	// 套节字通知事件
	afx_msg long OnSocket(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
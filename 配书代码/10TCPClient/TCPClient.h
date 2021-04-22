///////////////////////////////////////////
// TCPClient.h文件

#include <afxwin.h>	
#include <afxcmn.h>
#include <winsock2.h> 

// 告诉连接器与WS2_32库连接
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
	// 连接服务器
	BOOL Connect(LPCTSTR pszRemoteAddr, u_short nPort);
	// 向文本框中添加文本
	void AddStringToList(LPCTSTR pszText, BOOL bRecv = TRUE);

protected:
	// 状态栏子窗口控件
	CStatusBarCtrl m_bar;	
	// 用于与服务器取得连接的套节字句柄
	SOCKET m_socket;

protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	// 取得或断开连接
	afx_msg void OnButtonConnect();
	// 发送数据
	afx_msg void OnButtonSend();
	// 清空编辑框
	afx_msg void OnButtonClear();
	// 套节字通知事件
	afx_msg long OnSocket(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////
// TCPClient.cpp文件

#include "TCPClient.h"
#include "resource.h"

// 定义网络事件通知消息
#define WM_SOCKET WM_USER + 1	

CMyApp theApp;

BOOL CMyApp::InitInstance()
{
	// 初始化Winsock库
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 0);
	::WSAStartup(sockVersion, &wsaData);
	// 弹出主窗口对话框
	CMainDialog dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	// 释放Winsock库
	::WSACleanup();
	return FALSE;
}

CMainDialog::CMainDialog(CWnd* pParentWnd):CDialog(IDD_MAINDIALOG, pParentWnd)
{
}

BEGIN_MESSAGE_MAP(CMainDialog, CDialog)
ON_BN_CLICKED(IDC_CONNECT, OnButtonConnect)
ON_BN_CLICKED(IDC_SEND, OnButtonSend)
ON_BN_CLICKED(IDC_CLEAR, OnButtonClear)
ON_MESSAGE(WM_SOCKET, OnSocket)
END_MESSAGE_MAP()

BOOL CMainDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	// 设置图标
	SetIcon(theApp.LoadIcon(IDI_MAIN), FALSE);
	// 创建状态栏，设置它的属性
	m_bar.Create(WS_CHILD|WS_VISIBLE|SBS_SIZEGRIP, CRect(0, 0, 0, 0), this, 101);
	m_bar.SetBkColor(RGB(0xa6, 0xca, 0xf0));		// 背景色
	int arWidth[] = { 200, -1 };
	m_bar.SetParts(2, arWidth);				// 分栏
	m_bar.SetText(" Windows程序设计进阶之路！", 1, 0);	// 第一个栏的文本
	m_bar.SetText(" 空闲", 0, 0);				// 第二个栏的文本
	// 初始化发送按钮和发送编辑框的状态
	GetDlgItem(IDC_SEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_TEXT)->EnableWindow(FALSE);

	// 初始化连接套节字
	m_socket = INVALID_SOCKET;

	return TRUE;
}

void CMainDialog::OnCancel()
{
	if(m_socket != INVALID_SOCKET)
		::closesocket(m_socket);

	CDialog::OnCancel();
}

void CMainDialog::OnButtonClear()
{
	GetDlgItem(IDC_INFO)->SetWindowText("");	
}

void CMainDialog::OnButtonConnect()
{
	if(m_socket == INVALID_SOCKET)  // 连接服务器
	{
		// 取得服务器地址
		CString sAddr;
		GetDlgItem(IDC_ADDR)->GetWindowText(sAddr);
		if(sAddr.IsEmpty())
		{
			MessageBox("请输入服务器地址！");
			return;
		}

		// 取得端口号
		CString sPort;
		GetDlgItem(IDC_PORT)->GetWindowText(sPort);
		int nPort = atoi(sPort);
		if(nPort < 1 || nPort > 65535)
		{
			MessageBox("端口号错误！");
			return;
		}

		// 试图连接服务器
		if(!Connect(sAddr, nPort))
		{
			MessageBox("连接服务器出错！");
			return;
		}
		
		// 设置用户界面
		GetDlgItem(IDC_CONNECT)->SetWindowText("取消");
		m_bar.SetText(" 正在连接……", 0, 0);
	}
	else				// 断开服务器
	{
		// 关闭套节字
		::closesocket(m_socket);
		m_socket = INVALID_SOCKET;

		// 设置用户界面
		GetDlgItem(IDC_CONNECT)->SetWindowText("连接服务器");
		m_bar.SetText(" 空闲", 0, 0);	
		GetDlgItem(IDC_ADDR)->EnableWindow(TRUE);
		GetDlgItem(IDC_PORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_TEXT)->EnableWindow(FALSE);
		GetDlgItem(IDC_SEND)->EnableWindow(FALSE);
	}
}

long CMainDialog::OnSocket(WPARAM wParam, LPARAM lParam)
{
	// 取得有事件发生的套节字句柄
	SOCKET s = wParam;
	// 查看是否出错
	if(WSAGETSELECTERROR(lParam))
	{
		if(m_socket != SOCKET_ERROR)
			OnButtonConnect();
		m_bar.SetText(" 连接出错！", 0, 0);
		return 0;
	}
	// 处理发生的事件
	switch(WSAGETSELECTEVENT(lParam))
	{	
	case FD_CONNECT:	// 套节字正确的连接到服务器
		{
			// 设置用户界面
			GetDlgItem(IDC_CONNECT)->SetWindowText("断开连接");

			GetDlgItem(IDC_ADDR)->EnableWindow(FALSE);
			GetDlgItem(IDC_PORT)->EnableWindow(FALSE);
			GetDlgItem(IDC_TEXT)->EnableWindow(TRUE);
			GetDlgItem(IDC_SEND)->EnableWindow(TRUE);
			m_bar.SetText(" 已经连接到服务器", 0, 0);
		}
		break;

	case FD_READ:		// 套接字接受到对方发送过来的数据包
		{
			// 从服务器接受数据
			char szText[1024] = { 0 };
			::recv(s, szText, 1024, 0);
			// 显示给用户
			AddStringToList(CString(szText) + "\r\n");
		}
		break;

	case FD_CLOSE:
		OnButtonConnect();
		break;
	}

	return 0;
}

void CMainDialog::OnButtonSend()
{
	if(m_socket == INVALID_SOCKET)
	{
		return;
	}

	// 取得要发送的字符串
	CString sText;
	GetDlgItem(IDC_TEXT)->GetWindowText(sText);

	// 添加一个“回车换行”
	// 注意，添加它并不是必须的，但是如果使用本软件作为客户端调试网络协议，
	// 比如SMTP、FTP等，就要添加它了。因为这些协议都要求使用“回车换行”作为一个命令的结束标记
	sText += "\r\n";

	// 发送数据到服务器
	if(::send(m_socket, sText, sText.GetLength(), 0) != -1)
	{
		AddStringToList(sText, FALSE);
		GetDlgItem(IDC_TEXT)->SetWindowText("");
	}
}

BOOL CMainDialog::Connect(LPCTSTR pszRemoteAddr, u_short nPort)
{
	// 创建套节字
	m_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(m_socket == INVALID_SOCKET)
	{
		return FALSE;
	}
	
	// 设置socket为窗口通知消息类型
	::WSAAsyncSelect(m_socket, m_hWnd,
		WM_SOCKET, FD_CONNECT | FD_CLOSE | FD_WRITE | FD_READ);
	
	// 假定szAddr是IP地址
	ULONG uAddr = ::inet_addr(pszRemoteAddr);
        if(uAddr == INADDR_NONE)
	{
                // 不是IP地址，就认为这是主机名称
		// 从主机名取得IP地址
		hostent* pHost = ::gethostbyname(pszRemoteAddr);
		if(pHost == NULL)
		{
			::closesocket(m_socket);
			m_socket = INVALID_SOCKET;
			return FALSE;
		}
		// 得到以网络字节顺序排列的IP地址
                uAddr = ((struct in_addr*)*(pHost->h_addr_list))->s_addr;
        }
	
        // 填写服务器地址信息
	sockaddr_in remote;
        remote.sin_addr.S_un.S_addr = uAddr;
        remote.sin_family = AF_INET;
        remote.sin_port = htons(nPort);
	
        // 连接到远程机
        ::connect(m_socket, (sockaddr*)&remote, sizeof(sockaddr));
	
	return TRUE;
}

void CMainDialog::AddStringToList(LPCTSTR pszText, BOOL bRecv)
{
	CString strEdit;
	GetDlgItem(IDC_INFO)->GetWindowText(strEdit);

	if(bRecv)
	{
		strEdit += "【Recv】：";
		strEdit += pszText;
	}
	else
	{
		strEdit += "【Send】：";
		strEdit += pszText;
	}
	GetDlgItem(IDC_INFO)->SetWindowText(strEdit);	
}

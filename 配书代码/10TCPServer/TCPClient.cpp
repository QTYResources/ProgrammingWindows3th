/////////////////////////////////////////////////////
// TCPServer.cpp文件

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
ON_BN_CLICKED(IDC_START, OnStart)
ON_BN_CLICKED(IDC_CLEAR, OnClear)
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
	// 设置列表框控件到m_listInfo对象的关联
	m_listInfo.SubclassDlgItem(IDC_INFO, this);
	
	// 初始化监听套节字和连接列表
	m_socket = INVALID_SOCKET;
	m_nClient = 0;
	
		// 下面是取得本地IP地址的过程，将它显示在状态栏的第一个分栏中
	// 取得本机名称	
	char szHost[256];
	::gethostname(szHost, 256);
	// 通过本机名称取得地址信息
	HOSTENT* pHost = gethostbyname(szHost);
	if(pHost != NULL)
	{  	
		CString sIP;
		
		// 得到第一个IP地址
		in_addr *addr =(in_addr*) *(pHost->h_addr_list);
		
		// 显示给用户
		sIP.Format(" 本机IP：%s", inet_ntoa(addr[0]));
		m_bar.SetText(sIP, 0, 0);
	}
	
	return TRUE;
}

void CMainDialog::OnStart()
{
	if(m_socket == INVALID_SOCKET)  // 开启服务
	{
		// 取得端口号
		CString sPort;
		GetDlgItem(IDC_PORT)->GetWindowText(sPort);
		int nPort = atoi(sPort);
		if(nPort < 1 || nPort > 65535)
		{
			MessageBox("端口号错误！");
			return;
		}

		// 创建监听套节字，使它进入监听状态
		if(!CreateAndListen(nPort))
		{
			MessageBox("启动服务出错！");
			return;
		}
		
		// 设置相关子窗口控件状态
		GetDlgItem(IDC_START)->SetWindowText("停止服务");
		m_bar.SetText(" 正在监听……", 0, 0);
		GetDlgItem(IDC_PORT)->EnableWindow(FALSE);
	}
	else				// 停止服务
	{
		// 关闭所有连接
		CloseAllSocket();

		// 设置相关子窗口控件状态
		GetDlgItem(IDC_START)->SetWindowText("开启服务");
		m_bar.SetText(" 空闲", 0, 0);
		GetDlgItem(IDC_PORT)->EnableWindow(TRUE);
	}
}

void CMainDialog::OnCancel()
{
	CloseAllSocket();
	CDialog::OnCancel();
}

void CMainDialog::OnClear()
{
	m_listInfo.ResetContent();
}

long CMainDialog::OnSocket(WPARAM wParam, LPARAM lParam)
{
	// 取得有事件发生的套节字句柄
	SOCKET s = wParam;
	// 查看是否出错
	if(WSAGETSELECTERROR(lParam))
	{
		RemoveClient(s);
		::closesocket(s);
		return 0;
	}
	// 处理发生的事件
	switch(WSAGETSELECTEVENT(lParam))
	{
	case FD_ACCEPT:		// 监听中的套接字检测到有连接进入
		{
			if(m_nClient < MAX_SOCKET)
			{
				// 接受连接请求，新的套节字client是新连接的套节字
				SOCKET client = ::accept(s, NULL, NULL);
				// 设置新的套节字为窗口通知消息类型
				int i = ::WSAAsyncSelect(client, 
					m_hWnd, WM_SOCKET, FD_READ|FD_WRITE|FD_CLOSE);
				AddClient(client);
			}
			else
			{
				MessageBox("连接客户太多！");
			}
		}
		break;

	case FD_CLOSE:		// 检测到套接字对应的连接被关闭。
		{
			RemoveClient(s);
			::closesocket(s);
		}
		break;

	case FD_READ:		// 套接字接受到对方发送过来的数据包
		{

				// 取得对方的IP地址和端口号（使用getpeername函数）
			// Peer对方的地址信息
			sockaddr_in sockAddr;
			memset(&sockAddr, 0, sizeof(sockAddr));
			int nSockAddrLen = sizeof(sockAddr);
			::getpeername(s, (SOCKADDR*)&sockAddr, &nSockAddrLen);
			// 转化为主机字节顺序
			int nPeerPort = ::ntohs(sockAddr.sin_port);
			// 转化为字符串IP
			CString sPeerIP = ::inet_ntoa(sockAddr.sin_addr);
			
				// 取得对方的主机名称
			// 取得网络字节顺序的IP值
			DWORD dwIP = ::inet_addr(sPeerIP);
			// 获取主机名称，注意其中第一个参数的转化
			hostent* pHost = ::gethostbyaddr((LPSTR)&dwIP, 4, AF_INET);
			char szHostName[256];
			strncpy(szHostName, pHost->h_name, 256);		 

			// 接受真正的网络数据
			char szText[1024] = { 0 };
			::recv(s, szText, 1024, 0);

			// 显示给用户
			CString strItem = CString(szHostName) + "["+ sPeerIP+ "]: " + CString(szText);
			m_listInfo.InsertString(0, strItem);

		}
		break;
	}
	return 0;
}


BOOL CMainDialog::CreateAndListen(int nPort)
{
	if(m_socket == INVALID_SOCKET)
		::closesocket(m_socket);

	// 创建套节字
	m_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(m_socket == INVALID_SOCKET)
		return FALSE;
	
	// 填写要关联的本地地址
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(nPort);
	sin.sin_addr.s_addr = INADDR_ANY;
	// 绑定端口
	if(::bind(m_socket, (sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		return FALSE;
	}

	// 设置socket为窗口通知消息类型
	::WSAAsyncSelect(m_socket, m_hWnd, WM_SOCKET, FD_ACCEPT|FD_CLOSE);
	// 进入监听模式
	::listen(m_socket, 5);

	return TRUE;
}


BOOL CMainDialog::AddClient(SOCKET s)
{
	if(m_nClient < MAX_SOCKET)
	{
		// 添加新的成员
		m_arClient[m_nClient++] = s;
		return TRUE;
	}
	return FALSE;
}

void CMainDialog::RemoveClient(SOCKET s)
{
	BOOL bFind = FALSE;
	for(int i=0; i<m_nClient; i++)
	{
		if(m_arClient[i] == s)
		{
			bFind = TRUE;
			break;
		}
	}

	// 如果找到就将此成员从列表中移除
	if(bFind)
	{
		m_nClient--;
		// 将此成员后面的成员都向前移动一个单位
		for(int j=i; j<m_nClient; j++)
		{
			m_arClient[j] = m_arClient[j+1];
		}
	}
}

void CMainDialog::CloseAllSocket()
{
	// 关闭监听套节字
	if(m_socket != INVALID_SOCKET)
	{
		::closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}
	
	// 关闭所有客户的连接
	for(int i=0; i<m_nClient; i++)
	{
		::closesocket(m_arClient[i]);
	}
	m_nClient = 0;
}


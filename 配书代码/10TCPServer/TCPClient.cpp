/////////////////////////////////////////////////////
// TCPServer.cpp�ļ�

#include "TCPClient.h"
#include "resource.h"


// ���������¼�֪ͨ��Ϣ
#define WM_SOCKET WM_USER + 1	

CMyApp theApp;

BOOL CMyApp::InitInstance()
{
	// ��ʼ��Winsock��
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 0);
	::WSAStartup(sockVersion, &wsaData);
	// ���������ڶԻ���
	CMainDialog dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	// �ͷ�Winsock��
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
	// ����ͼ��
	SetIcon(theApp.LoadIcon(IDI_MAIN), FALSE);
	// ����״̬����������������
	m_bar.Create(WS_CHILD|WS_VISIBLE|SBS_SIZEGRIP, CRect(0, 0, 0, 0), this, 101);
	m_bar.SetBkColor(RGB(0xa6, 0xca, 0xf0));		// ����ɫ
	int arWidth[] = { 200, -1 };
	m_bar.SetParts(2, arWidth);				// ����
	m_bar.SetText(" Windows������ƽ���֮·��", 1, 0);	// ��һ�������ı�
	m_bar.SetText(" ����", 0, 0);				// �ڶ��������ı�
	// �����б��ؼ���m_listInfo����Ĺ���
	m_listInfo.SubclassDlgItem(IDC_INFO, this);
	
	// ��ʼ�������׽��ֺ������б�
	m_socket = INVALID_SOCKET;
	m_nClient = 0;
	
		// ������ȡ�ñ���IP��ַ�Ĺ��̣�������ʾ��״̬���ĵ�һ��������
	// ȡ�ñ�������	
	char szHost[256];
	::gethostname(szHost, 256);
	// ͨ����������ȡ�õ�ַ��Ϣ
	HOSTENT* pHost = gethostbyname(szHost);
	if(pHost != NULL)
	{  	
		CString sIP;
		
		// �õ���һ��IP��ַ
		in_addr *addr =(in_addr*) *(pHost->h_addr_list);
		
		// ��ʾ���û�
		sIP.Format(" ����IP��%s", inet_ntoa(addr[0]));
		m_bar.SetText(sIP, 0, 0);
	}
	
	return TRUE;
}

void CMainDialog::OnStart()
{
	if(m_socket == INVALID_SOCKET)  // ��������
	{
		// ȡ�ö˿ں�
		CString sPort;
		GetDlgItem(IDC_PORT)->GetWindowText(sPort);
		int nPort = atoi(sPort);
		if(nPort < 1 || nPort > 65535)
		{
			MessageBox("�˿ںŴ���");
			return;
		}

		// ���������׽��֣�ʹ���������״̬
		if(!CreateAndListen(nPort))
		{
			MessageBox("�����������");
			return;
		}
		
		// ��������Ӵ��ڿؼ�״̬
		GetDlgItem(IDC_START)->SetWindowText("ֹͣ����");
		m_bar.SetText(" ���ڼ�������", 0, 0);
		GetDlgItem(IDC_PORT)->EnableWindow(FALSE);
	}
	else				// ֹͣ����
	{
		// �ر���������
		CloseAllSocket();

		// ��������Ӵ��ڿؼ�״̬
		GetDlgItem(IDC_START)->SetWindowText("��������");
		m_bar.SetText(" ����", 0, 0);
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
	// ȡ�����¼��������׽��־��
	SOCKET s = wParam;
	// �鿴�Ƿ����
	if(WSAGETSELECTERROR(lParam))
	{
		RemoveClient(s);
		::closesocket(s);
		return 0;
	}
	// ���������¼�
	switch(WSAGETSELECTEVENT(lParam))
	{
	case FD_ACCEPT:		// �����е��׽��ּ�⵽�����ӽ���
		{
			if(m_nClient < MAX_SOCKET)
			{
				// �������������µ��׽���client�������ӵ��׽���
				SOCKET client = ::accept(s, NULL, NULL);
				// �����µ��׽���Ϊ����֪ͨ��Ϣ����
				int i = ::WSAAsyncSelect(client, 
					m_hWnd, WM_SOCKET, FD_READ|FD_WRITE|FD_CLOSE);
				AddClient(client);
			}
			else
			{
				MessageBox("���ӿͻ�̫�࣡");
			}
		}
		break;

	case FD_CLOSE:		// ��⵽�׽��ֶ�Ӧ�����ӱ��رա�
		{
			RemoveClient(s);
			::closesocket(s);
		}
		break;

	case FD_READ:		// �׽��ֽ��ܵ��Է����͹��������ݰ�
		{

				// ȡ�öԷ���IP��ַ�Ͷ˿ںţ�ʹ��getpeername������
			// Peer�Է��ĵ�ַ��Ϣ
			sockaddr_in sockAddr;
			memset(&sockAddr, 0, sizeof(sockAddr));
			int nSockAddrLen = sizeof(sockAddr);
			::getpeername(s, (SOCKADDR*)&sockAddr, &nSockAddrLen);
			// ת��Ϊ�����ֽ�˳��
			int nPeerPort = ::ntohs(sockAddr.sin_port);
			// ת��Ϊ�ַ���IP
			CString sPeerIP = ::inet_ntoa(sockAddr.sin_addr);
			
				// ȡ�öԷ�����������
			// ȡ�������ֽ�˳���IPֵ
			DWORD dwIP = ::inet_addr(sPeerIP);
			// ��ȡ�������ƣ�ע�����е�һ��������ת��
			hostent* pHost = ::gethostbyaddr((LPSTR)&dwIP, 4, AF_INET);
			char szHostName[256];
			strncpy(szHostName, pHost->h_name, 256);		 

			// ������������������
			char szText[1024] = { 0 };
			::recv(s, szText, 1024, 0);

			// ��ʾ���û�
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

	// �����׽���
	m_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(m_socket == INVALID_SOCKET)
		return FALSE;
	
	// ��дҪ�����ı��ص�ַ
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(nPort);
	sin.sin_addr.s_addr = INADDR_ANY;
	// �󶨶˿�
	if(::bind(m_socket, (sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		return FALSE;
	}

	// ����socketΪ����֪ͨ��Ϣ����
	::WSAAsyncSelect(m_socket, m_hWnd, WM_SOCKET, FD_ACCEPT|FD_CLOSE);
	// �������ģʽ
	::listen(m_socket, 5);

	return TRUE;
}


BOOL CMainDialog::AddClient(SOCKET s)
{
	if(m_nClient < MAX_SOCKET)
	{
		// ����µĳ�Ա
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

	// ����ҵ��ͽ��˳�Ա���б����Ƴ�
	if(bFind)
	{
		m_nClient--;
		// ���˳�Ա����ĳ�Ա����ǰ�ƶ�һ����λ
		for(int j=i; j<m_nClient; j++)
		{
			m_arClient[j] = m_arClient[j+1];
		}
	}
}

void CMainDialog::CloseAllSocket()
{
	// �رռ����׽���
	if(m_socket != INVALID_SOCKET)
	{
		::closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}
	
	// �ر����пͻ�������
	for(int i=0; i<m_nClient; i++)
	{
		::closesocket(m_arClient[i]);
	}
	m_nClient = 0;
}


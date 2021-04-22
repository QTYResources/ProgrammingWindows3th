/////////////////////////////////////////////////
// TCPClient.cpp�ļ�

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
ON_BN_CLICKED(IDC_CONNECT, OnButtonConnect)
ON_BN_CLICKED(IDC_SEND, OnButtonSend)
ON_BN_CLICKED(IDC_CLEAR, OnButtonClear)
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
	// ��ʼ�����Ͱ�ť�ͷ��ͱ༭���״̬
	GetDlgItem(IDC_SEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_TEXT)->EnableWindow(FALSE);

	// ��ʼ�������׽���
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
	if(m_socket == INVALID_SOCKET)  // ���ӷ�����
	{
		// ȡ�÷�������ַ
		CString sAddr;
		GetDlgItem(IDC_ADDR)->GetWindowText(sAddr);
		if(sAddr.IsEmpty())
		{
			MessageBox("�������������ַ��");
			return;
		}

		// ȡ�ö˿ں�
		CString sPort;
		GetDlgItem(IDC_PORT)->GetWindowText(sPort);
		int nPort = atoi(sPort);
		if(nPort < 1 || nPort > 65535)
		{
			MessageBox("�˿ںŴ���");
			return;
		}

		// ��ͼ���ӷ�����
		if(!Connect(sAddr, nPort))
		{
			MessageBox("���ӷ���������");
			return;
		}
		
		// �����û�����
		GetDlgItem(IDC_CONNECT)->SetWindowText("ȡ��");
		m_bar.SetText(" �������ӡ���", 0, 0);
	}
	else				// �Ͽ�������
	{
		// �ر��׽���
		::closesocket(m_socket);
		m_socket = INVALID_SOCKET;

		// �����û�����
		GetDlgItem(IDC_CONNECT)->SetWindowText("���ӷ�����");
		m_bar.SetText(" ����", 0, 0);	
		GetDlgItem(IDC_ADDR)->EnableWindow(TRUE);
		GetDlgItem(IDC_PORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_TEXT)->EnableWindow(FALSE);
		GetDlgItem(IDC_SEND)->EnableWindow(FALSE);
	}
}

long CMainDialog::OnSocket(WPARAM wParam, LPARAM lParam)
{
	// ȡ�����¼��������׽��־��
	SOCKET s = wParam;
	// �鿴�Ƿ����
	if(WSAGETSELECTERROR(lParam))
	{
		if(m_socket != SOCKET_ERROR)
			OnButtonConnect();
		m_bar.SetText(" ���ӳ���", 0, 0);
		return 0;
	}
	// ���������¼�
	switch(WSAGETSELECTEVENT(lParam))
	{	
	case FD_CONNECT:	// �׽�����ȷ�����ӵ�������
		{
			// �����û�����
			GetDlgItem(IDC_CONNECT)->SetWindowText("�Ͽ�����");

			GetDlgItem(IDC_ADDR)->EnableWindow(FALSE);
			GetDlgItem(IDC_PORT)->EnableWindow(FALSE);
			GetDlgItem(IDC_TEXT)->EnableWindow(TRUE);
			GetDlgItem(IDC_SEND)->EnableWindow(TRUE);
			m_bar.SetText(" �Ѿ����ӵ�������", 0, 0);
		}
		break;

	case FD_READ:		// �׽��ֽ��ܵ��Է����͹��������ݰ�
		{
			// �ӷ�������������
			char szText[1024] = { 0 };
			::recv(s, szText, 1024, 0);
			// ��ʾ���û�
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

	// ȡ��Ҫ���͵��ַ���
	CString sText;
	GetDlgItem(IDC_TEXT)->GetWindowText(sText);

	// ���һ�����س����С�
	// ע�⣬����������Ǳ���ģ��������ʹ�ñ������Ϊ�ͻ��˵�������Э�飬
	// ����SMTP��FTP�ȣ���Ҫ������ˡ���Ϊ��ЩЭ�鶼Ҫ��ʹ�á��س����С���Ϊһ������Ľ������
	sText += "\r\n";

	// �������ݵ�������
	if(::send(m_socket, sText, sText.GetLength(), 0) != -1)
	{
		AddStringToList(sText, FALSE);
		GetDlgItem(IDC_TEXT)->SetWindowText("");
	}
}

BOOL CMainDialog::Connect(LPCTSTR pszRemoteAddr, u_short nPort)
{
	// �����׽���
	m_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(m_socket == INVALID_SOCKET)
	{
		return FALSE;
	}
	
	// ����socketΪ����֪ͨ��Ϣ����
	::WSAAsyncSelect(m_socket, m_hWnd,
		WM_SOCKET, FD_CONNECT | FD_CLOSE | FD_WRITE | FD_READ);
	
	// �ٶ�szAddr��IP��ַ
	ULONG uAddr = ::inet_addr(pszRemoteAddr);
        if(uAddr == INADDR_NONE)
	{
                // ����IP��ַ������Ϊ������������
		// ��������ȡ��IP��ַ
		hostent* pHost = ::gethostbyname(pszRemoteAddr);
		if(pHost == NULL)
		{
			::closesocket(m_socket);
			m_socket = INVALID_SOCKET;
			return FALSE;
		}
		// �õ��������ֽ�˳�����е�IP��ַ
                uAddr = ((struct in_addr*)*(pHost->h_addr_list))->s_addr;
        }
	
        // ��д��������ַ��Ϣ
	sockaddr_in remote;
        remote.sin_addr.S_un.S_addr = uAddr;
        remote.sin_family = AF_INET;
        remote.sin_port = htons(nPort);
	
        // ���ӵ�Զ�̻�
        ::connect(m_socket, (sockaddr*)&remote, sizeof(sockaddr));
	
	return TRUE;
}

void CMainDialog::AddStringToList(LPCTSTR pszText, BOOL bRecv)
{
	CString strEdit;
	GetDlgItem(IDC_INFO)->GetWindowText(strEdit);

	if(bRecv)
	{
		strEdit += "��Recv����";
		strEdit += pszText;
	}
	else
	{
		strEdit += "��Send����";
		strEdit += pszText;
	}
	GetDlgItem(IDC_INFO)->SetWindowText(strEdit);	
}

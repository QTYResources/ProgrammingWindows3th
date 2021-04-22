///////////////////////////////////////////
// ServerDemo.cpp�ļ�


#include <winsock2.h>	// Ϊ��ʹ��Winsock API����
#include <stdio.h>
#include <windows.h>

// ������������WS2_32������
#pragma comment(lib,"WS2_32.lib")


int main(int argc, char* argv[])
{
	// ��ʼ��WS2_32.dll
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 0);
	::WSAStartup(sockVersion, &wsaData);
	
	// �����׽���
	SOCKET s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(s == INVALID_SOCKET)
	{
		printf("Failed socket() \n");
		::WSACleanup();
		return 0;
	}
	
	// ���sockaddr_in�ṹ
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	
	// ������׽��ֵ�һ�����ص�ַ
	if(::bind(s, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("Failed bind() \n");
		::WSACleanup();
		return 0;
	}
	
	// �������ģʽ
	if(::listen(s, 2) == SOCKET_ERROR)
	{
		printf("Failed listen()");
		::WSACleanup();
		return 0;
	}
	
	// ѭ�����ܿͻ�����������
	sockaddr_in remoteAddr; 
	int nAddrLen = sizeof(remoteAddr);
	SOCKET client;
	char szText[] = " 10ServerDemo! \r\n";
	while(TRUE)
	{
		// ����һ��������
		client = ::accept(s, (SOCKADDR*)&remoteAddr, &nAddrLen);
		if(client == INVALID_SOCKET)
		{
			printf("Failed accept()");
			continue;
		}
		
		printf(" ���ܵ�һ�����ӣ�%s \r\n", inet_ntoa(remoteAddr.sin_addr));

		// ��ͻ��˷�������
		::send(client, szText, strlen(szText), 0);

		// �ر�ͬ�ͻ��˵�����
		::closesocket(client);
	}
		
	// �رռ����׽���
	::closesocket(s);
	// �ͷ�WS2_32��
	::WSACleanup();	

	return 0;
}

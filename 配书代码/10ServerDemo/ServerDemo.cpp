///////////////////////////////////////////
// ServerDemo.cpp文件


#include <winsock2.h>	// 为了使用Winsock API函数
#include <stdio.h>
#include <windows.h>

// 告诉连接器与WS2_32库连接
#pragma comment(lib,"WS2_32.lib")


int main(int argc, char* argv[])
{
	// 初始化WS2_32.dll
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 0);
	::WSAStartup(sockVersion, &wsaData);
	
	// 创建套节字
	SOCKET s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(s == INVALID_SOCKET)
	{
		printf("Failed socket() \n");
		::WSACleanup();
		return 0;
	}
	
	// 填充sockaddr_in结构
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	
	// 绑定这个套节字到一个本地地址
	if(::bind(s, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("Failed bind() \n");
		::WSACleanup();
		return 0;
	}
	
	// 进入监听模式
	if(::listen(s, 2) == SOCKET_ERROR)
	{
		printf("Failed listen()");
		::WSACleanup();
		return 0;
	}
	
	// 循环接受客户的连接请求
	sockaddr_in remoteAddr; 
	int nAddrLen = sizeof(remoteAddr);
	SOCKET client;
	char szText[] = " 10ServerDemo! \r\n";
	while(TRUE)
	{
		// 接受一个新连接
		client = ::accept(s, (SOCKADDR*)&remoteAddr, &nAddrLen);
		if(client == INVALID_SOCKET)
		{
			printf("Failed accept()");
			continue;
		}
		
		printf(" 接受到一个连接：%s \r\n", inet_ntoa(remoteAddr.sin_addr));

		// 向客户端发送数据
		::send(client, szText, strlen(szText), 0);

		// 关闭同客户端的连接
		::closesocket(client);
	}
		
	// 关闭监听套节字
	::closesocket(s);
	// 释放WS2_32库
	::WSACleanup();	

	return 0;
}

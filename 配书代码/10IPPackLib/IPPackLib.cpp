///////////////////////////////////////////
// IPHookLib.cpp文件


#include <Winsock2.h>
#include "ShareMemory.h"
#include "ULHook.h"
#include "IPPackLib.h"

#pragma comment(lib, "WS2_32")

// 共享数据区
#pragma data_seg("YCIShared")
HWND g_hWndCaller = NULL;	
HHOOK g_hHook = NULL;
#pragma data_seg()


extern CULHook g_send;
extern CULHook g_sendto;
extern CULHook g_recv;
extern CULHook g_recvfrom;

int WINAPI hook_send(SOCKET s, const char FAR *buf, int len, int flags)
{
	// 申请指定长度的共享内存空间
	CShareMemory sm("IPPACK_SEND", sizeof(CMessageData) + len, TRUE);
	// 取得指向共享内存的指针
	CMessageData *pData = (CMessageData*)sm.GetBuffer();

	// 设置参数
	pData->dwThreadId = ::GetCurrentThreadId();
	pData->socket = s;
	pData->nDataLength = len;
	memcpy(pData->data(), buf, pData->nDataLength);

	// 通知主窗口，进行过滤
	::SendMessage(g_hWndCaller, HM_SEND, 0, 0);

	// 调用原来的函数，发送数据
	g_send.Unhook();
	int nRet = ::send(pData->socket, pData->data(), pData->nDataLength, flags);
	g_send.Rehook();

	return nRet;
}

int WINAPI hook_sendto(SOCKET s, const char* buf,
		int len, int flags, const struct sockaddr* to, int tolen)
{
	// 申请指定长度的共享内存空间
	CShareMemory sm("IPPACK_SEND", sizeof(CMessageData) + len, TRUE);
	// 取得指向共享内存的指针
	CMessageData *pData = (CMessageData*)sm.GetBuffer();

	// 设置参数
	pData->dwThreadId = ::GetCurrentThreadId();
	pData->socket = s;
	pData->nDataLength = len;
	memcpy(pData->data(), buf, len);

	// 通知主窗口，进行过滤
	::SendMessage(g_hWndCaller, HM_SEND, 0, 0);

	// 调用原来的函数，发送数据
	g_sendto.Unhook();
	int nRet = ::sendto(pData->socket, pData->data(), pData->nDataLength, flags, to, tolen);
	g_sendto.Rehook();
	return nRet;
}


int WINAPI hook_recv(SOCKET s, char FAR *buf, int len, int flags)
{
	CShareMemory sm("IPPACK_RECEIVE", sizeof(CMessageData) + len, TRUE);
	CMessageData *pData = (CMessageData*)sm.GetBuffer();

	// 调用原来的函数，接受数据，设置参数
	g_recv.Unhook();
	int nRet = ::recv(s, pData->data(), len, flags);
	g_recv.Rehook();

	pData->dwThreadId = ::GetCurrentThreadId();
	pData->socket = s;
	pData->nDataLength = nRet;

	// 通知主窗口，进行过滤
	::SendMessage(g_hWndCaller, HM_RECEIVE, 0, 0);

	// 返回数据
	memcpy(buf, pData->data(), pData->nDataLength);

	return nRet;
}


int WINAPI hook_recvfrom(SOCKET s, char* buf, int len, int flags, struct sockaddr* from, int* fromlen)
{
	CShareMemory sm("IPPACK_RECEIVE", sizeof(CMessageData) + len, TRUE);
	CMessageData *pData = (CMessageData*)sm.GetBuffer();

	// 调用原来的函数，接受数据，设置参数
	g_recvfrom.Unhook();
	int nRet = ::recvfrom(s, pData->data(), len, flags, from, fromlen);
	g_recvfrom.Rehook();

	pData->dwThreadId = ::GetCurrentThreadId();
	pData->socket = s;
	pData->nDataLength = nRet;

	// 通知主窗口，进行过滤
	::SendMessage(g_hWndCaller, HM_RECEIVE, 0, 0);

	// 返回数据
	memcpy(buf, pData->data(), pData->nDataLength);

	return nRet;
}

// HOOK相关API函数
CULHook g_send("Ws2_32.dll", "send", (PROC)hook_send);
CULHook g_sendto("Ws2_32.dll", "sendto", (PROC)hook_sendto);

CULHook g_recv("Ws2_32.dll", "recv", (PROC)hook_recv);
CULHook g_recvfrom("Ws2_32.dll", "recvfrom", (PROC)hook_recvfrom);



////////////////////////////////////////////

static HMODULE ModuleFromAddress(PVOID pv) 
{
	MEMORY_BASIC_INFORMATION mbi;
	if(::VirtualQuery(pv, &mbi, sizeof(mbi)) != 0)
	{
		return (HMODULE)mbi.AllocationBase;
	}
	else
	{
		return NULL;
	}
}

static LRESULT WINAPI GetMsgProc(int code, WPARAM wParam, LPARAM lParam) 
{
	return ::CallNextHookEx(g_hHook, code, wParam, lParam);
}

BOOL WINAPI SetHook(BOOL bInstall, DWORD dwThreadId, HWND hWndCaller)
{
	BOOL bOk;
	g_hWndCaller = hWndCaller;

	if(bInstall)
	{
		g_hHook = ::SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc, 
						ModuleFromAddress(GetMsgProc), dwThreadId);
		bOk = (g_hHook != NULL);
	} 
	else 
	{
		bOk = ::UnhookWindowsHookEx(g_hHook);
		g_hHook = NULL;
	}
	return bOk;
}




///////////////////////////////////////////////////////////////////////
///


extern CULHook g_WSARecv;
extern CULHook g_WSARecvFrom;



int WINAPI hook_WSARecv(
  SOCKET s,                                               
  LPWSABUF lpBuffers,                                     
  DWORD dwBufferCount,                                    
  LPDWORD lpNumberOfBytesRecvd,                           
  LPDWORD lpFlags,                                        
  LPWSAOVERLAPPED lpOverlapped,                           
  LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine  
)
{
	CShareMemory sm("IPPACK_RECEIVE", sizeof(CMessageData) + lpBuffers->len, TRUE);
	CMessageData *pData = (CMessageData*)sm.GetBuffer();

	// 保存原来接受数据的缓冲区
	char *pOrg = lpBuffers->buf;
	lpBuffers->buf = pData->data();

	// 调用原来的函数，接受数据，设置参数
	g_WSARecv.Unhook();
	int nRet = ::WSARecv(s, lpBuffers, 
		dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpOverlapped, lpCompletionRoutine);
	g_WSARecv.Rehook();

	pData->dwThreadId = ::GetCurrentThreadId();
	pData->socket = s;
	pData->nDataLength = lpBuffers->len;

	// 通知主窗口，进行过滤
	::SendMessage(g_hWndCaller, HM_RECEIVE, 0, 0);

	// 返回数据
	lpBuffers->buf = pOrg;
	memcpy(pOrg, pData->data(), pData->nDataLength);

	return nRet;
}

int WINAPI hook_WSARecvFrom(
  SOCKET s,                                               
  LPWSABUF lpBuffers,                                     
  DWORD dwBufferCount,                                    
  LPDWORD lpNumberOfBytesRecvd,                           
  LPDWORD lpFlags,                                        
  struct sockaddr FAR *lpFrom,                            
  LPINT lpFromlen,                                        
  LPWSAOVERLAPPED lpOverlapped,                           
  LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine  
)
{
	CShareMemory sm("IPPACK_RECEIVE", sizeof(CMessageData) + lpBuffers->len, TRUE);
	CMessageData *pData = (CMessageData*)sm.GetBuffer();

	// 保存原来接受数据的缓冲区
	char *pOrg = lpBuffers->buf;
	lpBuffers->buf = pData->data();

	// 调用原来的函数，接受数据，设置参数
	g_WSARecvFrom.Unhook();
	int nRet = ::WSARecvFrom(s, lpBuffers, dwBufferCount, 
		lpNumberOfBytesRecvd, lpFlags, lpFrom, lpFromlen, lpOverlapped, lpCompletionRoutine);
	g_WSARecvFrom.Rehook();

	pData->dwThreadId = ::GetCurrentThreadId();
	pData->socket = s;
	pData->nDataLength = lpBuffers->len;

	// 通知主窗口，进行过滤
	::SendMessage(g_hWndCaller, HM_RECEIVE, 0, 0);

	// 返回数据
	lpBuffers->buf = pOrg;
	memcpy(pOrg, pData->data(), pData->nDataLength);

	return nRet;
}


CULHook g_WSARecv("Ws2_32.dll", "WSARecv", (PROC)hook_WSARecv);
CULHook g_WSARecvFrom("Ws2_32.dll", "WSARecvFrom", (PROC)hook_WSARecvFrom);
///////////////////////////////////////////
// IPPackLib.h文件


#ifndef __IPPACKLIB_H__
#define __IPPACKLIB_H__

struct CMessageData
{
	DWORD dwThreadId;	// 当前的线程ID号
	SOCKET socket;		// 进行网络操作的套节字句柄
	int nDataLength;	// 真实数据的长度
	// TCHAR[nDataLength]

	TCHAR* data()           // 返回真正的数据
	{ return (TCHAR*)(this+1); }
};

enum HOOK_MESSAGE		// 两个自定义消息
{
	HM_RECEIVE = WM_USER + 100,	// 说明目标进程接受到数据
	HM_SEND				// 说明目标进程要发送数据
};


#endif // __IPPACKLIB_H__

///////////////////////////////////////////////////////////////
// PriorityDemo.cpp文件



#include <stdio.h>
#include <windows.h>

DWORD WINAPI ThreadIdle(LPVOID lpParam)
{
	int i = 0;
	while(i++<10)
		printf("Idle Thread is running \n");

	return 0;
}

DWORD WINAPI ThreadNormal(LPVOID lpParam)
{
	int i = 0;
	while(i++<10)
		printf(" Normal Thread is running \n");

	return 0;
}
int main(int argc, char* argv[])
{
	DWORD dwThreadID;
	HANDLE h[2];
	
	// 创建一个优先级为Idle的线程
	h[0] = ::CreateThread(NULL, 0, ThreadIdle, NULL,
		CREATE_SUSPENDED, &dwThreadID);
	::SetThreadPriority(h[0], THREAD_PRIORITY_IDLE);
	::ResumeThread(h[0]);

	// 创建一个优先级为Normal的线程
	h[1] = ::CreateThread(NULL, 0, ThreadNormal, NULL,
		0, &dwThreadID);
	
	// 等待两个线程内核对象都变成受信状态
	::WaitForMultipleObjects(
		2,	    // DWORD nCount  要等待的内核对象的数量
		h,	    // CONST HANDLE *lpHandles 句柄数组
		TRUE,	    // BOOL bWaitAll	指定是否等待所有内核对象变成受信状态
		INFINITE);  // DWORD dwMilliseconds 要等待的时间
   	
	::CloseHandle(h[0]);
	::CloseHandle(h[1]);

	return 0;
}

/*
	HANDLE h[2];
	h[0] = hThread1;
	h[1] = hThread2;
	DWORD dw = ::WaitForMultipleObjects(2, h, FALSE, 5000);
	switch(dw)
	{
	case WAIT_FAILED:
		// 调用WaitForMultipleObjects函数失败(句柄无效？)
		break;
	case WAIT_TIMEOUT:
		// 在5秒内没有一个内核对象受信
		break;
	case WAIT_OBJECT_0 + 0:
		// 句柄h[0]对应的内核对象受信
		break;
	case WAIT_OBJECT_0 + 1:
		// 句柄h[1]对应的内核对象受信
		break;
	}
*/
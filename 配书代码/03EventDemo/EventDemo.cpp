///////////////////////////////////////////////////////////////
// EventDemo.cpp文件


#include <stdio.h>
#include <windows.h>
#include <process.h>

HANDLE g_hEvent;
UINT __stdcall ChildFunc(LPVOID);

int main(int argc, char* argv[])
{
	HANDLE hChildThread;
	UINT uId;

	// 创建一个自动重置的（auto-reset events），未受信的（nonsignaled）事件内核对象
	g_hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);

	hChildThread = (HANDLE)::_beginthreadex(NULL, 0, ChildFunc, NULL, 0, &uId);

	// 通知子线程开始工作
	printf("Please input a char to tell the Child Thread to work: \n");
	getchar();
	::SetEvent(g_hEvent);

	// 等待子线程完成工作，释放资源
	::WaitForSingleObject(hChildThread, INFINITE);
	printf("All the work has been finished. \n");
	::CloseHandle(hChildThread);
	::CloseHandle(g_hEvent);
	return 0;
}

UINT __stdcall ChildFunc(LPVOID)
{
	::WaitForSingleObject(g_hEvent, INFINITE);
	printf("  Child thread is working...... \n");
	::Sleep(5*1000); // 暂停5秒，模拟真正的工作
	return 0;
}
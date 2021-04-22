///////////////////////////////////////////////////////////////
// CriticalSection.cpp文件

#include <stdio.h>
#include <windows.h>
#include <process.h>

BOOL g_bContinue = TRUE;
int g_nCount1 = 0;
int g_nCount2 = 0;
CRITICAL_SECTION g_cs; // 对存在同步问题的代码段使用临界区对象

UINT __stdcall ThreadFunc(LPVOID);

int main(int argc, char* argv[])
{
	UINT uId;
	HANDLE h[2];

	// 初始化临界区对象
	::InitializeCriticalSection(&g_cs);

	h[0] = (HANDLE)::_beginthreadex(NULL, 0, ThreadFunc, NULL, 0, &uId);
	h[1] = (HANDLE)::_beginthreadex(NULL, 0, ThreadFunc, NULL, 0, &uId);

	// 等待1秒后通知两个计数线程结束，关闭句柄
	Sleep(1000);
	g_bContinue = FALSE;
	::WaitForMultipleObjects(2, h, TRUE, INFINITE);
	::CloseHandle(h[0]);
	::CloseHandle(h[1]);

	// 删除临界区对象
	::DeleteCriticalSection(&g_cs);
	
	printf("g_nCount1 = %d \n", g_nCount1);
	printf("g_nCount2 = %d \n", g_nCount2);

	return 0;
}

UINT __stdcall ThreadFunc(LPVOID)
{
	while(g_bContinue)
	{
		::EnterCriticalSection(&g_cs);
		g_nCount1++;
		g_nCount2++;
		::LeaveCriticalSection(&g_cs);
	}
	return 0;
}
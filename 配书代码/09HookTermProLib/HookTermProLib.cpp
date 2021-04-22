//////////////////////////////////////////////////
// HookTermProLib.cpp文件


#include <windows.h>
#include "APIHook.h"

extern CAPIHook g_TerminateProcess;

// 自定义TerminateProcess函数
BOOL WINAPI Hook_TerminateProcess(HANDLE hProcess, UINT uExitCode)
{
	typedef BOOL (WINAPI *PFNTERMINATEPROCESS)(HANDLE, UINT);

	// 取得主模块的文件名称
	char szPathName[MAX_PATH];
	::GetModuleFileName(NULL, szPathName, MAX_PATH);

	// 构建发送给主窗口的字符串
	char sz[2048];
	wsprintf(sz, "\r\n 进程：（%d）%s\r\n\r\n 进程句柄：%X\r\n 退出代码：%d",
		::GetCurrentProcessId(), szPathName, hProcess, uExitCode);

	// 发送这个字符串到主对话框
	COPYDATASTRUCT cds = { ::GetCurrentProcessId(), strlen(sz) + 1, sz };
	if(::SendMessage(::FindWindow(NULL, "进程保护器"), WM_COPYDATA, 0, (LPARAM)&cds) != -1)
	{
		// 如果函数的返回值不是－1，我们就允许API函数执行
		return ((PFNTERMINATEPROCESS)(PROC)g_TerminateProcess)(hProcess, uExitCode);
	}
	return TRUE;
}

// 挂钩TerminateProcess函数
CAPIHook g_TerminateProcess("kernel32.dll", "TerminateProcess", 
						(PROC)Hook_TerminateProcess);


///////////////////////////////////////////////////////////////////////////

#pragma data_seg("YCIShared")
HHOOK g_hHook = NULL;
#pragma data_seg()

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

BOOL WINAPI SetSysHook(BOOL bInstall, DWORD dwThreadId)
{
	BOOL bOk;
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






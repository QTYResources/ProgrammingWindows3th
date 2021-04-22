//////////////////////////////////////////////////
// HookTermProLib.cpp�ļ�


#include <windows.h>
#include "APIHook.h"

extern CAPIHook g_TerminateProcess;

// �Զ���TerminateProcess����
BOOL WINAPI Hook_TerminateProcess(HANDLE hProcess, UINT uExitCode)
{
	typedef BOOL (WINAPI *PFNTERMINATEPROCESS)(HANDLE, UINT);

	// ȡ����ģ����ļ�����
	char szPathName[MAX_PATH];
	::GetModuleFileName(NULL, szPathName, MAX_PATH);

	// �������͸������ڵ��ַ���
	char sz[2048];
	wsprintf(sz, "\r\n ���̣���%d��%s\r\n\r\n ���̾����%X\r\n �˳����룺%d",
		::GetCurrentProcessId(), szPathName, hProcess, uExitCode);

	// ��������ַ��������Ի���
	COPYDATASTRUCT cds = { ::GetCurrentProcessId(), strlen(sz) + 1, sz };
	if(::SendMessage(::FindWindow(NULL, "���̱�����"), WM_COPYDATA, 0, (LPARAM)&cds) != -1)
	{
		// ��������ķ���ֵ���ǣ�1�����Ǿ�����API����ִ��
		return ((PFNTERMINATEPROCESS)(PROC)g_TerminateProcess)(hProcess, uExitCode);
	}
	return TRUE;
}

// �ҹ�TerminateProcess����
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






////////////////////////////////////////////////
// KeyHookLib.cpp�ļ�

#include <windows.h>

#define KEYHOOKLIB_EXPORTS
#include "KeyHookLib.h"


// �������ݶ�
#pragma data_seg("YCIShared")
HWND g_hWndCaller = NULL;	// ���������ھ��
HHOOK g_hHook = NULL;		// ���湳�Ӿ��
#pragma data_seg()

// һ��ͨ���ڴ��ַȡ��ģ�����İ�������
HMODULE WINAPI ModuleFromAddress(PVOID pv) 
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

// ���̹��Ӻ���
LRESULT CALLBACK KeyHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
        if(nCode < 0 || nCode == HC_NOREMOVE)
		return ::CallNextHookEx(g_hHook, nCode, wParam, lParam);
	
        if(lParam & 0x40000000)	// ��Ϣ�ظ��ͽ�����һ��hook��
	{
		return ::CallNextHookEx(g_hHook, nCode, wParam, lParam);
	}
	
	// ֪ͨ�����ڡ�wParam����Ϊ�������, lParam���������˴˼�����Ϣ
        ::PostMessage(g_hWndCaller, HM_KEY, wParam, lParam);
	
        return ::CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

// ��װ��ж�ع��ӵĺ���
BOOL WINAPI SetKeyHook(BOOL bInstall, DWORD dwThreadId, HWND hWndCaller)
{
	BOOL bOk;
	g_hWndCaller = hWndCaller;
	
	if(bInstall)
	{
		g_hHook = ::SetWindowsHookEx(WH_KEYBOARD, KeyHookProc, 
					ModuleFromAddress(KeyHookProc), dwThreadId);
		bOk = (g_hHook != NULL);
	} 
	else 
	{
		bOk = ::UnhookWindowsHookEx(g_hHook);
		g_hHook = NULL;
	}
	
	return bOk;
}
// 09DllDemo.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "09DllDemo.h"
#include <stdio.h>

HMODULE g_hModule;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_hModule = (HMODULE)hModule; // ����ģ����
		break;
	}
	return TRUE;
}

// �Զ��嵼������
void ExportFunc(LPCTSTR pszContent)
{
	char sz[MAX_PATH];
	::GetModuleFileName(g_hModule, sz, MAX_PATH);
	::MessageBox(NULL, pszContent, strrchr(sz, '\\') + 1, MB_OK);
}

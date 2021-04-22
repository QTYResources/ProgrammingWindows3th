////////////////////////////////////
// 09HookDemo.cpp�ļ�



#include <windows.h>
#include <stdio.h>

// �ҹ�ָ��ģ��hMod��MessageBoxA�ĵ���
BOOL SetHook(HMODULE hMod);
// ����MessageBoxA����ԭ��
typedef int (WINAPI *PFNMESSAGEBOX)(HWND, LPCSTR, LPCSTR, UINT uType);
// ����MessageBoxA��������ʵ��ַ
PROC g_orgProc = (PROC)MessageBoxA;

void main()
{
	// ����ԭAPI����
	::MessageBox(NULL, "ԭ����", "09HookDemo", 0);
	// �ҹ����ٵ���
	SetHook(::GetModuleHandle(NULL));
	::MessageBox(NULL, "ԭ����", "09HookDemo", 0);
}

// �����滻MessageBoxA���Զ��庯��
int WINAPI MyMessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
	return ((PFNMESSAGEBOX)g_orgProc)(hWnd, "�º���", "09HookDemo", uType);
}

BOOL SetHook(HMODULE hMod)
{
	IMAGE_DOS_HEADER* pDosHeader = (IMAGE_DOS_HEADER*)hMod;
	IMAGE_OPTIONAL_HEADER * pOptHeader =
		(IMAGE_OPTIONAL_HEADER *)((BYTE*)hMod + pDosHeader->e_lfanew + 24);
	
	IMAGE_IMPORT_DESCRIPTOR* pImportDesc = (IMAGE_IMPORT_DESCRIPTOR*)
		((BYTE*)hMod + pOptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
	
	// �ڵ�����в���user32.dllģ�顣��ΪMessageBoxA������user32.dllģ�鵼��
	while(pImportDesc->FirstThunk)
	{
		char* pszDllName = (char*)((BYTE*)hMod + pImportDesc->Name);
		if(lstrcmpiA(pszDllName, "user32.dll") == 0)
		{
			break;
		}
		pImportDesc++;
	}

	if(pImportDesc->FirstThunk)
	{
		
		// һ��IMAGE_THUNK_DATA����һ��˫�֣���ָ����һ�����뺯��
		// �����ַ����ʵ��IMAGE_THUNK_DATA�ṹ�����飬Ҳ����DWORD����
		IMAGE_THUNK_DATA* pThunk = (IMAGE_THUNK_DATA*)
				((BYTE*)hMod + pImportDesc->FirstThunk);
		while(pThunk->u1.Function)
		{
			// lpAddrָ����ڴ汣���˺����ĵ�ַ
			DWORD* lpAddr = (DWORD*)&(pThunk->u1.Function);
			if(*lpAddr == (DWORD)g_orgProc)
			{	
				// �޸�IAT���ʹ��ָ�������Զ���ĺ������൱�ڡ�*lpAddr = (DWORD)MyMessageBoxA;��
				DWORD* lpNewProc = (DWORD*)MyMessageBoxA;
				::WriteProcessMemory(::GetCurrentProcess(), 
						lpAddr, &lpNewProc, sizeof(DWORD), NULL);
				return TRUE;
			}
			
			pThunk++;
		}
	}
	return FALSE;
}



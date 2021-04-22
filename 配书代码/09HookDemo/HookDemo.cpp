////////////////////////////////////
// 09HookDemo.cpp文件



#include <windows.h>
#include <stdio.h>

// 挂钩指定模块hMod对MessageBoxA的调用
BOOL SetHook(HMODULE hMod);
// 定义MessageBoxA函数原型
typedef int (WINAPI *PFNMESSAGEBOX)(HWND, LPCSTR, LPCSTR, UINT uType);
// 保存MessageBoxA函数的真实地址
PROC g_orgProc = (PROC)MessageBoxA;

void main()
{
	// 调用原API函数
	::MessageBox(NULL, "原函数", "09HookDemo", 0);
	// 挂钩后再调用
	SetHook(::GetModuleHandle(NULL));
	::MessageBox(NULL, "原函数", "09HookDemo", 0);
}

// 用于替换MessageBoxA的自定义函数
int WINAPI MyMessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
	return ((PFNMESSAGEBOX)g_orgProc)(hWnd, "新函数", "09HookDemo", uType);
}

BOOL SetHook(HMODULE hMod)
{
	IMAGE_DOS_HEADER* pDosHeader = (IMAGE_DOS_HEADER*)hMod;
	IMAGE_OPTIONAL_HEADER * pOptHeader =
		(IMAGE_OPTIONAL_HEADER *)((BYTE*)hMod + pDosHeader->e_lfanew + 24);
	
	IMAGE_IMPORT_DESCRIPTOR* pImportDesc = (IMAGE_IMPORT_DESCRIPTOR*)
		((BYTE*)hMod + pOptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
	
	// 在导入表中查找user32.dll模块。因为MessageBoxA函数从user32.dll模块导出
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
		
		// 一个IMAGE_THUNK_DATA就是一个双字，它指定了一个导入函数
		// 调入地址表其实是IMAGE_THUNK_DATA结构的数组，也就是DWORD数组
		IMAGE_THUNK_DATA* pThunk = (IMAGE_THUNK_DATA*)
				((BYTE*)hMod + pImportDesc->FirstThunk);
		while(pThunk->u1.Function)
		{
			// lpAddr指向的内存保存了函数的地址
			DWORD* lpAddr = (DWORD*)&(pThunk->u1.Function);
			if(*lpAddr == (DWORD)g_orgProc)
			{	
				// 修改IAT表项，使其指向我们自定义的函数，相当于“*lpAddr = (DWORD)MyMessageBoxA;”
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



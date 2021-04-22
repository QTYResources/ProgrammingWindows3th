//////////////////////////////////////////////////
// ImportTable.cpp文件


#include <stdio.h>
#include <windows.h>

void main()	// 09ImportTable
{
	HMODULE hMod = ::GetModuleHandle(NULL);

	IMAGE_DOS_HEADER* pDosHeader = (IMAGE_DOS_HEADER*)hMod;
	IMAGE_OPTIONAL_HEADER * pOptHeader =
		(IMAGE_OPTIONAL_HEADER *)((BYTE*)hMod + pDosHeader->e_lfanew + 24);

	IMAGE_IMPORT_DESCRIPTOR* pImportDesc = (IMAGE_IMPORT_DESCRIPTOR*)
		((BYTE*)hMod + pOptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

	while(pImportDesc->FirstThunk)
	{
		char* pszDllName = (char*)((BYTE*)hMod +pImportDesc->Name);
		printf("\n模块名称：%s \n", pszDllName);
	
		
		// 一个IMAGE_THUNK_DATA就是一个双字，它指定了一个导入函数
		IMAGE_THUNK_DATA* pThunk = (IMAGE_THUNK_DATA*)
			((BYTE*)hMod + pImportDesc->OriginalFirstThunk);
		int n = 0;
		while(pThunk->u1.Function)
		{
			// 取得函数名称。hint/name表前两个字节是函数的序号，后4个字节是函数名称字符串的地址
			char* pszFunName = (char*)
				((BYTE*)hMod + (DWORD)pThunk->u1.AddressOfData + 2);
			// 取得函数地址。IAT表就是一个DWORD类型的数组，每个成员记录一个函数的地址
			PDWORD lpAddr = (DWORD*)((BYTE*)hMod + pImportDesc->FirstThunk) + n;

			// 打印出函数名称和地址
			printf("  从此模块导入的函数：%-25s，", pszFunName);
			printf("函数地址：%X \n", lpAddr);
			n++; pThunk++;
		}
		
		pImportDesc++;
	}
}



/*
struct CAPISpyData
{
	char szModName[256];
	char szFuncName[256];
	HWND hWndCaller;
};

#define SHAREMEM "APISpyLib"

void main()
{

	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	::CreateProcess(NULL, "notepad.exe", NULL, NULL, FALSE, 
							0, NULL, NULL, &si, &pi);

	return ;

	char szDll[] = "E:\\MyWork\\book_code\\09APISpyLib\\Debug\\09APISpyLib.dll";
	CShareMemory* m_pShareMem = new CShareMemory(SHAREMEM, sizeof(CAPISpyData), TRUE);
	CAPISpyData* pData = (CAPISpyData*)m_pShareMem->GetBuffer();

	pData->hWndCaller = NULL;
	strcpy(pData->szFuncName, "MessageBoxA");
	strcpy(pData->szModName, "User32.dll");


	CRemThreadInjector injector(szDll);

	injector.InjectModuleInto(1100);

	getchar();

	injector.EjectModuleFrom(1100);


	return;
	HMODULE h = ::LoadLibrary(szDll);

	::MessageBox(0, "ddd", "ddddddddddd", 0);

	::FreeLibrary(h);

	::GetProcAddress(h, 0);


}
*/


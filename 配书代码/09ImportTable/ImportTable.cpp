//////////////////////////////////////////////////
// ImportTable.cpp�ļ�


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
		printf("\nģ�����ƣ�%s \n", pszDllName);
	
		
		// һ��IMAGE_THUNK_DATA����һ��˫�֣���ָ����һ�����뺯��
		IMAGE_THUNK_DATA* pThunk = (IMAGE_THUNK_DATA*)
			((BYTE*)hMod + pImportDesc->OriginalFirstThunk);
		int n = 0;
		while(pThunk->u1.Function)
		{
			// ȡ�ú������ơ�hint/name��ǰ�����ֽ��Ǻ�������ţ���4���ֽ��Ǻ��������ַ����ĵ�ַ
			char* pszFunName = (char*)
				((BYTE*)hMod + (DWORD)pThunk->u1.AddressOfData + 2);
			// ȡ�ú�����ַ��IAT�����һ��DWORD���͵����飬ÿ����Ա��¼һ�������ĵ�ַ
			PDWORD lpAddr = (DWORD*)((BYTE*)hMod + pImportDesc->FirstThunk) + n;

			// ��ӡ���������ƺ͵�ַ
			printf("  �Ӵ�ģ�鵼��ĺ�����%-25s��", pszFunName);
			printf("������ַ��%X \n", lpAddr);
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


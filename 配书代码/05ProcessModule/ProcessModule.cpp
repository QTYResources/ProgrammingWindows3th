///////////////////////////////////////////////////////////////
// ProcessModule.cpp文件


#include <iostream.h>
#include <windows.h>
#include <tlhelp32.h>

void main()
{
	MODULEENTRY32 me32 = { 0 };
	
	// 在本进程中拍一个所有模块的快照
	HANDLE hModuleSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0);
	if(hModuleSnap == INVALID_HANDLE_VALUE)
		return;

	// 遍历快照中记录的模块
	me32.dwSize = sizeof(MODULEENTRY32);
	if(::Module32First(hModuleSnap, &me32))
	{
		do
		{
			cout << me32.szExePath << "\n";
			cout << "	模块在本进程中的地址：" << me32.hModule << "\n";
		}
		while(::Module32Next(hModuleSnap, &me32));
	}
	::CloseHandle(hModuleSnap);
}
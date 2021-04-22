///////////////////////////////////////////////////////////////
// ProcessModule.cpp�ļ�


#include <iostream.h>
#include <windows.h>
#include <tlhelp32.h>

void main()
{
	MODULEENTRY32 me32 = { 0 };
	
	// �ڱ���������һ������ģ��Ŀ���
	HANDLE hModuleSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0);
	if(hModuleSnap == INVALID_HANDLE_VALUE)
		return;

	// ���������м�¼��ģ��
	me32.dwSize = sizeof(MODULEENTRY32);
	if(::Module32First(hModuleSnap, &me32))
	{
		do
		{
			cout << me32.szExePath << "\n";
			cout << "	ģ���ڱ������еĵ�ַ��" << me32.hModule << "\n";
		}
		while(::Module32Next(hModuleSnap, &me32));
	}
	::CloseHandle(hModuleSnap);
}
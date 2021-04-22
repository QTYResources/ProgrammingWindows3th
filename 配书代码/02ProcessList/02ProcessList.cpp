///////////////////////////////////////////////////////////////
// 02ProcessList.cpp�ļ�


#include "stdafx.h"
#include <windows.h>
#include <tlhelp32.h> // �������պ�����ͷ�ļ�

int main(int argc, char* argv[])
{
	PROCESSENTRY32 pe32;
	// ��ʹ������ṹ֮ǰ�����������Ĵ�С
	pe32.dwSize = sizeof(pe32); 
	
	// ��ϵͳ�ڵ����н�����һ������
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hProcessSnap == INVALID_HANDLE_VALUE)
	{
		printf(" CreateToolhelp32Snapshot����ʧ�ܣ� \n");
		return -1;
	}
	
	// �������̿��գ�������ʾÿ�����̵���Ϣ
	BOOL bMore = ::Process32First(hProcessSnap, &pe32);
	while(bMore)
	{
		printf(" �������ƣ�%s \n", pe32.szExeFile);
		printf(" ����ID�ţ�%u \n\n", pe32.th32ProcessID);

		bMore = ::Process32Next(hProcessSnap, &pe32);
	}

	// ��Ҫ���������snapshot����
	::CloseHandle(hProcessSnap);
	return 0;
}


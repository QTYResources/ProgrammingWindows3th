///////////////////////////////////////////////////////////////
// 02TerminateProcess.cpp�ļ�


#include "stdafx.h"
#include <windows.h>

BOOL TerminateProcessFromId(DWORD dwId)
{
	BOOL bRet = FALSE;
	// ��Ŀ����̣�ȡ�ý��̾��
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwId);
	if(hProcess != NULL)
	{
		// ��ֹ����
		bRet = ::TerminateProcess(hProcess, 0);
	}
	CloseHandle(hProcess);
	return bRet;
}

int main(int argc, char* argv[])
{
	DWORD dwId;
	printf(" ��������Ҫ��ֹ�Ľ��̵�ID�ţ� \n");
	scanf("%u", &dwId);
	if(TerminateProcessFromId(dwId))
	{
		printf(" ��ֹ���̳ɹ��� \n");
	}
	else
	{
		printf(" ��ֹ����ʧ�ܣ� \n");
	}

	return 0;
}





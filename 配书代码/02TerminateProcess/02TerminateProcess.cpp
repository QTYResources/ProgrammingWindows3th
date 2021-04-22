///////////////////////////////////////////////////////////////
// 02TerminateProcess.cpp文件


#include "stdafx.h"
#include <windows.h>

BOOL TerminateProcessFromId(DWORD dwId)
{
	BOOL bRet = FALSE;
	// 打开目标进程，取得进程句柄
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwId);
	if(hProcess != NULL)
	{
		// 终止进程
		bRet = ::TerminateProcess(hProcess, 0);
	}
	CloseHandle(hProcess);
	return bRet;
}

int main(int argc, char* argv[])
{
	DWORD dwId;
	printf(" 请输入您要终止的进程的ID号： \n");
	scanf("%u", &dwId);
	if(TerminateProcessFromId(dwId))
	{
		printf(" 终止进程成功！ \n");
	}
	else
	{
		printf(" 终止进程失败！ \n");
	}

	return 0;
}





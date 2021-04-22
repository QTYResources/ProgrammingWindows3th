///////////////////////////////////////////////////////////////
// 02MemRepair.cpp文件

#include "stdafx.h"
#include "windows.h"
#include "stdio.h"
#include <iostream.h>


BOOL FindFirst(DWORD dwValue);	// 在目标进程空间进行第一次查找
BOOL FindNext(DWORD dwValue);	// 在目标进程地址空间进行第2、3、4……次查找

DWORD g_arList[1024];		// 地址列表
int g_nListCnt;			// 有效地址的个数
HANDLE g_hProcess;		// 目标进程句柄


//////////////////////

BOOL WriteMemory(DWORD dwAddr, DWORD dwValue);
void ShowList();


int main(int argc, char* argv[])
{
	// 启动02testor进程
	char szFileName[] = "..\\02testor\\debug\\02testor.exe";
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	::CreateProcess(NULL, szFileName, NULL, NULL, FALSE, 
							CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
	// 关闭线程句柄，既然我们不使用它
	::CloseHandle(pi.hThread);
	g_hProcess = pi.hProcess;

	// 输入要修改的值
	int	iVal;
	printf(" Input val = ");
	scanf("%d", &iVal);

	// 进行第一次查找
	FindFirst(iVal);
	
	// 打印出搜索的结果
	ShowList();


	while(g_nListCnt > 1)
	{
		printf(" Input val = ");
		scanf("%d", &iVal);

		// 进行下次搜索
		FindNext(iVal);

		// 显示搜索结果
		ShowList();
	}


	// 取得新值
	printf(" New value = ");
	scanf("%d", &iVal);	     

	// 写入新值
	if(WriteMemory(g_arList[0], iVal))
		printf(" Write data success \n");	


	::CloseHandle(g_hProcess);
	return 0;
}

BOOL CompareAPage(DWORD dwBaseAddr, DWORD dwValue)
{
	// 读取1页内存
	BYTE arBytes[4096];
	if(!::ReadProcessMemory(g_hProcess, (LPVOID)dwBaseAddr, arBytes, 4096, NULL))
		return FALSE;	// 此页不可读

	// 在这1页内存中查找
	DWORD* pdw;
	for(int i=0; i<(int)4*1024-3; i++)
	{
		pdw = (DWORD*)&arBytes[i];
		if(pdw[0] == dwValue)	// 等于要查找的值？
		{
			if(g_nListCnt >= 1024)
				return FALSE;
			// 添加到全局变量中
			g_arList[g_nListCnt++] = dwBaseAddr + i;
		}
	}

	return TRUE;
}

BOOL FindFirst(DWORD dwValue)
{
	const DWORD dwOneGB = 1024*1024*1024;	// 1GB
	const DWORD dwOnePage = 4*1024;		// 4KB

	if(g_hProcess == NULL)
		return FALSE;
	
	// 查看操作系统类型，以决定开始地址
	DWORD dwBase;
	OSVERSIONINFO vi = { sizeof(vi) };
	::GetVersionEx(&vi);
	if (vi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
		dwBase = 4*1024*1024;		// Windows 98系列，4MB	
	else
		dwBase = 640*1024;		// Windows NT系列，64KB

	// 在开始地址到2GB的地址空间进行查找
	for(; dwBase < 2*dwOneGB; dwBase += dwOnePage)
	{
		// 比较1页大小的内存
		CompareAPage(dwBase, dwValue);
	}

	return TRUE;
}

BOOL FindNext(DWORD dwValue)
{
	// 保存m_arList数组中有效地址的个数，初始化新的m_nListCnt值
	int nOrgCnt = g_nListCnt;
	g_nListCnt = 0;	

	// 在m_arList数组记录的地址处查找
	BOOL bRet = FALSE;	// 假设失败	
	DWORD dwReadValue;
	for(int i=0; i<nOrgCnt; i++)
	{
		if(::ReadProcessMemory(g_hProcess, (LPVOID)g_arList[i], &dwReadValue, sizeof(DWORD), NULL))
		{
			if(dwReadValue == dwValue)
			{
				g_arList[g_nListCnt++] = g_arList[i];
				bRet = TRUE;
			}
		}
	}
	
	return bRet;
}

// 打印出搜索到的地址
void ShowList()
{
	for(int i=0; i< g_nListCnt; i++)
	{
		printf("%08lX \n", g_arList[i]);
	}
}

BOOL WriteMemory(DWORD dwAddr, DWORD dwValue)
{
	return ::WriteProcessMemory(g_hProcess, (LPVOID)dwAddr, &dwValue, sizeof(DWORD), NULL);
}
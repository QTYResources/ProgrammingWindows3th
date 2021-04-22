///////////////////////////////////////////////////////////////
// 02MemRepair.cpp�ļ�

#include "stdafx.h"
#include "windows.h"
#include "stdio.h"
#include <iostream.h>


BOOL FindFirst(DWORD dwValue);	// ��Ŀ����̿ռ���е�һ�β���
BOOL FindNext(DWORD dwValue);	// ��Ŀ����̵�ַ�ռ���е�2��3��4�����β���

DWORD g_arList[1024];		// ��ַ�б�
int g_nListCnt;			// ��Ч��ַ�ĸ���
HANDLE g_hProcess;		// Ŀ����̾��


//////////////////////

BOOL WriteMemory(DWORD dwAddr, DWORD dwValue);
void ShowList();


int main(int argc, char* argv[])
{
	// ����02testor����
	char szFileName[] = "..\\02testor\\debug\\02testor.exe";
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	::CreateProcess(NULL, szFileName, NULL, NULL, FALSE, 
							CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
	// �ر��߳̾������Ȼ���ǲ�ʹ����
	::CloseHandle(pi.hThread);
	g_hProcess = pi.hProcess;

	// ����Ҫ�޸ĵ�ֵ
	int	iVal;
	printf(" Input val = ");
	scanf("%d", &iVal);

	// ���е�һ�β���
	FindFirst(iVal);
	
	// ��ӡ�������Ľ��
	ShowList();


	while(g_nListCnt > 1)
	{
		printf(" Input val = ");
		scanf("%d", &iVal);

		// �����´�����
		FindNext(iVal);

		// ��ʾ�������
		ShowList();
	}


	// ȡ����ֵ
	printf(" New value = ");
	scanf("%d", &iVal);	     

	// д����ֵ
	if(WriteMemory(g_arList[0], iVal))
		printf(" Write data success \n");	


	::CloseHandle(g_hProcess);
	return 0;
}

BOOL CompareAPage(DWORD dwBaseAddr, DWORD dwValue)
{
	// ��ȡ1ҳ�ڴ�
	BYTE arBytes[4096];
	if(!::ReadProcessMemory(g_hProcess, (LPVOID)dwBaseAddr, arBytes, 4096, NULL))
		return FALSE;	// ��ҳ���ɶ�

	// ����1ҳ�ڴ��в���
	DWORD* pdw;
	for(int i=0; i<(int)4*1024-3; i++)
	{
		pdw = (DWORD*)&arBytes[i];
		if(pdw[0] == dwValue)	// ����Ҫ���ҵ�ֵ��
		{
			if(g_nListCnt >= 1024)
				return FALSE;
			// ��ӵ�ȫ�ֱ�����
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
	
	// �鿴����ϵͳ���ͣ��Ծ�����ʼ��ַ
	DWORD dwBase;
	OSVERSIONINFO vi = { sizeof(vi) };
	::GetVersionEx(&vi);
	if (vi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
		dwBase = 4*1024*1024;		// Windows 98ϵ�У�4MB	
	else
		dwBase = 640*1024;		// Windows NTϵ�У�64KB

	// �ڿ�ʼ��ַ��2GB�ĵ�ַ�ռ���в���
	for(; dwBase < 2*dwOneGB; dwBase += dwOnePage)
	{
		// �Ƚ�1ҳ��С���ڴ�
		CompareAPage(dwBase, dwValue);
	}

	return TRUE;
}

BOOL FindNext(DWORD dwValue)
{
	// ����m_arList��������Ч��ַ�ĸ�������ʼ���µ�m_nListCntֵ
	int nOrgCnt = g_nListCnt;
	g_nListCnt = 0;	

	// ��m_arList�����¼�ĵ�ַ������
	BOOL bRet = FALSE;	// ����ʧ��	
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

// ��ӡ���������ĵ�ַ
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
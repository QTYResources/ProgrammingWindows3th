///////////////////////////////////////////////////////////////
// 02CreateProcess.cpp�ļ�

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	char szCommandLine[] = "cmd";
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;

	si.dwFlags = STARTF_USESHOWWINDOW;	// ָ��wShowWindow��Ա��Ч
	si.wShowWindow = TRUE;			// �˳�Ա��ΪTRUE�Ļ�����ʾ�½����̵������ڣ�
						// ΪFALSE�Ļ�����ʾ
	BOOL bRet = ::CreateProcess (
		NULL,			// ���ڴ�ָ����ִ���ļ����ļ���
		szCommandLine,		// �����в���
		NULL,			// Ĭ�Ͻ��̰�ȫ��
		NULL,			// Ĭ���̰߳�ȫ��
		FALSE,			// ָ����ǰ�����ڵľ�������Ա��ӽ��̼̳�
		CREATE_NEW_CONSOLE,	// Ϊ�½��̴���һ���µĿ���̨����
		NULL,			// ʹ�ñ����̵Ļ�������
		NULL,			// ʹ�ñ����̵���������Ŀ¼
		&si,
		&pi);

	if(bRet)
	{
		// ��Ȼ���ǲ�ʹ�������������������̽����ǹر�
		::CloseHandle (pi.hThread);
		::CloseHandle (pi.hProcess);

		printf(" �½��̵Ľ���ID�ţ�%d \n", pi.dwProcessId);
		printf(" �½��̵����߳�ID�ţ�%d \n", pi.dwThreadId);	
	}
	return 0;
}

///////////////////////////////////////////////////////////////
// ThreadDemo.cpp�ļ�



#include <stdio.h>
#include <windows.h>

// �̺߳���
DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	int i = 0;
	while(i < 20)
	{
		printf(" I am from a thread, count = %d \n", i++);
	}
	return 0;
}

int  main(int argc, char* argv[])
{
	HANDLE hThread;
	DWORD dwThreadId;
	
	// ����һ���߳�
	hThread = ::CreateThread (
		NULL,		// Ĭ�ϰ�ȫ����
		NULL,		// Ĭ�϶�ջ��С
		ThreadProc,	// �߳���ڵ�ַ��ִ���̵߳ĺ�����
		NULL,		// ���������Ĳ���
		0,		// ָ���߳���������
		&dwThreadId);	// �����̵߳�ID��
	printf(" Now another thread has been created. ID = %d \n", dwThreadId);

	// �ȴ����߳����н���
	::WaitForSingleObject (hThread, INFINITE);
	::CloseHandle (hThread);
	return 0;
}


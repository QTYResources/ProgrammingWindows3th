///////////////////////////////////////////////////////////////
// 02UseTLS.cpp.cpp�ļ�


#include <stdio.h>
#include <windows.h>
#include <process.h>

// ����TLS��¼�̵߳�����ʱ��

DWORD g_tlsUsedTime;
void InitStartTime();
DWORD GetUsedTime();


UINT __stdcall ThreadFunc(LPVOID)
{
	int i;

	// ��ʼ����ʼʱ��
	InitStartTime();

	// ģ�ⳤʱ�乤��
	i = 10000*10000;
	while(i--) { }

	// ��ӡ�����߳����е�ʱ��
	printf(" This thread is coming to end. Thread ID: %-5d, Used Time: %d \n", 
						::GetCurrentThreadId(), GetUsedTime());
	return 0;
}

int main(int argc, char* argv[])
{
	UINT uId;
	int i;
	HANDLE h[10];

	// ͨ���ڽ���λ����������һ����������ʼ���߳�����ʱ���¼ϵͳ
	g_tlsUsedTime = ::TlsAlloc(); 

	// ��ʮ���߳�ͬʱ���У����ȴ����Ǹ��Ե�������
	for(i=0; i<10; i++)
	{
		h[i] = (HANDLE)::_beginthreadex(NULL, 0, ThreadFunc, NULL, 0, &uId);
	}
	for(i=0; i<10; i++)
	{
		::WaitForSingleObject(h[i], INFINITE);
		::CloseHandle(h[i]);
	}

	// ͨ���ͷ��ֲ߳̾��洢�������ͷ�ʱ���¼ϵͳռ�õ���Դ
	::TlsFree(g_tlsUsedTime);
	return 0;
}

// ��ʼ���̵߳Ŀ�ʼʱ��
void InitStartTime()
{
	// ��õ�ǰʱ�䣬���̵߳Ĵ���ʱ�����̶߳��������
	DWORD dwStart = ::GetTickCount();
	::TlsSetValue(g_tlsUsedTime, (LPVOID)dwStart);
}

// ȡ��һ���߳��Ѿ����е�ʱ��
DWORD GetUsedTime()
{
	// ��õ�ǰʱ�䣬���ص�ǰʱ����̴߳���ʱ��Ĳ�ֵ
	DWORD dwElapsed = ::GetTickCount();
	dwElapsed = dwElapsed - (DWORD)::TlsGetValue(g_tlsUsedTime);
	return dwElapsed;
}


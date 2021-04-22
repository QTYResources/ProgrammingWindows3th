///////////////////////////////////////////////////////////////
// EventDemo.cpp�ļ�


#include <stdio.h>
#include <windows.h>
#include <process.h>

HANDLE g_hEvent;
UINT __stdcall ChildFunc(LPVOID);

int main(int argc, char* argv[])
{
	HANDLE hChildThread;
	UINT uId;

	// ����һ���Զ����õģ�auto-reset events����δ���ŵģ�nonsignaled���¼��ں˶���
	g_hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);

	hChildThread = (HANDLE)::_beginthreadex(NULL, 0, ChildFunc, NULL, 0, &uId);

	// ֪ͨ���߳̿�ʼ����
	printf("Please input a char to tell the Child Thread to work: \n");
	getchar();
	::SetEvent(g_hEvent);

	// �ȴ����߳���ɹ������ͷ���Դ
	::WaitForSingleObject(hChildThread, INFINITE);
	printf("All the work has been finished. \n");
	::CloseHandle(hChildThread);
	::CloseHandle(g_hEvent);
	return 0;
}

UINT __stdcall ChildFunc(LPVOID)
{
	::WaitForSingleObject(g_hEvent, INFINITE);
	printf("  Child thread is working...... \n");
	::Sleep(5*1000); // ��ͣ5�룬ģ�������Ĺ���
	return 0;
}
///////////////////////////////////////////////////////////////
// MyThread.cppÎÄ¼þ


#include <stdio.h>
#include "../common/_afxwin.h"



UINT MyFunc(LPVOID lpParam)
{
	printf(" Thread Identify: %d \n", AfxGetThread()->m_nThreadID);
	return 0;
}

void main()
{
	for(int i=0; i<10; i++)
	{
		AfxBeginThread(MyFunc, NULL);
	}
	system("pause");
}



/*

UINT MyFunc(LPVOID lpParam)
{
	printf(" Thread Identify: %d \n", AfxGetThread()->m_nThreadID);
	return 0;
}

void main()
{
	CWinThread* pThread = AfxBeginThread (MyFunc, NULL,
				THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
//	pThread->m_bAutoDelete = FALSE;
	pThread->ResumeThread ();
	
	// Sometime later
	DWORD dwExitCode;
	::GetExitCodeThread (pThread->m_hThread, &dwExitCode);
	if (dwExitCode == STILL_ACTIVE) 
	{
		// The thread is still running.
	}
	else 
	{
		// The thread has terminated. Delete the CWinThread object.
		delete pThread;
	}

	system("pause");
}
*/







///////////////////////////////////////////////////////////////
// MyTls.cpp�ļ�

#include "../common/_afxtls_.h"
#include <process.h>

#include <iostream>
using namespace std;

struct CMyThreadData : public CNoTrackObject
{
	int nSomeData;
};

// ����Ĵ���չ�����൱�ڡ�CThreadLocal<CMyThreadData> g_myThreadData;��
THREAD_LOCAL(CMyThreadData, g_myThreadData)

void ShowData();
UINT __stdcall ThreadFunc(LPVOID lpParam)
{
	g_myThreadData->nSomeData = (int)lpParam;
	ShowData();
	return 0;
}

void main()
{
	HANDLE h[10];
	UINT uID;

	// ����ʮ���̣߳���i��Ϊ�̺߳����Ĳ�������ȥ
	for(int i=0; i<10; i++)
		h[i] = (HANDLE) ::_beginthreadex(NULL, 0, ThreadFunc, (void*)i, 0, &uID);
	::WaitForMultipleObjects(10, h, TRUE, INFINITE);
	for(i=0; i<10; i++)
		::CloseHandle(h[i]);
}

void ShowData()
{
	int nData = g_myThreadData->nSomeData;
	printf(" Thread ID: %-5d, nSomeData = %d \n", ::GetCurrentThreadId(), nData);
}



/*
2.3.1�еĴ��룺


struct MyThreadData
{
	MyThreadData* pNext;
	int nSomeData;
};

void main()
{
	MyThreadData* pData;
	CSimpleList list;
	list.Construct(offsetof(MyThreadData, pNext)); // ����CSimpleList��pNext��Ա��ƫ����

	// ����������ӳ�Ա
	for(int i=0; i<10; i++)
	{
		pData = new MyThreadData;
		pData->nSomeData = i;
		list.AddHead(pData);
	}

	// ��������   // ʹ�������е�����

	// �������������ͷ�MyThreadData����ռ�õĿռ�
	pData = (MyThreadData*)list.GetHead();
	while(pData != NULL)
	{
		MyThreadData* pNextData = pData->pNext;
		printf(" The value of nSomeData is: %d \n", pData->nSomeData);
		delete pData;
		pData = pNextData;
	}
}

*/


/*

// ʹ��CTypedSimpleList��ģ�������

struct MyThreadData
{
	MyThreadData* pNext;
	int nSomeData;
};

void main()
{
	MyThreadData* pData;
	CTypedSimpleList<MyThreadData*> list;
	list.Construct(offsetof(MyThreadData, pNext));

	// ����������ӳ�Ա
	for(int i=0; i<10; i++)
	{
		pData = new MyThreadData;
		pData->nSomeData = i;
		list.AddHead(pData);
	}

	// ��������   // ʹ�������е�����

	// �������������ͷ�MyThreadData����ռ�õĿռ�
	pData = list;  // �����˳�Ա���� operator TYPE()���൱�ڡ�pData = list.GetHead();�����
	while(pData != NULL)
	{
		MyThreadData* pNextData = pData->pNext;
		printf(" The value of nSomeData is: %d \n", pData->nSomeData);
		delete pData;
		pData = pNextData;
	}
}



*/
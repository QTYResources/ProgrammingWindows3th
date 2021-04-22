///////////////////////////////////////////////////////////////
// MyTls.cpp文件

#include "../common/_afxtls_.h"
#include <process.h>

#include <iostream>
using namespace std;

struct CMyThreadData : public CNoTrackObject
{
	int nSomeData;
};

// 下面的代码展开后相当于“CThreadLocal<CMyThreadData> g_myThreadData;”
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

	// 启动十个线程，将i做为线程函数的参数传过去
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
2.3.1中的代码：


struct MyThreadData
{
	MyThreadData* pNext;
	int nSomeData;
};

void main()
{
	MyThreadData* pData;
	CSimpleList list;
	list.Construct(offsetof(MyThreadData, pNext)); // 告诉CSimpleList类pNext成员的偏移量

	// 向链表中添加成员
	for(int i=0; i<10; i++)
	{
		pData = new MyThreadData;
		pData->nSomeData = i;
		list.AddHead(pData);
	}

	// …………   // 使用链表中的数据

	// 遍历整个链表，释放MyThreadData对象占用的空间
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

// 使用CTypedSimpleList类模板的例子

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

	// 向链表中添加成员
	for(int i=0; i<10; i++)
	{
		pData = new MyThreadData;
		pData->nSomeData = i;
		list.AddHead(pData);
	}

	// …………   // 使用链表中的数据

	// 遍历整个链表，释放MyThreadData对象占用的空间
	pData = list;  // 调用了成员函数 operator TYPE()，相当于“pData = list.GetHead();”语句
	while(pData != NULL)
	{
		MyThreadData* pNextData = pData->pNext;
		printf(" The value of nSomeData is: %d \n", pData->nSomeData);
		delete pData;
		pData = pNextData;
	}
}



*/
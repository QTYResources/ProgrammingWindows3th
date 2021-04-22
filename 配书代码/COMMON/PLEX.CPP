///////////////////////////////////////////////////////
// PLEX.CPP文件



#include "_afxplex_.h"

CPlex* CPlex::Create(CPlex*& pHead, UINT nMax, UINT cbElement)
{
	CPlex* p = (CPlex*)new BYTE[sizeof(CPlex) + nMax*cbElement];

	// 将新增加的内存块添加到链中，并将其地址做为首地址
	p->pNext = pHead;
	pHead = p;		// 以相反方向添加数据项的方式大大减化了程序设计
	return p;
}

void CPlex::FreeDataChain()
{
	// 以当前内存块的地址为首地址
	CPlex* p = this;
	// 释放链中所有内存块占用的内存
	while(p != NULL)
	{
		BYTE* pBytes = (BYTE*)p;
		CPlex* pNext = p->pNext;
		delete[] pBytes;
		p = pNext;
	}
}
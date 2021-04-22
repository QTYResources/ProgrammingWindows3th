///////////////////////////////////////////
// _AFXPLEX_.H文件

#ifndef __AFXPLEX_H__
#define __AFXPLEX_H__

#include "_afxwin.h"

struct CPlex
{
	CPlex* pNext; // 向每个内层块中添加的额外信息，指向下一个内存块首地址的指针

	// 这里是真正的数据区，BYTE data[maxNum*elementSize];
	void* data() { return this + 1; }

	// 用于申请内存的全局函数。申请cbElement大小的空间nMax个
	static CPlex* Create(CPlex*& pHead, UINT nMax, UINT cbElement);

	// 释放以当前对象为首地址（this指针）的内存链中的所有内存
	void FreeDataChain();
};

#endif // __AFXPLEX_H__
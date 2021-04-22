///////////////////////////////////////////////////////////////
// CPlexDemo.cpp文件

#include "../common/_afxplex_.h"  // 定义了CPlex结构

struct CMyData 			// 04CPlex工程下。你没有必要去做这个练习，懂得它的工作机制就行了，
{					// CPlex结构的实际应用在后面的类中会被体现出来
	int nSomeData;
	int nSomeMoreData;
};

void main()
{
	CPlex* pBlocks = NULL; // 用于保存链中第一个内存块的首地址，必须被初始化为NULL

	CPlex::Create(pBlocks, 10, sizeof(CMyData));
	CMyData* pData = (CMyData*)pBlocks->data();
	// 现在pData是CPlex::Create函数申请的10个CMyData结构的首地址
	//...		// 使用pData指向的内存

	// 使用完毕，继续申请
	CPlex::Create(pBlocks, 10, sizeof(CMyData));
	pData = (CMyData*)pBlocks->data();

	// 最后释放链中的所有内存块
	pBlocks->FreeDataChain();
}
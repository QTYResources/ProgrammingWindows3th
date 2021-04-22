///////////////////////////////////////////////////////////////
// OutputDebugString.cpp文件


#include "../common/_afx.h"

void main()
{
	// 向调试器输出字符串
	::OutputDebugString(" 在调试器窗口，你看到我了吗？ \n");
}

  
   

/* 
      
	CRuntimeClass* pClass = RUNTIME_CLASS(CObject);
	ASSERT(pClass != NULL);
	//...		使用pClass指针

  */
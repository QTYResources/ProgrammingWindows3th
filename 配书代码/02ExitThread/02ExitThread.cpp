///////////////////////////////////////////////////////////////
// 02ExitThread.cpp文件


#include "stdafx.h"
#include <windows.h>

class CMyClass
{
public:
	CMyClass() { printf(" Constructor\n"); }
	~CMyClass() { printf(" Destructor\n"); }
};

void main()
{
	CMyClass theObject;
	::ExitThread(0);

	// 在这个函数的结尾，编译器会自动添加一些必要的代码，
	// 来调用theObject的析构函数
}


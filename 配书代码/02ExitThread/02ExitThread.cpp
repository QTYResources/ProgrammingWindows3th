///////////////////////////////////////////////////////////////
// 02ExitThread.cpp�ļ�


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

	// ����������Ľ�β�����������Զ����һЩ��Ҫ�Ĵ��룬
	// ������theObject����������
}


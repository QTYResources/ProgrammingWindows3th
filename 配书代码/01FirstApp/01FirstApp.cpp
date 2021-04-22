///////////////////////////////////////////////////////////////
// 01FirstApp.cpp文件

#include "stdafx.h"	// 这是VC自动添加的头文件，没有什么用途
#include <windows.h>	// 包含MessageBox函数声明的头文件

int main(int argc, char* argv[])
{
	// 调用API函数MessageBox
	int nSelect = ::MessageBox(NULL, "Hello, Windows XP", "Greetings", MB_OKCANCEL);
	if(nSelect == IDOK)
		printf(" 用户选择了“确定”按钮 \n");
	else
		printf(" 用户选择了“取消”按钮 \n");
	return 0;
}
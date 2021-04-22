///////////////////////////////////////////////////////////////
// 01UseAPI.cpp文件

#include "stdafx.h"
#include <windows.h>

int main(int argc, char* argv[])
{
	int nSelect = ::MessageBox(NULL, "Hello, Windows XP", "Greetings", MB_OKCANCEL);
	if(nSelect == IDOK)
		printf(" 用户选择了“确定”按钮 \n");
	else
		printf(" 用户选择了“取消”按钮 \n");
	
	return 0;
}


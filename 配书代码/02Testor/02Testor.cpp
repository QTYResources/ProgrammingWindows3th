///////////////////////////////////////////////////////////////
// 02Testor.cpp文件


#include "stdafx.h"


#include <stdio.h>

// 全局变量测试
int g_nNum;	
int main(int argc, char* argv[])
{
	int i = 198;	// 局部变量测试
	g_nNum = 1003;

	while(1)
	{
		// 输出个变量的值和地址
		printf(" i = %d, addr = %08lX;   g_nNum = %d, addr = %08lX \n",
								++i, &i, --g_nNum, &g_nNum);
		getchar();
	}

	return 0;
}

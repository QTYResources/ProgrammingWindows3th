///////////////////////////////////////////////////////////////
// 02Testor.cpp�ļ�


#include "stdafx.h"


#include <stdio.h>

// ȫ�ֱ�������
int g_nNum;	
int main(int argc, char* argv[])
{
	int i = 198;	// �ֲ���������
	g_nNum = 1003;

	while(1)
	{
		// �����������ֵ�͵�ַ
		printf(" i = %d, addr = %08lX;   g_nNum = %d, addr = %08lX \n",
								++i, &i, --g_nNum, &g_nNum);
		getchar();
	}

	return 0;
}

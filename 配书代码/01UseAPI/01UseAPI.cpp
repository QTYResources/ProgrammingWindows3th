///////////////////////////////////////////////////////////////
// 01UseAPI.cpp�ļ�

#include "stdafx.h"
#include <windows.h>

int main(int argc, char* argv[])
{
	int nSelect = ::MessageBox(NULL, "Hello, Windows XP", "Greetings", MB_OKCANCEL);
	if(nSelect == IDOK)
		printf(" �û�ѡ���ˡ�ȷ������ť \n");
	else
		printf(" �û�ѡ���ˡ�ȡ������ť \n");
	
	return 0;
}


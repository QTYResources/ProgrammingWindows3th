///////////////////////////////////////////////////////////////
// 01FirstApp.cpp�ļ�

#include "stdafx.h"	// ����VC�Զ���ӵ�ͷ�ļ���û��ʲô��;
#include <windows.h>	// ����MessageBox����������ͷ�ļ�

int main(int argc, char* argv[])
{
	// ����API����MessageBox
	int nSelect = ::MessageBox(NULL, "Hello, Windows XP", "Greetings", MB_OKCANCEL);
	if(nSelect == IDOK)
		printf(" �û�ѡ���ˡ�ȷ������ť \n");
	else
		printf(" �û�ѡ���ˡ�ȡ������ť \n");
	return 0;
}
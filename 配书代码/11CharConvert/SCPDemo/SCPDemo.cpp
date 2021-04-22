// SCPDemo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <windows.h>
#include <winioctl.h>

#include "Driver.h"
#include "CharConvert.h"

int main(int argc, char* argv[])
{
	// ����������������Ŀ¼
	char szPath[256];
	char* p;
	::GetFullPathName("CharConvert.SYS", 256, szPath, &p);
	// ���������ں�����
	CDriver driver(szPath, "slCharConvert");
	if(driver.StartDriver())
	{
		printf(" CharConvert����ɹ����� \n");
		if(driver.OpenDevice())
		{
			printf(" ����Ѿ��� \n");
			printf(" ������һ�����֣�0��9����\n");
			
			char c, strOut[16] = "";
			// ������������
			scanf("%c", &c);
			// ������������ת��
			if(driver.IoControl(CHAR_CONVERT, &c, 1, strOut, 2) != -1)
				printf("ת��Ϊ��%s \n", strOut);
		}
	}

	return 0;
}


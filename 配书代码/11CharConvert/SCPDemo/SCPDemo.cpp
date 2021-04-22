// SCPDemo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <windows.h>
#include <winioctl.h>

#include "Driver.h"
#include "CharConvert.h"

int main(int argc, char* argv[])
{
	// 驱动驱动程序完整目录
	char szPath[256];
	char* p;
	::GetFullPathName("CharConvert.SYS", 256, szPath, &p);
	// 加载启动内核驱动
	CDriver driver(szPath, "slCharConvert");
	if(driver.StartDriver())
	{
		printf(" CharConvert服务成功启动 \n");
		if(driver.OpenDevice())
		{
			printf(" 句柄已经打开 \n");
			printf(" 请输入一个数字（0～9）：\n");
			
			char c, strOut[16] = "";
			// 接收数字输入
			scanf("%c", &c);
			// 请求驱动程序转化
			if(driver.IoControl(CHAR_CONVERT, &c, 1, strOut, 2) != -1)
				printf("转化为：%s \n", strOut);
		}
	}

	return 0;
}


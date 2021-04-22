///////////////////////////////////////////////////////////////
// 04TellToClose.cpp文件


#include "stdafx.h"
#include <windows.h>

int main(int argc, char* argv[])
{
	// 查找标题为“无标题 - 记事本”的窗口 
	// 也可以使用类名来查找，如::FindWindow("Notepad", NULL);
	HWND hWnd = ::FindWindow(NULL, "无标题 - 记事本");
	if(hWnd != NULL)
	{
		// 向目标窗口发送WM_CLOSE消息
		::SendMessage(hWnd, WM_CLOSE, 0, 0);
	}
	return 0;
}


///////////////////////////////////////////////////////////////
// 01CodeStyle.cpp文件


#include "stdafx.h"
#include <windows.h>

void Alert(int i);


int main(int argc, char* argv[])
{
	Alert(2);

	return 0;
}

void Alert(int i)
{
	while(i > 0)
	{
		// Beep函数会使扬声器发出简单的声音，
		// 要调用这个函数你应该包含上头文件“windows.h”
		Beep(1000, 1000);   
		i--;
	}
}

/*

MessageBox(NULL, "Hello, Windows XP", "Greetings", MB_OKCANCEL);// 用空格分割参数
int i = 10*(123 + 100/5);					// 用空格分割赋值语句
if((a > b) && (c < d) && (e == f))
……

  int nSelect = ::MessageBox(
			NULL,				// 窗口句柄
			"Hello, Windows XP",		// 对话框要显示的文字
			"Greetings",			// 对话框的标题
			MB_YESNOCANCEL|MB_DEFBUTTON2);	// 对话框的风格				// 用空格分割表达式等


  
	// 创建一个新的线程
	HANDLE hThread = ::CreateThread(……);
	if(hThread == NULL)
	{
		return FALSE;	// 失败！
	}
	else
	{
		return TRUE;	// 成功！
	}
*/


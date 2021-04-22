///////////////////////////////////////////////////////////////
// 04Win32AppDemo.cpp文件


#include "stdafx.h"

int APIENTRY WinMain(HINSTANCE hInstance,	// 本模块的实例句柄
                     HINSTANCE hPrevInstance,	// Win16留下的废物，现在已经不用了
                     LPSTR     lpCmdLine,	// 命令行参数
                     int       nCmdShow)	// 主窗口初始化时的显示方式
{
	int nSel = ::MessageBox(NULL, "Hello, Win32 Application", 
			"04Win32AppDemo", MB_YESNOCANCEL|MB_ICONQUESTION|MB_DEFBUTTON3);
	if(nSel == IDYES)		// 用户选择了“是”按钮
	{
	} else if(nSel == IDNO)		// 用户选择了“否”按钮
	{
	} else if(nSel == IDCANCEL)	// 用户选择了“取消”按钮
	{
	}

	return 0;
}




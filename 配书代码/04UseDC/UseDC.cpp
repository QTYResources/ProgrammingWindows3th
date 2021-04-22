///////////////////////////////////////////////////////////////
// UseDC.cpp文件


#include <windows.h>

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	HDC hdc;
	HWND hWnd;
	char sz[] = "大家好";
	
	// 查找记事本程序的窗口句柄
	hWnd = ::FindWindow("Notepad", NULL);

	// 如果记事本程序在运行，就向其客户区绘制文本
	while(::IsWindow(hWnd))		// IsWindow函数用于判断一个窗口句柄是否有效
	{
		hdc = ::GetDC(hWnd);
		::TextOut(hdc, 10, 10, sz, strlen(sz));
		::ReleaseDC(hWnd, NULL);
		::Sleep(1000);
	}
	::MessageBox(NULL, "记事本程序已经退出", "04UseDC", MB_OK);
	
	return 0;
}




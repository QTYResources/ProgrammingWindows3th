///////////////////////////////////////////////////////////////
// 04FirstWindow.cpp文件



#include "stdafx.h"

// 窗口函数的函数原形
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	char szClassName[] = "MainWClass";	
	WNDCLASSEX wndclass;
	
	// 用描述主窗口的参数填充WNDCLASSEX结构
	wndclass.cbSize = sizeof(wndclass);	// 结构的大小
	wndclass.style = CS_HREDRAW|CS_VREDRAW;	// 指定如果大小改变就重画
	wndclass.lpfnWndProc = MainWndProc;	// 窗口函数指针
	wndclass.cbClsExtra = 0;		// 没有额外的类内存
	wndclass.cbWndExtra = 0;		// 没有额外的窗口内存
	wndclass.hInstance = hInstance;		// 实例句柄 
	wndclass.hIcon = ::LoadIcon(NULL, 
			IDI_APPLICATION);	// 使用预定义图标
	wndclass.hCursor = ::LoadCursor(NULL,
			IDC_ARROW);		// 使用预定义的光标
	wndclass.hbrBackground = (HBRUSH)
		::GetStockObject(WHITE_BRUSH);	// 使用白色背景画刷
	wndclass.lpszMenuName = NULL;		// 不指定菜单
	wndclass.lpszClassName = szClassName ;	// 窗口类的名称	
	wndclass.hIconSm = NULL;		// 没有类的小图标
	
	// 注册这个窗口类
	::RegisterClassEx(&wndclass); 
	
	// 创建主窗口
	HWND hwnd = ::CreateWindowEx( 
		0,			// dwExStyle，扩展样式	
		szClassName,		// lpClassName，类名			
		"My first Window!",	// lpWindowName，标题		
		WS_OVERLAPPEDWINDOW,	// dwStyle，窗口风格	
		CW_USEDEFAULT,		// X，初始 X 坐标		
		CW_USEDEFAULT,		// Y，初始 Y 坐标		
		CW_USEDEFAULT,		// nWidth，宽度			
		CW_USEDEFAULT,		// nHeight，高度			
		NULL,			// hWndParent，父窗口句柄			
		NULL,			// hMenu，菜单句柄		
		hInstance,		// hlnstance，程序实例句柄		
		NULL) ;			// lpParam，用户数据			

	if(hwnd == NULL)
	{
		::MessageBox(NULL, "创建窗口出错！", "error", MB_OK);
		return -1;
	}

	// 显示窗口，刷新窗口客户区
	::ShowWindow(hwnd, nCmdShow);
	::UpdateWindow(hwnd);
	
	// 从消息堆中取出消息
	MSG msg;	
	while(::GetMessage(&msg, NULL, 0, 0))
	{
		// 转化键盘消息
		::TranslateMessage(&msg);
		// 将消息发送到相应的窗口函数
		::DispatchMessage(&msg);
	}

	// 当GetMessage返回0时程序结束
	return msg.wParam;
}


LRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	char szText[] = "最简单的窗口程序！";
	switch (message)
	{	 	
	case WM_PAINT: // 窗口客户区需要重画
		{
			HDC hdc;
			PAINTSTRUCT ps;

			// 使无效的客户区变的有效，并取得设备环境句柄
			hdc = ::BeginPaint (hwnd, &ps) ;	
			// 显示文字
			::TextOut(hdc, 10, 10, szText, strlen(szText));
			::EndPaint(hwnd, &ps);
			return 0;
		}	
	case WM_DESTROY: // 正在销毁窗口

		// 向消息队列投递一个WM_QUIT消息，促使GetMessage函数返回0，结束消息循环
		::PostQuitMessage(0) ;
		return 0 ;
	}

	// 将我们不处理的消息交给系统做默认处理
	return ::DefWindowProc(hwnd, message, wParam, lParam);
}

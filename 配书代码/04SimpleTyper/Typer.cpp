///////////////////////////////////////////////////////////////
// Typer.cpp文件



#include <windows.h>
#include "resource.h"
#include <string>

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
	wndclass.hIcon = ::LoadIcon(hInstance, 
			(LPSTR)IDI_TYPER);	// 使用自定义图标	
	wndclass.hCursor = ::LoadCursor(NULL,
			IDC_ARROW);		// 使用预定义的光标
	wndclass.hbrBackground =  
		(HBRUSH)(COLOR_3DFACE + 1);	// 使用预定义画刷
	wndclass.lpszMenuName = (LPSTR)IDR_TYPER;	
	wndclass.lpszClassName = szClassName ;	// 窗口类的名称	
	wndclass.hIconSm = NULL;		// 没有类的小图标
	
	// 注册这个窗口类
	::RegisterClassEx(&wndclass); 
	
	// 创建主窗口
	HWND hwnd = ::CreateWindowEx( 
		WS_EX_CLIENTEDGE,	// dwExStyle，扩展样式
					// 注意，WS_EX_CLIENTEDGE风格指定了这个窗口有一个边框和一个下沉的边缘
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
	// str对象用于保存窗口客户区显示的字符串
	// 为了使用string类，应该包含头文件：“#include <string>”
	static std::string str; 

	switch (message)
	{
	case WM_CREATE:
		{
			// 设置窗口的标题
			::SetWindowText(hwnd, "最简陋的打字程序");
			//::SendMessage(hwnd, WM_SETTEXT, 0, (long)"最简陋的打字程序");
			return 0;
		}

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case ID_FILE_EXIT:
			// 向hwnd指定的窗口发送一个WM_CLOSE消息。
			::SendMessage(hwnd, WM_CLOSE, 0, 0);
			// ::PostMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		}

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = ::BeginPaint(hwnd, &ps);
				
			// 设置输出文本的背景颜色和文字颜色
			::SetTextColor(hdc, RGB(255, 0, 0));  
			::SetBkColor(hdc, ::GetSysColor(COLOR_3DFACE));
	
			::TextOut(hdc, 0, 0, str.c_str(), str.length ());
			::EndPaint(hwnd, &ps);
			return 0;
		}

	case WM_CHAR:
		{
			// 保存ansi码
			str = str + char(wParam);
			
			// 使整个客户区无效
			::InvalidateRect(hwnd,NULL,0);
			return 0;
		}

	case WM_LBUTTONDOWN:
		{	
			char szPoint[56];
			// 保存坐标信息
			wsprintf(szPoint,"X =%d,Y =%d",LOWORD(lParam),HIWORD(lParam));
			
			str = szPoint;
			
			if(wParam & MK_SHIFT)
				str = str + "   Shift Key is down";
			::InvalidateRect(hwnd,NULL,1);
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

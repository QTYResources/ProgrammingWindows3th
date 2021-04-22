///////////////////////////////////////////////////////////////
// TimerDemo.cpp文件

#include <windows.h>

LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	char szWindowClass[] = "Timer";

	// 注册窗口类
	WNDCLASSEX wcex;

	wcex.cbSize		= sizeof(WNDCLASSEX); 
	wcex.style		= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon		= 0;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)::GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= NULL;

	::RegisterClassEx(&wcex);


	// 创建并线程主窗口
	HWND hWnd = ::CreateWindowEx( 
		WS_EX_CLIENTEDGE,	// 扩展样式
		szWindowClass,		// 类名
		"定时器的使用",	// 标题
		WS_OVERLAPPEDWINDOW,	// 窗口样式
		CW_USEDEFAULT,	// 初始 X 坐标
		CW_USEDEFAULT,	// 初始 X 坐标
		CW_USEDEFAULT,	// 宽度
		CW_USEDEFAULT,	// 高度
		NULL,		// 父窗口句柄
		NULL,	        // 菜单句柄
		hInstance,	// 程序实例句柄
		NULL); 	

	::ShowWindow(hWnd, nShowCmd);
	::UpdateWindow(hWnd);

	// 进入消息循环
	MSG msg;
	while(::GetMessage(&msg, NULL, 0, 0))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg); 
	}
	
	return 1;
}


#define IDT_TIMER1 1

// 消息处理函数
LRESULT __stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int nNum;	// 计数
	static int bSetTimer;	// 指示是否安装了定时器
	char szText[56];
	PAINTSTRUCT ps;
	HDC hdc;
	
	switch(message)
	{
	case WM_CREATE:		// 窗口正在被创建
		bSetTimer = FALSE;
		break;
	case WM_PAINT:		// 窗口客户区需要重画
		hdc = ::BeginPaint(hWnd, &ps);
		wsprintf(szText, "计数：%d", nNum);
		::TextOut(hdc, 10, 10, szText, strlen(szText));
		::EndPaint(hWnd, &ps);
		break;
	case WM_TIMER:		// 定时器时间已到
		if(wParam == IDT_TIMER1)
		{
			hdc = GetDC(hWnd);
			wsprintf(szText, "计数：%d", nNum++);
			::TextOut(hdc, 10, 10, szText, strlen(szText));

			// 发一声“嘟”的声音
			::MessageBeep(MB_OK);
		}
		break;
	case WM_LBUTTONDOWN:	// 用户单击鼠标左键
		if(bSetTimer)
		{
			// 插销一个已经安装的定时器
			::KillTimer(hWnd, IDT_TIMER1);
			bSetTimer = FALSE;
		}
		else
		{
			// 安装一个时间周期为250ms的定时器
			if(::SetTimer(hWnd, IDT_TIMER1, 250, NULL) == 0)
			// SetTimer函数调用成功会返回新的定时器的ID号，失败的话返回0
			{
				::MessageBox(hWnd, "安装定时器失败！", "03Timer", MB_OK);
			}
			else
			{
				bSetTimer = TRUE;
			}
		}
		break;
	case WM_CLOSE:		// 用户要求关闭窗口
		if(bSetTimer)
			::KillTimer(hWnd, IDT_TIMER1);
		break;
	case WM_DESTROY:	// 窗口正在被销毁
		::PostQuitMessage(0);
		break;
	}
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}


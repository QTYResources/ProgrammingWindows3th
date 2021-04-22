///////////////////////////////////////////////////////////////
// SineWave.cpp文件


#include <windows.h>
#include <math.h>

LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	char szWindowClass[] = "SineWave";

	// 注册窗口类
	WNDCLASSEX wcex;

	wcex.cbSize		= sizeof(WNDCLASSEX); 
	wcex.style		= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon		= NULL;
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
		"正弦线",		// 标题
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


// 消息处理函数    // 03SineWave工程下
LRESULT __stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
#define SEGMENTS 500  // 取的点数（在一个周期内取500个点）
#define PI 3.1415926  // 圆周率

	HDC hdc;
	PAINTSTRUCT ps;
	RECT rt;
	int cxClient, cyClient;
	POINT pt[SEGMENTS];
	int i;
	
	switch(message)
	{
	case WM_PAINT:
		hdc = ::BeginPaint(hWnd, &ps);
		::GetClientRect(hWnd, &rt);
		cxClient = rt.right - rt.left;
		cyClient = rt.bottom - rt.top;

		// 画横坐标轴
		::MoveToEx(hdc, 0, cyClient/2, NULL);
		::LineTo(hdc, cxClient, cyClient/2);
		// 找出500个点的坐标
		for(i=0; i<SEGMENTS; i++)
		{
			pt[i].x = cxClient*i/SEGMENTS;
			pt[i].y = (int)((cyClient/2)*(1 - sin(2*PI*i/SEGMENTS)));
		}
		// 将各点连在一起
		::Polyline(hdc, pt, SEGMENTS);

		::EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
	}
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

///////////////////////////////////////////////////////////////
// Clock.cpp文件


#include <windows.h>


#include "resource.h"
#include <math.h>

LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	char szWindowClass[] = "Clock";

	// 注册窗口类
	WNDCLASSEX wcex;

	wcex.cbSize		= sizeof(WNDCLASSEX); 
	wcex.style		= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon		= ::LoadIcon(hInstance, (LPCTSTR)IDI_MAIN);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_3DFACE + 1); // 此域可以是一个系统定义的颜色值
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= NULL;

	::RegisterClassEx(&wcex);


	// 创建并显示主窗口
	HWND hWnd = ::CreateWindowEx(NULL, szWindowClass, "时钟", 
		WS_POPUP|WS_SYSMENU|WS_SIZEBOX, 100, 100, 300, 300, NULL, NULL, hInstance, NULL); 	
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



//////////////////////////////////////////////////////////////////////////////////////
// 消息处理代码

#define IDT_CLOCK 1
const int IDM_HELP = 100;
const int IDM_TOPMOST = 101;

// 实现函数
void SetIsotropic(HDC hdc,  int cxClient, int cyClient);
void DrawClockFace(HDC hdc);
void DrawHand(HDC hdc, int nLength, int nWidth, int nDegrees, COLORREF clrColor);

// 上一次Windows通知时的时间
static int s_nPreHour;		// 小时	
static int s_nPreMinute;	// 分钟
static int s_nPreSecond;	// 秒
// 窗口客户区的大小
static int s_cxClient;		
static int s_cyClient;
// 是否位于最顶层
static BOOL s_bTopMost;

LRESULT __stdcall WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch(nMsg)
	{
	case WM_CREATE:
		{	
			// 向系统菜单中添加自定义的项
			HMENU hSysMenu;
			hSysMenu = ::GetSystemMenu(hWnd, FALSE);
			::AppendMenu(hSysMenu, MF_SEPARATOR, 0, NULL);
			::AppendMenu(hSysMenu, MF_STRING, IDM_TOPMOST, "总在最前");
			::AppendMenu(hSysMenu, MF_STRING, IDM_HELP, "帮助");
			
			// 设置时间
			SYSTEMTIME time;
			::GetLocalTime(&time);
			s_nPreHour = time.wHour%12;
			s_nPreMinute = time.wMinute;
			s_nPreSecond = time.wSecond;
			
			// 创建定时器
			::SetTimer(hWnd, IDT_CLOCK, 1000, NULL);
			return 0;
		}
	case WM_SIZE:
		{
			s_cxClient = LOWORD(lParam);
			s_cyClient = HIWORD(lParam);
			return 0;
		}
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = ::BeginPaint(hWnd, &ps);
			
			// 设置坐标系
			SetIsotropic(hdc, s_cxClient, s_cyClient);
			
			// 绘制时钟外观
			DrawClockFace(hdc);
			
			// 绘制指针
			
			// 经过1个小时时针走30度（360/12），经过1分钟时针走0.5度（30/60）
			DrawHand(hdc, 200, 8, s_nPreHour*30 + s_nPreMinute/2, RGB(0, 0, 0));
			// 经过1分钟分针走6度（360/60）
			DrawHand(hdc, 400, 6, s_nPreMinute*6, RGB(0, 0, 0));
			// 经过1秒钟秒针走6度（360/60）
			DrawHand(hdc, 400, 1, s_nPreSecond*6, RGB(0, 0, 0));
			
			::EndPaint(hWnd, &ps);
			return 0;
		}
	case WM_TIMER:
		{
			// 如果窗口处于最小化状态就什么也不做
			if(::IsIconic(hWnd))	// IsIconic函数用来判断窗口是否处于最小化状态
				return 0;
			
			// 取得系统时间
			SYSTEMTIME time; 
			::GetLocalTime(&time);
			
			// 建立坐标系
			HDC hdc = ::GetDC(hWnd);
			SetIsotropic(hdc, s_cxClient, s_cyClient);
			
			// 以COLOR_3DFACE为背景色就可以擦除指针了（因为窗口的背景色也是COLOR_3DFACE）
			COLORREF crfColor = ::GetSysColor(COLOR_3DFACE); 
			
			// 如果分钟改变的话就擦除时针和分针
			if(time.wMinute != s_nPreMinute)
			{
				// 擦除时针和分针
				DrawHand(hdc, 200, 8, s_nPreHour*30 + s_nPreMinute/2, crfColor);
				DrawHand(hdc, 400, 6, s_nPreMinute*6, crfColor);
				s_nPreHour = time.wHour;
				s_nPreMinute = time.wMinute;
			}
			
			// 如果秒改变的话就擦除秒针，然后重画所有指针
			if(time.wSecond != s_nPreSecond)
			{
				// 擦除秒针
				DrawHand(hdc, 400, 1, s_nPreSecond*6, crfColor);
				
				// 重画所有指针
				DrawHand(hdc, 400, 1, time.wSecond*6, RGB(0, 0, 0));
				DrawHand(hdc, 200, 8, time.wHour*30 + time.wMinute/2, RGB(0, 0, 0));
				DrawHand(hdc, 400, 6, time.wMinute*6, RGB(0, 0, 0));
				s_nPreSecond = time.wSecond;
			}
			return 0;
		}
	case WM_CONTEXTMENU:
		POINT pt; pt.x = LOWORD(lParam); pt.y = HIWORD(lParam);
		{
			// 取得系统菜单的句柄
			HMENU hSysMenu = ::GetSystemMenu(hWnd, FALSE);
			
			// 弹出系统菜单
			int nID = ::TrackPopupMenu(hSysMenu,TPM_LEFTALIGN|TPM_RETURNCMD, 
				pt.x, pt.y, 0, hWnd, NULL);
			if(nID > 0)
				::SendMessage(hWnd, WM_SYSCOMMAND, nID, 0);	
			return 0;
		}
	case WM_CLOSE:
		{
			::KillTimer(hWnd, IDT_CLOCK);
			::DestroyWindow(hWnd);
			return 0;
		}
	case WM_DESTROY:
		{
			::PostQuitMessage(0);
			return 0;
		}
	case WM_NCHITTEST:
		{
			UINT nHitTest;
			nHitTest = ::DefWindowProc(hWnd, WM_NCHITTEST, wParam, lParam);
			if (nHitTest == HTCLIENT &&
				::GetAsyncKeyState(MK_LBUTTON) < 0) // 如果鼠标左键按下，GetAsyncKeyState函数的返回值小于0
				nHitTest = HTCAPTION;
			return nHitTest;
		}
	case WM_SYSCOMMAND:
		int nID = wParam;
		{
			if(nID == IDM_HELP)
			{
				::MessageBox(hWnd, "一个时钟的例子", "时钟", 0);
			}
			else if(nID == IDM_TOPMOST)
			{
				HMENU hSysMenu = ::GetSystemMenu(hWnd, FALSE);
				if(s_bTopMost)
				{
					::CheckMenuItem(hSysMenu, IDM_TOPMOST, MF_UNCHECKED);
					::SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, 
								SWP_NOMOVE | SWP_NOREDRAW | SWP_NOSIZE);
					s_bTopMost = FALSE;
				}
				else
				{
					::CheckMenuItem(hSysMenu, IDM_TOPMOST, MF_CHECKED);
					::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0,
								SWP_NOMOVE | SWP_NOREDRAW | SWP_NOSIZE);
					s_bTopMost = TRUE;
				}
			}
			return ::DefWindowProc(hWnd, WM_SYSCOMMAND, nID, 0);
		}
	}

	return ::DefWindowProc(hWnd, nMsg, wParam, lParam);
}




void SetIsotropic(HDC hdc, int cx, int cy)
{
	::SetMapMode(hdc, MM_ISOTROPIC);
	::SetWindowExtEx(hdc, 1000, 1000, NULL);
	::SetViewportExtEx(hdc, cx, -cy, NULL);
	::SetViewportOrgEx(hdc, cx/2, cy/2, NULL);
}

// 绘制时钟的外观
void DrawClockFace(HDC hdc)
{
	const int SQUARESIZE = 20;
	static POINT pt[] =
	{
		0, 450,		// 12点
		225, 390,	// 1点
		390, 225,	// 2点
		450, 0,		// 3点
		390, -225,	//... 下面的坐标是上面的点的对称点（以X轴、Y轴或原点对称）
		225, -390,
		0, -450,
		-225, -390,
		-390, -225,
		-450, 0,
		-390, 225,
		-225, 390
	};

	// 选择一个黑色的画刷
	::SelectObject(hdc, ::GetStockObject(BLACK_BRUSH));

	// 画12个圆
	for(int i=0; i<12; i++)
	{
		::Ellipse(hdc, pt[i].x - SQUARESIZE, pt[i].y + SQUARESIZE,
			pt[i].x + SQUARESIZE, pt[i].y - SQUARESIZE);
	}
}



// 指针的长度、宽度、相对于0点偏移的角度、颜色分别由参数nLength、nWidth、nDegrees、clrColor指定
void DrawHand(HDC hdc, int nLength, int nWidth, int nDegrees, COLORREF clrColor)
{
	// 将角度nDegrees转化成弧度 .	2*3.1415926/360 == 0.0174533
	double nRadians = (double)nDegrees*0.0174533;

	// 计算坐标
	POINT pt[2];
	pt[0].x = (int)(nLength*sin(nRadians));
	pt[0].y = (int)(nLength*cos(nRadians));
	pt[1].x = -pt[0].x/5;
	pt[1].y = -pt[0].y/5;

	// 创建画笔，并选如DC结构中
	HPEN hPen = ::CreatePen(PS_SOLID, nWidth, clrColor);
	HPEN hOldPen = (HPEN)::SelectObject(hdc, hPen);

	// 画线
	::MoveToEx(hdc, pt[0].x, pt[0].y, NULL);
	::LineTo(hdc, pt[1].x, pt[1].y);

	::SelectObject(hdc, hOldPen);
	::DeleteObject(hPen);
}
///////////////////////////////////////////////////////////////
// Clock.cpp�ļ�


#include <windows.h>


#include "resource.h"
#include <math.h>

LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	char szWindowClass[] = "Clock";

	// ע�ᴰ����
	WNDCLASSEX wcex;

	wcex.cbSize		= sizeof(WNDCLASSEX); 
	wcex.style		= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon		= ::LoadIcon(hInstance, (LPCTSTR)IDI_MAIN);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_3DFACE + 1); // ���������һ��ϵͳ�������ɫֵ
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= NULL;

	::RegisterClassEx(&wcex);


	// ��������ʾ������
	HWND hWnd = ::CreateWindowEx(NULL, szWindowClass, "ʱ��", 
		WS_POPUP|WS_SYSMENU|WS_SIZEBOX, 100, 100, 300, 300, NULL, NULL, hInstance, NULL); 	
	::ShowWindow(hWnd, nShowCmd);
	::UpdateWindow(hWnd);

	// ������Ϣѭ��
	MSG msg;
	while(::GetMessage(&msg, NULL, 0, 0))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg); 
	}

	return 1;
}



//////////////////////////////////////////////////////////////////////////////////////
// ��Ϣ�������

#define IDT_CLOCK 1
const int IDM_HELP = 100;
const int IDM_TOPMOST = 101;

// ʵ�ֺ���
void SetIsotropic(HDC hdc,  int cxClient, int cyClient);
void DrawClockFace(HDC hdc);
void DrawHand(HDC hdc, int nLength, int nWidth, int nDegrees, COLORREF clrColor);

// ��һ��Windows֪ͨʱ��ʱ��
static int s_nPreHour;		// Сʱ	
static int s_nPreMinute;	// ����
static int s_nPreSecond;	// ��
// ���ڿͻ����Ĵ�С
static int s_cxClient;		
static int s_cyClient;
// �Ƿ�λ�����
static BOOL s_bTopMost;

LRESULT __stdcall WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch(nMsg)
	{
	case WM_CREATE:
		{	
			// ��ϵͳ�˵�������Զ������
			HMENU hSysMenu;
			hSysMenu = ::GetSystemMenu(hWnd, FALSE);
			::AppendMenu(hSysMenu, MF_SEPARATOR, 0, NULL);
			::AppendMenu(hSysMenu, MF_STRING, IDM_TOPMOST, "������ǰ");
			::AppendMenu(hSysMenu, MF_STRING, IDM_HELP, "����");
			
			// ����ʱ��
			SYSTEMTIME time;
			::GetLocalTime(&time);
			s_nPreHour = time.wHour%12;
			s_nPreMinute = time.wMinute;
			s_nPreSecond = time.wSecond;
			
			// ������ʱ��
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
			
			// ��������ϵ
			SetIsotropic(hdc, s_cxClient, s_cyClient);
			
			// ����ʱ�����
			DrawClockFace(hdc);
			
			// ����ָ��
			
			// ����1��Сʱʱ����30�ȣ�360/12��������1����ʱ����0.5�ȣ�30/60��
			DrawHand(hdc, 200, 8, s_nPreHour*30 + s_nPreMinute/2, RGB(0, 0, 0));
			// ����1���ӷ�����6�ȣ�360/60��
			DrawHand(hdc, 400, 6, s_nPreMinute*6, RGB(0, 0, 0));
			// ����1����������6�ȣ�360/60��
			DrawHand(hdc, 400, 1, s_nPreSecond*6, RGB(0, 0, 0));
			
			::EndPaint(hWnd, &ps);
			return 0;
		}
	case WM_TIMER:
		{
			// ������ڴ�����С��״̬��ʲôҲ����
			if(::IsIconic(hWnd))	// IsIconic���������жϴ����Ƿ�����С��״̬
				return 0;
			
			// ȡ��ϵͳʱ��
			SYSTEMTIME time; 
			::GetLocalTime(&time);
			
			// ��������ϵ
			HDC hdc = ::GetDC(hWnd);
			SetIsotropic(hdc, s_cxClient, s_cyClient);
			
			// ��COLOR_3DFACEΪ����ɫ�Ϳ��Բ���ָ���ˣ���Ϊ���ڵı���ɫҲ��COLOR_3DFACE��
			COLORREF crfColor = ::GetSysColor(COLOR_3DFACE); 
			
			// ������Ӹı�Ļ��Ͳ���ʱ��ͷ���
			if(time.wMinute != s_nPreMinute)
			{
				// ����ʱ��ͷ���
				DrawHand(hdc, 200, 8, s_nPreHour*30 + s_nPreMinute/2, crfColor);
				DrawHand(hdc, 400, 6, s_nPreMinute*6, crfColor);
				s_nPreHour = time.wHour;
				s_nPreMinute = time.wMinute;
			}
			
			// �����ı�Ļ��Ͳ������룬Ȼ���ػ�����ָ��
			if(time.wSecond != s_nPreSecond)
			{
				// ��������
				DrawHand(hdc, 400, 1, s_nPreSecond*6, crfColor);
				
				// �ػ�����ָ��
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
			// ȡ��ϵͳ�˵��ľ��
			HMENU hSysMenu = ::GetSystemMenu(hWnd, FALSE);
			
			// ����ϵͳ�˵�
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
				::GetAsyncKeyState(MK_LBUTTON) < 0) // ������������£�GetAsyncKeyState�����ķ���ֵС��0
				nHitTest = HTCAPTION;
			return nHitTest;
		}
	case WM_SYSCOMMAND:
		int nID = wParam;
		{
			if(nID == IDM_HELP)
			{
				::MessageBox(hWnd, "һ��ʱ�ӵ�����", "ʱ��", 0);
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

// ����ʱ�ӵ����
void DrawClockFace(HDC hdc)
{
	const int SQUARESIZE = 20;
	static POINT pt[] =
	{
		0, 450,		// 12��
		225, 390,	// 1��
		390, 225,	// 2��
		450, 0,		// 3��
		390, -225,	//... ���������������ĵ�ĶԳƵ㣨��X�ᡢY���ԭ��Գƣ�
		225, -390,
		0, -450,
		-225, -390,
		-390, -225,
		-450, 0,
		-390, 225,
		-225, 390
	};

	// ѡ��һ����ɫ�Ļ�ˢ
	::SelectObject(hdc, ::GetStockObject(BLACK_BRUSH));

	// ��12��Բ
	for(int i=0; i<12; i++)
	{
		::Ellipse(hdc, pt[i].x - SQUARESIZE, pt[i].y + SQUARESIZE,
			pt[i].x + SQUARESIZE, pt[i].y - SQUARESIZE);
	}
}



// ָ��ĳ��ȡ���ȡ������0��ƫ�ƵĽǶȡ���ɫ�ֱ��ɲ���nLength��nWidth��nDegrees��clrColorָ��
void DrawHand(HDC hdc, int nLength, int nWidth, int nDegrees, COLORREF clrColor)
{
	// ���Ƕ�nDegreesת���ɻ��� .	2*3.1415926/360 == 0.0174533
	double nRadians = (double)nDegrees*0.0174533;

	// ��������
	POINT pt[2];
	pt[0].x = (int)(nLength*sin(nRadians));
	pt[0].y = (int)(nLength*cos(nRadians));
	pt[1].x = -pt[0].x/5;
	pt[1].y = -pt[0].y/5;

	// �������ʣ���ѡ��DC�ṹ��
	HPEN hPen = ::CreatePen(PS_SOLID, nWidth, clrColor);
	HPEN hOldPen = (HPEN)::SelectObject(hdc, hPen);

	// ����
	::MoveToEx(hdc, pt[0].x, pt[0].y, NULL);
	::LineTo(hdc, pt[1].x, pt[1].y);

	::SelectObject(hdc, hOldPen);
	::DeleteObject(hPen);
}
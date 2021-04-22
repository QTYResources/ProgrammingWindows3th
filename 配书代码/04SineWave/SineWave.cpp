///////////////////////////////////////////////////////////////
// SineWave.cpp�ļ�


#include <windows.h>
#include <math.h>

LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	char szWindowClass[] = "SineWave";

	// ע�ᴰ����
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


	// �������߳�������
	HWND hWnd = ::CreateWindowEx( 
		WS_EX_CLIENTEDGE,	// ��չ��ʽ
		szWindowClass,		// ����
		"������",		// ����
		WS_OVERLAPPEDWINDOW,	// ������ʽ
		CW_USEDEFAULT,	// ��ʼ X ����
		CW_USEDEFAULT,	// ��ʼ X ����
		CW_USEDEFAULT,	// ���
		CW_USEDEFAULT,	// �߶�
		NULL,		// �����ھ��
		NULL,	        // �˵����
		hInstance,	// ����ʵ�����
		NULL); 	

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


// ��Ϣ������    // 03SineWave������
LRESULT __stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
#define SEGMENTS 500  // ȡ�ĵ�������һ��������ȡ500���㣩
#define PI 3.1415926  // Բ����

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

		// ����������
		::MoveToEx(hdc, 0, cyClient/2, NULL);
		::LineTo(hdc, cxClient, cyClient/2);
		// �ҳ�500���������
		for(i=0; i<SEGMENTS; i++)
		{
			pt[i].x = cxClient*i/SEGMENTS;
			pt[i].y = (int)((cyClient/2)*(1 - sin(2*PI*i/SEGMENTS)));
		}
		// ����������һ��
		::Polyline(hdc, pt, SEGMENTS);

		::EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
	}
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

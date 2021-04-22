///////////////////////////////////////////////////////////////
// TimerDemo.cpp�ļ�

#include <windows.h>

LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	char szWindowClass[] = "Timer";

	// ע�ᴰ����
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


	// �������߳�������
	HWND hWnd = ::CreateWindowEx( 
		WS_EX_CLIENTEDGE,	// ��չ��ʽ
		szWindowClass,		// ����
		"��ʱ����ʹ��",	// ����
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


#define IDT_TIMER1 1

// ��Ϣ������
LRESULT __stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int nNum;	// ����
	static int bSetTimer;	// ָʾ�Ƿ�װ�˶�ʱ��
	char szText[56];
	PAINTSTRUCT ps;
	HDC hdc;
	
	switch(message)
	{
	case WM_CREATE:		// �������ڱ�����
		bSetTimer = FALSE;
		break;
	case WM_PAINT:		// ���ڿͻ�����Ҫ�ػ�
		hdc = ::BeginPaint(hWnd, &ps);
		wsprintf(szText, "������%d", nNum);
		::TextOut(hdc, 10, 10, szText, strlen(szText));
		::EndPaint(hWnd, &ps);
		break;
	case WM_TIMER:		// ��ʱ��ʱ���ѵ�
		if(wParam == IDT_TIMER1)
		{
			hdc = GetDC(hWnd);
			wsprintf(szText, "������%d", nNum++);
			::TextOut(hdc, 10, 10, szText, strlen(szText));

			// ��һ����ཡ�������
			::MessageBeep(MB_OK);
		}
		break;
	case WM_LBUTTONDOWN:	// �û�����������
		if(bSetTimer)
		{
			// ����һ���Ѿ���װ�Ķ�ʱ��
			::KillTimer(hWnd, IDT_TIMER1);
			bSetTimer = FALSE;
		}
		else
		{
			// ��װһ��ʱ������Ϊ250ms�Ķ�ʱ��
			if(::SetTimer(hWnd, IDT_TIMER1, 250, NULL) == 0)
			// SetTimer�������óɹ��᷵���µĶ�ʱ����ID�ţ�ʧ�ܵĻ�����0
			{
				::MessageBox(hWnd, "��װ��ʱ��ʧ�ܣ�", "03Timer", MB_OK);
			}
			else
			{
				bSetTimer = TRUE;
			}
		}
		break;
	case WM_CLOSE:		// �û�Ҫ��رմ���
		if(bSetTimer)
			::KillTimer(hWnd, IDT_TIMER1);
		break;
	case WM_DESTROY:	// �������ڱ�����
		::PostQuitMessage(0);
		break;
	}
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}


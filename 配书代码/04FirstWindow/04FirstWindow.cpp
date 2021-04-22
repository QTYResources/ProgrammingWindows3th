///////////////////////////////////////////////////////////////
// 04FirstWindow.cpp�ļ�



#include "stdafx.h"

// ���ں����ĺ���ԭ��
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	char szClassName[] = "MainWClass";	
	WNDCLASSEX wndclass;
	
	// �����������ڵĲ������WNDCLASSEX�ṹ
	wndclass.cbSize = sizeof(wndclass);	// �ṹ�Ĵ�С
	wndclass.style = CS_HREDRAW|CS_VREDRAW;	// ָ�������С�ı���ػ�
	wndclass.lpfnWndProc = MainWndProc;	// ���ں���ָ��
	wndclass.cbClsExtra = 0;		// û�ж�������ڴ�
	wndclass.cbWndExtra = 0;		// û�ж���Ĵ����ڴ�
	wndclass.hInstance = hInstance;		// ʵ����� 
	wndclass.hIcon = ::LoadIcon(NULL, 
			IDI_APPLICATION);	// ʹ��Ԥ����ͼ��
	wndclass.hCursor = ::LoadCursor(NULL,
			IDC_ARROW);		// ʹ��Ԥ����Ĺ��
	wndclass.hbrBackground = (HBRUSH)
		::GetStockObject(WHITE_BRUSH);	// ʹ�ð�ɫ������ˢ
	wndclass.lpszMenuName = NULL;		// ��ָ���˵�
	wndclass.lpszClassName = szClassName ;	// �����������	
	wndclass.hIconSm = NULL;		// û�����Сͼ��
	
	// ע�����������
	::RegisterClassEx(&wndclass); 
	
	// ����������
	HWND hwnd = ::CreateWindowEx( 
		0,			// dwExStyle����չ��ʽ	
		szClassName,		// lpClassName������			
		"My first Window!",	// lpWindowName������		
		WS_OVERLAPPEDWINDOW,	// dwStyle�����ڷ��	
		CW_USEDEFAULT,		// X����ʼ X ����		
		CW_USEDEFAULT,		// Y����ʼ Y ����		
		CW_USEDEFAULT,		// nWidth�����			
		CW_USEDEFAULT,		// nHeight���߶�			
		NULL,			// hWndParent�������ھ��			
		NULL,			// hMenu���˵����		
		hInstance,		// hlnstance������ʵ�����		
		NULL) ;			// lpParam���û�����			

	if(hwnd == NULL)
	{
		::MessageBox(NULL, "�������ڳ���", "error", MB_OK);
		return -1;
	}

	// ��ʾ���ڣ�ˢ�´��ڿͻ���
	::ShowWindow(hwnd, nCmdShow);
	::UpdateWindow(hwnd);
	
	// ����Ϣ����ȡ����Ϣ
	MSG msg;	
	while(::GetMessage(&msg, NULL, 0, 0))
	{
		// ת��������Ϣ
		::TranslateMessage(&msg);
		// ����Ϣ���͵���Ӧ�Ĵ��ں���
		::DispatchMessage(&msg);
	}

	// ��GetMessage����0ʱ�������
	return msg.wParam;
}


LRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	char szText[] = "��򵥵Ĵ��ڳ���";
	switch (message)
	{	 	
	case WM_PAINT: // ���ڿͻ�����Ҫ�ػ�
		{
			HDC hdc;
			PAINTSTRUCT ps;

			// ʹ��Ч�Ŀͻ��������Ч����ȡ���豸�������
			hdc = ::BeginPaint (hwnd, &ps) ;	
			// ��ʾ����
			::TextOut(hdc, 10, 10, szText, strlen(szText));
			::EndPaint(hwnd, &ps);
			return 0;
		}	
	case WM_DESTROY: // �������ٴ���

		// ����Ϣ����Ͷ��һ��WM_QUIT��Ϣ����ʹGetMessage��������0��������Ϣѭ��
		::PostQuitMessage(0) ;
		return 0 ;
	}

	// �����ǲ��������Ϣ����ϵͳ��Ĭ�ϴ���
	return ::DefWindowProc(hwnd, message, wParam, lParam);
}

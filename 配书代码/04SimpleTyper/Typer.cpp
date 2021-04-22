///////////////////////////////////////////////////////////////
// Typer.cpp�ļ�



#include <windows.h>
#include "resource.h"
#include <string>

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
	wndclass.hIcon = ::LoadIcon(hInstance, 
			(LPSTR)IDI_TYPER);	// ʹ���Զ���ͼ��	
	wndclass.hCursor = ::LoadCursor(NULL,
			IDC_ARROW);		// ʹ��Ԥ����Ĺ��
	wndclass.hbrBackground =  
		(HBRUSH)(COLOR_3DFACE + 1);	// ʹ��Ԥ���廭ˢ
	wndclass.lpszMenuName = (LPSTR)IDR_TYPER;	
	wndclass.lpszClassName = szClassName ;	// �����������	
	wndclass.hIconSm = NULL;		// û�����Сͼ��
	
	// ע�����������
	::RegisterClassEx(&wndclass); 
	
	// ����������
	HWND hwnd = ::CreateWindowEx( 
		WS_EX_CLIENTEDGE,	// dwExStyle����չ��ʽ
					// ע�⣬WS_EX_CLIENTEDGE���ָ�������������һ���߿��һ���³��ı�Ե
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
	// str�������ڱ��洰�ڿͻ�����ʾ���ַ���
	// Ϊ��ʹ��string�࣬Ӧ�ð���ͷ�ļ�����#include <string>��
	static std::string str; 

	switch (message)
	{
	case WM_CREATE:
		{
			// ���ô��ڵı���
			::SetWindowText(hwnd, "���ª�Ĵ��ֳ���");
			//::SendMessage(hwnd, WM_SETTEXT, 0, (long)"���ª�Ĵ��ֳ���");
			return 0;
		}

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case ID_FILE_EXIT:
			// ��hwndָ���Ĵ��ڷ���һ��WM_CLOSE��Ϣ��
			::SendMessage(hwnd, WM_CLOSE, 0, 0);
			// ::PostMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		}

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = ::BeginPaint(hwnd, &ps);
				
			// ��������ı��ı�����ɫ��������ɫ
			::SetTextColor(hdc, RGB(255, 0, 0));  
			::SetBkColor(hdc, ::GetSysColor(COLOR_3DFACE));
	
			::TextOut(hdc, 0, 0, str.c_str(), str.length ());
			::EndPaint(hwnd, &ps);
			return 0;
		}

	case WM_CHAR:
		{
			// ����ansi��
			str = str + char(wParam);
			
			// ʹ�����ͻ�����Ч
			::InvalidateRect(hwnd,NULL,0);
			return 0;
		}

	case WM_LBUTTONDOWN:
		{	
			char szPoint[56];
			// ����������Ϣ
			wsprintf(szPoint,"X =%d,Y =%d",LOWORD(lParam),HIWORD(lParam));
			
			str = szPoint;
			
			if(wParam & MK_SHIFT)
				str = str + "   Shift Key is down";
			::InvalidateRect(hwnd,NULL,1);
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

///////////////////////////////////////////////////////////////
// UseDC.cpp�ļ�


#include <windows.h>

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	HDC hdc;
	HWND hWnd;
	char sz[] = "��Һ�";
	
	// ���Ҽ��±�����Ĵ��ھ��
	hWnd = ::FindWindow("Notepad", NULL);

	// ������±����������У�������ͻ��������ı�
	while(::IsWindow(hWnd))		// IsWindow���������ж�һ�����ھ���Ƿ���Ч
	{
		hdc = ::GetDC(hWnd);
		::TextOut(hdc, 10, 10, sz, strlen(sz));
		::ReleaseDC(hWnd, NULL);
		::Sleep(1000);
	}
	::MessageBox(NULL, "���±������Ѿ��˳�", "04UseDC", MB_OK);
	
	return 0;
}




///////////////////////////////////////////////////////////////
// 04TellToClose.cpp�ļ�


#include "stdafx.h"
#include <windows.h>

int main(int argc, char* argv[])
{
	// ���ұ���Ϊ���ޱ��� - ���±����Ĵ��� 
	// Ҳ����ʹ�����������ң���::FindWindow("Notepad", NULL);
	HWND hWnd = ::FindWindow(NULL, "�ޱ��� - ���±�");
	if(hWnd != NULL)
	{
		// ��Ŀ�괰�ڷ���WM_CLOSE��Ϣ
		::SendMessage(hWnd, WM_CLOSE, 0, 0);
	}
	return 0;
}


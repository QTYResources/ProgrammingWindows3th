///////////////////////////////////////////////////////////////
// 04Win32AppDemo.cpp�ļ�


#include "stdafx.h"

int APIENTRY WinMain(HINSTANCE hInstance,	// ��ģ���ʵ�����
                     HINSTANCE hPrevInstance,	// Win16���µķ�������Ѿ�������
                     LPSTR     lpCmdLine,	// �����в���
                     int       nCmdShow)	// �����ڳ�ʼ��ʱ����ʾ��ʽ
{
	int nSel = ::MessageBox(NULL, "Hello, Win32 Application", 
			"04Win32AppDemo", MB_YESNOCANCEL|MB_ICONQUESTION|MB_DEFBUTTON3);
	if(nSel == IDYES)		// �û�ѡ���ˡ��ǡ���ť
	{
	} else if(nSel == IDNO)		// �û�ѡ���ˡ��񡱰�ť
	{
	} else if(nSel == IDCANCEL)	// �û�ѡ���ˡ�ȡ������ť
	{
	}

	return 0;
}




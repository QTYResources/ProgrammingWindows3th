///////////////////////////////////////////////////////////////
// 01CodeStyle.cpp�ļ�


#include "stdafx.h"
#include <windows.h>

void Alert(int i);


int main(int argc, char* argv[])
{
	Alert(2);

	return 0;
}

void Alert(int i)
{
	while(i > 0)
	{
		// Beep������ʹ�����������򵥵�������
		// Ҫ�������������Ӧ�ð�����ͷ�ļ���windows.h��
		Beep(1000, 1000);   
		i--;
	}
}

/*

MessageBox(NULL, "Hello, Windows XP", "Greetings", MB_OKCANCEL);// �ÿո�ָ����
int i = 10*(123 + 100/5);					// �ÿո�ָֵ���
if((a > b) && (c < d) && (e == f))
����

  int nSelect = ::MessageBox(
			NULL,				// ���ھ��
			"Hello, Windows XP",		// �Ի���Ҫ��ʾ������
			"Greetings",			// �Ի���ı���
			MB_YESNOCANCEL|MB_DEFBUTTON2);	// �Ի���ķ��				// �ÿո�ָ���ʽ��


  
	// ����һ���µ��߳�
	HANDLE hThread = ::CreateThread(����);
	if(hThread == NULL)
	{
		return FALSE;	// ʧ�ܣ�
	}
	else
	{
		return TRUE;	// �ɹ���
	}
*/


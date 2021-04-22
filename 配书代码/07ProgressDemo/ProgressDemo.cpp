///////////////////////////////////////////
// ProgressDemo.cpp�ļ�	


#include <windows.h>
#include <commctrl.h>
#include "resource.h"

// ���ӵ�comctl32.lib��
#pragma comment(lib,"comctl32.lib")

BOOL __stdcall DlgProc(HWND, UINT, WPARAM, LPARAM);

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	::InitCommonControls();

	::DialogBoxParam(
		hInstance,		// ʵ�����
		(LPCTSTR)IDD_MAIN,	// �Ի�����ԴID��
		NULL,			// �����ھ��
		DlgProc,		// ��Ϣ������
		NULL); 			// �Ի����ʼ����ֵ����WM_INITDIALOG��Ϣ��lParam������ȡ��

	return 0;
}


BOOL __stdcall DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{	
	case WM_INITDIALOG:
		{
			// ��ʼ���������ؼ�
			HWND hWndProgress = ::GetDlgItem(hDlg, IDC_PROGRESS);

			// ���ý�������ȡֵ��Χ
			::SendMessage(hWndProgress, PBM_SETRANGE, 0, MAKELPARAM(0, 20));
			// ���ò���
			::SendMessage(hWndProgress, PBM_SETSTEP, 1, 0);
			// ���ñ���ɫ
			::SendMessage(hWndProgress, PBM_SETBKCOLOR, 0, RGB(0, 0, 0xff));
			// ���ý���������ɫ
			::SendMessage(hWndProgress, PBM_SETBARCOLOR, 0, RGB(0xff, 0, 0));
		}
		break;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			// ���ӽ���������
			::SendDlgItemMessage(hDlg, IDC_PROGRESS, PBM_STEPIT, 0, 0);
			break;
		case IDCANCEL:
			::EndDialog (hDlg, IDCANCEL);
			break;
		}
		break;
	}
	return 0;
}




/////////////////////////////////////////////////
// FirstDialog.cpp�ļ�



#include <windows.h>
#include "resource.h"

BOOL __stdcall DlgProc(HWND, UINT, WPARAM, LPARAM);

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	int nResult = ::DialogBoxParam(
		hInstance,		// ʵ�����
		(LPCTSTR)IDD_MAIN,	// �Ի�����ԴID��
		NULL,			// �����ھ��
		DlgProc,		// ��Ϣ������
		NULL); 			// �Ի����ʼ����ֵ����WM_INITDIALOG��Ϣ��lParam������ȡ��
	if(nResult == IDOK)
		::MessageBox(NULL, "�û�ѡ����OK��ť", "07FirstDialog", MB_OK);
	else
		::MessageBox(NULL, "�û�ѡ����CANCEL��ť", "07FirstDialog", MB_OK);

	return 0;
}

BOOL __stdcall DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{	
	case WM_INITDIALOG: // ��ʼ���Ի���
		::SetWindowText(hDlg,"��һ���Ի���");
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			::EndDialog(hDlg, IDOK);
			break;
		case IDCANCEL:
			::EndDialog (hDlg, IDCANCEL);
			break;
		}
		break;
	}
	return 0;
}



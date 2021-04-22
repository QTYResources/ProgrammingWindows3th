/////////////////////////////////////////////////
// CommDlg.cpp�ļ�


#include <windows.h>
#include "resource.h"
#include "DirDialog.h"

BOOL __stdcall DlgProc(HWND, UINT, WPARAM, LPARAM);

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	int nResult = ::DialogBoxParam(
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
		case IDC_FILEBROWSER:
			{
				char szFileName[MAX_PATH] = "";
				OPENFILENAME file = { 0 };
				file.lStructSize = sizeof(file);
				

				file.lpstrFile = szFileName;
				file.nMaxFile = MAX_PATH;
		
				file.lpstrFilter = "Text Files(*.txt)\0*.txt\0All Files\0*.*\0\0";
				file.nFilterIndex = 1;
				
				// �������ļ��ĶԻ���
				if(::GetOpenFileName(&file))
				{
					::SetWindowText(::GetDlgItem(hDlg, IDC_FILE), szFileName);
				}
			}
			break;

		case IDC_DIRBROWSER:
			{
				CDirDialog dir;
				if(dir.DoBrowse(hDlg))
				{
					::SetWindowText(::GetDlgItem(hDlg, IDC_DIR), dir.GetPath());
				}
			}
			break;
		}
		break;
	}

	return 0;
}



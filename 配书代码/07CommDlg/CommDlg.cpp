/////////////////////////////////////////////////
// CommDlg.cpp文件


#include <windows.h>
#include "resource.h"
#include "DirDialog.h"

BOOL __stdcall DlgProc(HWND, UINT, WPARAM, LPARAM);

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	int nResult = ::DialogBoxParam(
		hInstance,		// 实例句柄
		(LPCTSTR)IDD_MAIN,	// 对话框资源ID号
		NULL,			// 父窗口句柄
		DlgProc,		// 消息处理函数
		NULL); 			// 对话框初始化的值，在WM_INITDIALOG消息的lParam参数中取出

	return 0;
}

BOOL __stdcall DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{	
	case WM_INITDIALOG: // 初始化对话框
		::SetWindowText(hDlg,"第一个对话框！");
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
				
				// 弹出打开文件的对话框
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



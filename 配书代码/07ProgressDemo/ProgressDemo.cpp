///////////////////////////////////////////
// ProgressDemo.cpp文件	


#include <windows.h>
#include <commctrl.h>
#include "resource.h"

// 链接到comctl32.lib库
#pragma comment(lib,"comctl32.lib")

BOOL __stdcall DlgProc(HWND, UINT, WPARAM, LPARAM);

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	::InitCommonControls();

	::DialogBoxParam(
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
	case WM_INITDIALOG:
		{
			// 初始化进度条控件
			HWND hWndProgress = ::GetDlgItem(hDlg, IDC_PROGRESS);

			// 设置进度条的取值范围
			::SendMessage(hWndProgress, PBM_SETRANGE, 0, MAKELPARAM(0, 20));
			// 设置步长
			::SendMessage(hWndProgress, PBM_SETSTEP, 1, 0);
			// 设置背景色
			::SendMessage(hWndProgress, PBM_SETBKCOLOR, 0, RGB(0, 0, 0xff));
			// 设置进度条的颜色
			::SendMessage(hWndProgress, PBM_SETBARCOLOR, 0, RGB(0xff, 0, 0));
		}
		break;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			// 增加进度条进度
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




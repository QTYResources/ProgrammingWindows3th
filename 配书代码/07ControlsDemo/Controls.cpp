//////////////////////////////////////////////////
// Controls.cpp文件



#include <windows.h>
#include "resource.h"

BOOL __stdcall DlgProc(HWND, UINT, WPARAM, LPARAM);

HBITMAP g_hBitmap1;	// 第一个图片的句柄
HBITMAP g_hBitmap2;	// 第二个图片的句柄
HICON	g_hIcon;	// 对话框图标句柄
HBRUSH	g_hBgBrush;	// 背景刷子

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	// 从资源中加载BMP文件和图标，这些工作也可以在WM_INITDIALOG消息中进行
	g_hBitmap1 = ::LoadBitmap(hInstance, (LPCTSTR)IDB_BITMAP1);
	g_hBitmap2 = ::LoadBitmap(hInstance, (LPCTSTR)IDB_BITMAP2);
	g_hIcon = ::LoadIcon(hInstance, (LPCTSTR)IDI_MAIN);
	// 创建背景刷子
	g_hBgBrush = ::CreateSolidBrush(RGB(0xa6, 0xca, 0xf0));

	int nResult = ::DialogBoxParam(
		hInstance,		// 实例句柄
		(LPCTSTR)IDD_MAIN,	// 对话框资源ID号
		NULL,			// 父窗口句柄
		DlgProc,		// 消息处理函数
		NULL); 			// 对话框初始化的值，在WM_INITDIALOG消息的lParam参数中取出

	::DeleteObject(g_hBgBrush);

	return 0;
}

BOOL __stdcall DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{	
	case WM_INITDIALOG:
		{
			// 设置标题栏图标
			::SendMessage(hDlg, WM_SETICON, ICON_BIG, (long)g_hIcon);

			// 初始化显示图片的静态框架

			HWND hWndBmp = ::GetDlgItem(hDlg, IDC_BMP);
			// 设置SS_BITMAP风格
			LONG nStyle = ::GetWindowLong(hWndBmp, GWL_STYLE);
			::SetWindowLong(hWndBmp, GWL_STYLE, nStyle | SS_BITMAP);
			// 设置图片
			::SendDlgItemMessage(hDlg, IDC_BMP, STM_SETIMAGE, IMAGE_BITMAP, (long)g_hBitmap1);

			// 初始化复选框
			::CheckDlgButton(hDlg, IDC_BMP1, BST_CHECKED);
		}
		break;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:	// 向列表框中添加文本
			{
				HWND hWndEdit = ::GetDlgItem(hDlg, IDC_INPUT);

				// 取得文本框中的文本
				char szText[256];
				int nLen = ::GetWindowText(hWndEdit, szText, 256);
				if(nLen > 0)
				{
					// 向列表框控件中添加文本
					::SendDlgItemMessage(hDlg, IDC_LIST, LB_ADDSTRING, NULL, (long)szText);
					// 清空文本框中的文本
					::SetWindowText(hWndEdit, "");
				}
			
			}
			break;

		case IDCANCEL:		// 退出程序
			::EndDialog (hDlg, IDCANCEL);
			break;

		case IDC_TOPMOST:	// 设置对话框的Z轴位置 
			{
				HWND hWndCheck = ::GetDlgItem(hDlg, IDC_TOPMOST);
				int nRet = ::SendMessage(hWndCheck, BM_GETCHECK, 0, 0);
				if(nRet == BST_CHECKED)
				{
					::SetWindowPos(hDlg, HWND_TOPMOST, 0, 0, 0, 0, 
						SWP_NOMOVE|SWP_NOSIZE|SWP_NOREDRAW);
				}
				else
				{
					::SetWindowPos(hDlg, HWND_NOTOPMOST, 0, 0, 0, 0, 
						SWP_NOMOVE|SWP_NOSIZE|SWP_NOREDRAW);
				}
			}
			break;

		case IDC_BMP1:		// 更换到第一个图片
			{
				int nRet = ::IsDlgButtonChecked(hDlg, IDC_BMP1);
				if(nRet == BST_CHECKED)
				{
					::SendDlgItemMessage(hDlg, IDC_BMP, 
						STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)g_hBitmap1);
				}
			
			}
			break;

		case IDC_BMP2:		// 更换到第二个图片
			{
				int nRet = ::IsDlgButtonChecked(hDlg, IDC_BMP2);
				if(nRet == BST_CHECKED)
				{
					::SendDlgItemMessage(hDlg, IDC_BMP, 
						STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)g_hBitmap2);
				}
			}
			break;

		case IDC_HIDE:		// 更换图片的显示状态
			{
				HWND hWndBmp = ::GetDlgItem(hDlg, IDC_BMP);
				HWND hWndCheck = ::GetDlgItem(hDlg, IDC_HIDE);
				int nRet = ::SendMessage(hWndCheck, BM_GETCHECK, 0, 0);
				if(nRet == BST_CHECKED)
					::ShowWindow(hWndBmp, SW_HIDE);
				else
					::ShowWindow(hWndBmp, SW_SHOW);
			}
			break;
		}
		break;

	case WM_CTLCOLORSTATIC:	// 设置静态文本框的背景色
	case WM_CTLCOLORDLG:	// 设置对话框的背景色
		{
			HDC hdc = (HDC)wParam;

			// 为静态文本框设置文本背景色
			::SetBkColor(hdc, RGB(0xa6, 0xca, 0xf0));

			if((HWND)lParam == ::GetDlgItem(hDlg, IDC_AUTHOR))
			{
				::SetTextColor(hdc, RGB(0, 0, 0xff));
			}
			
			return (int)g_hBgBrush;
		}
	}

	return 0;
}



//////////////////////////////////////////////////
// Controls.cpp�ļ�



#include <windows.h>
#include "resource.h"

BOOL __stdcall DlgProc(HWND, UINT, WPARAM, LPARAM);

HBITMAP g_hBitmap1;	// ��һ��ͼƬ�ľ��
HBITMAP g_hBitmap2;	// �ڶ���ͼƬ�ľ��
HICON	g_hIcon;	// �Ի���ͼ����
HBRUSH	g_hBgBrush;	// ����ˢ��

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	// ����Դ�м���BMP�ļ���ͼ�꣬��Щ����Ҳ������WM_INITDIALOG��Ϣ�н���
	g_hBitmap1 = ::LoadBitmap(hInstance, (LPCTSTR)IDB_BITMAP1);
	g_hBitmap2 = ::LoadBitmap(hInstance, (LPCTSTR)IDB_BITMAP2);
	g_hIcon = ::LoadIcon(hInstance, (LPCTSTR)IDI_MAIN);
	// ��������ˢ��
	g_hBgBrush = ::CreateSolidBrush(RGB(0xa6, 0xca, 0xf0));

	int nResult = ::DialogBoxParam(
		hInstance,		// ʵ�����
		(LPCTSTR)IDD_MAIN,	// �Ի�����ԴID��
		NULL,			// �����ھ��
		DlgProc,		// ��Ϣ������
		NULL); 			// �Ի����ʼ����ֵ����WM_INITDIALOG��Ϣ��lParam������ȡ��

	::DeleteObject(g_hBgBrush);

	return 0;
}

BOOL __stdcall DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{	
	case WM_INITDIALOG:
		{
			// ���ñ�����ͼ��
			::SendMessage(hDlg, WM_SETICON, ICON_BIG, (long)g_hIcon);

			// ��ʼ����ʾͼƬ�ľ�̬���

			HWND hWndBmp = ::GetDlgItem(hDlg, IDC_BMP);
			// ����SS_BITMAP���
			LONG nStyle = ::GetWindowLong(hWndBmp, GWL_STYLE);
			::SetWindowLong(hWndBmp, GWL_STYLE, nStyle | SS_BITMAP);
			// ����ͼƬ
			::SendDlgItemMessage(hDlg, IDC_BMP, STM_SETIMAGE, IMAGE_BITMAP, (long)g_hBitmap1);

			// ��ʼ����ѡ��
			::CheckDlgButton(hDlg, IDC_BMP1, BST_CHECKED);
		}
		break;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:	// ���б��������ı�
			{
				HWND hWndEdit = ::GetDlgItem(hDlg, IDC_INPUT);

				// ȡ���ı����е��ı�
				char szText[256];
				int nLen = ::GetWindowText(hWndEdit, szText, 256);
				if(nLen > 0)
				{
					// ���б��ؼ�������ı�
					::SendDlgItemMessage(hDlg, IDC_LIST, LB_ADDSTRING, NULL, (long)szText);
					// ����ı����е��ı�
					::SetWindowText(hWndEdit, "");
				}
			
			}
			break;

		case IDCANCEL:		// �˳�����
			::EndDialog (hDlg, IDCANCEL);
			break;

		case IDC_TOPMOST:	// ���öԻ����Z��λ�� 
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

		case IDC_BMP1:		// ��������һ��ͼƬ
			{
				int nRet = ::IsDlgButtonChecked(hDlg, IDC_BMP1);
				if(nRet == BST_CHECKED)
				{
					::SendDlgItemMessage(hDlg, IDC_BMP, 
						STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)g_hBitmap1);
				}
			
			}
			break;

		case IDC_BMP2:		// �������ڶ���ͼƬ
			{
				int nRet = ::IsDlgButtonChecked(hDlg, IDC_BMP2);
				if(nRet == BST_CHECKED)
				{
					::SendDlgItemMessage(hDlg, IDC_BMP, 
						STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)g_hBitmap2);
				}
			}
			break;

		case IDC_HIDE:		// ����ͼƬ����ʾ״̬
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

	case WM_CTLCOLORSTATIC:	// ���þ�̬�ı���ı���ɫ
	case WM_CTLCOLORDLG:	// ���öԻ���ı���ɫ
		{
			HDC hdc = (HDC)wParam;

			// Ϊ��̬�ı��������ı�����ɫ
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



/////////////////////////////////////////////////
// ComctlDemo.cpp文件



#include <windows.h>
#include <commctrl.h>
#include <tlhelp32.h>

#include "resource.h"

// 链接到comctl32.lib库
#pragma comment(lib,"comctl32.lib")

// 状态栏ID号
#define IDC_STATUS 101

BOOL __stdcall DlgProc(HWND, UINT, WPARAM, LPARAM);
void UpdateProcess(HWND hWndList);

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	// 初始化Comctl32.dll库
	::InitCommonControls();

	::DialogBoxParam(hInstance, (LPCTSTR)IDD_MAIN, NULL, DlgProc, NULL); 		

	return 0;
}

BOOL __stdcall DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{	
	case WM_INITDIALOG:
		{
			// 初始化列表视图控件

			HWND hWndList = ::GetDlgItem(hDlg, IDC_LIST);
	
			// 设置它的扩展风格
			::SendMessage(hWndList, LVM_SETEXTENDEDLISTVIEWSTYLE, 
						0, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

			LVCOLUMN column;
			// 指定LVCOLUMN结构中的pszText、fmt、cx域有效
			column.mask = LVCF_TEXT|LVCF_FMT|LVCF_WIDTH;	
			// 设置有效的域的属性
			column.fmt = LVCFMT_CENTER;	// 指定文本居中显示
			column.cx = 100;		// 指定此栏的宽度
			column.pszText = "映象名称";	// 指定此栏显示的文本

			// 添加一个新的专栏
			::SendMessage(hWndList, LVM_INSERTCOLUMN, 0, (LPARAM)&column);
			// 再添加一个专栏
			column.pszText = "PID";
			column.cx = 50;
			::SendMessage(hWndList, LVM_INSERTCOLUMN, 1, (LPARAM)&column);

			// 初始化状态栏

			// 创建状态栏
			HWND hWndStatus = ::CreateStatusWindow(WS_CHILD|WS_VISIBLE|SBS_SIZEGRIP, 
										NULL, hDlg, IDC_STATUS);
			// 设置背景色
			::SendMessage(hWndStatus, SB_SETBKCOLOR, 0, RGB(0xa6, 0xca, 0xf0));
			// 给状态栏分栏
			int pInt[] = { 152, -1 };
			::SendMessage(hWndStatus, SB_SETPARTS, 2, (long)pInt);
			// 设置各栏的文本
			::SendMessage(hWndStatus, SB_SETTEXT, 0, (long)" 准备就绪");
			::SendMessage(hWndStatus, SB_SETTEXT, 1, (long)" Windows程序设计进阶之路！");

			// 刷新进程列表
			UpdateProcess(hWndList);
		}
		break;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			::EndDialog(hDlg, IDOK);
			break;
		case IDCANCEL:
			::EndDialog(hDlg, IDCANCEL);
			break;
		case IDC_UPDATE:
			UpdateProcess(::GetDlgItem(hDlg, IDC_LIST));
			break;
		}
		break;

	case WM_NOTIFY:
		{
			if(wParam == IDC_LIST)
			{
				NMHDR* pHeader = (NMHDR*)lParam;
				HWND hWndList = pHeader->hwndFrom;

				if(pHeader->code == NM_DBLCLK)	// 双击事件
				{
					NMLISTVIEW* pNMListView = (NMLISTVIEW*)pHeader;
					// 用户双击的项号
					int nIndex = pNMListView->iItem;
					
					// 取得进程ID号
					char szID[56];
					LVITEM lvi;
					memset(&lvi, 0, sizeof(LVITEM));
					lvi.iSubItem = 1;	// nIndex项目中的第1个子项
					lvi.cchTextMax = 56;
					lvi.pszText = szID;
					::SendMessage(hWndList, LVM_GETITEMTEXT, (WPARAM)nIndex, (long)&lvi);

					// 询问用户
					if(::MessageBox(hDlg, "确实要终止进程吗？", 
						"07ComctlDemo", MB_OKCANCEL|MB_DEFBUTTON2) == IDCANCEL)
						return 0;

					// 试图打开目标进程，终止它
					HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, atoi(szID));
					if(hProcess != NULL)
					{
						HWND hWndStatus = ::GetDlgItem(hDlg, IDC_STATUS);
						if(::TerminateProcess(hProcess, 0))
						{
							::SendMessage(hWndStatus, SB_SETTEXT, 0, (long)"终止进程成功！");
							UpdateProcess(hWndList);
						}
						else
						{
							::SendMessage(hWndStatus, SB_SETTEXT, 0, (long)"终止进程失败！");
						}
						
					}
				}
			}
		}
		break;
	}
	return 0;
}

void UpdateProcess(HWND hWndList)
{
	// 删除所有的项
	::SendMessage(hWndList, LVM_DELETEALLITEMS, 0, 0);

	int nItem = 0;	// 项计数

	PROCESSENTRY32 pe32 = { sizeof(PROCESSENTRY32) }; 
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 
	if(hProcessSnap == INVALID_HANDLE_VALUE) 
		return; 
	if(Process32First(hProcessSnap, &pe32)) 
	{ 
		do 
		{ 
			// 取得进程ID号
			char szID[56];
			wsprintf(szID, "%u", pe32.th32ProcessID);
			
			// 插入一个项
			LVITEM item = { 0 };
			item.iItem = nItem;

			item.mask = LVIF_TEXT;			// 指定pszText域有效
			item.pszText = (LPTSTR)pe32.szExeFile;	// 设置文本
			
			::SendMessage(hWndList, LVM_INSERTITEM, 0, (long)&item);
			
			// 设置新项的文本
			LVITEM lvi;
			lvi.iSubItem = 1;		// 指定要设置第1个专栏的文本
			lvi.pszText = (LPTSTR)szID;	// 要设置的文本
			::SendMessage(hWndList, LVM_SETITEMTEXT, nItem, (LPARAM)&lvi);

			nItem++;
			
		} 
		while(Process32Next(hProcessSnap, &pe32)); 
	}
	::CloseHandle(hProcessSnap);
}

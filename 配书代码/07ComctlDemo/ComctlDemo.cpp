/////////////////////////////////////////////////
// ComctlDemo.cpp�ļ�



#include <windows.h>
#include <commctrl.h>
#include <tlhelp32.h>

#include "resource.h"

// ���ӵ�comctl32.lib��
#pragma comment(lib,"comctl32.lib")

// ״̬��ID��
#define IDC_STATUS 101

BOOL __stdcall DlgProc(HWND, UINT, WPARAM, LPARAM);
void UpdateProcess(HWND hWndList);

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	// ��ʼ��Comctl32.dll��
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
			// ��ʼ���б���ͼ�ؼ�

			HWND hWndList = ::GetDlgItem(hDlg, IDC_LIST);
	
			// ����������չ���
			::SendMessage(hWndList, LVM_SETEXTENDEDLISTVIEWSTYLE, 
						0, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

			LVCOLUMN column;
			// ָ��LVCOLUMN�ṹ�е�pszText��fmt��cx����Ч
			column.mask = LVCF_TEXT|LVCF_FMT|LVCF_WIDTH;	
			// ������Ч���������
			column.fmt = LVCFMT_CENTER;	// ָ���ı�������ʾ
			column.cx = 100;		// ָ�������Ŀ��
			column.pszText = "ӳ������";	// ָ��������ʾ���ı�

			// ���һ���µ�ר��
			::SendMessage(hWndList, LVM_INSERTCOLUMN, 0, (LPARAM)&column);
			// �����һ��ר��
			column.pszText = "PID";
			column.cx = 50;
			::SendMessage(hWndList, LVM_INSERTCOLUMN, 1, (LPARAM)&column);

			// ��ʼ��״̬��

			// ����״̬��
			HWND hWndStatus = ::CreateStatusWindow(WS_CHILD|WS_VISIBLE|SBS_SIZEGRIP, 
										NULL, hDlg, IDC_STATUS);
			// ���ñ���ɫ
			::SendMessage(hWndStatus, SB_SETBKCOLOR, 0, RGB(0xa6, 0xca, 0xf0));
			// ��״̬������
			int pInt[] = { 152, -1 };
			::SendMessage(hWndStatus, SB_SETPARTS, 2, (long)pInt);
			// ���ø������ı�
			::SendMessage(hWndStatus, SB_SETTEXT, 0, (long)" ׼������");
			::SendMessage(hWndStatus, SB_SETTEXT, 1, (long)" Windows������ƽ���֮·��");

			// ˢ�½����б�
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

				if(pHeader->code == NM_DBLCLK)	// ˫���¼�
				{
					NMLISTVIEW* pNMListView = (NMLISTVIEW*)pHeader;
					// �û�˫�������
					int nIndex = pNMListView->iItem;
					
					// ȡ�ý���ID��
					char szID[56];
					LVITEM lvi;
					memset(&lvi, 0, sizeof(LVITEM));
					lvi.iSubItem = 1;	// nIndex��Ŀ�еĵ�1������
					lvi.cchTextMax = 56;
					lvi.pszText = szID;
					::SendMessage(hWndList, LVM_GETITEMTEXT, (WPARAM)nIndex, (long)&lvi);

					// ѯ���û�
					if(::MessageBox(hDlg, "ȷʵҪ��ֹ������", 
						"07ComctlDemo", MB_OKCANCEL|MB_DEFBUTTON2) == IDCANCEL)
						return 0;

					// ��ͼ��Ŀ����̣���ֹ��
					HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, atoi(szID));
					if(hProcess != NULL)
					{
						HWND hWndStatus = ::GetDlgItem(hDlg, IDC_STATUS);
						if(::TerminateProcess(hProcess, 0))
						{
							::SendMessage(hWndStatus, SB_SETTEXT, 0, (long)"��ֹ���̳ɹ���");
							UpdateProcess(hWndList);
						}
						else
						{
							::SendMessage(hWndStatus, SB_SETTEXT, 0, (long)"��ֹ����ʧ�ܣ�");
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
	// ɾ�����е���
	::SendMessage(hWndList, LVM_DELETEALLITEMS, 0, 0);

	int nItem = 0;	// �����

	PROCESSENTRY32 pe32 = { sizeof(PROCESSENTRY32) }; 
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 
	if(hProcessSnap == INVALID_HANDLE_VALUE) 
		return; 
	if(Process32First(hProcessSnap, &pe32)) 
	{ 
		do 
		{ 
			// ȡ�ý���ID��
			char szID[56];
			wsprintf(szID, "%u", pe32.th32ProcessID);
			
			// ����һ����
			LVITEM item = { 0 };
			item.iItem = nItem;

			item.mask = LVIF_TEXT;			// ָ��pszText����Ч
			item.pszText = (LPTSTR)pe32.szExeFile;	// �����ı�
			
			::SendMessage(hWndList, LVM_INSERTITEM, 0, (long)&item);
			
			// ����������ı�
			LVITEM lvi;
			lvi.iSubItem = 1;		// ָ��Ҫ���õ�1��ר�����ı�
			lvi.pszText = (LPTSTR)szID;	// Ҫ���õ��ı�
			::SendMessage(hWndList, LVM_SETITEMTEXT, nItem, (LPARAM)&lvi);

			nItem++;
			
		} 
		while(Process32Next(hProcessSnap, &pe32)); 
	}
	::CloseHandle(hProcessSnap);
}

///////////////////////////////////////
// PredefineClass.cpp

#include "PredefineClass.h"

CMyApp theApp;

///////////////////////////////////////

BOOL CMyApp::InitInstance()
{
	m_pMainWnd = new CMainWindow;
	m_pMainWnd->ShowWindow(m_nCmdShow);
	return TRUE;
}

////////////////////////////////////////

CMainWindow::CMainWindow()
{
	LPCTSTR lpszClassName = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW, 
		::LoadCursor(NULL, IDC_ARROW), (HBRUSH)(COLOR_3DFACE+1));

	CreateEx(WS_EX_CLIENTEDGE, lpszClassName, 
		"框架程序创建的窗口", WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL);
}

BEGIN_MESSAGE_MAP(CMainWindow, CWnd)
ON_WM_CREATE()
END_MESSAGE_MAP()

void CMainWindow::PostNcDestroy()
{
	delete this;
}


BOOL CMainWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// 创建三个不同风格的按钮
	::CreateWindowEx(0, "button", "push button", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
		50, 30, 110, 30, m_hWnd, (HMENU)IDC_BUTTON, AfxGetApp()->m_hInstance, NULL);
	::CreateWindowEx(0, "button", "radio button", WS_CHILD|WS_VISIBLE|BS_RADIOBUTTON,
		50, 70, 110, 30, m_hWnd, (HMENU)IDC_RADIO, AfxGetApp()->m_hInstance, NULL);
	::CreateWindowEx(0, "button", "check box", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,
		50, 110, 110, 30, m_hWnd, (HMENU)IDC_CHECKBOX, AfxGetApp()->m_hInstance, NULL);

	// 创建静态文本
	::CreateWindowEx(0, "static", "static text", WS_CHILD|WS_VISIBLE|SS_SUNKEN,
		50, 150, 150, 60, m_hWnd, (HMENU)IDC_STATIC, AfxGetApp()->m_hInstance, NULL);

	// 创建文本框
	::CreateWindowEx(0, "edit", "edit text", WS_CHILD|WS_VISIBLE|WS_BORDER|ES_MULTILINE,
		50, 220, 150, 60, m_hWnd, (HMENU)IDC_EDITTEXT, AfxGetApp()->m_hInstance, NULL);

	return TRUE;
}

BOOL CMainWindow::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(wParam))
	{
	case IDC_BUTTON:
		::MessageBox(m_hWnd, "大家好！", "Button", MB_OK);
		break;

	case IDC_RADIO:
		{
			// 是否选中单选按钮
			BOOL bChecked = ::IsDlgButtonChecked(m_hWnd, IDC_RADIO);
			// 设置上面的button有效状态
			::EnableWindow(::GetDlgItem(m_hWnd, IDC_BUTTON), bChecked);
			// 设置本控件状态
			::CheckDlgButton(m_hWnd, IDC_RADIO, !bChecked);
		}
		break;

	case IDC_EDITTEXT:
		{
			// 如果是改变文本框中的文本，则在静态文本框中也做相应的修改
			if(HIWORD(wParam) == EN_CHANGE)
			{
				char sz[256];
				::GetWindowText(::GetDlgItem(m_hWnd, IDC_EDITTEXT), sz, 256);
				::SetWindowText(::GetDlgItem(m_hWnd, IDC_STATIC), sz);
			}
		}
		break;
	}

		

	return 0;
}

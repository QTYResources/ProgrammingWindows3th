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
		"��ܳ��򴴽��Ĵ���", WS_OVERLAPPEDWINDOW, 
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
	// ����������ͬ���İ�ť
	::CreateWindowEx(0, "button", "push button", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
		50, 30, 110, 30, m_hWnd, (HMENU)IDC_BUTTON, AfxGetApp()->m_hInstance, NULL);
	::CreateWindowEx(0, "button", "radio button", WS_CHILD|WS_VISIBLE|BS_RADIOBUTTON,
		50, 70, 110, 30, m_hWnd, (HMENU)IDC_RADIO, AfxGetApp()->m_hInstance, NULL);
	::CreateWindowEx(0, "button", "check box", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,
		50, 110, 110, 30, m_hWnd, (HMENU)IDC_CHECKBOX, AfxGetApp()->m_hInstance, NULL);

	// ������̬�ı�
	::CreateWindowEx(0, "static", "static text", WS_CHILD|WS_VISIBLE|SS_SUNKEN,
		50, 150, 150, 60, m_hWnd, (HMENU)IDC_STATIC, AfxGetApp()->m_hInstance, NULL);

	// �����ı���
	::CreateWindowEx(0, "edit", "edit text", WS_CHILD|WS_VISIBLE|WS_BORDER|ES_MULTILINE,
		50, 220, 150, 60, m_hWnd, (HMENU)IDC_EDITTEXT, AfxGetApp()->m_hInstance, NULL);

	return TRUE;
}

BOOL CMainWindow::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(wParam))
	{
	case IDC_BUTTON:
		::MessageBox(m_hWnd, "��Һã�", "Button", MB_OK);
		break;

	case IDC_RADIO:
		{
			// �Ƿ�ѡ�е�ѡ��ť
			BOOL bChecked = ::IsDlgButtonChecked(m_hWnd, IDC_RADIO);
			// ���������button��Ч״̬
			::EnableWindow(::GetDlgItem(m_hWnd, IDC_BUTTON), bChecked);
			// ���ñ��ؼ�״̬
			::CheckDlgButton(m_hWnd, IDC_RADIO, !bChecked);
		}
		break;

	case IDC_EDITTEXT:
		{
			// ����Ǹı��ı����е��ı������ھ�̬�ı�����Ҳ����Ӧ���޸�
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

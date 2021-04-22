///////////////////////////////////////
// Meminfo.cpp�ļ�

#include "Meminfo.h"
#include "resource.h"

#define IDT_TIMER	101

CMyApp theApp;

BOOL CMyApp::InitInstance()
{
	m_pMainWnd = new CMainWindow;
	::ShowWindow(*m_pMainWnd, m_nCmdShow);
	::UpdateWindow(*m_pMainWnd);

	return TRUE;
}

CMainWindow::CMainWindow()
{
	m_szText[0] = '\0';

	LPCTSTR lpszClassName = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW, 
		::LoadCursor(NULL, IDC_ARROW), (HBRUSH)(COLOR_3DFACE+1), AfxGetApp()->LoadIcon(IDI_MAIN));

	CreateEx(WS_EX_CLIENTEDGE, lpszClassName, 
		"�ڴ�ʹ�ü�����", WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, CW_USEDEFAULT, 300, 230, NULL, NULL);
}

// CMainWindow�����Ϣӳ���
BEGIN_MESSAGE_MAP(CMainWindow, CWnd)
ON_WM_CREATE()
ON_WM_PAINT()
ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CMainWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// ������ʾ�ı����ڷ���Ĵ�С
	::GetClientRect(m_hWnd, &m_rcInfo);
	m_rcInfo.left = 30;
	m_rcInfo.top = 20;
	m_rcInfo.right = m_rcInfo.right - 30;
	m_rcInfo.bottom = m_rcInfo.bottom - 30;

	// ��װ��ʱ��
	::SetTimer(m_hWnd, IDT_TIMER, 500, NULL);

	// �������ᵽ���
	::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOREDRAW | SWP_NOSIZE);

	return TRUE;
}

void CMainWindow::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == IDT_TIMER)
	{
		char szBuff[128];
		MEMORYSTATUS ms;
		
		// ȡ���ڴ�״̬��Ϣ
		::GlobalMemoryStatus(&ms);


		// ��ȡ�õ���Ϣ���뻺����m_szText��
	
		m_szText[0] = '\0';
		
		wsprintf(szBuff, "\n  �����ڴ�������  %-5d MB", ms.dwTotalPhys/(1024*1024));
		strcat(m_szText, szBuff);
		wsprintf(szBuff, "\n  ���������ڴ棺  %-5d MB", ms.dwAvailPhys/(1024*1024));
		strcat(m_szText, szBuff);
		
		wsprintf(szBuff, "\n\n  �����ڴ�������  %-5d MB", ms.dwTotalVirtual/(1024*1024));
		strcat(m_szText, szBuff);
		wsprintf(szBuff, "\n  ���������ڴ棺  %-5d MB", ms.dwAvailVirtual/(1024*1024));
		strcat(m_szText, szBuff);
		
		wsprintf(szBuff, "\n\n  �ڴ�ʹ���ʣ�     %d%%", ms.dwMemoryLoad);
		strcat(m_szText, szBuff);
		
		// ��Ч��ʾ�ı�����������ʹϵͳ����WM_PAINT��Ϣ��������ʾ��Ϣ
		::InvalidateRect(m_hWnd, &m_rcInfo, TRUE);
	}	
}

void CMainWindow::OnPaint()
{
	PAINTSTRUCT ps;
	HDC hdc = ::BeginPaint(m_hWnd, &ps);

	// ���ñ���Ϊ͸��ģʽ
	::SetBkMode(hdc, TRANSPARENT);

	// ��������
	HFONT hFont = ::CreateFont(12, 0, 0, 0, FW_HEAVY, 0, 0, 0, ANSI_CHARSET, \
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, \
		VARIABLE_PITCH | FF_SWISS, "MS Sans Serif" );
	// ������ˢ
	HBRUSH hBrush = ::CreateSolidBrush(RGB(0xa0, 0xa0, 0xa0));
	// ������ѡ�뵽�豸������
	HFONT hOldFont = (HFONT)::SelectObject(hdc, hFont);
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hBrush);

	// �����ı���ɫ
	::SetTextColor(hdc, RGB(0x32, 0x32, 0xfa));
	// ��һ��Բ�Ǿ���
	::RoundRect(hdc, m_rcInfo.left, m_rcInfo.top, m_rcInfo.right, m_rcInfo.bottom, 5, 5);
	
	// �����ı�
	::DrawText(hdc, m_szText, strlen(m_szText), &m_rcInfo, 0);
	
	// �����Դ
	::DeleteObject(::SelectObject(hdc, hOldFont));
	::DeleteObject(::SelectObject(hdc, hOldBrush));
	::EndPaint(m_hWnd, &ps);
}

void CMainWindow::PostNcDestroy()
{
	delete this;
}

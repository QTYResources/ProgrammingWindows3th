/////////////////////////////////////////
// looker.cpp�ļ�

#include "looker.h"
#include "resource.h"
#include "MyButton.h"
#include <tlhelp32.h>

#define IDB_CLOSE 10
#define MAX_STRINGS 5

CMyApp theApp;		// Ӧ�ó���ʵ������

///////////////////////////////////////
// CMyApp��

BOOL CMyApp::InitInstance()
{
	m_pMainWnd = new CMainWindow;
	m_pMainWnd->ShowWindow(m_nCmdShow);
	m_pMainWnd->UpdateWindow();
	return TRUE;	// ��ʼ���ɹ���������Ϣѭ��
}

////////////////////////////////////////
// CWindowInfo��

CWindowInfo::CWindowInfo()
{
	m_hWnd = NULL;
}

void CWindowInfo::GetInfo(HWND hWnd)
{
	// ȡ�þ�������⡢����
	m_hWnd = hWnd;
	::GetWindowText(m_hWnd, m_szWindowCaption, BUFFER_SIZE);
	::GetClassName(m_hWnd, m_szWindowClass, BUFFER_SIZE);

	// ȡ�ô�����.exe�ļ�������
	m_szExeFile[0] = '\0';
	DWORD nPID;
	// ȡ�ð������ڵĽ��̵�ID��
	::GetWindowThreadProcessId(m_hWnd, &nPID);
	// ��ϵͳ�е����н�����һ�����գ�����ID��ΪnPID�Ľ��̵���Ϣ
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, nPID);
	if(hProcessSnap == INVALID_HANDLE_VALUE)
		return;
	// ��ʼ����
	BOOL bFind = FALSE;
	PROCESSENTRY32 pe32 = { sizeof(pe32) };
	if(::Process32First(hProcessSnap, &pe32))
	{
		do
		{
			if(pe32.th32ProcessID == nPID)
			{
				bFind = TRUE;
				break;
			}
		}while(::Process32Next(hProcessSnap, &pe32));
	}
	::CloseHandle(hProcessSnap);
	// ֻ�����ļ����ṹ���ļ������ƣ�������Ŀ¼��
	if(bFind)
	{
		const char* pszExeFile = strrchr(pe32.szExeFile, '\\');
		if(pszExeFile == NULL)
			pszExeFile = pe32.szExeFile;
		else
			pszExeFile++;
		strcpy(m_szExeFile, pszExeFile);
	}	
}

void CWindowInfo::DrawFrame()
{
	// Ŀ�괰�ڵ��豸�������
	HDC hdc = ::GetWindowDC(m_hWnd);
	// Ŀ�괰�����Ĵ�С
	RECT rcFrame;
	::GetWindowRect(m_hWnd, &rcFrame);
	int nWidth = rcFrame.right - rcFrame.left;
	int nHeight = rcFrame.bottom - rcFrame.top;

	// �ú�ɫ�ʻ���������ܻ���
	HPEN hPen = ::CreatePen(PS_SOLID, 3, RGB(255,0,0));
	HPEN hOldPen = (HPEN)::SelectObject(hdc, hPen);

	::MoveToEx(hdc, 0, 0, NULL);
	::LineTo(hdc, nWidth, 0);
	::LineTo(hdc, nWidth, nHeight);
	::LineTo(hdc, 0, nHeight);
	::LineTo(hdc, 0, 0);

	::SelectObject(hdc, hOldPen);
	::DeleteObject(hPen);
	::ReleaseDC(m_hWnd, hdc);
}

void CWindowInfo::EraseFrame()
{
	// �ػ������ڵķǿͻ������֣�RDW_FRAME��RDW_INVALIDATE��ǣ���
	// �������£�RDW_UPDATENOW��ǣ�
	::RedrawWindow(m_hWnd, NULL, NULL,
		RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW);

	HWND hWndParent = ::GetParent(m_hWnd);
	if(::IsWindow(hWndParent))
	{
		// �ػ������ڵ������ͻ�����RDW_ERASE��RDW_INVALIDATE��ǣ���
		// �������£�RDW_UPDATENOW��ǣ��� ���������Ӵ��ڣ�RDW_ALLCHILDREN��ǣ�
		::RedrawWindow(hWndParent, NULL, NULL, 
			RDW_ERASE | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
	} 
}

///////////////////////////////////////
// CMainWindow��

BEGIN_MESSAGE_MAP(CMainWindow, CWnd)
ON_WM_CREATE()
ON_WM_PAINT()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

CMainWindow::CMainWindow()
{
	m_bCatchMouseDown = FALSE;

	// �����������
	m_hCursorArrow = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	m_hCursorTarget = AfxGetApp()->LoadCursor(IDC_TARGET);

	// ע�ᴰ����
	LPCTSTR lpszClassName = AfxRegisterWndClass(0, NULL,
			(HBRUSH)(COLOR_3DFACE + 1), AfxGetApp()->LoadIcon(IDI_MAIN));

	// ��������
	CreateEx(0, lpszClassName, "���ڲ鿴��", 
		WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX, 
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL);

}

int CMainWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CClientDC dc(this);

	TEXTMETRIC tm;
	// GetTextMetrics����ȡ��ָ���豸�������ַ��Ĵ�С���� 
	::GetTextMetrics(dc, &tm);
	m_cxChar = tm.tmAveCharWidth;
	m_cyChar = tm.tmHeight;
	m_cyLine = tm.tmHeight + tm.tmExternalLeading;

	// ���ô������Ͻ������������λ������
	::SetRect(&m_rcMouseDown, 12, 12, 48, 48);

	// ���ñ������ʼ����
	m_ptHeaderOrigin.x = 48 + 6;
	m_ptHeaderOrigin.y = 12 + 4;

	// ������Ϣ���λ������
	m_rcMsgBoxBorder.left = m_ptHeaderOrigin.x + 8*m_cxChar;
	m_rcMsgBoxBorder.top = 12;
	m_rcMsgBoxBorder.right = m_rcMsgBoxBorder.left + m_cxChar*32 + 8;
	m_rcMsgBoxBorder.bottom = m_rcMsgBoxBorder.top + m_cyLine*MAX_STRINGS + 8;
	m_rcMsgBox = m_rcMsgBoxBorder;
	// inflate�����͵���˼��InflateRect����ʹ�����εĿ�͸��������Сһ��������
	::InflateRect(&m_rcMsgBox, -4, -4);
	
	// ������ť���ڶ���
	RECT rcButton = {12, m_rcMsgBoxBorder.bottom - 18, 64, m_rcMsgBoxBorder.bottom };
	new CMyButton("Close", rcButton, this, IDB_CLOSE);

	// ���ñ����ڵĴ�С
	RECT rect;
	::SetRect(&rect, 0, 0, m_rcMsgBoxBorder.right + 12, m_rcMsgBoxBorder.bottom + 12);
	// ����õ����Ǵ��ڿͻ����Ĵ�С��AdjustWindowRect���ͻ����Ĵ�Сת�������մ��ڵĴ�С
	::AdjustWindowRect(&rect, GetStyle(), FALSE);
	// �������ô��ڵĴ�С
	::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
		SWP_NOMOVE | SWP_NOREDRAW);
	
	// ���ù����״
	::SetCursor(m_hCursorArrow);
	return 0;
}


void CMainWindow::OnPaint()
{
	CPaintDC dc(this);

	// ���������Ͻǵ�������
	DrawMouseInput(&dc);
	// ������
	DrawMsgHeader(&dc);
	// ����Ϣ��DrawEdge��������ָ�����εı߿�
	::DrawEdge(dc, &m_rcMsgBoxBorder, EDGE_SUNKEN, BF_RECT);
	DrawMsg(&dc);
}

void CMainWindow::DrawMouseInput(CDC* pDC)
{
	HBRUSH hBrush = ::CreateSolidBrush(::GetSysColor(COLOR_3DFACE));
	HBRUSH hOldBrush = (HBRUSH)pDC->SelectObject(hBrush);
	// ������
	pDC->Rectangle(&m_rcMouseDown);
	pDC->SelectObject(hOldBrush);
	::DeleteObject(hBrush);
}

void CMainWindow::DrawMsgHeader(CDC* pDC)
{
	char* sz1 = "Caption:";
	char* sz2 = "Class:";
	char* sz3 = "Handle:";
	char* sz4 = "Name:";

	::SetBkColor(*pDC, ::GetSysColor (COLOR_3DFACE));
	
	pDC->TextOut(m_ptHeaderOrigin.x, m_ptHeaderOrigin.y, sz1, strlen(sz1));
	pDC->TextOut(m_ptHeaderOrigin.x, m_ptHeaderOrigin.y + m_cyLine*1, sz2, strlen(sz2));
	pDC->TextOut(m_ptHeaderOrigin.x, m_ptHeaderOrigin.y + m_cyLine*2, sz3, strlen(sz3));
	pDC->TextOut(m_ptHeaderOrigin.x, m_ptHeaderOrigin.y + m_cyLine*3, sz4, strlen(sz4));

}

void CMainWindow::DrawMsg(CDC* pDC)
{
	if(m_wndInfo.m_hWnd == NULL)
		return;	
	
	int xPos = m_rcMsgBox.left;
	int yPos = m_rcMsgBox.top;
	char sz[32];
	wsprintf(sz, "0X%0X", (int)m_wndInfo.m_hWnd);

	::SetBkColor(*pDC, ::GetSysColor(COLOR_3DFACE));

	pDC->TextOut(xPos, yPos, 
		m_wndInfo.m_szWindowCaption, strlen(m_wndInfo.m_szWindowCaption));
	pDC->TextOut(xPos, yPos + m_cyLine*1, 
		m_wndInfo.m_szWindowClass, strlen(m_wndInfo.m_szWindowClass));
	pDC->TextOut(xPos, yPos + m_cyLine*2, 
		sz, strlen(sz));
	pDC->TextOut(xPos, yPos + m_cyLine*3, 
		m_wndInfo.m_szExeFile, strlen(m_wndInfo.m_szExeFile));
}

void CMainWindow::OnLButtonDown(UINT nFlags, CPoint point)
{
	// PtInRect���������ж�point��λ���Ƿ���m_rcMouseDownָ���ľ���������
	if(!m_bCatchMouseDown && ::PtInRect(&m_rcMouseDown, point))
	{
		// �ڵĻ��͸��������״������������룬���ñ�־��Ϣ
		m_wndInfo.m_hWnd = NULL;
		::SetCursor(m_hCursorTarget);
		::SetCapture(m_hWnd);
		m_bCatchMouseDown = TRUE;
	}
}

void CMainWindow::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_bCatchMouseDown)
	{
		// �ָ����״̬���ͷŲ����������룬����Ŀ�괰�ڵľ��ο��
		::SetCursor(m_hCursorArrow);
		::ReleaseCapture();
		m_bCatchMouseDown = FALSE;
		if(m_wndInfo.m_hWnd != NULL)
			m_wndInfo.EraseFrame();
	}
}

void CMainWindow::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_bCatchMouseDown)
	{
		// ���ͻ�������ת��Ϊ��Ļ����
		::ClientToScreen(m_hWnd, &point);
		// ȡ��������ڴ��Ĵ��ڵľ��
		HWND hWnd = ::WindowFromPoint(point);
		if(hWnd == m_wndInfo.m_hWnd)
			return;

		// ����ǰһ�������ϵĺ�ɫ��ܣ�ȡ���µ�Ŀ�괰�ڵ���Ϣ�����ƿ��
		m_wndInfo.EraseFrame();
		m_wndInfo.GetInfo(hWnd);
		m_wndInfo.DrawFrame();

		// ͨ����Ч��ʾ����ʹ���ڿͻ����ػ�
		::InvalidateRect(m_hWnd, &m_rcMsgBox, TRUE);
	}
}

void CMainWindow::PostNcDestroy()
{
	delete this;
}

BOOL CMainWindow::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if(LOWORD(wParam) == IDB_CLOSE)
	{
		DestroyWindow();
		return TRUE;	// ����TRUE˵������Ϣ�Ѿ�������ֹCWnd���������
	}

	return FALSE;
}
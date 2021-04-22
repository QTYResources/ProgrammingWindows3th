/////////////////////////////////////////////
// MyButton.cpp文件

#include "MyButton.h"

BEGIN_MESSAGE_MAP(CMyButton, CWnd)
ON_WM_PAINT()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

CMyButton::CMyButton(LPCTSTR lpszText, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	m_bIsDown = FALSE;
	strncpy(m_szText, lpszText, 256);
	
	LPCTSTR pszClassName = AfxRegisterWndClass(0, 0, 
		(HBRUSH)(COLOR_BTNFACE + 1), AfxGetApp()->LoadStandardCursor(IDC_ARROW));

	Create(pszClassName, NULL, WS_CHILD|WS_VISIBLE, rect, pParentWnd, nID);

	// 相当于调用CreateEx
	// CreateEx(0, pszClassName, NULL, WS_CHILD|WS_VISIBLE, 
	//	rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 
	//	pParentWnd->GetSafeHwnd(), (HMENU)nID);
}

void CMyButton::OnPaint()
{
	CPaintDC dc(this);
	::SetBkMode(dc, TRANSPARENT);

	// 创建字体
	HFONT hFont = ::CreateFont(12, 0, 0, 0, FW_HEAVY, 0, 0, 0, ANSI_CHARSET, 
				OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
				VARIABLE_PITCH | FF_SWISS, "MS Sans Serif" );
	HFONT hOldFont = (HFONT)SelectObject(dc, hFont);
	// 创建画刷和画笔
	HBRUSH hBrush, hOldBrush;
	HPEN hPen, hOldPen;
	if(m_bIsDown)
	{
		hBrush = ::CreateSolidBrush(RGB(0xa0, 0xa0, 0xa0));
		hPen = ::CreatePen(PS_SOLID, 1, RGB(0x64, 0x64, 0x64));
		::SetTextColor(dc, RGB(0x32, 0x32, 0xfa));
	}
	else
	{	
		hBrush = ::CreateSolidBrush(RGB(0xf0, 0xf0, 0xf0));
		hPen = ::CreatePen(PS_SOLID, 1, RGB(0x78, 0x78, 0x78));
		::SetTextColor(dc, RGB(0x32, 0x32, 0x32));	
	}
	hOldBrush = (HBRUSH)::SelectObject(dc, hBrush);
	hOldPen = (HPEN)::SelectObject(dc, hPen);
	
	// 绘制外框和文本
	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);
	::RoundRect(dc, rcClient.left, rcClient.top, rcClient.right, rcClient.bottom, 2, 2);
	::DrawText(dc, m_szText, strlen(m_szText), &rcClient, DT_CENTER|DT_SINGLELINE|DT_VCENTER);

	// 清除资源
	::DeleteObject(::SelectObject(dc, hOldFont));
	::DeleteObject(::SelectObject(dc, hOldPen));
	::DeleteObject(::SelectObject(dc, hOldBrush));	
}


void CMyButton::OnLButtonDown(UINT nFlags, POINT point)
{
	m_bIsDown = TRUE;
	::InvalidateRect(m_hWnd, NULL, TRUE);
}

void CMyButton::OnLButtonUp(UINT nFlags, POINT point)
{
	if(m_bIsDown)
	{
		::InvalidateRect(m_hWnd, NULL, TRUE);
		::SendMessage(::GetParent(m_hWnd), WM_COMMAND, 
			MAKEWPARAM(::GetDlgCtrlID(m_hWnd), BN_CLICKED), (LPARAM)m_hWnd);
		m_bIsDown = FALSE;
	}
}

void CMyButton::OnMouseMove(UINT nFlags, POINT point)
{
	RECT rc;
	::GetClientRect(m_hWnd, &rc);
	if(::PtInRect(&rc, point))
	{
		::SetCapture(m_hWnd);
	}
	else
	{
		::InvalidateRect(m_hWnd, NULL, TRUE);
		::ReleaseCapture();
		m_bIsDown = FALSE;
	}		
}

void CMyButton::PostNcDestroy()
{
	delete this;
}

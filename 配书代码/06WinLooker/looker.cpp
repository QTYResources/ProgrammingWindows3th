/////////////////////////////////////////
// looker.cpp文件

#include "looker.h"
#include "resource.h"
#include "MyButton.h"
#include <tlhelp32.h>

#define IDB_CLOSE 10
#define MAX_STRINGS 5

CMyApp theApp;		// 应用程序实例对象

///////////////////////////////////////
// CMyApp类

BOOL CMyApp::InitInstance()
{
	m_pMainWnd = new CMainWindow;
	m_pMainWnd->ShowWindow(m_nCmdShow);
	m_pMainWnd->UpdateWindow();
	return TRUE;	// 初始化成功，进入消息循环
}

////////////////////////////////////////
// CWindowInfo类

CWindowInfo::CWindowInfo()
{
	m_hWnd = NULL;
}

void CWindowInfo::GetInfo(HWND hWnd)
{
	// 取得句柄、标题、类名
	m_hWnd = hWnd;
	::GetWindowText(m_hWnd, m_szWindowCaption, BUFFER_SIZE);
	::GetClassName(m_hWnd, m_szWindowClass, BUFFER_SIZE);

	// 取得磁盘上.exe文件的名称
	m_szExeFile[0] = '\0';
	DWORD nPID;
	// 取得包含窗口的进程的ID号
	::GetWindowThreadProcessId(m_hWnd, &nPID);
	// 给系统中的所有进程拍一个快照，查找ID号为nPID的进程的信息
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, nPID);
	if(hProcessSnap == INVALID_HANDLE_VALUE)
		return;
	// 开始查找
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
	// 只保存文件名结构中文件的名称（不包括目录）
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
	// 目标窗口的设备环境句柄
	HDC hdc = ::GetWindowDC(m_hWnd);
	// 目标窗口外框的大小
	RECT rcFrame;
	::GetWindowRect(m_hWnd, &rcFrame);
	int nWidth = rcFrame.right - rcFrame.left;
	int nHeight = rcFrame.bottom - rcFrame.top;

	// 用红色笔画沿外框四周画线
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
	// 重画本窗口的非客户区部分（RDW_FRAME、RDW_INVALIDATE标记），
	// 立即更新（RDW_UPDATENOW标记）
	::RedrawWindow(m_hWnd, NULL, NULL,
		RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW);

	HWND hWndParent = ::GetParent(m_hWnd);
	if(::IsWindow(hWndParent))
	{
		// 重画父窗口的整个客户区（RDW_ERASE、RDW_INVALIDATE标记），
		// 立即更新（RDW_UPDATENOW标记）， 包括所有子窗口（RDW_ALLCHILDREN标记）
		::RedrawWindow(hWndParent, NULL, NULL, 
			RDW_ERASE | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
	} 
}

///////////////////////////////////////
// CMainWindow类

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

	// 加载两个光标
	m_hCursorArrow = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	m_hCursorTarget = AfxGetApp()->LoadCursor(IDC_TARGET);

	// 注册窗口类
	LPCTSTR lpszClassName = AfxRegisterWndClass(0, NULL,
			(HBRUSH)(COLOR_3DFACE + 1), AfxGetApp()->LoadIcon(IDI_MAIN));

	// 创建窗口
	CreateEx(0, lpszClassName, "窗口查看器", 
		WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX, 
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL);

}

int CMainWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CClientDC dc(this);

	TEXTMETRIC tm;
	// GetTextMetrics函数取得指定设备环境中字符的大小属性 
	::GetTextMetrics(dc, &tm);
	m_cxChar = tm.tmAveCharWidth;
	m_cyChar = tm.tmHeight;
	m_cyLine = tm.tmHeight + tm.tmExternalLeading;

	// 设置窗口左上角正方形区域的位置坐标
	::SetRect(&m_rcMouseDown, 12, 12, 48, 48);

	// 设置标题的起始坐标
	m_ptHeaderOrigin.x = 48 + 6;
	m_ptHeaderOrigin.y = 12 + 4;

	// 设置消息框的位置坐标
	m_rcMsgBoxBorder.left = m_ptHeaderOrigin.x + 8*m_cxChar;
	m_rcMsgBoxBorder.top = 12;
	m_rcMsgBoxBorder.right = m_rcMsgBoxBorder.left + m_cxChar*32 + 8;
	m_rcMsgBoxBorder.bottom = m_rcMsgBoxBorder.top + m_cyLine*MAX_STRINGS + 8;
	m_rcMsgBox = m_rcMsgBoxBorder;
	// inflate是膨胀的意思，InflateRect函数使长方形的宽和高增大或缩小一定的数量
	::InflateRect(&m_rcMsgBox, -4, -4);
	
	// 创建按钮窗口对象
	RECT rcButton = {12, m_rcMsgBoxBorder.bottom - 18, 64, m_rcMsgBoxBorder.bottom };
	new CMyButton("Close", rcButton, this, IDB_CLOSE);

	// 设置本窗口的大小
	RECT rect;
	::SetRect(&rect, 0, 0, m_rcMsgBoxBorder.right + 12, m_rcMsgBoxBorder.bottom + 12);
	// 上面得到的是窗口客户区的大小，AdjustWindowRect将客户区的大小转化成最终窗口的大小
	::AdjustWindowRect(&rect, GetStyle(), FALSE);
	// 重新设置窗口的大小
	::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
		SWP_NOMOVE | SWP_NOREDRAW);
	
	// 设置光标形状
	::SetCursor(m_hCursorArrow);
	return 0;
}


void CMainWindow::OnPaint()
{
	CPaintDC dc(this);

	// 画窗口左上角的正方形
	DrawMouseInput(&dc);
	// 画标题
	DrawMsgHeader(&dc);
	// 画消息框。DrawEdge函数绘制指定矩形的边框
	::DrawEdge(dc, &m_rcMsgBoxBorder, EDGE_SUNKEN, BF_RECT);
	DrawMsg(&dc);
}

void CMainWindow::DrawMouseInput(CDC* pDC)
{
	HBRUSH hBrush = ::CreateSolidBrush(::GetSysColor(COLOR_3DFACE));
	HBRUSH hOldBrush = (HBRUSH)pDC->SelectObject(hBrush);
	// 画矩形
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
	// PtInRect函数用于判断point的位置是否在m_rcMouseDown指定的矩形区域中
	if(!m_bCatchMouseDown && ::PtInRect(&m_rcMouseDown, point))
	{
		// 在的话就更换光标形状，捕获鼠标输入，设置标志信息
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
		// 恢复光标状态，释放捕获的鼠标输入，擦除目标窗口的矩形框架
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
		// 将客户区坐标转换为屏幕坐标
		::ClientToScreen(m_hWnd, &point);
		// 取得鼠标所在处的窗口的句柄
		HWND hWnd = ::WindowFromPoint(point);
		if(hWnd == m_wndInfo.m_hWnd)
			return;

		// 擦除前一个窗口上的红色框架，取得新的目标窗口的信息，绘制框架
		m_wndInfo.EraseFrame();
		m_wndInfo.GetInfo(hWnd);
		m_wndInfo.DrawFrame();

		// 通过无效显示区域，使窗口客户区重画
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
		return TRUE;	// 返回TRUE说明此消息已经处理，阻止CWnd类继续处理
	}

	return FALSE;
}
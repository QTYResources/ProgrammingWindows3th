///////////////////////////////////////////////
// Example.cpp文件


#include "Example.h"
CMyApp theApp;

///////////////////////////////////////////////
// CMyApp成员函数的实现代码
BOOL CMyApp::InitInstance()
{
	m_pMainWnd = new CMyWnd;
	::ShowWindow(*m_pMainWnd, this->m_nCmdShow);
	::UpdateWindow(*m_pMainWnd);
	return TRUE; // 返回TRUE进入消息循环
}

///////////////////////////////////////////////
// CMyWnd成员函数的实现代码
CMyWnd::CMyWnd()
{
	LPCTSTR lpszClassName = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW, 
		::LoadCursor(NULL, IDC_ARROW), (HBRUSH)(COLOR_3DFACE+1));

	CreateEx(WS_EX_CLIENTEDGE, lpszClassName, 
		"框架程序创建的窗口", WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL);
}

LRESULT CMyWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_NCDESTROY)
	{
		::PostQuitMessage(0);
		delete this; 
		return 0; // CMyWnd对象已经不存在了，必须在这里返回，不能再访问任何非静态成员了
	}
	return Default();
}
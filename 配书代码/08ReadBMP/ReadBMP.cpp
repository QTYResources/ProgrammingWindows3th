///////////////////////////////////////
// ReadBMP.cpp

#include <afxdlgs.h>
#include "resource.h"
#include "ReadBMP.h"

CMyApp theApp;

BOOL CMyApp::InitInstance()
{
	m_pMainWnd = new CMainWindow;
	m_pMainWnd->ShowWindow(m_nCmdShow);
	return TRUE;
}

CMainWindow::CMainWindow()
{
	LPCTSTR lpszClassName = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW, 
		::LoadCursor(NULL, IDC_ARROW), (HBRUSH)(COLOR_WINDOW+1), theApp.LoadIcon(IDI_MAIN));

	CreateEx(NULL, lpszClassName, "BMP文件浏览器", 
		WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL);
}

BEGIN_MESSAGE_MAP(CMainWindow, CWnd)
ON_WM_CREATE()
ON_WM_PAINT()
ON_COMMAND(FILE_OPEN, OnFileOpen)	// 文件菜单项下的子项“打开”的ID号为FILE_OPEN
END_MESSAGE_MAP()

void CMainWindow::PostNcDestroy()
{
	delete this;
}

BOOL CMainWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	::SetMenu(m_hWnd, ::LoadMenu(theApp.m_hInstance, (LPCTSTR)IDR_MAIN));

	CClientDC dc(this);
	// 初始化内存DC
	m_hMemDC = ::CreateCompatibleDC(dc);

	m_nHeight = 0;
	m_nWidth = 0;
	return TRUE;
}

void CMainWindow::OnPaint()
{
	CPaintDC dc(this);
	::BitBlt(dc, 0, 0, 
		m_nWidth, m_nHeight, m_hMemDC, 0, 0, SRCCOPY); 


/*	CPaintDC dcClient(this);	// 客户区 DC	（目标 DC）
	CWindowDC dcWindow(this);	// 整个窗口 DC	（源 DC）	
	
	// 将窗口左上角30×30大小的图像拷贝到客户区
	::BitBlt(
		dcClient,	// hdcDst	目标 DC
		10,		// xDst		指定目标 DC 中接受图像的起始位置（xDst, yDst）
		10,		// yDst
		30,		// cx		欲传输图象的宽度（cx）和高度（cy）
		30,		// cy
		dcWindow,	// hdcSrc	源 DC
		0,		// xSrc		指定源 DC 中要拷贝的图像的起始坐标（xSrc,ySrc）
		0,		// ySrc		
		SRCCOPY);	// dwROP	传输过程要执行的光栅运算
		*/

}

void CMainWindow::OnDestroy()
{
	::DeleteDC(m_hMemDC);
}

void CMainWindow::OnFileOpen()		// 用户点击打开菜单命令时
{
	CFileDialog file(TRUE);
	if(!file.DoModal())
		return;
	
		// 下面是映射BMP文件到内存的过程
	// 打开要映射的文件
	HANDLE hFile = ::CreateFile(file.GetFileName(), GENERIC_READ, 
		FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if(hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox("读取文件出错！");
                return;
	}

	// 创建内存映射对象
	HANDLE hMap = ::CreateFileMapping(hFile, NULL, PAGE_READONLY, NULL, NULL, NULL);
	// 映射整个BMP文件到内存，返回这块内存的首地址
	LPVOID lpBase = ::MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);

		// 下面是获取BMP文件信息的过程
	BITMAPFILEHEADER *pFileHeader;		// bitmap file-header
	BITMAPINFO *pInfoHeader;		// bitmap info-header

	// 取得file-header指针，以获得位图象素
	pFileHeader = (BITMAPFILEHEADER*)lpBase;
	if(pFileHeader->bfType != MAKEWORD('B', 'M'))
	{
		MessageBox("本程序仅读取BMP文件！");
		::UnmapViewOfFile(lpBase);
		::CloseHandle(hMap);
		::CloseHandle(hFile);
		return;
	}
	BYTE *pBits = (BYTE*)lpBase + pFileHeader->bfOffBits;

	// 取得info-header指针，以获得文件的大小
	pInfoHeader = (BITMAPINFO*)((BYTE*)lpBase + sizeof(BITMAPFILEHEADER));
	m_nHeight = pInfoHeader->bmiHeader.biHeight;
	m_nWidth = pInfoHeader->bmiHeader.biWidth;

		// 下面是显示BMP文件到内存设备的过程
	CClientDC dc(this);
	// 创建一个与指定DC兼容的未初始化的位图，选入到内存兼容DC中
	HBITMAP hBitmap = ::CreateCompatibleBitmap(dc, m_nWidth, m_nHeight);
	::SelectObject(m_hMemDC, hBitmap);
	// 把象图像据放到建立的设备中
	int nRet = ::SetDIBitsToDevice(m_hMemDC,
		0,			// xDest
		0,			// yDest
		m_nWidth,
		m_nHeight,
		0,			// xSrc
		0,			// ySrc
		0,			// uStartScan  开始复制的扫描线和要复制的扫描线数
		m_nHeight,		// cScanLines
		pBits,			// lpvBits 指向DIB中的象素数据部分
		pInfoHeader,		// lpbmi 指向BITMAPINFO结构
		DIB_RGB_COLORS);	// fuColorUse 指定了DIB中数据的类型

	::InvalidateRect(m_hWnd, NULL, TRUE);

	::DeleteObject(hBitmap);
	::UnmapViewOfFile(lpBase);
	::CloseHandle(hMap);
	::CloseHandle(hFile);
}

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

	CreateEx(NULL, lpszClassName, "BMP�ļ������", 
		WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL);
}

BEGIN_MESSAGE_MAP(CMainWindow, CWnd)
ON_WM_CREATE()
ON_WM_PAINT()
ON_COMMAND(FILE_OPEN, OnFileOpen)	// �ļ��˵����µ�����򿪡���ID��ΪFILE_OPEN
END_MESSAGE_MAP()

void CMainWindow::PostNcDestroy()
{
	delete this;
}

BOOL CMainWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	::SetMenu(m_hWnd, ::LoadMenu(theApp.m_hInstance, (LPCTSTR)IDR_MAIN));

	CClientDC dc(this);
	// ��ʼ���ڴ�DC
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


/*	CPaintDC dcClient(this);	// �ͻ��� DC	��Ŀ�� DC��
	CWindowDC dcWindow(this);	// �������� DC	��Դ DC��	
	
	// ���������Ͻ�30��30��С��ͼ�񿽱����ͻ���
	::BitBlt(
		dcClient,	// hdcDst	Ŀ�� DC
		10,		// xDst		ָ��Ŀ�� DC �н���ͼ�����ʼλ�ã�xDst, yDst��
		10,		// yDst
		30,		// cx		������ͼ��Ŀ�ȣ�cx���͸߶ȣ�cy��
		30,		// cy
		dcWindow,	// hdcSrc	Դ DC
		0,		// xSrc		ָ��Դ DC ��Ҫ������ͼ�����ʼ���꣨xSrc,ySrc��
		0,		// ySrc		
		SRCCOPY);	// dwROP	�������Ҫִ�еĹ�դ����
		*/

}

void CMainWindow::OnDestroy()
{
	::DeleteDC(m_hMemDC);
}

void CMainWindow::OnFileOpen()		// �û�����򿪲˵�����ʱ
{
	CFileDialog file(TRUE);
	if(!file.DoModal())
		return;
	
		// ������ӳ��BMP�ļ����ڴ�Ĺ���
	// ��Ҫӳ����ļ�
	HANDLE hFile = ::CreateFile(file.GetFileName(), GENERIC_READ, 
		FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if(hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox("��ȡ�ļ�����");
                return;
	}

	// �����ڴ�ӳ�����
	HANDLE hMap = ::CreateFileMapping(hFile, NULL, PAGE_READONLY, NULL, NULL, NULL);
	// ӳ������BMP�ļ����ڴ棬��������ڴ���׵�ַ
	LPVOID lpBase = ::MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);

		// �����ǻ�ȡBMP�ļ���Ϣ�Ĺ���
	BITMAPFILEHEADER *pFileHeader;		// bitmap file-header
	BITMAPINFO *pInfoHeader;		// bitmap info-header

	// ȡ��file-headerָ�룬�Ի��λͼ����
	pFileHeader = (BITMAPFILEHEADER*)lpBase;
	if(pFileHeader->bfType != MAKEWORD('B', 'M'))
	{
		MessageBox("���������ȡBMP�ļ���");
		::UnmapViewOfFile(lpBase);
		::CloseHandle(hMap);
		::CloseHandle(hFile);
		return;
	}
	BYTE *pBits = (BYTE*)lpBase + pFileHeader->bfOffBits;

	// ȡ��info-headerָ�룬�Ի���ļ��Ĵ�С
	pInfoHeader = (BITMAPINFO*)((BYTE*)lpBase + sizeof(BITMAPFILEHEADER));
	m_nHeight = pInfoHeader->bmiHeader.biHeight;
	m_nWidth = pInfoHeader->bmiHeader.biWidth;

		// ��������ʾBMP�ļ����ڴ��豸�Ĺ���
	CClientDC dc(this);
	// ����һ����ָ��DC���ݵ�δ��ʼ����λͼ��ѡ�뵽�ڴ����DC��
	HBITMAP hBitmap = ::CreateCompatibleBitmap(dc, m_nWidth, m_nHeight);
	::SelectObject(m_hMemDC, hBitmap);
	// ����ͼ��ݷŵ��������豸��
	int nRet = ::SetDIBitsToDevice(m_hMemDC,
		0,			// xDest
		0,			// yDest
		m_nWidth,
		m_nHeight,
		0,			// xSrc
		0,			// ySrc
		0,			// uStartScan  ��ʼ���Ƶ�ɨ���ߺ�Ҫ���Ƶ�ɨ������
		m_nHeight,		// cScanLines
		pBits,			// lpvBits ָ��DIB�е��������ݲ���
		pInfoHeader,		// lpbmi ָ��BITMAPINFO�ṹ
		DIB_RGB_COLORS);	// fuColorUse ָ����DIB�����ݵ�����

	::InvalidateRect(m_hWnd, NULL, TRUE);

	::DeleteObject(hBitmap);
	::UnmapViewOfFile(lpBase);
	::CloseHandle(hMap);
	::CloseHandle(hFile);
}

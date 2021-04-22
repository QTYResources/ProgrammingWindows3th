/////////////////////////////////////////////////
// ReadBMP.h文件

#include <afxwin.h>

class CMyApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
};

class CMainWindow : public CWnd
{
public:
	CMainWindow();

protected:
	HDC m_hMemDC;	// 与客户区兼容的内存DC句柄
	int m_nWidth;	// BMP图像的宽度
	int m_nHeight;	// BMP图像的高度

protected:
	virtual void PostNcDestroy();
	afx_msg BOOL OnCreate(LPCREATESTRUCT);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnFileOpen();
	DECLARE_MESSAGE_MAP()
};
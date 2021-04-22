/////////////////////////////////////////////////
// ReadBMP.h�ļ�

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
	HDC m_hMemDC;	// ��ͻ������ݵ��ڴ�DC���
	int m_nWidth;	// BMPͼ��Ŀ��
	int m_nHeight;	// BMPͼ��ĸ߶�

protected:
	virtual void PostNcDestroy();
	afx_msg BOOL OnCreate(LPCREATESTRUCT);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnFileOpen();
	DECLARE_MESSAGE_MAP()
};
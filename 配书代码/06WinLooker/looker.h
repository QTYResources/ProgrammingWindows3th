////////////////////////////////////////////////////
// looker.h�ļ�

#include "afxwin.h"
#include "TLHELP32.H"

class CMyApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
};


#define BUFFER_SIZE 256

class CWindowInfo
{
public:
	CWindowInfo();
	// ��������
	void GetInfo(HWND hWnd);
	// ���ƾ������
	void DrawFrame();
	// �����������
	void EraseFrame();
	
	HWND m_hWnd;
	char m_szWindowCaption[BUFFER_SIZE];
	char m_szWindowClass[BUFFER_SIZE];
	char m_szExeFile[MAX_PATH];
};


class CMainWindow : public CWnd
{
protected:
	int m_cxChar;	// �ַ���ƽ�����
	int m_cyChar;	// �ַ��ĸ�
	int m_cyLine;	// һ���ַ�ռ�õĿռ�Ĵ�ֱ����
	
	HCURSOR m_hCursorArrow;		// ͨ��ģʽ��ʹ�õĹ��������ͷ��꣩
	HCURSOR m_hCursorTarget;	// �û�ѡ������ʱʹ�õĹ�������Զ����꣩

	RECT m_rcMouseDown;		// ��������°��ķ����λ������
	RECT m_rcMsgBoxBorder;		// ��Ϣ��߿��λ������
	RECT m_rcMsgBox;		// ��Ϣ���λ������
	CPoint m_ptHeaderOrigin;	// ���Ʊ������ʼλ��
	
	BOOL m_bCatchMouseDown;		// �Ƿ�׽������°��¼�
	CWindowInfo m_wndInfo;		// һ��Ŀ�괰�ڶ���
public:
	CMainWindow();

protected:
	void DrawMouseInput(CDC* pDC);
	void DrawMsg(CDC* pDC);
	void DrawMsgHeader(CDC* pDC);
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void PostNcDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};
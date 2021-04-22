///////////////////////////////////////////////
//Example.hÎÄ¼þ


#include "..\COMMON\_AFXWIN.H"

class CMyApp : public CWinApp  
{
public:
	virtual BOOL InitInstance();
};

class CMyWnd : public CWnd  
{ 
public:
	CMyWnd();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
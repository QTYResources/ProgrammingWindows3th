///////////////////////////////////////////////
// MyButton.h文件

#ifndef __MYBUTTON_H__
#define __MYBUTTON_H__

#include "afxwin.h"

class CMyButton : public CWnd  
{
public:
	CMyButton(LPCTSTR lpszText, const RECT& rect, CWnd* pParentWnd, UINT nID);
protected:
	char m_szText[256];	// 按钮显示的文本
	BOOL m_bIsDown;		// 指示用户是否按下鼠标左键

	virtual void PostNcDestroy();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, POINT point);
	afx_msg void OnLButtonUp(UINT nFlags, POINT point);
	afx_msg void OnMouseMove(UINT nFlags, POINT point);

	DECLARE_MESSAGE_MAP()
};


#endif // __MYBUTTON_H__
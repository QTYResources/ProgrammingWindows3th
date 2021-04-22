////////////////////////////////////////////
// _AFXIMPL.H文件

#ifndef __AFXIMPL_H__
#define __AFXIMPL_H__

#include "_afxwin.h"

// 窗口类的类型标志
#define AFX_WND_REG (0x0001)		// 使用第1位	
#define AFX_WNDFRAMEORVIEW_REG (0x0002) // 使用第2位

#define AFX_WND ("Wnd")
#define AFX_WNDFRAMEORVIEW ("FrameOrView")

// 框架程序注册窗口类时使用的类名，这些变量定义在WINCORE.CPP文件
extern const TCHAR _afxWnd[];
extern const TCHAR _afxWndFrameOrView[];


union MessageMapFunctions
{
	AFX_PMSG pfn;

	void (CWnd::*pfn_vv)(void);
	void (CWnd::*pfn_vw)(UINT);
	int (CWnd::*pfn_is)(LPTSTR);
};

#endif	// __AFXIMPL_H__
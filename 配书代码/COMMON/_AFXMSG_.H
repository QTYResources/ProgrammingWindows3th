/////////////////////////////////////////
// _AFXMSG_.H文件

#ifndef __AFXMSG_H__
#define __AFXMSG_H__

enum AfxSig // 函数签名标识
{
	AfxSig_end = 0,	// 结尾标识

	AfxSig_vv,	// void (void)，比如，void OnPaint()函数
	AfxSig_vw,	// void (UINT)，比如，void OnTimer(UINT nIDEvent)函数
	AfxSig_is,	// int	(LPTSTR)，比如，BOOL OnCreate(LPCREATESTRUCT)函数
};

// for Windows messages


#define ON_WM_CREATE() \
	{ WM_CREATE, 0, 0, 0, AfxSig_is, \
		(AFX_PMSG)(AFX_PMSGW)(int (CWnd::*)(LPCREATESTRUCT))&OnCreate },

#define ON_WM_PAINT() \
	{ WM_PAINT, 0, 0, 0, AfxSig_vv, \
		(AFX_PMSG)(AFX_PMSGW)(int (CWnd::*)(HDC))&OnPaint },

#define ON_WM_CLOSE() \
	{ WM_CLOSE, 0, 0, 0, AfxSig_vv, \
		(AFX_PMSG)(AFX_PMSGW)(int (CWnd::*)(void))&OnClose },

#define ON_WM_DESTROY() \
	{ WM_DESTROY, 0, 0, 0, AfxSig_vv, \
		(AFX_PMSG)(AFX_PMSGW)(int (CWnd::*)(void))&OnDestroy },

#define ON_WM_NCDESTROY() \
	{ WM_NCDESTROY, 0, 0, 0, AfxSig_vv, \
		(AFX_PMSG)(AFX_PMSGW)(int (CWnd::*)(void))&OnNcDestroy },

#define ON_WM_TIMER() \
	{ WM_TIMER, 0, 0, 0, AfxSig_vw, \
		(AFX_PMSG)(AFX_PMSGW)(void (CWnd::*)(UINT))&OnTimer },








///////////////////////////////////////////////////////////////////////

/*
// User extensions for message map entries

#define ON_MESSAGE(message, memberFxn) \
	{ message, 0, 0, 0, AfxSig_lwl, \
		(AFX_PMSG)(AFX_PMSGW)(LRESULT (CWnd::*)(WPARAM, LPARAM))&memberFxn },








// 通知消息

#define ON_COMMAND(id, memberFxn) \
	{ WM_COMMAND, 0, (WORD)id, (WORD)id, AfxSig_vv, (AFX_PMSG)&memberFxn },


#define ON_NOTIFY(wNotifyCode, id, memberFxn) \
	{ WM_NOTIFY, (WORD)(int)wNotifyCode, (WORD)id, (WORD)id, AfxSig_vNMHDRpl, \
		(AFX_PMSG)(void (CCmdTarget::*)(NMHDR*, LRESULT*))&memberFxn },

// for general controls
#define ON_CONTROL(wNotifyCode, id, memberFxn) \
	{ WM_COMMAND, (WORD)wNotifyCode, (WORD)id, (WORD)id, AfxSig_vv, \
		(AFX_PMSG)&memberFxn },


/////////////////////////////////////////////////////////////////////////////
// Message map tables for Control Notification messages

// User Button Notification Codes
#define ON_BN_CLICKED(id, memberFxn) \
	ON_CONTROL(BN_CLICKED, id, memberFxn)
#define ON_BN_DOUBLECLICKED(id, memberFxn) \
	ON_CONTROL(BN_DOUBLECLICKED, id, memberFxn)
#define ON_BN_SETFOCUS(id, memberFxn) \
	ON_CONTROL(BN_SETFOCUS, id, memberFxn)
#define ON_BN_KILLFOCUS(id, memberFxn) \
	ON_CONTROL(BN_KILLFOCUS, id, memberFxn)


/////////////////////////////////////////////////////////////////////////////
*/

#endif // __AFXMSG_H__

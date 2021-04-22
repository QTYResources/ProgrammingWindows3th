// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__9CF87A30_AD69_4B76_89A4_D172C49AC031__INCLUDED_)
#define AFX_STDAFX_H__9CF87A30_AD69_4B76_89A4_D172C49AC031__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls

#include "winioctl.h"		// 为了使用IOCTL设备控制代码
#include "DrvFltIp.h"
#include <winsock2.h>		// 为了使用Winsock API
#pragma comment(lib, "WS2_32.lib")

#define MAX_RULES 56		// 定义运行安装的最大过滤条件数量


#endif // _AFX_NO_AFXCMN_SUPPORT


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__9CF87A30_AD69_4B76_89A4_D172C49AC031__INCLUDED_)

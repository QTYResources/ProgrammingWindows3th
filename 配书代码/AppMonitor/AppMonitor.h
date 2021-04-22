// AppMonitor.h : main header file for the APPMONITOR application
//

#if !defined(AFX_APPMONITOR_H__21D409E0_33E5_4924_90F2_15EB105D961C__INCLUDED_)
#define AFX_APPMONITOR_H__21D409E0_33E5_4924_90F2_15EB105D961C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAppMonitorApp:
// See AppMonitor.cpp for the implementation of this class
//

class CAppMonitorApp : public CWinApp
{
public:
	CAppMonitorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAppMonitorApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAppMonitorApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APPMONITOR_H__21D409E0_33E5_4924_90F2_15EB105D961C__INCLUDED_)

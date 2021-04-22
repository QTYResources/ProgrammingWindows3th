// Firewall.h : main header file for the FIREWALL application
//

#if !defined(AFX_FIREWALL_H__C68E9159_F54F_424E_9A1C_D3FDFB073E7C__INCLUDED_)
#define AFX_FIREWALL_H__C68E9159_F54F_424E_9A1C_D3FDFB073E7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CFirewallApp:
// See Firewall.cpp for the implementation of this class
//

class CFirewallApp : public CWinApp
{
public:
	CFirewallApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFirewallApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CFirewallApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIREWALL_H__C68E9159_F54F_424E_9A1C_D3FDFB073E7C__INCLUDED_)

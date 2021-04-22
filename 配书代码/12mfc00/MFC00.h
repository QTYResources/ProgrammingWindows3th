// MFC00.h : main header file for the MFC00 application
//

#if !defined(AFX_MFC00_H__04523366_7560_41F6_8BFA_F2212ED51EF5__INCLUDED_)
#define AFX_MFC00_H__04523366_7560_41F6_8BFA_F2212ED51EF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMFC00App:
// See MFC00.cpp for the implementation of this class
//

class CMFC00App : public CWinApp
{
public:
	CMFC00App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMFC00App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMFC00App)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFC00_H__04523366_7560_41F6_8BFA_F2212ED51EF5__INCLUDED_)

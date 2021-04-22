///////////////////////////////////////////
// 07UseWizard.cppÎÄ¼þ



#include "stdafx.h"
#include "07UseWizard.h"
#include "07UseWizardDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMy07UseWizardApp

BEGIN_MESSAGE_MAP(CMy07UseWizardApp, CWinApp)
	//{{AFX_MSG_MAP(CMy07UseWizardApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMy07UseWizardApp construction

CMy07UseWizardApp::CMy07UseWizardApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMy07UseWizardApp object

CMy07UseWizardApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMy07UseWizardApp initialization

BOOL CMy07UseWizardApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CMy07UseWizardDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

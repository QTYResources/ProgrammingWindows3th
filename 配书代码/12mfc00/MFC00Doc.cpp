// MFC00Doc.cpp : implementation of the CMFC00Doc class
//

#include "stdafx.h"
#include "MFC00.h"

#include "MFC00Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMFC00Doc

IMPLEMENT_DYNCREATE(CMFC00Doc, CDocument)

BEGIN_MESSAGE_MAP(CMFC00Doc, CDocument)
	//{{AFX_MSG_MAP(CMFC00Doc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMFC00Doc construction/destruction

CMFC00Doc::CMFC00Doc()
{
	// TODO: add one-time construction code here

}

CMFC00Doc::~CMFC00Doc()
{
}

BOOL CMFC00Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMFC00Doc serialization

void CMFC00Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMFC00Doc diagnostics

#ifdef _DEBUG
void CMFC00Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMFC00Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMFC00Doc commands

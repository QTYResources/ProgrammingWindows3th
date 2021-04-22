// ImageProcessDoc.cpp : implementation of the CImageProcessDoc class
//

#include "stdafx.h"
#include "ImageProcess.h"

#include "ImageProcessDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageProcessDoc

IMPLEMENT_DYNCREATE(CImageProcessDoc, CDocument)

BEGIN_MESSAGE_MAP(CImageProcessDoc, CDocument)
	//{{AFX_MSG_MAP(CImageProcessDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageProcessDoc construction/destruction

CImageProcessDoc::CImageProcessDoc()
{
	// TODO: add one-time construction code here

}

CImageProcessDoc::~CImageProcessDoc()
{
}

BOOL CImageProcessDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CImageProcessDoc serialization

void CImageProcessDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		if(!m_OutImage.SaveToFile(*ar.GetFile()))
		{
			AfxMessageBox(" ±£¥Ê ß∞‹£°");
			return;
		}
	}
	else
	{
		if(!m_Image.AttachFromFile(*ar.GetFile()))
		{
			AfxMessageBox(" º”‘ÿ ß∞‹£°");
			return;
		}
		m_OutImage = m_Image;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CImageProcessDoc diagnostics

#ifdef _DEBUG
void CImageProcessDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImageProcessDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageProcessDoc commands

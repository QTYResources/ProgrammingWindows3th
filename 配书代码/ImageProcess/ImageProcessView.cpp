// ImageProcessView.cpp : implementation of the CImageProcessView class
//

#include "stdafx.h"
#include "ImageProcess.h"

#include "ImageProcessDoc.h"
#include "ImageProcessView.h"

#include "IntensityDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageProcessView

IMPLEMENT_DYNCREATE(CImageProcessView, CScrollView)

BEGIN_MESSAGE_MAP(CImageProcessView, CScrollView)
	//{{AFX_MSG_MAP(CImageProcessView)
	ON_COMMAND(ID_VIEW_INTENSITY, OnViewIntensity)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageProcessView construction/destruction

CImageProcessView::CImageProcessView()
{
	// TODO: add construction code here

	m_bOut = TRUE;

}

CImageProcessView::~CImageProcessView()
{
}

BOOL CImageProcessView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CImageProcessView drawing

void CImageProcessView::OnDraw(CDC* pDC)
{
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(!pDoc->m_Image.IsValidate())
		return;


	CSize sizeTotal;
	sizeTotal.cx = sizeTotal.cy = 100;
	sizeTotal.cx = pDoc->m_Image.GetWidthPixel();
	sizeTotal.cy = pDoc->m_Image.GetHeight();
	SetScrollSizes(MM_TEXT, sizeTotal);

	if(m_bOut)
	{
		pDoc->m_OutImage.Draw(pDC);
	}
	else
	{
		pDoc->m_Image.Draw(pDC);
	}
}

void CImageProcessView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

/////////////////////////////////////////////////////////////////////////////
// CImageProcessView printing

BOOL CImageProcessView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CImageProcessView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CImageProcessView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CImageProcessView diagnostics

#ifdef _DEBUG
void CImageProcessView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImageProcessView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImageProcessDoc* CImageProcessView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageProcessDoc)));
	return (CImageProcessDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageProcessView message handlers

void CImageProcessView::OnViewIntensity() 
{	
	CImageProcessDoc* pDoc = GetDocument();
	if(!pDoc->m_Image.IsValidate())
		return;	


	CIntensityDlg dlg(this);

	memset(dlg.m_btCount, 0, 256*sizeof(int));

	for(int j=0; j<pDoc->m_OutImage.GetHeight(); j++)
	{
		for(int i=0; i<pDoc->m_OutImage.GetWidthPixel(); i++)
		{
			BYTE bt = pDoc->m_OutImage.GetGray(i, j);
			dlg.m_btCount[bt] ++;
		}
	}
	
	dlg.m_nLowGray = 0;
	dlg.m_nHighGray = 255;




	dlg.DoModal();
}

// ImageProcessView.h : interface of the CImageProcessView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEPROCESSVIEW_H__0FE0FA6D_962D_4809_86C1_01EC67E62D7D__INCLUDED_)
#define AFX_IMAGEPROCESSVIEW_H__0FE0FA6D_962D_4809_86C1_01EC67E62D7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CImageProcessView : public CScrollView
{
protected: // create from serialization only
	CImageProcessView();
	DECLARE_DYNCREATE(CImageProcessView)

// Attributes
public:
	CImageProcessDoc* GetDocument();


	BOOL m_bOut;




// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageProcessView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CImageProcessView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CImageProcessView)
	afx_msg void OnViewIntensity();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ImageProcessView.cpp
inline CImageProcessDoc* CImageProcessView::GetDocument()
   { return (CImageProcessDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEPROCESSVIEW_H__0FE0FA6D_962D_4809_86C1_01EC67E62D7D__INCLUDED_)

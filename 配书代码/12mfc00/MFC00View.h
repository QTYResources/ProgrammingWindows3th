// MFC00View.h : interface of the CMFC00View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFC00VIEW_H__F87DC3A5_E2FC_404A_8EE8_7BE1E807A22F__INCLUDED_)
#define AFX_MFC00VIEW_H__F87DC3A5_E2FC_404A_8EE8_7BE1E807A22F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMFC00View : public CView
{
protected: // create from serialization only
	CMFC00View();
	DECLARE_DYNCREATE(CMFC00View)

// Attributes
public:
	CMFC00Doc* GetDocument();

// Operations
public:


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMFC00View)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMFC00View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	
private:
	HGLRC m_hRC;
	CClientDC* m_pDC; 
	bool bSetDCPixelFormat();	
	bool initOpenGL();
	void myDraw();
	GLUquadricObj *gluObj;

// Generated message map functions
protected:
	//{{AFX_MSG(CMFC00View)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


};


#ifndef _DEBUG  // debug version in MFC00View.cpp
inline CMFC00Doc* CMFC00View::GetDocument()
   { return (CMFC00Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFC00VIEW_H__F87DC3A5_E2FC_404A_8EE8_7BE1E807A22F__INCLUDED_)

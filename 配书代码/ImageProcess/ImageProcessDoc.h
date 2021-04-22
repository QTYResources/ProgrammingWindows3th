// ImageProcessDoc.h : interface of the CImageProcessDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEPROCESSDOC_H__69675B55_7A89_4948_BF22_BC76DF6BD412__INCLUDED_)
#define AFX_IMAGEPROCESSDOC_H__69675B55_7A89_4948_BF22_BC76DF6BD412__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CImageProcessDoc : public CDocument
{
protected: // create from serialization only
	CImageProcessDoc();
	DECLARE_DYNCREATE(CImageProcessDoc)

// Attributes
public:

	CGray m_Image;
	CGray m_OutImage;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageProcessDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CImageProcessDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CImageProcessDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEPROCESSDOC_H__69675B55_7A89_4948_BF22_BC76DF6BD412__INCLUDED_)

// MFC00Doc.h : interface of the CMFC00Doc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFC00DOC_H__6FE10336_A05D_4C5F_A78E_B39D39240F43__INCLUDED_)
#define AFX_MFC00DOC_H__6FE10336_A05D_4C5F_A78E_B39D39240F43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMFC00Doc : public CDocument
{
protected: // create from serialization only
	CMFC00Doc();
	DECLARE_DYNCREATE(CMFC00Doc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMFC00Doc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMFC00Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMFC00Doc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFC00DOC_H__6FE10336_A05D_4C5F_A78E_B39D39240F43__INCLUDED_)

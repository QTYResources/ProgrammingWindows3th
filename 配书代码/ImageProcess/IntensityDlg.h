#if !defined(AFX_INTENSITYDLG_H__CE3ED11B_15CD_4A74_8BF3_2B7629320CFB__INCLUDED_)
#define AFX_INTENSITYDLG_H__CE3ED11B_15CD_4A74_8BF3_2B7629320CFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IntensityDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIntensityDlg dialog

class CIntensityDlg : public CDialog
{
// Construction
public:
	CIntensityDlg(CWnd* pParent = NULL);   // standard constructor

	CGray *m_pImage;

	void DrawIntersity(CDC *pDC);

	int m_btCount[256];

	int m_nLowGray;
	int m_nHighGray;

// Dialog Data
	//{{AFX_DATA(CIntensityDlg)
	enum { IDD = IDD_INTENSITY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIntensityDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIntensityDlg)
	afx_msg void OnOk();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INTENSITYDLG_H__CE3ED11B_15CD_4A74_8BF3_2B7629320CFB__INCLUDED_)

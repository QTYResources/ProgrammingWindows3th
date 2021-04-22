#if !defined(AFX_RULEDLG_H__53FA152B_4385_4AE7_AA9F_6D53FAC8FC2F__INCLUDED_)
#define AFX_RULEDLG_H__53FA152B_4385_4AE7_AA9F_6D53FAC8FC2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RuleDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRuleDlg dialog

class CRuleDlg : public CDialog
{
public:
	UINT m_srcIP;
	UINT m_srcMask;
	USHORT m_srcPort;

	UINT m_destIP;
	UINT m_destMask;
	USHORT m_destPort;

	UINT m_nProtocol;
	BOOL m_bDrop;

// Construction
public:
	CRuleDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRuleDlg)
	enum { IDD = IDD_ADDRULEDLG };
	CComboBox	m_cmbProtocol;
	CComboBox	m_cmbAction;
	CString	m_strSourceIP;
	CString	m_strDestIP;
	CString	m_strDestMask;
	CString	m_strSourceMask;
	UINT	m_nDestPort;
	UINT	m_nSourcePort;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRuleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRuleDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RULEDLG_H__53FA152B_4385_4AE7_AA9F_6D53FAC8FC2F__INCLUDED_)

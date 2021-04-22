// 07UseWizardDlg.h : header file
//

#if !defined(AFX_07USEWIZARDDLG_H__02F5BB31_FA7A_43B4_876C_E35B0A1CF853__INCLUDED_)
#define AFX_07USEWIZARDDLG_H__02F5BB31_FA7A_43B4_876C_E35B0A1CF853__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMy07UseWizardDlg dialog

class CMy07UseWizardDlg : public CDialog
{
// Construction
public:
	CMy07UseWizardDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMy07UseWizardDlg)
	enum { IDD = IDD_MY07USEWIZARD_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMy07UseWizardDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMy07UseWizardDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_07USEWIZARDDLG_H__02F5BB31_FA7A_43B4_876C_E35B0A1CF853__INCLUDED_)

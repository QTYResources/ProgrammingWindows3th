// AppMonitorDlg.h : header file
//

#if !defined(AFX_APPMONITORDLG_H__F991CC38_A9BA_4A32_B551_0BF66E8ED810__INCLUDED_)
#define AFX_APPMONITORDLG_H__F991CC38_A9BA_4A32_B551_0BF66E8ED810__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAppMonitorDlg dialog

class CAppMonitorDlg : public CDialog
{
// Construction
public:
	CAppMonitorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CAppMonitorDlg)
	enum { IDD = IDD_APPMONITOR_DIALOG };
	CButton	m_Browse;
	CButton	m_Start;
	CEdit	m_Name;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAppMonitorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	BOOL IsStarted();

	BOOL m_bStart;

	int Proc();

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAppMonitorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnbrowse();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APPMONITORDLG_H__F991CC38_A9BA_4A32_B551_0BF66E8ED810__INCLUDED_)

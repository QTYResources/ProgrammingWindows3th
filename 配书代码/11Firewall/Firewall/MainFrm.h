// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////



#if !defined(AFX_MAINFRM_H__9253B462_11DE_4E99_ADD5_780FD9756444__INCLUDED_)
#define AFX_MAINFRM_H__9253B462_11DE_4E99_ADD5_780FD9756444__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDriver;

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CDriver* m_pIPFltDrv;	// IP过滤驱动指针
	CDriver* m_pFilterDrv;	// IP过滤钩子驱动指针
	BOOL m_bStarted;	// 指示是否启动

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRulesAdd();
	afx_msg void OnRulesDelete();
	afx_msg void OnFileConserve();
	afx_msg void OnFileLoad();
	afx_msg void OnFileStart();
	afx_msg void OnUpdateFileStart(CCmdUI* pCmdUI);
	afx_msg void OnFileStop();
	afx_msg void OnUpdateFileStop(CCmdUI* pCmdUI);
	afx_msg void OnRulesInstall();
	afx_msg void OnRulesUninstall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__9253B462_11DE_4E99_ADD5_780FD9756444__INCLUDED_)

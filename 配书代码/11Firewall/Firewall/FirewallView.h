// FirewallView.h : interface of the CFirewallView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIREWALLVIEW_H__DFB049E2_A9ED_4090_9223_A0F0732DE7B1__INCLUDED_)
#define AFX_FIREWALLVIEW_H__DFB049E2_A9ED_4090_9223_A0F0732DE7B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CFirewallView : public CFormView
{
protected: // create from serialization only
	CFirewallView();
	DECLARE_DYNCREATE(CFirewallView)

public:
	//{{AFX_DATA(CFirewallView)
	enum { IDD = IDD_FIREWALL_FORM };
	CListCtrl	m_rules;
	//}}AFX_DATA

// Attributes
public:
	CFirewallDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFirewallView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFirewallView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	// 更新用户显示
	void CFirewallView::UpdateList();
	// 添加一个过滤条件
	void AddRuleToList(UINT srcIp, UINT srcMask, USHORT srcPort,
			UINT dstIp, UINT dstMask, USHORT dstPort, UINT protocol, BOOL bDrop);


// Generated message map functions
protected:
	//{{AFX_MSG(CFirewallView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in FirewallView.cpp
inline CFirewallDoc* CFirewallView::GetDocument()
   { return (CFirewallDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIREWALLVIEW_H__DFB049E2_A9ED_4090_9223_A0F0732DE7B1__INCLUDED_)

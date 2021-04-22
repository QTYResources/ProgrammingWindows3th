// FirewallDoc.h : interface of the CFirewallDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIREWALLDOC_H__59801A6E_C04D_437A_A80E_3B3D32F66741__INCLUDED_)
#define AFX_FIREWALLDOC_H__59801A6E_C04D_437A_A80E_3B3D32F66741__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CFirewallDoc : public CDocument
{
protected: // create from serialization only
	CFirewallDoc();
	DECLARE_DYNCREATE(CFirewallDoc)

// Attributes
public:
	CIPFilter m_rules[MAX_RULES];	// 过滤条件数组
	int m_nRules;			// 数组大小

	// 添加一个过滤条件
	BOOL AddRule(UINT srcIp, UINT srcMask, USHORT srcPort,
			UINT dstIp, UINT dstMask, USHORT dstPort, UINT protocol, BOOL bDrop);
	// 删除指定过滤条件
	void DeleteRules(int nPos);
	// 清除过滤条件
	void ClearRules();


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFirewallDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFirewallDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFirewallDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIREWALLDOC_H__59801A6E_C04D_437A_A80E_3B3D32F66741__INCLUDED_)

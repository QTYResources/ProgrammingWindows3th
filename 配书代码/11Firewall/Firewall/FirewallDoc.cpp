// FirewallDoc.cpp : implementation of the CFirewallDoc class
//

#include "stdafx.h"
#include "Firewall.h"

#include "FirewallDoc.h"
#include "FirewallView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFirewallDoc

IMPLEMENT_DYNCREATE(CFirewallDoc, CDocument)

BEGIN_MESSAGE_MAP(CFirewallDoc, CDocument)
	//{{AFX_MSG_MAP(CFirewallDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFirewallDoc construction/destruction

CFirewallDoc::CFirewallDoc()
{
	// TODO: add one-time construction code here
	m_nRules = 0;

}

CFirewallDoc::~CFirewallDoc()
{
}

BOOL CFirewallDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CFirewallDoc serialization

void CFirewallDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

/////////////////////////////////////////////////////////////////////////////
// CFirewallDoc diagnostics

#ifdef _DEBUG
void CFirewallDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFirewallDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFirewallDoc commands


BOOL CFirewallDoc::AddRule(UINT srcIp, UINT srcMask, USHORT srcPort,
		UINT dstIp, UINT dstMask, USHORT dstPort, UINT protocol, BOOL bDrop)
{
	if(m_nRules >= MAX_RULES)
	{
		return FALSE;
	}
	else
	{
		// 添加到m_rules数组
		m_rules[m_nRules].sourceIP	  = srcIp;
		m_rules[m_nRules].sourceMask	  = srcMask;
		m_rules[m_nRules].sourcePort	  = srcPort;
		m_rules[m_nRules].destinationIP   = dstIp;
		m_rules[m_nRules].destinationMask = dstMask;
		m_rules[m_nRules].destinationPort = dstPort;
		m_rules[m_nRules].protocol	  = protocol;
		m_rules[m_nRules].bDrop 	  = bDrop;
		
		m_nRules++;
	}
	
	return TRUE;
}

void CFirewallDoc::DeleteRules(int nPos)
{
	if(nPos >= m_nRules)
		return;
	
	if(nPos != m_nRules - 1)
	{
		for(int i = nPos + 1; i<m_nRules; i++)
		{
			m_rules[i - 1].sourceIP		 = m_rules[i].sourceIP;
			m_rules[i - 1].sourceMask	 = m_rules[i].sourceMask;
			m_rules[i - 1].sourcePort	 = m_rules[i].sourcePort;
			m_rules[i - 1].destinationIP     = m_rules[i].destinationIP;
			m_rules[i - 1].destinationMask   = m_rules[i].destinationMask;
			m_rules[i - 1].destinationPort   = m_rules[i].destinationPort;
			m_rules[i - 1].protocol	         = m_rules[i].protocol;
			m_rules[i - 1].bDrop 		 = m_rules[i].bDrop;
		}
	}
	m_nRules--;
	
}

void CFirewallDoc::ClearRules()
{
	m_nRules = 0;
}


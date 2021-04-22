// FirewallView.cpp : implementation of the CFirewallView class
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
// CFirewallView

IMPLEMENT_DYNCREATE(CFirewallView, CFormView)

BEGIN_MESSAGE_MAP(CFirewallView, CFormView)
	//{{AFX_MSG_MAP(CFirewallView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFirewallView construction/destruction

CFirewallView::CFirewallView()
	: CFormView(CFirewallView::IDD)
{
	//{{AFX_DATA_INIT(CFirewallView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// TODO: add construction code here

}

CFirewallView::~CFirewallView()
{
}

void CFirewallView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFirewallView)
	DDX_Control(pDX, IDC_LISTRULES, m_rules);
	//}}AFX_DATA_MAP
}

BOOL CFirewallView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CFirewallView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	RECT rc;
	m_rules.GetClientRect(&rc);

	int nWidth = rc.right - rc.left - 110;
	int nLength = nWidth/6;

	m_rules.InsertColumn(0, "源IP", LVCFMT_LEFT, nLength, 0);
	m_rules.InsertColumn(1, "源屏蔽位", LVCFMT_LEFT, nLength, 1);
	m_rules.InsertColumn(2, "源端口号", LVCFMT_LEFT, nLength, 2);
	m_rules.InsertColumn(3, "目的IP", LVCFMT_LEFT, nLength, 3);
	m_rules.InsertColumn(4, "目的屏蔽位", LVCFMT_LEFT, nLength, 4);
	m_rules.InsertColumn(5, "目的端口号", LVCFMT_LEFT, nLength, 5);
	m_rules.InsertColumn(6, "协议", LVCFMT_LEFT, 60, 6);
	m_rules.InsertColumn(7, "动作", LVCFMT_LEFT, 50, 7);

	m_rules.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

}

/////////////////////////////////////////////////////////////////////////////
// CFirewallView diagnostics

#ifdef _DEBUG
void CFirewallView::AssertValid() const
{
	CFormView::AssertValid();
}

void CFirewallView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CFirewallDoc* CFirewallView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFirewallDoc)));
	return (CFirewallDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFirewallView message handlers


void CFirewallView::AddRuleToList(UINT srcIP, UINT srcMask, USHORT srcPort,
			UINT dstIP, UINT dstMask, USHORT dstPort, UINT protocol, BOOL bDrop)
{
	char szTemp[6];
	int nCurrent = m_rules.GetItemCount();

	// 源IP地址
	CString s = (srcIP == 0) ? "所有" : ::inet_ntoa(*((in_addr*)&srcIP));
	m_rules.InsertItem(nCurrent, s, 0);
	// 源IP位掩码
	s = ::inet_ntoa(*((in_addr*)&srcMask));
	m_rules.SetItemText(nCurrent, 1, s);
	// 源端口号
	s = (srcPort == 0) ? "所有" : ::itoa(srcPort, szTemp, 10);
	m_rules.SetItemText(nCurrent, 2, s);
	// 目的IP地址
	s = (dstIP == 0) ? "所有" : ::inet_ntoa(*((in_addr*)&dstIP));
	m_rules.SetItemText(nCurrent, 3, s);
	// 目的IP位掩码
	s = ::inet_ntoa(*((in_addr*)&dstMask));
	m_rules.SetItemText(nCurrent, 4, s);
	// 目的端口号
	s = (dstPort == 0) ? "所有" : ::itoa(dstPort, szTemp, 10);
	m_rules.SetItemText(nCurrent, 5, s);
	// 协议
	if(protocol == 1)
		m_rules.SetItemText(nCurrent, 6, "ICMP");
	else if(protocol == 6)
		m_rules.SetItemText(nCurrent, 6, "TCP");
	else if(protocol == 17)
		m_rules.SetItemText(nCurrent, 6, "UDP");
	else
		m_rules.SetItemText(nCurrent, 6, "所有");
	// 动作
	s = bDrop ? "丢弃" : "通过";
	m_rules.SetItemText(nCurrent, 7, s);
}

void CFirewallView::UpdateList()
{
	// 取得文档对象指针
	CFirewallDoc* pDoc = GetDocument();
	// 更新显示
	m_rules.DeleteAllItems();
	for(int i=0; i<pDoc->m_nRules; i++)
	{
		AddRuleToList(pDoc->m_rules[i].sourceIP,
					  pDoc->m_rules[i].sourceMask,
					  pDoc->m_rules[i].sourcePort,
					  pDoc->m_rules[i].destinationIP,
					  pDoc->m_rules[i].destinationMask,
					  pDoc->m_rules[i].destinationPort,
					  pDoc->m_rules[i].protocol,
					  pDoc->m_rules[i].bDrop);
	}
}



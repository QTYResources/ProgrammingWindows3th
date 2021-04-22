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

	m_rules.InsertColumn(0, "ԴIP", LVCFMT_LEFT, nLength, 0);
	m_rules.InsertColumn(1, "Դ����λ", LVCFMT_LEFT, nLength, 1);
	m_rules.InsertColumn(2, "Դ�˿ں�", LVCFMT_LEFT, nLength, 2);
	m_rules.InsertColumn(3, "Ŀ��IP", LVCFMT_LEFT, nLength, 3);
	m_rules.InsertColumn(4, "Ŀ������λ", LVCFMT_LEFT, nLength, 4);
	m_rules.InsertColumn(5, "Ŀ�Ķ˿ں�", LVCFMT_LEFT, nLength, 5);
	m_rules.InsertColumn(6, "Э��", LVCFMT_LEFT, 60, 6);
	m_rules.InsertColumn(7, "����", LVCFMT_LEFT, 50, 7);

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

	// ԴIP��ַ
	CString s = (srcIP == 0) ? "����" : ::inet_ntoa(*((in_addr*)&srcIP));
	m_rules.InsertItem(nCurrent, s, 0);
	// ԴIPλ����
	s = ::inet_ntoa(*((in_addr*)&srcMask));
	m_rules.SetItemText(nCurrent, 1, s);
	// Դ�˿ں�
	s = (srcPort == 0) ? "����" : ::itoa(srcPort, szTemp, 10);
	m_rules.SetItemText(nCurrent, 2, s);
	// Ŀ��IP��ַ
	s = (dstIP == 0) ? "����" : ::inet_ntoa(*((in_addr*)&dstIP));
	m_rules.SetItemText(nCurrent, 3, s);
	// Ŀ��IPλ����
	s = ::inet_ntoa(*((in_addr*)&dstMask));
	m_rules.SetItemText(nCurrent, 4, s);
	// Ŀ�Ķ˿ں�
	s = (dstPort == 0) ? "����" : ::itoa(dstPort, szTemp, 10);
	m_rules.SetItemText(nCurrent, 5, s);
	// Э��
	if(protocol == 1)
		m_rules.SetItemText(nCurrent, 6, "ICMP");
	else if(protocol == 6)
		m_rules.SetItemText(nCurrent, 6, "TCP");
	else if(protocol == 17)
		m_rules.SetItemText(nCurrent, 6, "UDP");
	else
		m_rules.SetItemText(nCurrent, 6, "����");
	// ����
	s = bDrop ? "����" : "ͨ��";
	m_rules.SetItemText(nCurrent, 7, s);
}

void CFirewallView::UpdateList()
{
	// ȡ���ĵ�����ָ��
	CFirewallDoc* pDoc = GetDocument();
	// ������ʾ
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



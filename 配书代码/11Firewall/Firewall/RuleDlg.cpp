// RuleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Firewall.h"
#include "RuleDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRuleDlg dialog


CRuleDlg::CRuleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRuleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRuleDlg)
	m_strSourceIP = _T("0.0.0.0");
	m_strDestIP = _T("0.0.0.0");
	m_strDestMask = _T("255.255.255.255");
	m_strSourceMask = _T("255.255.255.255");
	m_nDestPort = 0;
	m_nSourcePort = 0;
	//}}AFX_DATA_INIT
}


void CRuleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRuleDlg)
	DDX_Control(pDX, IDC_PROTOCOL, m_cmbProtocol);
	DDX_Control(pDX, IDC_ACTION, m_cmbAction);
	DDX_Text(pDX, IDC_SOURCE_IP, m_strSourceIP);
	DDX_Text(pDX, IDC_DEST_IP, m_strDestIP);
	DDX_Text(pDX, IDC_DEST_MASK, m_strDestMask);
	DDX_Text(pDX, IDC_SOURCE_MASK, m_strSourceMask);
	DDX_Text(pDX, IDC_DEST_PORT, m_nDestPort);
	DDX_Text(pDX, IDC_SOURCE_PORT, m_nSourcePort);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRuleDlg, CDialog)
	//{{AFX_MSG_MAP(CRuleDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRuleDlg message handlers

BOOL CRuleDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// 初始化子窗口控件
	m_cmbProtocol.InsertString(0, "所有");
	m_cmbProtocol.InsertString(1, "TCP");
	m_cmbProtocol.InsertString(2, "UDP");
	m_cmbProtocol.InsertString(3, "ICMP");
	m_cmbProtocol.SetCurSel(0);

	m_cmbAction.InsertString(0, "丢弃");
	m_cmbAction.InsertString(1, "放行");
	m_cmbAction.SetCurSel(0);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRuleDlg::OnOK() 
{	
	UpdateData(TRUE);

	// 源IP地址
	m_srcIP = ::inet_addr(m_strSourceIP);
	if(m_srcIP == INADDR_NONE)
	{
		MessageBox("源IP地址错误！");
		return;
	}
	// 源屏蔽码
	m_srcMask = ::inet_addr(m_strSourceMask);
	// 目的IP地址
	m_destIP = ::inet_addr(m_strDestIP);
	if(m_destIP == INADDR_NONE)
	{
		MessageBox("目标IP地址错误！");
		return;
	}
	// 目的屏蔽码
	m_destMask = ::inet_addr(m_strDestMask);
	// 协议
	CString s;
	m_cmbProtocol.GetWindowText(s);
	if(s == "TCP")
	{
		m_nProtocol = 6;
	}
	else if(s == "UDP")
	{
		m_nProtocol = 17;
	}
	else if( s == "ICMP")
	{
		m_nProtocol = 1;
	}
	else
	{
		m_nProtocol = 0;
	}
	// 行动
	m_cmbAction.GetWindowText(s);
	if(s == "丢弃")
		m_bDrop = TRUE;
	else
		m_bDrop = FALSE;

	// 两个端口号
	m_srcPort = m_nSourcePort;
	m_destPort = m_nDestPort;

	CDialog::OnOK();
}

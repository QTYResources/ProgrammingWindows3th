// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Firewall.h"

#include "MainFrm.h"

#include "FirewallDoc.h"
#include "FirewallView.h"

#include "RuleDlg.h"

#include "Driver.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_RULES_ADD, OnRulesAdd)
	ON_COMMAND(ID_RULES_DELETE, OnRulesDelete)
	ON_COMMAND(ID_FILE_CONSERVE, OnFileConserve)
	ON_COMMAND(ID_FILE_LOAD, OnFileLoad)
	ON_COMMAND(ID_FILE_START, OnFileStart)
	ON_UPDATE_COMMAND_UI(ID_FILE_START, OnUpdateFileStart)
	ON_COMMAND(ID_FILE_STOP, OnFileStop)
	ON_UPDATE_COMMAND_UI(ID_FILE_STOP, OnUpdateFileStop)
	ON_COMMAND(ID_RULES_INSTALL, OnRulesInstall)
	ON_COMMAND(ID_RULES_UNINSTALL, OnRulesUninstall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// ȷ��IP����������������������Ϊ����װ���ӣ���
	m_pIPFltDrv = new CDriver("IpFltDrv.sys", "IpFltDrv");
	m_pIPFltDrv->StartDriver();

	// ����IP���˹�������
	char szPath[256];
	char* p;
	::GetFullPathName("DrvFltIp.sys", 256, szPath, &p);
	m_pFilterDrv = new CDriver(szPath, "DrvFltIp");
	if(!m_pFilterDrv->StartDriver() || !m_pFilterDrv->OpenDevice())
	{
		MessageBox("��������ʧ�ܣ�");
		exit(-1);
	}
	m_bStarted = FALSE;
}

CMainFrame::~CMainFrame()
{
	if(m_bStarted)
		m_pFilterDrv->IoControl(STOP_IP_HOOK, NULL, 0, NULL, 0);
	if(m_pFilterDrv != NULL)
		delete m_pFilterDrv;
	if(m_pIPFltDrv != NULL)
		delete m_pIPFltDrv;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnFileConserve()	// �û�����������˹���
{
	CFirewallDoc* pDoc = (CFirewallDoc*)GetActiveDocument();
	if(pDoc->m_nRules == 0)
	{
		AfxMessageBox("û�й���");
		return;
	}

	// ��������Ի���
	CFileDialog dlg(FALSE, "rul", NULL, 
		OFN_HIDEREADONLY | OFN_CREATEPROMPT, "Rule Files(*.rul)|*.rul|all(*.*)|*.*||", NULL);
	if(dlg.DoModal() == IDCANCEL)
		return;

	// д���ļ�
	CFile file;
	if(file.Open(dlg.GetPathName(), CFile::modeCreate | CFile::modeWrite))
	{
		for(int i=0; i<pDoc->m_nRules; i++)
		{
			file.Write(&pDoc->m_rules[i], sizeof(CIPFilter));
		}
	}
	else
	{
		AfxMessageBox("�����ļ�����");
	}	
}

void CMainFrame::OnFileLoad()		// �û��������ع�������
{
	// �����򿪶Ի���
	CFileDialog dlg(TRUE,NULL, NULL, 
		OFN_HIDEREADONLY | OFN_CREATEPROMPT,"Rule Files(*.rul)|*.rul|all(*.*)|*.*||", NULL);
	if(dlg.DoModal() == IDCANCEL)
		return;

	// ��ȡ���˹���
	CFile file;
	if(file.Open(dlg.GetPathName(), CFile::modeRead))
	{	
		CFirewallDoc* pDoc = (CFirewallDoc*)GetActiveDocument();
		pDoc->ClearRules();
		CIPFilter rule;
		
		// ���ļ��������ݣ���ӵ��ĵ�����
		do
		{
			if(file.Read(&rule, sizeof(rule)) == 0)
				break;
			if(!pDoc->AddRule(rule.sourceIP, rule.sourceMask, rule.sourcePort, rule.destinationIP, 
					rule.destinationMask, rule.destinationPort, rule.protocol, rule.bDrop))
			{
				AfxMessageBox("��ӹ������");
				break;
			}
		}
		while(1);
		// ������ʾ
		CFirewallView* pView = (CFirewallView*)GetActiveView();
		pView->UpdateList();
	}
	else
	{
		AfxMessageBox("���ļ�����");
	}
		
}

void CMainFrame::OnRulesAdd()		// ��ӹ���
{	
	CFirewallDoc* pDoc = (CFirewallDoc*)GetActiveDocument();
	if(pDoc->m_nRules > MAX_RULES)
	{
		MessageBox(" ��������ӹ���");
		return;
	}

	CRuleDlg dlg;
	// ������ӹ���Ի���
	if(dlg.DoModal() == IDOK)
	{
		// ���ĵ�����ӹ���
		BOOL bOK = pDoc->AddRule(dlg.m_srcIP, dlg.m_srcMask, dlg.m_srcPort, 
				dlg.m_destIP, dlg.m_destMask, dlg.m_destPort, dlg.m_nProtocol, dlg.m_bDrop);
		if(bOK)
		{
			CFirewallView* pView = (CFirewallView*)GetActiveView();
			pView->UpdateList();
		}
		else
		{
			MessageBox("��ӹ������");
		}
	}
}

void CMainFrame::OnRulesDelete()	// ɾ��һ������
{
	// �õ��û�ѡ�������
	CFirewallView* pView = (CFirewallView*)GetActiveView();
	POSITION pos = pView->m_rules.GetFirstSelectedItemPosition();
	if(pos == NULL)
	{
		MessageBox("��ѡ��һ������");
		return;
	}
	int nIndex = pView->m_rules.GetNextSelectedItem(pos);
	// ���ĵ���ɾ��һ������
	CFirewallDoc* pDoc = (CFirewallDoc*)GetActiveDocument();
	pDoc->DeleteRules(nIndex);

	// ������ʾ
	pView->UpdateList();
}



void CMainFrame::OnFileStart()		// ��ʼ����
{
	if(m_bStarted)
		return;
	// ֪ͨIP����������ʼ����
	if(m_pFilterDrv->IoControl(START_IP_HOOK, NULL, 0, NULL, 0) == -1)
	{
		MessageBox("�����������");
		return;
	}

	m_bStarted = TRUE;
}

void CMainFrame::OnFileStop()		// ֹͣ����
{
	if(m_bStarted)
	{
		// ֪ͨIP��������ֹͣ����
		m_pFilterDrv->IoControl(STOP_IP_HOOK, NULL, 0, NULL, 0);
		m_bStarted = FALSE;
	}
}

void CMainFrame::OnRulesInstall()	// ��װ��������
{
	// ���������������
	m_pFilterDrv->IoControl(CLEAR_FILTER, NULL, 0, NULL, 0);
	// ���ĵ�����ȡ�����ݣ���װ��������
	int nRet;
	CFirewallDoc* pDoc = (CFirewallDoc*)GetActiveDocument();
	for(int i=0; i<pDoc->m_nRules; i++)
	{
		// ת���ֽ�˳��
		CIPFilter pf;
		memcpy(&pf, &(pDoc->m_rules[i]), sizeof(CIPFilter));
		pf.sourcePort = (USHORT)htonl(pDoc->m_rules[i].sourcePort);
		pf.destinationPort = (USHORT)htonl(pDoc->m_rules[i].destinationPort);
		// �����豸���ƴ���
		nRet = m_pFilterDrv->IoControl(ADD_FILTER, &pf, sizeof(pf), NULL, 0);
		if(nRet == -1)
		{
			AfxMessageBox("��װ������������");
			break;
		}
	}
}

void CMainFrame::OnRulesUninstall()	// ж�ع�������
{	
	// �����������
	m_pFilterDrv->IoControl(CLEAR_FILTER, NULL, 0, NULL, 0);
}

void CMainFrame::OnUpdateFileStart(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_bStarted);
}

void CMainFrame::OnUpdateFileStop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bStarted);
}



// IntensityDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "IntensityDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIntensityDlg dialog


CIntensityDlg::CIntensityDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIntensityDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIntensityDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pImage = NULL;
}


void CIntensityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIntensityDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIntensityDlg, CDialog)
	//{{AFX_MSG_MAP(CIntensityDlg)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIntensityDlg message handlers


void CIntensityDlg::OnOk() 
{
	CString str;
	GetDlgItem(IDC_EDITLOW)->GetWindowText(str);
	m_nLowGray = atoi(str);

	GetDlgItem(IDC_EDITHIGH)->GetWindowText(str);
	m_nHighGray = atoi(str);

	if(m_nLowGray > m_nHighGray)
	{
		int nTmp = m_nLowGray;
		m_nLowGray = m_nHighGray;
		m_nHighGray = nTmp;
	}

	if(m_nHighGray > 255)
		m_nHighGray = 255;

	Invalidate(FALSE);
}

void CIntensityDlg::DrawIntersity(CDC *pDC)
{
	int i, j;

	// X、Y轴的长度
	int nXLen = 290;
	int nYLen = 290;

	// X、Y轴与边框的间距
	int nXSpan = 15;
	int nYSpan = 25;

	// 绘制矩形区域
	POINT ptLT, ptRB;
	ptLT.x = 10;
	ptLT.y = 10;

	ptRB.x = ptLT.x + nXLen + nXSpan * 2;
	ptRB.y = ptLT.y + nYLen + nYSpan * 2;

	pDC->Rectangle(ptLT.x, ptLT.y, ptRB.x, ptRB.y);

	// 获取原点坐标
	POINT ptSrc = { ptLT.x + nXSpan, ptRB.y - nYSpan };


	////////////
	CPen RedPen;
	RedPen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	CGdiObject *pOldPen = pDC->SelectObject(&RedPen);

	// 绘制X轴
	pDC->MoveTo(ptSrc);
	pDC->LineTo(ptSrc.x + nXLen, ptSrc.y);


	// 写X轴刻度，每个象素为一个单位
	for(i = 0; i <= 255; i += 50)
	{
		// 刻度值
		CString str;
		str.Format("%d", i);
		pDC->TextOut(ptSrc.x + i, ptSrc.y + 3, str);
	}

	// 标记
	for(i = 0; i <= 255; i += 5)
	{
		if((i & 1) == 0)
		{
			pDC->MoveTo(ptSrc.x + i, ptSrc.y);
			pDC->LineTo(ptSrc.x + i, ptSrc.y + 4);
		}
		else
		{
			pDC->MoveTo(ptSrc.x + i, ptSrc.y);
			pDC->LineTo(ptSrc.x + i, ptSrc.y + 2);
		}
	}

	// 绘制X轴的箭头
	pDC->MoveTo(ptSrc.x + nXLen, ptSrc.y);
	pDC->LineTo(ptSrc.x + nXLen - 5, ptSrc.y - 5);
	pDC->MoveTo(ptSrc.x + nXLen, ptSrc.y);
	pDC->LineTo(ptSrc.x + nXLen - 5, ptSrc.y + 5);


	// 绘制Y轴
	pDC->MoveTo(ptSrc);
	pDC->LineTo(ptSrc.x, ptSrc.y - nYLen);

	// 绘制Y轴的箭头
	pDC->MoveTo(ptSrc.x, ptSrc.y - nYLen);
	pDC->LineTo(ptSrc.x - 5, ptSrc.y - nYLen + 5);
	pDC->MoveTo(ptSrc.x, ptSrc.y - nYLen);
	pDC->LineTo(ptSrc.x + 5, ptSrc.y - nYLen + 5);

	// 获取最大数值
	int nMaxCount = 0;
	for(i = m_nLowGray; i <= m_nHighGray; i ++)
	{
		if(m_btCount[i] > nMaxCount)
			nMaxCount = m_btCount[i];
	}

	// 输出最大计数值
	pDC->MoveTo(ptSrc.x, ptSrc.y - nYLen + 20);
	pDC->LineTo(ptSrc.x + 4, ptSrc.y - nYLen + 20);
	CString str;
	str.Format("%d", nMaxCount);
	pDC->TextOut(ptSrc.x + 1, ptSrc.y - nYLen + 21, str);


	// 绘制上下限
	CPen GreenPen;
	GreenPen.CreatePen(PS_DOT, 1, RGB(0, 255, 0));
	pDC->SelectObject(&GreenPen);

	pDC->MoveTo(ptSrc.x + m_nLowGray, ptSrc.y);
	pDC->LineTo(ptSrc.x + m_nLowGray, ptSrc.y - nYLen + 18);
	pDC->MoveTo(ptSrc.x + m_nHighGray, ptSrc.y);
	pDC->LineTo(ptSrc.x + m_nHighGray, ptSrc.y - nYLen + 18);


	CPen BluePen;
	BluePen.CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	pDC->SelectObject(&BluePen);

	// 绘制直方图
	if(nMaxCount > 0)
	{
		for(i = m_nLowGray; i <= m_nHighGray; i ++)
		{
			pDC->MoveTo(ptSrc.x + i, ptSrc.y);
			int n = ptSrc.y + 1 - (int)(m_btCount[i]*256/nMaxCount);
			pDC->LineTo(ptSrc.x + i, n);
		}
	}

	pDC->SelectObject(pOldPen);
}


void CIntensityDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	DrawIntersity(&dc);
}

BOOL CIntensityDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
//	

	CString str;
	str.Format("%d", m_nLowGray);
	GetDlgItem(IDC_EDITLOW)->SetWindowText(str);
	str.Format("%d", m_nHighGray);
	GetDlgItem(IDC_EDITHIGH)->SetWindowText(str);
	


	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

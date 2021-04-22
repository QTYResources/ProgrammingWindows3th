// MFC00View.cpp : implementation of the CMFC00View class
//

#include "stdafx.h"
#include "MFC00.h"

#include "MFC00Doc.h"
#include "MFC00View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMFC00View

IMPLEMENT_DYNCREATE(CMFC00View, CView)

BEGIN_MESSAGE_MAP(CMFC00View, CView)
//{{AFX_MSG_MAP(CMFC00View)
ON_WM_CREATE()
ON_WM_DESTROY()
ON_WM_SIZE()
ON_WM_CANCELMODE()
//}}AFX_MSG_MAP
// Standard printing commands
ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMFC00View construction/destruction

CMFC00View::CMFC00View()
{
	// TODO: add construction code here
	
}

CMFC00View::~CMFC00View()
{
}

BOOL CMFC00View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMFC00View drawing

void CMFC00View::OnDraw(CDC* pDC)
{
	CMFC00Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	myDraw();
	SwapBuffers(wglGetCurrentDC());
}

/////////////////////////////////////////////////////////////////////////////
// CMFC00View printing

BOOL CMFC00View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMFC00View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMFC00View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMFC00View diagnostics

#ifdef _DEBUG
void CMFC00View::AssertValid() const
{
	CView::AssertValid();
}

void CMFC00View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC00Doc* CMFC00View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC00Doc)));
	return (CMFC00Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMFC00View message handlers

bool CMFC00View::bSetDCPixelFormat() 
{ 
	// ??????
	static PIXELFORMATDESCRIPTOR pfd = 
	{ 
		sizeof(PIXELFORMATDESCRIPTOR), // ?????
			1, // ?????
			PFD_DRAW_TO_WINDOW | // ???(?????)???
			PFD_SUPPORT_OPENGL | // ????????OpenGL??
			PFD_DOUBLEBUFFER, // ?????
			PFD_TYPE_RGBA, // RGBA???? 
			32, // ??32???
			0, 0, 0, 0, 0, 0, // ???????
			0, 0, // ???????
			0, 0, 0, 0, 0, // ???????
			16, // ????????
			0, // ?????
			0, // ?????
			0, // ?????
			0, // ?????
			0, 0, 0 // ?????
	}; 
	// ?????pfd????????????
	int nPixelFormat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd); 
	if(0 == nPixelFormat) return false; 
	// ???????????
	return SetPixelFormat(m_pDC->GetSafeHdc(), nPixelFormat, &pfd); 
} 

bool CMFC00View::initOpenGL()
{
	m_pDC = new CClientDC(this); 
	ASSERT(m_pDC != NULL); 
	// ??????
	if(!bSetDCPixelFormat()) return -1; 
	// ??????, ???????????
	m_hRC = wglCreateContext(m_pDC->GetSafeHdc()); 
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC); 
	glClearColor(0.0, 0.0, 0.0, 1.0); 	
	glEnable(GL_DEPTH_TEST);
	gluObj = gluNewQuadric();
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	return 0;
}

void CMFC00View::myDraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();                                 
    glColor3f(1.0f,1.0f,1.0f);
    glBegin(GL_LINES);
    glVertex3f(-0.5f,0.0f,-5.0f);
    glVertex3f(0.5f,0.0f,-5.0f);
    glEnd();
}

int CMFC00View::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	initOpenGL();
	return 0;
} 

void CMFC00View::OnDestroy() 
{
	CView::OnDestroy();
	
	// TODO: Add your message handler code here
	wglMakeCurrent(NULL, NULL); 
	wglDeleteContext(m_hRC); 
	delete m_pDC; 	
}

void CMFC00View::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	// ????
	glViewport(0, 0, cx, cy); 
	// ??????(????) 
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 
	gluPerspective(60.0, (GLfloat)cx/(GLfloat)cy, 1.0, 1000.0); 
	// ????????
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity(); 
}

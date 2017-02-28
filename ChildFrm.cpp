// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "Ex_Draw.h"

#include "ChildFrm.h"
#include "Ex_Ctrl.h"
#include "Ex_DrawView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
	
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
    CRect rect;
    GetWindowRect( &rect );
    BOOL bRes = m_wndSplitter.CreateStatic(this, 1, 2);
    m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CEx_Ctrl), CSize(0,0), pContext);
    m_wndSplitter.CreateView(0,1,RUNTIME_CLASS(CEx_DrawView), CSize(0,0), pContext);
    m_wndSplitter.SetColumnInfo(0, rect.Width()*2/7, 10);
    m_wndSplitter.SetColumnInfo(1, rect.Width()*5/7, 10);
    m_wndSplitter.SetRowInfo(0, rect.Height(), 10);
    m_wndSplitter.RecalcLayout();
    return bRes;

// 	return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
    if (!m_toolbar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
        | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
        !m_toolbar.LoadToolBar(IDR_TOOLS))
    {
        TRACE0("Failed to create toolbar\n");
        return -1;      // fail to create
    }

    m_toolbar.EnableDocking(CBRS_ALIGN_ANY);
    EnableDocking(CBRS_ALIGN_ANY);
    DockControlBar(&m_toolbar);

	return 0;
}

// Ex_DrawView.cpp : implementation of the CEx_DrawView class
//

#include "stdafx.h"
#include "Ex_Draw.h"

#include "Ex_DrawDoc.h"
#include "Ex_DrawView.h"
#include "MainFrm.h"
#include "ConfigDlg.h"
#include "HelpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define VIEW_WIDTH 800
#define VIEW_HEIGHT 600

/////////////////////////////////////////////////////////////////////////////
// CEx_DrawView

IMPLEMENT_DYNCREATE(CEx_DrawView, CScrollView)

BEGIN_MESSAGE_MAP(CEx_DrawView, CScrollView)
    ON_COMMAND_EX_RANGE(ID_TOOL_SEL, ID_OP_NEXT, OnTools)
//     ON_COMMAND_EX_RANGE(ID_OP_PREV, ID_OP_NEXT, OnTools)
	//{{AFX_MSG_MAP(CEx_DrawView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_COMMAND(ID_SAVE_AS_BITMAP, OnSaveAsBitmap)
	ON_WM_SETCURSOR()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_SHOW_TAB, OnShowTab)
	ON_COMMAND(ID_SHOW_HIDDEN, OnShowHidden)
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_CALL_HELP, OnCallHelp)
	ON_UPDATE_COMMAND_UI(ID_OP_PREV, OnCmdUIPrev)
	ON_UPDATE_COMMAND_UI(ID_OP_NEXT, OnCmdUINext)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEx_DrawView construction/destruction

CEx_DrawView::CEx_DrawView()
{
	// TODO: add construction code here
    m_bLButtonDown = FALSE;
    m_bCtrlDown = FALSE;
    m_size.cx = VIEW_WIDTH;
    m_size.cy = VIEW_HEIGHT;
    m_pMoveTarget = NULL;
    m_pBar = NULL;
    m_bSetCursor = FALSE;
    m_bShiftDown = FALSE;
    m_bFirstMove = FALSE;
    m_bDup = FALSE;
    m_iTrackID = -1;
    m_bContextMenu = FALSE;
    m_bSizingMode = FALSE;
    m_sizingX = NULL;
    m_sizingY = NULL;
    m_pLogBuffer = NULL;
    m_pToolbar = NULL;
}

CEx_DrawView::~CEx_DrawView()
{
}

BOOL CEx_DrawView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CEx_DrawView drawing

void CEx_DrawView::OnDraw(CDC* pDC)
{
	// TODO: add draw code for native data here
    CEx_DrawDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    m_bufferDC.BitBlt(0, 0, m_size.cx, m_size.cy, pDC, 0, 0, WHITENESS);
    DrawGraphic(&m_bufferDC);
    if (pDoc->m_sel != -1)
    {
		ASSERT(pDoc->m_sel >= 0 && pDoc->m_sel < pDoc->m_obArray.GetSize());
        CGraphic* P = Convert_To_Graphic(pDoc->m_obArray[pDoc->m_sel]);
		ASSERT_VALID(P);
        P->Draw(&m_bufferDC);
        m_bufferDC.SelectObject(GetStockObject(WHITE_BRUSH));
        P->DrawSelectedEdge(&m_bufferDC);
        if (pDoc->m_bShowTab)
        {
            CString str;
            int bkMode = m_bufferDC.SetBkMode(TRANSPARENT);
            str.Format(_T("%d"), pDoc->m_sel);
            m_bufferDC.SelectObject(&m_tabFont);
            m_bufferDC.TextOut(P->m_pt1.x, P->m_pt1.y, str);
        }
    }
    pDC->BitBlt(0, 0, m_size.cx, m_size.cy, &m_bufferDC, 0, 0, SRCCOPY);
}

void CEx_DrawView::DrawGraphic( CDC* pDC, int skip /*= -1*/ )
{
    CEx_DrawDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    CString str;
    BOOL bShowHidden = pDoc->m_bShowHidden;
    for (int i = 0; i < pDoc->m_obArray.GetSize(); i++)
    {
        CGraphic* p = Convert_To_Graphic(pDoc->m_obArray[i]);
        if (i == skip || (!bShowHidden && p->m_bHidden))
        {
            continue;
        }
        p->Draw(pDC);
        if (pDoc->m_bShowTab) GraphicDrawTab(pDC, p, i);
    }
}

void CEx_DrawView::GraphicDrawTab( CDC* pDC, CGraphic* P, int n )
{
    CString str;
    CRect rt;
    P->GetRect(rt);
    str.Format(_T("%d"), n);
    int bkMode = pDC->SetBkMode(TRANSPARENT);
    pDC->SelectObject(&m_tabFont);
    pDC->TextOut(rt.left + 5, rt.top + 5, str);
    pDC->SetBkMode(bkMode);
}

void CEx_DrawView::GraphicDeleteById( int id )
{
    CEx_DrawDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    int size = pDoc->m_obArray.GetSize();
    ASSERT(id >= 0 && id < size);
    CRect rt;
    CGraphic* pDelete = Convert_To_Graphic(pDoc->m_obArray[id]);
    pDelete->GetRect(rt);
    pDoc->WriteLog(CGraphicLog::GOS_DELETE, pDelete, id);
    pDelete = NULL;
    pDoc->UpdateAllViews(NULL, 0x4000);
    rt.OffsetRect(-GetScrollPosition());
    InvalidateRect(&rt, TRUE);
}

void CEx_DrawView::GraphicHitTest( CPoint& pt, BOOL& bResult, int& sel )
{
    CEx_DrawDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    BOOL bShowHidden = pDoc->m_bShowHidden;
    if (pDoc->m_sel != -1) // 已选中者优先
    {
		if (pDoc->m_sel == pDoc->m_obArray.GetSize()) pDoc->m_sel--;
        CGraphic* P = Convert_To_Graphic(pDoc->m_obArray[pDoc->m_sel]);
        P->HitTest(pt, bResult);
        if ( bResult && (bShowHidden || (!bShowHidden && !P->m_bHidden)) ) return;
        bResult = FALSE;
    }
    for (int i = pDoc->m_obArray.GetSize() - 1; i >= 0; i--)
    {
        CGraphic* P = Convert_To_Graphic(pDoc->m_obArray[i]);
        P->HitTest(pt, bResult);
        if ( bResult && (bShowHidden || (!bShowHidden && !P->m_bHidden)) ) { sel = i; return; }
        bResult = FALSE;
    }
}

void CEx_DrawView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

    m_pBar = &static_cast<CMainFrame*>(AfxGetMainWnd())->m_wndStatusBar;
    m_pToolbar = &static_cast<CMainFrame*>(AfxGetMainWnd())->m_wndToolBar;
    ASSERT(m_pBar && m_pToolbar);

    m_tabFont.CreateFont(20, 10, 0, 0, 12, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, TEXT("宋体"));

    GetDocument()->UpdateAllViews(NULL, 0x4000);
	SetScrollSizes( MM_TEXT, CSize( m_size.cx, m_size.cy ) );

    AdjustWindow();
}

void CEx_DrawView::AdjustWindow()
{
    CClientDC dc(this);
    m_backDC.DeleteDC();
    m_backDC.CreateCompatibleDC(&dc);
    m_backBitmap.DeleteObject();
    m_backBitmap.CreateCompatibleBitmap(&dc, m_size.cx, m_size.cy );
    m_backDC.SelectObject(&m_backBitmap);
    m_bufferDC.DeleteDC();
    m_bufferDC.CreateCompatibleDC(&dc);
    m_bufferBitmap.DeleteObject();
    m_bufferBitmap.CreateCompatibleBitmap(&dc, m_size.cx, m_size.cy );
    m_bufferDC.SelectObject(&m_bufferBitmap);
}

/////////////////////////////////////////////////////////////////////////////
// CEx_DrawView printing

BOOL CEx_DrawView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CEx_DrawView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CEx_DrawView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CEx_DrawView diagnostics

#ifdef _DEBUG
void CEx_DrawView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CEx_DrawView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CEx_DrawDoc* CEx_DrawView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEx_DrawDoc)));
	return (CEx_DrawDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEx_DrawView message handlers

void CEx_DrawView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
    CEx_DrawDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    m_ptLocalScroll = GetScrollPosition();
    CPoint ptLocal = point + m_ptLocalScroll;

    m_bLButtonDown = TRUE;
    m_bContextMenu = FALSE;

    SetCapture();

    CClientDC dc(this);

    UpdateWindow(); // 强制刷新
    m_backDC.BitBlt(0, 0, m_size.cx, m_size.cy, NULL, 0, 0, WHITENESS);
    m_backDC.BitBlt(0, 0, m_size.cx, m_size.cy, &dc, 0, 0, SRCCOPY);

    if (pDoc->m_GM.m_DrawType != NONE)
    {
        pDoc->m_GM.m_pt1 = ptLocal;

        m_lastPt = point;
        pDoc->m_GM.m_pts.RemoveAll();

        if (pDoc->m_GM.m_DrawType == CURVE)
        {
            GraphicMember* pGM = &pDoc->m_GM;
            m_pen.DeleteObject();
            m_pen.CreatePen(pGM->m_iPenStyle, pGM->m_iPenWidth, pGM->m_clrPen);
            m_bufferDC.BitBlt(0, 0, m_size.cx, m_size.cy, NULL, 0, 0, WHITENESS);
            m_bufferDC.BitBlt(0, 0, m_size.cx, m_size.cy, &m_backDC, 0, 0, SRCCOPY);
        }
    }
    else if (pDoc->m_GM.m_DrawType == SELECT)
    {
        BOOL bResult = FALSE;
        int last_sel = pDoc->m_sel;
        GraphicHitTest(ptLocal, bResult, pDoc->m_sel);
        if (!bResult) { pDoc->m_sel = -1; TRACE("Hit NONE\n") ;}
        else {
            CGraphic* P = Convert_To_Graphic(pDoc->m_obArray[pDoc->m_sel]);
            TRACE("Hit (%d,%d) in graphic [ID: %d TYPE: \"%s\" RECT: (%d,%d,%d,%d)]\n",
                ptLocal.x, ptLocal.y, pDoc->m_sel, P->GetName(),
                P->m_pt1.x, P->m_pt1.y, P->m_pt2.x, P->m_pt2.y);
        }
        if (last_sel != -1 && !m_bSizingMode)
        {
			if (last_sel == pDoc->m_obArray.GetSize()) last_sel--;
            CRect rt;
            Convert_To_Graphic(pDoc->m_obArray[last_sel])->GetRect(rt);
            rt.OffsetRect(-GetScrollPosition());
            InvalidateRect(&rt, TRUE);
        }
        if (pDoc->m_sel != -1)
        {
            ASSERT(m_pMoveTarget == NULL);
            
            static GraphicMember GM;
            Convert_To_Graphic(pDoc->m_obArray[pDoc->m_sel])->UpdateData(&GM, FALSE);
            m_pLogBuffer = CGraphic::CreateGraphic(&GM);
            m_pLogBuffer->m_modifiedTime = pDoc->m_GM.m_modifiedTime;
            m_pMoveTarget = CGraphic::CreateGraphic(&GM);
            if (last_sel == pDoc->m_sel) // 缩放
            {
                BOOL bRet = FALSE;
                m_sizingX = NULL;
                m_sizingY = NULL;
                m_pMoveTarget->HitSizingTest(ptLocal, bRet, &m_sizingX, &m_sizingY);
                if (bRet) m_bSizingMode = TRUE;
            }
            m_bufferDC.BitBlt(0, 0, m_size.cx, m_size.cy, NULL, 0, 0, WHITENESS);
            m_bufferDC.BitBlt(m_ptLocalScroll.x, m_ptLocalScroll.y,
                m_size.cx - m_ptLocalScroll.x, m_size.cy - m_ptLocalScroll.y, &m_backDC, 0, 0, SRCCOPY);
            m_pMoveTarget->Draw(&m_bufferDC);
            m_bufferDC.SelectObject(GetStockObject(WHITE_BRUSH));
            m_bufferDC.SelectObject(GetStockObject(BLACK_PEN));
            m_pMoveTarget->DrawSelectedEdge(&m_bufferDC);
            if (pDoc->m_bShowTab) GraphicDrawTab(&m_bufferDC, m_pMoveTarget, pDoc->m_sel);
            dc.BitBlt(0, 0, m_size.cx - m_ptLocalScroll.x, m_size.cy - m_ptLocalScroll.y, &m_bufferDC,
                m_ptLocalScroll.x, m_ptLocalScroll.y, SRCCOPY);
            m_ptCurTarget1 = m_pMoveTarget->m_pt1;
            m_ptCurTarget2 = m_pMoveTarget->m_pt2;
            m_ptOriginHit = point;
        }
    }

    m_bFirstMove = TRUE;

    if (pDoc->m_sel != -1)
    {
        static CString strPane;
        static CGraphic* P;
        P = Convert_To_Graphic(pDoc->m_obArray[pDoc->m_sel]);
        strPane.Format(_T("您选中了：%s"), P->GetName());
        m_pBar->SetPaneText(0, strPane.GetBuffer(0));
    }
    else
    {
        m_pBar->SetPaneText(0, _T("您当前没有选中任何图形"));
    }

    pDoc->UpdateAllViews(NULL, 0x4001);

    CScrollView::OnLButtonDown(nFlags, point);
}

void CEx_DrawView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
    CEx_DrawDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    m_ptLocalScroll = GetScrollPosition();
    CPoint ptLocal = point + m_ptLocalScroll;

    ReleaseCapture();

    if (m_bLButtonDown)
    {
        m_bLButtonDown = FALSE;

        pDoc->m_GM.m_pt2 = ptLocal;

        if (pDoc->m_GM.m_DrawType != SELECT)
        {
			BOOL bCreate = TRUE;

			if (pDoc->m_GM.m_DrawType == RECTANGLE || pDoc->m_GM.m_DrawType == ELLIPSE)
			{
				CRect rt;
				rt.SetRect(pDoc->m_GM.m_pt1, pDoc->m_GM.m_pt2);
				rt.NormalizeRect();
				if (rt.Width() < 5 || rt.Height() < 5)
				{
					bCreate = FALSE;
				}
			}

			if (bCreate)
			{
				// 创建新的对象
				CGraphic* pGraphic = CGraphic::CreateGraphic(&pDoc->m_GM);
				pGraphic->m_createTime = CTime::GetCurrentTime();
				pGraphic->m_modifiedTime = pGraphic->m_createTime;
				pGraphic->m_bHidden = FALSE;
				pGraphic->m_lpszName.Format(_T("%s_%d"), pGraphic->GetName(), ++pDoc->m_createID);
				
				ASSERT(pGraphic);
				if (pDoc->m_GM.m_DrawType == CURVE)
				{
					pDoc->m_GM.m_pts.RemoveAll();
					m_pen.DeleteObject();
				}
				else
				{
					if (m_bShiftDown)
						pGraphic->m_pt2.y = pGraphic->m_pt1.y + labs (pGraphic->m_pt2.x - pGraphic->m_pt1.x);
				}
				CRect rt;
				pGraphic->GetRect(rt);
				InvalidateRect(&rt, TRUE);
				pDoc->WriteLog(CGraphicLog::GOS_ADD, pGraphic, pDoc->m_obArray.GetSize());
				
				pDoc->UpdateAllViews(NULL, 0x4000);
				pDoc->UpdateAllViews(NULL, 0x4001);
			}
        }
        else
        {
            if (m_pMoveTarget)
            {
                if (!m_bFirstMove) m_pMoveTarget->m_modifiedTime = CTime::GetCurrentTime();
                static GraphicMember GM;
                m_pMoveTarget->UpdateData(&GM, FALSE);
                Convert_To_Graphic(pDoc->m_obArray[pDoc->m_sel])->UpdateData(&GM); // 修改坐标
                if (m_pLogBuffer)
                {
                    if (!m_bFirstMove || m_bSizingMode)
                    {
                        pDoc->WriteLog(CGraphicLog::GOS_UPDATE, m_pLogBuffer, pDoc->m_sel);
                    }
                    else
                    {
                        delete m_pLogBuffer;
                        m_pLogBuffer = NULL;
                    }
                }
                if (!m_bDup) delete m_pMoveTarget;
                else m_bDup = FALSE;
                m_pMoveTarget = NULL;
                m_bSizingMode = FALSE;
            }
        }
    }

    m_bFirstMove = FALSE;

	CScrollView::OnLButtonUp(nFlags, point);
}

void CEx_DrawView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
    CEx_DrawDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    m_ptLocalScroll = GetScrollPosition();
    CPoint ptLocal = point + m_ptLocalScroll;

    if (m_bLButtonDown)
    {
        CClientDC dc(this);
        switch (pDoc->m_GM.m_DrawType)
        {
        case SELECT:
            if (m_pMoveTarget)
            {
                if (m_bFirstMove && m_bLButtonDown)
                {                    
                    if (m_bCtrlDown && !m_bSizingMode)
                    {
                        static GraphicMember GM;
                        m_pMoveTarget->UpdateData(&GM, FALSE);
                        m_pMoveTarget = CGraphic::CreateGraphic(&GM);
                        m_pMoveTarget->m_lpszName.Format(_T("%s_%d"), m_pMoveTarget->GetName(), ++pDoc->m_createID);
                        pDoc->WriteLog(CGraphicLog::GOS_ADD, m_pMoveTarget, pDoc->m_obArray.GetSize());
                        pDoc->m_sel = pDoc->m_obArray.GetSize() - 1;
                        pDoc->UpdateAllViews(NULL, 0x4000);
                        pDoc->UpdateAllViews(NULL, 0x4001);
                        m_bDup = TRUE;
                    }

                    m_backDC.BitBlt(0, 0, m_size.cx, m_size.cy, NULL, 0, 0, WHITENESS);
                    DrawGraphic(&m_backDC, pDoc->m_sel);
                }

                static CPoint offset;                
                offset = point;
                offset -= m_ptOriginHit;
                if (m_bSizingMode)
                {
                    if (m_sizingX) *m_sizingX = ptLocal.x;
                    if (m_sizingY) *m_sizingY = ptLocal.y;
                }
                else
                {
                    m_pMoveTarget->m_pt1 = m_ptCurTarget1 + offset;
                    m_pMoveTarget->m_pt2 = m_ptCurTarget2 + offset;
                }

                m_bufferDC.BitBlt(0, 0, m_size.cx, m_size.cy, NULL, 0, 0, WHITENESS);
                m_bufferDC.BitBlt(0, 0, m_size.cx, m_size.cy, &m_backDC, 0, 0, SRCCOPY);
                m_pMoveTarget->Draw(&m_bufferDC);
                m_bufferDC.SelectObject(GetStockObject(WHITE_BRUSH));
                m_bufferDC.SelectObject(GetStockObject(BLACK_PEN));
                m_pMoveTarget->DrawSelectedEdge(&m_bufferDC);
                if (pDoc->m_bShowTab) GraphicDrawTab(&m_bufferDC, m_pMoveTarget, pDoc->m_sel);
                dc.BitBlt(0, 0, m_size.cx - m_ptLocalScroll.x, m_size.cy - m_ptLocalScroll.y, &m_bufferDC,
                    m_ptLocalScroll.x, m_ptLocalScroll.y, SRCCOPY);
            }
            break;
        case CURVE:
            pDoc->m_GM.m_pts.Add((CPoint)(ptLocal - pDoc->m_GM.m_pt1)); // 相对位置
            m_bufferDC.SelectObject(&m_pen);
            m_bufferDC.MoveTo(m_lastPt);
            m_bufferDC.LineTo(point);
            dc.BitBlt(0, 0, m_size.cx, m_size.cy, &m_bufferDC, 0, 0, SRCCOPY);
            break;
        case LINE:
        case RECTANGLE:
        case ELLIPSE:
            CGraphic* pGraphic = CGraphic::CreateGraphic(&pDoc->m_GM);
            ASSERT(pGraphic);
            pGraphic->m_pt1 -= GetScrollPosition();
            pGraphic->m_pt2 = point;
            if (m_bShiftDown)
                pGraphic->m_pt2.y = pGraphic->m_pt1.y + labs (pGraphic->m_pt2.x - pGraphic->m_pt1.x);
            m_bufferDC.BitBlt(0, 0, m_size.cx, m_size.cy, NULL, 0, 0, WHITENESS);
            m_bufferDC.BitBlt(0, 0, m_size.cx, m_size.cy, &m_backDC, 0, 0, SRCCOPY);
            pGraphic->Draw(&m_bufferDC);
            dc.BitBlt(0, 0, m_size.cx, m_size.cy, &m_bufferDC, 0, 0, SRCCOPY);
            delete pGraphic;
            break;
        }

        m_lastPt = point;
    }

    m_ptCursor = ptLocal;
    m_bFirstMove = FALSE;

	CScrollView::OnMouseMove(nFlags, point);
}

void CEx_DrawView::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
    BOOL bNeedAdjustWindow = FALSE;
    if (cx > VIEW_WIDTH)  { m_size.cx = cx + 20;   bNeedAdjustWindow = TRUE; }
    if (cy > VIEW_HEIGHT) { m_size.cy = cy + 20;   bNeedAdjustWindow = TRUE; }
    if (bNeedAdjustWindow) AdjustWindow();
}

void CEx_DrawView::OnSaveAsBitmap() 
{
	// TODO: Add your command handler code here
	CFileDialog dlg(FALSE, _T(".bmp"), _T("图片1"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("位图(*.bmp)|*.bmp||"));
    if (dlg.DoModal() != IDOK)
    {
        return;
    }

    CFile file;
    file.Open(dlg.GetPathName(), CFile::modeWrite | CFile::modeCreate);

    CDC* pDC = GetDC();
    BITMAP bm;
//     pDC->GetCurrentBitmap()->GetBitmap(&bm); // 这个在XP下会失败
    CDC dc;
    CBitmap bitmap;
    dc.CreateCompatibleDC(pDC);
    BITMAPINFO biPicInfo;
    LPVOID pvBits = NULL;
    biPicInfo.bmiHeader.biSize          = sizeof(biPicInfo.bmiHeader);
    biPicInfo.bmiHeader.biWidth         = m_size.cx;
    biPicInfo.bmiHeader.biHeight        = m_size.cy;
    biPicInfo.bmiHeader.biPlanes        = 1;
    biPicInfo.bmiHeader.biBitCount      = 32;
    biPicInfo.bmiHeader.biCompression   = BI_RGB;      // = 0 不压缩
    biPicInfo.bmiHeader.biSizeImage     = (m_size.cx * biPicInfo.bmiHeader.biBitCount + 31) / 32 * 4 * m_size.cy;
    biPicInfo.bmiHeader.biXPelsPerMeter = 0;    //
    biPicInfo.bmiHeader.biYPelsPerMeter = 0;    //
    biPicInfo.bmiHeader.biClrUsed       = 0;    // use all
    biPicInfo.bmiHeader.biClrImportant  = 0;    // use all
    bitmap.Attach(CreateDIBSection(NULL, &biPicInfo, DIB_RGB_COLORS, &pvBits, NULL, 0));
    bitmap.GetBitmap(&bm);
    dc.SelectObject(&bitmap);
    dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, NULL, 0, 0, WHITENESS);
    DrawGraphic(&dc);
    ReleaseDC(pDC);

    DWORD size = bm.bmWidthBytes * bm.bmHeight;

    BITMAPFILEHEADER bf;
    bf.bfType = 0x4d42;
    bf.bfSize = size + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bf.bfReserved1 = 0;
    bf.bfReserved2 = 0;
    bf.bfOffBits = 0x36;

    LPVOID pBits = (LPVOID) GlobalAlloc(GMEM_ZEROINIT, size);
    CopyMemory(pBits, pvBits, size);

    file.Write(&bf, sizeof(BITMAPFILEHEADER));
    file.Write(&biPicInfo.bmiHeader, sizeof(BITMAPINFOHEADER));
    file.Write(pBits, size);
    
    GlobalFree(pBits);
}

void CEx_DrawView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
    CEx_DrawDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    if (lHint == 0x1000)
    {
		if (pDoc->m_sel == -1)
		{
			Invalidate();
		}

        CRect rt;
        Convert_To_Graphic(pDoc->m_obArray[pDoc->m_sel])->GetRect(rt);
        pDoc->m_sel = -1;
        InvalidateRect(&rt, TRUE);  
    }
    else if (lHint == 0x3000)
    {
        OnSaveAsBitmap();
    }
    else if (lHint == 0x3001)
    {
        OnShowTab();
    }
    else if (lHint == 0x3002)
    {
        OnShowHidden();
    }
    else if (lHint == 0x5000)
    {
		Invalidate();
		SetFocus();
    }
}

BOOL CEx_DrawView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
    // TODO: Add your message handler code here and/or call default
    BOOL bResult = FALSE;
    GraphicHitTest(m_ptCursor, bResult, m_iTrackID);
    
    if ((bResult || (m_bLButtonDown && m_pMoveTarget != NULL)) && GetDocument()->m_GM.m_DrawType == SELECT && !m_bContextMenu)
    {
        BOOL bRet = FALSE;
        CGraphic* P = Convert_To_Graphic(GetDocument()->m_obArray[m_iTrackID]);
		ASSERT_VALID(P);
        LPCTSTR cursor = P->HitSizingTest(m_ptCursor, bRet);
        if (bRet) { SetCursor(AfxGetApp()->LoadStandardCursor(cursor)); return TRUE; }
        SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
        return TRUE;
    }
    
	return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}

void CEx_DrawView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
    CEx_DrawDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    UINT type = pDoc->m_GM.m_DrawType;

	if (nChar == VK_SHIFT && type != SELECT)
	{
        m_bShiftDown = TRUE;
        CPoint pt;
        GetCursorPos(&pt);
        ScreenToClient(&pt);
        OnMouseMove(0, pt);
	}
    else if (nChar == VK_CONTROL && type == SELECT)
    {       
        m_bCtrlDown = TRUE;
    }
    else if (nChar == VK_DELETE && type == SELECT)
    {
        int sel = pDoc->m_sel;
        if (sel != -1)
        {
            GraphicDeleteById(sel);
            pDoc->m_sel = -1;
        }
    }
    else if (nChar >= VK_LEFT && nChar <= VK_DOWN)
    {
        int sel = pDoc->m_sel;
        if (sel != -1)
        {
            CGraphic* P = Convert_To_Graphic(pDoc->m_obArray[pDoc->m_sel]);
            switch (nChar)
            {
            case VK_LEFT:   P->m_pt1.x--; P->m_pt2.x--; break;
            case VK_RIGHT:  P->m_pt1.x++; P->m_pt2.x++; break;
            case VK_DOWN:   P->m_pt1.y++; P->m_pt2.y++; break;
            case VK_UP:     P->m_pt1.y--; P->m_pt2.y--; break;
            }
            CRect rt;
            P->GetRect(rt);
            P->m_modifiedTime = CTime::GetCurrentTime();
            rt.OffsetRect(-GetScrollPosition());
            InvalidateRect(&rt, FALSE);
        }
    }
	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CEx_DrawView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default  
    CEx_DrawDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    if (nChar == VK_SHIFT)
    {
        m_bShiftDown = FALSE;
        CPoint pt;
        GetCursorPos(&pt);
        ScreenToClient(&pt);
        OnMouseMove(0, pt);
    }
    else if (nChar == VK_CONTROL)
    {       
        m_bCtrlDown = FALSE;
    }
	CScrollView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CEx_DrawView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
    CEx_DrawDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    
    m_bContextMenu = TRUE;

    if (pDoc->m_GM.m_DrawType == SELECT)
    {
        CPoint pt;
        GetCursorPos(&pt);
        ScreenToClient(&pt);
        pt += m_ptLocalScroll;
        BOOL bResult = FALSE;
        GraphicHitTest(pt, bResult, m_iTrackID);
        
        if (bResult && pDoc->m_sel == m_iTrackID && m_iTrackID != -1)
        {          
            CGraphic* P = Convert_To_Graphic(pDoc->m_obArray[m_iTrackID]);
            CRect rt;
            P->GetRect(rt);
            GraphicMember GM;
            P->UpdateData(&GM, FALSE);
            CMenu menu;
            CString title;
            GetCursorPos(&pt);
            menu.CreatePopupMenu();
            
            menu.AppendMenu(MF_STRING, 0x80, _T("----===== 画布信息 =====----"));
            title.Format(_T("图布大小： %dx%d"), m_size.cx, m_size.cy);
            menu.AppendMenu(MF_STRING, 0x81, title);
            
            menu.AppendMenu(MF_STRING, 0x200, _T("----===== 操作命令 =====----"));
            menu.AppendMenu(MF_STRING | MF_BYCOMMAND, 0x201, _T("编辑当前图形"));
            menu.AppendMenu(MF_STRING | MF_BYCOMMAND, 0x202, _T("删除当前图形"));
            
            menu.AppendMenu(MF_STRING, 0x100, _T("----===== 图形信息 =====----"));
            title.Format(_T("自定义名称： %s"), GM.m_lpszName);
            menu.AppendMenu(MF_STRING, 0x106, title);
            title.Format(_T("图像类型： %s"), P->GetName());
            menu.AppendMenu(MF_STRING, 0x101, title);
            title.Format(_T("存放序号： %d"), m_iTrackID);
            menu.AppendMenu(MF_STRING, 0x102, title);
            title.Format(_T("节点数量： %d"), P->GetPts());
            menu.AppendMenu(MF_STRING, 0x103, title);
            title.Format(_T("图像大小： %dx%d"), rt.Width(), rt.Height());
            menu.AppendMenu(MF_STRING, 0x104, title);
            title.Format(_T("是否隐藏： %s"), GM.m_bHidden ? _T("是") : _T("否"));
            menu.AppendMenu(MF_STRING, 0x107, title);
            title.Format(_T("逻辑坐标： (%d, %d, %d, %d)"), rt.left, rt.top, rt.right, rt.bottom);
            menu.AppendMenu(MF_STRING, 0x105, title);
            
            menu.AppendMenu(MF_SEPARATOR);
            
            title.Format(_T("创建时间： %s"), GM.m_createTime.Format("%c"));
            menu.AppendMenu(MF_STRING, 0x300, title);
            title.Format(_T("修改时间： %s"), GM.m_modifiedTime.Format("%c"));
            menu.AppendMenu(MF_STRING, 0x301, title);
            
            menu.AppendMenu(MF_SEPARATOR);
            
            title.Format(_T("画笔类型： %s"), CGraphic::GetPenStyleById(GM.m_iPenStyle, FALSE));
            menu.AppendMenu(MF_STRING, 0x125, title);
            
            if (!IF_NULL_PEN(GM.m_iPenStyle))
            {
                title.Format(_T("画笔颜色： RGB(%d, %d, %d)"), GetRValue(GM.m_clrPen), GetGValue(GM.m_clrPen), GetBValue(GM.m_clrPen));
                menu.AppendMenu(MF_STRING, 0x120, title);
                title.Format(_T("画笔粗细： %d"), GM.m_iPenWidth);
                menu.AppendMenu(MF_STRING, 0x122, title);
            }
            
            menu.AppendMenu(MF_SEPARATOR);
            
            if (P->EnableBrush())
            {
                title.Format(_T("画笔类型： %s"), CGraphic::GetBrushStyleById(GM.m_iBrushStyle, FALSE));
                menu.AppendMenu(MF_STRING, 0x400, title);
                if (!IF_NULL_BRUSH(GM.m_iBrushStyle))
                {
                    title.Format(_T("画刷颜色： RGB(%d, %d, %d)"), GetRValue(GM.m_clrBrush), GetGValue(GM.m_clrBrush), GetBValue(GM.m_clrBrush));
                    menu.AppendMenu(MF_STRING, 0x401, title);
                }
            }
            
            menu.TrackPopupMenu(0, pt.x, pt.y, this);
        }
    }
    else
    {
        pDoc->m_GM.m_DrawType = SELECT;
        pDoc->UpdateAllViews(NULL, 0x2000);
    }
}

BOOL CEx_DrawView::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
    CEx_DrawDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

	if (wParam == 0x200)
	{
        GraphicDeleteById(pDoc->m_sel);
        pDoc->m_sel = -1;
        return TRUE;
	}

    if (wParam == 0x201)
    {
        ASSERT(pDoc->m_sel >= 0);
        ConfigDlg dlg;
        CGraphic* P = Convert_To_Graphic(pDoc->m_obArray[pDoc->m_sel]);

        ASSERT_VALID(P);
        CRect rt;
        P->GetRect(rt);
        P->UpdateData(&dlg.m_GM, FALSE);
        CGraphic* pSave = CGraphic::CreateGraphic(&dlg.m_GM);
        pSave->m_modifiedTime = dlg.m_GM.m_modifiedTime;
        dlg.m_typeName = P->GetName();
        if (dlg.DoModal() == IDOK)
        {
            if (dlg.m_GM.m_bHidden) pDoc->m_sel = -1;
            P->UpdateData(&dlg.m_GM);
            pDoc->WriteLog(CGraphicLog::GOS_UPDATE, pSave, pDoc->m_sel);
            pDoc->UpdateAllViews(NULL, 0x4000);
            pDoc->UpdateAllViews(NULL, 0x4001);
            Invalidate();
        }
        else
        {
            delete pSave;
        }
        return TRUE;
    }

    if (wParam == 0x202)
    {
        GraphicDeleteById(pDoc->m_sel);
        pDoc->m_sel = -1;
        return TRUE;
    }

	return CScrollView::OnCommand(wParam, lParam);
}

BOOL CEx_DrawView::OnTools( UINT wParam )
{
    if (wParam >= ID_TOOL_SEL && wParam <= ID_TOOL_CURVE)
    {
        GetDocument()->m_GM.m_DrawType = wParam - ID_TOOL_SEL;
        GetDocument()->UpdateAllViews(NULL, 0x2000);
        return TRUE;
    }

    if (wParam == ID_OP_PREV)
    {
        GetDocument()->Undo();
        return TRUE;
    }
    if (wParam == ID_OP_NEXT)
    {
        GetDocument()->Done();
        return TRUE;
    }

    return FALSE;
}

void CEx_DrawView::OnShowTab() 
{
    BOOL& bS = GetDocument()->m_bShowTab;
    AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_SHOW_TAB, bS ? MF_UNCHECKED : MF_CHECKED);
    bS = !bS;
    Invalidate();
}

void CEx_DrawView::OnShowHidden()
{
    BOOL& bS = GetDocument()->m_bShowHidden;
    AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_SHOW_HIDDEN, bS ? MF_UNCHECKED : MF_CHECKED);
    bS = !bS;
    Invalidate();
}

BOOL CEx_DrawView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
    CPoint loc = GetScrollPosition();
    loc.y -= zDelta;
    if (loc.y > m_size.cy) loc.y = m_size.cy;
    if (loc.y < 0) loc.y = 0;
    SetScrollPos(SB_VERT, loc.y);
    Invalidate();

	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}

void CEx_DrawView::OnCallHelp() 
{
	// TODO: Add your command handler code here
	CHelpDlg dlg;
    dlg.DoModal();
}

void CEx_DrawView::OnCmdUIPrev(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    pCmdUI->Enable(GetDocument()->m_log.CanUndo());
}

void CEx_DrawView::OnCmdUINext(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    pCmdUI->Enable(GetDocument()->m_log.CanDo());
}

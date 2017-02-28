// Ex_Ctrl.cpp : implementation file
//

#include "stdafx.h"
#include "Ex_Draw.h"
#include "Ex_Ctrl.h"
#include "HelpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEx_Ctrl

IMPLEMENT_DYNCREATE(CEx_Ctrl, CFormView)

CEx_Ctrl::CEx_Ctrl()
	: CFormView(CEx_Ctrl::IDD)
{
	//{{AFX_DATA_INIT(CEx_Ctrl)
	//}}AFX_DATA_INIT
}

CEx_Ctrl::~CEx_Ctrl()
{
}

void CEx_Ctrl::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEx_Ctrl)
	DDX_Control(pDX, IDC_GRAPH_LIST, m_lists);
	DDX_Control(pDX, IDC_SELBS, m_sel_bs);
	DDX_Control(pDX, IDC_CHANGECOLOR2, m_btn_setBrush);
	DDX_Control(pDX, IDC_CHANGECOLOR, m_btn_setColor);
	DDX_Control(pDX, IDC_COLOR2, m_brush_view);
	DDX_Control(pDX, IDC_COLOR_V2, m_brush_val);
	DDX_Control(pDX, IDC_COLOR_V, m_color_val);
	DDX_Control(pDX, IDC_SELPW, m_sel_pw);
	DDX_Control(pDX, IDC_SPINPW, m_sp_pw);
	DDX_Control(pDX, IDC_SELPS, m_sel_ps);
	DDX_Control(pDX, IDC_SELGRAPH, m_sel_graph);
	DDX_Control(pDX, IDC_COLOR, m_color_view);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEx_Ctrl, CFormView)
	//{{AFX_MSG_MAP(CEx_Ctrl)
	ON_BN_CLICKED(IDC_CHANGECOLOR, OnChangePenColor)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_SELGRAPH, OnSelGraph)
	ON_CBN_SELCHANGE(IDC_SELPS, OnSelPs)
	ON_EN_CHANGE(IDC_SELPW, OnSelPw)
	ON_BN_CLICKED(IDC_CHANGECOLOR2, OnChangeBrushColor)
    ON_CBN_SELCHANGE(IDC_SELBS, OnSelBs)
	ON_LBN_DBLCLK(IDC_GRAPH_LIST, OnRandomSelGraph)
	ON_UPDATE_COMMAND_UI(ID_OP_NEXT, OnUpdateOpNext)
	ON_UPDATE_COMMAND_UI(ID_OP_PREV, OnUpdateOpPrev)
	//}}AFX_MSG_MAP
    ON_COMMAND_EX_RANGE(ID_TOOL_SEL, ID_OP_NEXT, OnTools)
    ON_COMMAND_EX(ID_SAVE_AS_BITMAP, OnTools)
    ON_COMMAND_EX(ID_SHOW_TAB, OnTools)
    ON_COMMAND_EX(ID_SHOW_HIDDEN, OnTools)
    ON_COMMAND_EX(ID_CALL_HELP, OnTools)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEx_Ctrl diagnostics

#ifdef _DEBUG
void CEx_Ctrl::AssertValid() const
{
	CFormView::AssertValid();
}

void CEx_Ctrl::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CEx_DrawDoc* CEx_Ctrl::GetDocument() // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEx_DrawDoc)));
    return (CEx_DrawDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEx_Ctrl message handlers

void CEx_Ctrl::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
    m_sp_pw.SetBuddy(&m_sel_pw);
    m_sp_pw.SetRange(1, 10);
    m_sp_pw.SetPos(1);

    m_lists.ResetContent();

    m_sel_graph.AddString(_T("0 - 选择模式"));
    m_sel_graph.AddString(_T("1 - 直线"));
    m_sel_graph.AddString(_T("2 - 矩形"));
    m_sel_graph.AddString(_T("3 - 椭圆"));
    m_sel_graph.AddString(_T("4 - 曲线"));
    m_sel_graph.SetCurSel(0);

    int i;
    for (i = 0; i < 6; i++)
    {
        m_sel_ps.AddString(CGraphic::GetPenStyleById(i));
    }
    m_sel_ps.SetCurSel(0);

    for (i = 0; i < 8; i++)
    {
        m_sel_bs.AddString(CGraphic::GetBrushStyleById(i));
    }
    m_sel_bs.SetCurSel(0);

    DisplayRGB();
    OnSelGraph();
}

HBRUSH CEx_Ctrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
    if (m_color_view.m_hWnd == pWnd->m_hWnd)
    {
        m_clrBG.DeleteObject();
        m_clrBG.CreateSolidBrush(GetDocument()->m_GM.m_clrPen);
        return m_clrBG;
    }
    if (m_brush_view.m_hWnd == pWnd->m_hWnd)
    {
        m_brBG.DeleteObject();
        m_brBG.CreateSolidBrush(GetDocument()->m_GM.m_clrBrush);
        return m_brBG;
    }
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CEx_Ctrl::OnSelGraph()
{
	// TODO: Add your control notification handler code here
    int sel = m_sel_graph.GetCurSel();
    UINT type = NONE;
    if (sel > 0)
    {
        type = sel + GRAPH_BASE;
    }

    GetDocument()->m_GM.m_DrawType = type;

    SelGraphicEx(type);
}

void CEx_Ctrl::SelGraphicEx( UINT type )
{
    switch (type)
    {
    case NONE:
        EnablePen(FALSE);
        EnableBrush(FALSE);
        break;
    case LINE:
    case CURVE:
        EnablePen(TRUE);
        EnableBrush(FALSE);
        break;
    case ELLIPSE:
    case RECTANGLE:
        EnablePen(TRUE);
        EnableBrush(TRUE);
        break;
    }
    
    if (GetDocument()->m_sel != -1)
    {
        GetDocument()->UpdateAllViews(NULL, 0x1000);
    }
}

void CEx_Ctrl::OnSelPs()
{
	// TODO: Add your control notification handler code here
    GetDocument()->m_GM.m_iPenStyle = CGraphic::GetIdBySelection(CGraphic::GBS_PEN, m_sel_ps.GetCurSel());
}

void CEx_Ctrl::OnSelBs() 
{
    // TODO: Add your control notification handler code here
    GetDocument()->m_GM.m_iBrushStyle = CGraphic::GetIdBySelection(CGraphic::GBS_BRUSH, m_sel_bs.GetCurSel());
}

void CEx_Ctrl::OnSelPw() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CFormView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
    CString pw;
    if (m_sel_pw.m_hWnd != NULL)
    {
        m_sel_pw.GetWindowText(pw);
        int w = _ttoi(pw.GetBuffer(0));
        if (w > 0 && w <= 10)
        {
            GetDocument()->m_GM.m_iPenWidth = w;
        }
        else
        {
            AfxMessageBox("线长无效！\n范围 1~10");
            CString num;
            num.Format(_T("%d"), GetDocument()->m_GM.m_iPenWidth);
            m_sel_pw.SetWindowText(num);
        }
    }
}

void CEx_Ctrl::OnChangePenColor()
{
    // TODO: Add your control notification handler code here
    CColorDialog dlg(GetDocument()->m_GM.m_clrPen);
    if (dlg.DoModal() == IDOK)
    {
        GetDocument()->m_GM.m_clrPen = dlg.GetColor();
        Invalidate();
        DisplayRGB();
    }
}

void CEx_Ctrl::OnChangeBrushColor() 
{
    // TODO: Add your control notification handler code here
    CColorDialog dlg(GetDocument()->m_GM.m_iBrushStyle);
    if (dlg.DoModal() == IDOK)
    {
        GetDocument()->m_GM.m_clrBrush = dlg.GetColor();
        Invalidate();
        DisplayRGB();
    }
}

void CEx_Ctrl::DisplayRGB()
{
    COLORREF clr = GetDocument()->m_GM.m_clrPen;
    CString rgb;
    rgb.Format(_T("#%02x%02x%02x"), GetRValue(clr), GetGValue(clr), GetBValue(clr));
    m_color_val.SetWindowText(rgb);
    clr = GetDocument()->m_GM.m_clrBrush;
    rgb.Format(_T("#%02x%02x%02x"), GetRValue(clr), GetGValue(clr), GetBValue(clr));
    m_brush_val.SetWindowText(rgb);
}

void CEx_Ctrl::EnablePen( BOOL bEnable )
{
    if (!bEnable)
    {
        m_btn_setColor.EnableWindow(FALSE);
        m_color_view.EnableWindow(FALSE);
        m_color_val.EnableWindow(FALSE);
        m_sel_ps.EnableWindow(FALSE);
        m_sel_pw.EnableWindow(FALSE);
        m_sp_pw.EnableWindow(FALSE);
    }
    else
    {
        m_btn_setColor.EnableWindow(TRUE);
        m_color_view.EnableWindow(TRUE);
        m_color_val.EnableWindow(TRUE);
        m_sel_ps.EnableWindow(TRUE);
        m_sel_pw.EnableWindow(TRUE);
        m_sp_pw.EnableWindow(TRUE);
    }
}

void CEx_Ctrl::EnableBrush( BOOL bEnable )
{
    if (!bEnable)
    {
        m_btn_setBrush.EnableWindow(FALSE);
        m_brush_view.EnableWindow(FALSE);
        m_brush_val.EnableWindow(FALSE);
        m_sel_bs.EnableWindow(FALSE);
    }
    else
    {
        m_btn_setBrush.EnableWindow(TRUE);
        m_brush_view.EnableWindow(TRUE);
        m_brush_val.EnableWindow(TRUE);
        m_sel_bs.EnableWindow(TRUE);
    }
}

BOOL CEx_Ctrl::OnTools(UINT wParam)
{
    if (wParam >= ID_TOOL_SEL && wParam <= ID_TOOL_CURVE)
    {
        m_sel_graph.SetCurSel(wParam - ID_TOOL_SEL);
        OnSelGraph();
        return TRUE;
    }

    if (wParam == ID_SAVE_AS_BITMAP)
    {
        GetDocument()->UpdateAllViews(NULL, 0x3000);
        return TRUE;
    }
    
    if (wParam == ID_SHOW_TAB)
    {
        GetDocument()->UpdateAllViews(NULL, 0x3001);
        return TRUE;
    }
    
    if (wParam == ID_SHOW_HIDDEN)
    {
        GetDocument()->UpdateAllViews(NULL, 0x3002);
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
    if (wParam == ID_CALL_HELP)
    {
        CHelpDlg dlg;
        dlg.DoModal();
        return TRUE;
    }
    
    return FALSE;
}

BOOL CEx_Ctrl::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class

	return CFormView::OnCommand(wParam, lParam);
}

void CEx_Ctrl::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	
    if (lHint == 0x2000)
    {
        m_sel_graph.SetCurSel(GetDocument()->m_GM.m_DrawType);
        OnSelGraph();
    }
    else if (lHint == 0x4000)
    {
        CObArray& arr = GetDocument()->m_obArray;
        CString str;
        CGraphic* P = NULL;
        m_lists.ResetContent();
        for (int i = 0; i < arr.GetSize(); i++)
        {
            P = Convert_To_Graphic(arr[i]);
            str.Format(_T("%d - %s - %s"), i, P->GetName(), P->m_lpszName);
            m_lists.InsertString(0, str);
        }
    }
    else if (lHint == 0x4001)
    {
        int sel = GetDocument()->m_sel;
        if (sel != -1)
        {
            m_lists.SetCurSel(m_lists.GetCount() - sel - 1);
        }
    }
}

void CEx_Ctrl::OnRandomSelGraph() 
{
	// TODO: Add your control notification handler code here
    m_sel_graph.SetCurSel(0);
    OnSelGraph();

    int size = m_lists.GetCount();
    int ID = size - m_lists.GetCurSel() - 1;
    CObArray& arr = GetDocument()->m_obArray;
    ASSERT(ID >= 0 && ID < arr.GetSize());
    
    GetDocument()->m_sel = ID;
    GetDocument()->UpdateAllViews(this, 0x5000);
}

void CEx_Ctrl::OnUpdateOpNext(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetDocument()->m_log.CanDo());
}

void CEx_Ctrl::OnUpdateOpPrev(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    pCmdUI->Enable(GetDocument()->m_log.CanUndo());
}

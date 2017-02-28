// onfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ex_draw.h"
#include "ConfigDlg.h"
#include "Grapic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ConfigDlg dialog


ConfigDlg::ConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(ConfigDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void ConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ConfigDlg)
	DDX_Control(pDX, IDC_SELNAME, m_define_name);
	DDX_Control(pDX, IDC_EDIT_GRAPH, m_edit_type);
    DDX_Control(pDX, IDC_SELBS_, m_sel_bs);
    DDX_Control(pDX, IDC_CHANGECOLOR2_, m_btn_setBrush);
    DDX_Control(pDX, IDC_CHANGECOLOR_, m_btn_setColor);
    DDX_Control(pDX, IDC_COLOR2_, m_brush_view);
    DDX_Control(pDX, IDC_COLOR_V2_, m_brush_val);
    DDX_Control(pDX, IDC_COLOR_V_, m_color_val);
    DDX_Control(pDX, IDC_SELPW_, m_sel_pw);
    DDX_Control(pDX, IDC_SPINPW_, m_sp_pw);
    DDX_Control(pDX, IDC_SELPS_, m_sel_ps);
	DDX_Control(pDX, IDC_COLOR_, m_color_view);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ConfigDlg, CDialog)
	//{{AFX_MSG_MAP(ConfigDlg)
    ON_BN_CLICKED(IDC_CHANGECOLOR_, OnChangePenColor)
    ON_CBN_SELCHANGE(IDC_SELPS_, OnSelPs)
    ON_EN_CHANGE(IDC_SELPW_, OnSelPw)
    ON_BN_CLICKED(IDC_CHANGECOLOR2_, OnChangeBrushColor)
    ON_CBN_SELCHANGE(IDC_SELBS_, OnSelBs)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ConfigDlg message handlers

BOOL ConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_sp_pw.SetBuddy(&m_sel_pw);
    m_sp_pw.SetRange(1, 10);
    m_sp_pw.SetPos(1);

    m_edit_type.SetWindowText(m_typeName);
    m_define_name.SetWindowText(m_GM.m_lpszName);

    CheckDlgButton(IDC_CFG_HIDDEN, m_GM.m_bHidden ? BST_CHECKED : BST_UNCHECKED);

    int i;
    for (i = 0; i < 6; i++)
    {
        m_sel_ps.AddString(CGraphic::GetPenStyleById(i));
    }
    m_sel_ps.SetCurSel(CGraphic::GetSelectionById(CGraphic::GBS_PEN, m_GM.m_iPenStyle));
    
    for (i = 0; i < 8; i++)
    {
        m_sel_bs.AddString(CGraphic::GetBrushStyleById(i));
    }
    m_sel_bs.SetCurSel(CGraphic::GetSelectionById(CGraphic::GBS_BRUSH, m_GM.m_iBrushStyle));
    
    SelGraphicEx(m_GM.m_DrawType);
    DisplayRGB();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH ConfigDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
    if (m_color_view.m_hWnd == pWnd->m_hWnd)
    {
        m_clrBG.DeleteObject();
        m_clrBG.CreateSolidBrush(m_GM.m_clrPen);
        return m_clrBG;
    }
    if (m_brush_view.m_hWnd == pWnd->m_hWnd)
    {
        m_brBG.DeleteObject();
        m_brBG.CreateSolidBrush(m_GM.m_clrBrush);
        return m_brBG;
    }

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void ConfigDlg::SelGraphicEx( UINT type )
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
}


void ConfigDlg::OnSelPs()
{
	// TODO: Add your control notification handler code here
    m_GM.m_iPenStyle = CGraphic::GetIdBySelection(CGraphic::GBS_PEN, m_sel_ps.GetCurSel());
}

void ConfigDlg::OnSelBs() 
{
    // TODO: Add your control notification handler code here
    m_GM.m_iBrushStyle = CGraphic::GetIdBySelection(CGraphic::GBS_BRUSH, m_sel_bs.GetCurSel());
}

void ConfigDlg::OnSelPw() 
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
            m_GM.m_iPenWidth = w;
        }
        else
        {
            AfxMessageBox("线长无效！\n范围 1~10");
            CString num;
            num.Format(_T("%d"), m_GM.m_iPenWidth);
            m_sel_pw.SetWindowText(num);
        }
    }
}

void ConfigDlg::OnChangePenColor()
{
    // TODO: Add your control notification handler code here
    CColorDialog dlg(m_GM.m_clrPen);
    if (dlg.DoModal() == IDOK)
    {
        m_GM.m_clrPen = dlg.GetColor();
        Invalidate();
        DisplayRGB();
    }
}

void ConfigDlg::OnChangeBrushColor() 
{
    // TODO: Add your control notification handler code here
    CColorDialog dlg(m_GM.m_iBrushStyle);
    if (dlg.DoModal() == IDOK)
    {
        m_GM.m_clrBrush = dlg.GetColor();
        Invalidate();
        DisplayRGB();
    }
}

void ConfigDlg::DisplayRGB()
{
    COLORREF clr = m_GM.m_clrPen;
    CString rgb;
    rgb.Format(_T("#%02x%02x%02x"), GetRValue(clr), GetGValue(clr), GetBValue(clr));
    m_color_val.SetWindowText(rgb);
    clr = m_GM.m_clrBrush;
    rgb.Format(_T("#%02x%02x%02x"), GetRValue(clr), GetGValue(clr), GetBValue(clr));
    m_brush_val.SetWindowText(rgb);
}

void ConfigDlg::EnablePen( BOOL bEnable )
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

void ConfigDlg::EnableBrush( BOOL bEnable )
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

void ConfigDlg::OnOK() 
{
	// TODO: Add extra validation here
    CString strDefine;
    m_define_name.GetWindowText(m_GM.m_lpszName);
    m_GM.m_bHidden = IsDlgButtonChecked(IDC_CFG_HIDDEN);    

	CDialog::OnOK();
}

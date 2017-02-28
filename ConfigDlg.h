#if !defined(AFX_ONFIGDLG_H__C22378BD_0598_427A_A11E_36F115B89E02__INCLUDED_)
#define AFX_ONFIGDLG_H__C22378BD_0598_427A_A11E_36F115B89E02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Ex_DrawDoc.h"
// onfigDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ConfigDlg dialog

class ConfigDlg : public CDialog
{
// Construction
public:
	ConfigDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ConfigDlg)
	enum { IDD = IDD_CONFIG };
	CEdit	m_define_name;
	CEdit	m_edit_type;
    CComboBox	m_sel_bs;
    CButton	m_btn_setBrush;
    CButton	m_btn_setColor;
    CStatic	m_brush_view;
    CStatic	m_brush_val;
    CStatic	m_color_val;
    CEdit	m_sel_pw;
    CSpinButtonCtrl	m_sp_pw;
    CComboBox	m_sel_ps;
    CComboBox	m_sel_graph;
    CStatic	m_color_view;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
    
public:
    GraphicMember m_GM;
    CString m_typeName;
        
        // Implementation
protected:
    CBrush m_clrBG;
    CBrush m_brBG;
    
    void DisplayRGB();
    void EnableBrush( BOOL );
    void EnablePen( BOOL );
    void SelGraphicEx( UINT);

	// Generated message map functions
	//{{AFX_MSG(ConfigDlg)
    afx_msg void OnChangePenColor();
    afx_msg void OnSelPs();
    afx_msg void OnSelPw();
    afx_msg void OnChangeBrushColor();
    afx_msg void OnSelBs();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ONFIGDLG_H__C22378BD_0598_427A_A11E_36F115B89E02__INCLUDED_)

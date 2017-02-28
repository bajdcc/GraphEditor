#if !defined(AFX_EX_CTRL_H__D831EBEA_D621_42A7_AAD6_013FE1B307DF__INCLUDED_)
#define AFX_EX_CTRL_H__D831EBEA_D621_42A7_AAD6_013FE1B307DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Ex_Ctrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEx_Ctrl form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "Ex_DrawDoc.h"

class CEx_Ctrl : public CFormView
{
protected:
	CEx_Ctrl();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CEx_Ctrl)

// Form Data
public:
	//{{AFX_DATA(CEx_Ctrl)
	enum { IDD = IDD_EX_CTRL_FORM };
	CListBox	m_lists;
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

// Attributes
public:
    CEx_DrawDoc* GetDocument();

protected:
    CBrush m_clrBG;
    CBrush m_brBG;

    void DisplayRGB();
    void EnableBrush( BOOL );
    void EnablePen( BOOL );
    void SelGraphicEx( UINT);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEx_Ctrl)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CEx_Ctrl();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CEx_Ctrl)
	afx_msg void OnChangePenColor();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSelGraph();
	afx_msg void OnSelPs();
	afx_msg void OnSelPw();
	afx_msg void OnChangeBrushColor();
	afx_msg void OnSelBs();
	afx_msg void OnRandomSelGraph();
	afx_msg void OnUpdateOpNext(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOpPrev(CCmdUI* pCmdUI);
	//}}AFX_MSG
    afx_msg BOOL OnTools(UINT);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#ifndef _DEBUG  // debug version in Ex_DrawView.cpp
inline CEx_DrawDoc* CEx_DrawView::GetDocument()
{ return (CEx_DrawDoc*)m_pDocument; }
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EX_CTRL_H__D831EBEA_D621_42A7_AAD6_013FE1B307DF__INCLUDED_)

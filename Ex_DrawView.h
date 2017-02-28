// Ex_DrawView.h : interface of the CEx_DrawView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_EX_DRAWVIEW_H__2DB4383E_FC9D_4F9A_98CD_4E8799021C54__INCLUDED_)
#define AFX_EX_DRAWVIEW_H__2DB4383E_FC9D_4F9A_98CD_4E8799021C54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Ex_DrawDoc.h"


class CEx_DrawView : public CScrollView
{
protected: // create from serialization only
	CEx_DrawView();
	DECLARE_DYNCREATE(CEx_DrawView)

// Attributes
public:
	CEx_DrawDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEx_DrawView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEx_DrawView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    CSize m_size;
    BOOL m_bSizingMode;
    BOOL m_bLButtonDown;
    BOOL m_bShiftDown;
    BOOL m_bCtrlDown;
    BOOL m_bFirstMove;
    BOOL m_bDup;
    BOOL m_bContextMenu;
    CPoint m_lastPt;
    CDC m_backDC;
    CDC m_bufferDC;
    CBitmap m_backBitmap;
    CBitmap m_bufferBitmap;
    CPen m_pen;

    CGraphic* m_pMoveTarget; // 选中的备份
    CGraphic* m_pLogBuffer; // 修改的备份
    CPoint m_ptCurTarget1, m_ptCurTarget2; // 原始位置
    CPoint m_ptOriginHit; // 相对起始位置
    CPoint m_ptLocalScroll;
    CPoint m_ptCursor;

    CStatusBar* m_pBar;
    CToolBar* m_pToolbar;
    BOOL m_bSetCursor;
    int m_iTrackID;

    CFont m_tabFont;
    LONG* m_sizingX;
    LONG* m_sizingY;

    void DrawGraphic( CDC* pD, int skip = -1 );
    void AdjustWindow();
    void GraphicHitTest( CPoint& pt, BOOL& bResult, int& sel );
    void GraphicDeleteById( int id );
    void GraphicDrawTab( CDC*, CGraphic*, int );

// Generated message map functions
protected:
	//{{AFX_MSG(CEx_DrawView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSaveAsBitmap();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    afx_msg void OnShowTab();
	afx_msg void OnShowHidden();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnCallHelp();
	afx_msg void OnCmdUIPrev(CCmdUI* pCmdUI);
	afx_msg void OnCmdUINext(CCmdUI* pCmdUI);
	//}}AFX_MSG
    afx_msg BOOL OnTools(UINT);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Ex_DrawView.cpp
inline CEx_DrawDoc* CEx_DrawView::GetDocument()
   { return (CEx_DrawDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EX_DRAWVIEW_H__2DB4383E_FC9D_4F9A_98CD_4E8799021C54__INCLUDED_)

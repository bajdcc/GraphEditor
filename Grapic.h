#if !defined(AFX_GRAPIC_H__1DEA71AF_451A_4A0F_8E60_EBAA4164532C__INCLUDED_)
#define AFX_GRAPIC_H__1DEA71AF_451A_4A0F_8E60_EBAA4164532C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxtempl.h>
// Grapic.h : header file
//

#define NONE            0
#define SELECT          NONE
#define GRAPH_BASE      0x1000
#define LINE            0x1001
#define RECTANGLE       0x1002
#define ELLIPSE         0x1003
#define CURVE           0x1004

#define INF_WIDTH 4
#define LOG_MAX_SAVE 5

#define NULL_OBJECT_BASE 0x1000
#define HATCH_BRUSH_BASE NULL_OBJECT_BASE
#define IF_NULL_OBJECT(id) ((id&NULL_OBJECT_BASE)!=0)
#define IF_HATCH_BRUSH(id) (IF_NULL_OBJECT(id))
#define IF_NULL_PEN(id) (IF_NULL_OBJECT(id)&&((id&(~NULL_OBJECT_BASE))==PS_NULL))
#define IF_NULL_BRUSH(id) ((id)==BS_NULL)

#define Convert_To_Graphic(p) ((CGraphic*)(static_cast<CGraphic*>(p)))

class GraphicMember
{
public:
    UINT m_DrawType;
    BOOL m_bHidden;
    CString  m_lpszName;

    COLORREF m_clrPen;
    COLORREF m_clrBrush;
    int m_iPenStyle;
    int m_iBrushStyle;
    int m_iPenWidth;
    CPoint m_pt1, m_pt2;
    CTime m_createTime, m_modifiedTime;
    CArray<CPoint, CPoint&> m_pts;
    
    GraphicMember();
};

class CGraphic : public CObject
{
    DECLARE_SERIAL( CGraphic )
public:
    virtual void Serialize( CArchive &ar );

public:
	CGraphic( UINT type = NONE );
    virtual void UpdateData( GraphicMember* pSrc, BOOL bSave = TRUE );

    virtual void Draw( CDC* pDC );
    virtual void DrawSelectedEdge( CDC* pDC );
    virtual void HitTest( CPoint& pt, BOOL& bResult );
    virtual LPCTSTR HitSizingTest( CPoint& pt, BOOL& bResult, LONG** PtX = NULL, LONG** PtY = NULL );
    virtual void GetRect( CRect& rt );
    virtual LPCTSTR GetName() const;
    virtual int GetPts() const;
    virtual BOOL EnableBrush() const;

public:
    enum _GBS { GBS_PEN = 0x1, GBS_BRUSH };

    static CGraphic* CreateGraphic( GraphicMember* );
    static void GraphicDrawSelectedEdge( CDC* pDC, CPoint& pt, int& inflate );
    static int GetIdBySelection( _GBS SelectType, int ID );
    static int GetSelectionById( _GBS SelectType, int sel );
    static LPCTSTR GetPenStyleById( int ID, BOOL bConvert = TRUE );
    static LPCTSTR GetBrushStyleById( int ID, BOOL bConvert = TRUE );
    static void CreateGdiObjectFromId( _GBS GdiType, int ID, CGdiObject* object, int width, int color );
    static void GraphicHitSizingTest( LONG& x, LONG& y, int inf, CPoint& pt, BOOL& bResult,
        LONG** X = NULL, LONG** Y = NULL );

protected:
    static LONG DotsLengthSquare( CPoint& p1, CPoint& p2 );
    static void LineHitTest( CPoint& p1, CPoint& p2, CPoint& p3, BOOL& bResult );
    BOOL PtInRectTest( CPoint& pt );

public:
    UINT    m_DrawType;
    BOOL    m_bHidden;
    CString m_lpszName;
    CPoint  m_pt1, m_pt2;
    CTime   m_createTime, m_modifiedTime;
};

//////////////////////////////////////////////////////////////////////////

class CGraphicLog
{
public:
    CGraphicLog( CObArray* arr );
    ~CGraphicLog();

    enum { MAX_SAVE = LOG_MAX_SAVE };
    enum GOS
    {
        GOS_NONE,
        GOS_ADD,
        GOS_DELETE,
        GOS_UPDATE,
    };

public:
    void Clear();
    BOOL CanUndo() const;
    BOOL CanDo() const;
    void Undo(); // 撤消纪录
    void Done(); // 恢复纪录
    void Operator( GOS, CGraphic*, int, BOOL bClear = TRUE); // 添加操作纪录
    void DoneOper( GOS, CGraphic*, int ); // 添加恢复纪录

    BOOL Add( CGraphic* pOb ); // 添加数据
    BOOL Add( CGraphic* pOb, int ID ); // 添加数据

protected:
    void ClearDone();
    void ClearUndo();
    void ClearArray();
    void Delete( CGraphic* pOb );
    BOOL AddRef( CGraphic* pOb );

public:
    typedef struct GraphicOperation
    {
        GOS         oper;
        CGraphic*   pGraphic;
        int         index;

        CString Trace();
    } _GO ;

    CList<_GO, _GO&>    m_listDone;
    CList<_GO, _GO&>    m_listUndo;
    int                 m_dones;
    int                 m_undos;
    CObArray*           m_parr;
    CMap<CGraphic*, CGraphic*&, int, int&> m_refs; // 引用表
};

//////////////////////////////////////////////////////////////////////////

class CLine : public CGraphic
{
    DECLARE_SERIAL( CLine )
public:
    CLine();
    virtual void UpdateData( GraphicMember* pSrc, BOOL bSave = TRUE );

    virtual LPCTSTR GetName() const;
    virtual void GetRect( CRect& rt );
    virtual void Serialize( CArchive &ar );    
    virtual LPCTSTR HitSizingTest( CPoint& pt, BOOL& bResult, LONG** PtX = NULL, LONG** PtY = NULL );
    virtual void Draw( CDC* pDC );    
    virtual void DrawSelectedEdge( CDC* pDC );    
    virtual void HitTest( CPoint& pt, BOOL& bResult );  
    virtual BOOL EnableBrush() const;

public:
    COLORREF m_clrPen;
    int m_iPenStyle;
    int m_iPenWidth;
};

class CRectangle : public CGraphic
{
    DECLARE_SERIAL( CRectangle )
public:
    CRectangle();
    virtual void UpdateData( GraphicMember* pSrc, BOOL bSave = TRUE );
    
    virtual LPCTSTR GetName() const;
    virtual void GetRect( CRect& rt );
    virtual void Serialize( CArchive &ar );    
    virtual LPCTSTR HitSizingTest( CPoint& pt, BOOL& bResult, LONG** PtX = NULL, LONG** PtY = NULL );
    virtual void Draw( CDC* pDC );    
    virtual void DrawSelectedEdge( CDC* pDC );
    virtual void HitTest( CPoint& pt, BOOL& bResult );

public:
    COLORREF m_clrPen;
    COLORREF m_clrBrush;
    int m_iPenStyle;
    int m_iBrushStyle;
    int m_iPenWidth;
};


class CEllipse : public CGraphic
{
    DECLARE_SERIAL( CEllipse )
public:
    CEllipse();
    virtual void UpdateData( GraphicMember* pSrc, BOOL bSave = TRUE );
    
    virtual LPCTSTR GetName() const;
    virtual void GetRect( CRect& rt );
    virtual void Serialize( CArchive &ar );    
    virtual LPCTSTR HitSizingTest( CPoint& pt, BOOL& bResult, LONG** PtX = NULL, LONG** PtY = NULL );
    virtual void Draw( CDC* pDC );    
    virtual void DrawSelectedEdge( CDC* pDC );
    virtual void HitTest( CPoint& pt, BOOL& bResult );
    
public:
    COLORREF m_clrPen;
    COLORREF m_clrBrush;
    int m_iPenStyle;
    int m_iBrushStyle;
    int m_iPenWidth;
};


class CCurve : public CGraphic
{
    DECLARE_SERIAL( CCurve )
public:
    CCurve();
    virtual void UpdateData( GraphicMember* pSrc, BOOL bSave = TRUE );
        
    virtual LPCTSTR GetName() const;
    virtual void Draw( CDC* pDC );    
    virtual void Serialize( CArchive &ar );    
    virtual void DrawSelectedEdge( CDC* pDC );
    virtual void HitTest( CPoint& pt, BOOL& bResult );  
    virtual void GetRect( CRect& rt );
    virtual int GetPts() const;
    virtual BOOL EnableBrush() const;

public:
    COLORREF m_clrPen;
    int m_iPenStyle;
    int m_iPenWidth;
    CArray<CPoint, CPoint&>  m_pts;
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_GRAPIC_H__1DEA71AF_451A_4A0F_8E60_EBAA4164532C__INCLUDED_)

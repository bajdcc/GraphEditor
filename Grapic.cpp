// Grapic.cpp : implementation file
//

#include "stdafx.h"
#include "Ex_Draw.h"
#include "Grapic.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 选择误差
#define SELECT_MISS_LINE 1.005
#define SELECT_MISS_ELLIPSE 0.2f
#define DRAG_INF_WIDTH 8

GraphicMember::GraphicMember(): m_DrawType(SELECT), m_clrPen(RGB(0, 0, 0)), m_clrBrush(RGB(128, 128, 128)),
m_iPenStyle(PS_SOLID), m_iBrushStyle(BS_SOLID), m_iPenWidth(1), m_bHidden(FALSE) {}

/////////////////////////////////////////////////////////////////////////////
// CGrapic

/*
    添加新图形类的步骤：

  1. 定义ID
  2. 继承自CGraphic
  3. 在CreateGraphic中添加
  4. 重写Draw方法
  5. 重写序列化方法(*)
  6. 添加按钮
  7. 改写鼠标事件(*)

  */

CGraphic::CGraphic(UINT type /*= NONE*/): m_DrawType(type)
{
    m_bHidden = FALSE;
    m_createTime = CTime::GetCurrentTime();
    m_modifiedTime = CTime::GetCurrentTime();
}

void CGraphic::Draw( CDC* pDC )
{

}

IMPLEMENT_SERIAL( CGraphic, CObject, 1 )
void CGraphic::Serialize( CArchive &ar )
{
    if ( ar.IsStoring() )
    {
        ar << m_DrawType << m_bHidden << m_lpszName << m_pt1 << m_pt2 << m_createTime << m_modifiedTime;
    }
    else
    {
        ar >> m_DrawType >> m_bHidden >> m_lpszName >> m_pt1 >> m_pt2 >> m_createTime >> m_modifiedTime;
	}
}

LPCTSTR CGraphic::GetName() const
{
    return _T("图形基类");
}

void CGraphic::DrawSelectedEdge( CDC* pDC )
{
    
}

void CGraphic::HitTest( CPoint& pt, BOOL& bResult )
{
    
}

void CGraphic::LineHitTest( CPoint& a, CPoint& b, CPoint& c, BOOL& bResult )
{
    static double A, B, C;
    A = DotsLengthSquare(b, c);
    B = DotsLengthSquare(a, c);
    C = DotsLengthSquare(a, b);
    double z = ( 1.0 * (A + B + 2 * sqrt(A * B)) / C);
    bResult = ( z >= 0.99 && z < SELECT_MISS_LINE ) ? TRUE : FALSE;
}

CGraphic* CGraphic::CreateGraphic( GraphicMember* pSrc )
{
    ASSERT(pSrc);
    CGraphic* pRet = NULL;
    switch (pSrc->m_DrawType)
    {
    case LINE:          pRet = new CLine;       break;
    case RECTANGLE:     pRet = new CRectangle;  break;
    case ELLIPSE:       pRet = new CEllipse;    break;
    case CURVE:         pRet = new CCurve;      break;
    default: return NULL;
    }
    pRet->UpdateData(pSrc);
    return pRet;
}

void CGraphic::GraphicDrawSelectedEdge( CDC* pDC, CPoint& pt, int& inflate )
{
    static CRect rt;
    rt.SetRect(pt, pt);
    rt.InflateRect(inflate, inflate);
    pDC->Ellipse(&rt);
}

LONG CGraphic::DotsLengthSquare( CPoint& p1, CPoint& p2 )
{
    return (p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y);
}

int CGraphic::GetIdBySelection( _GBS SelectType, int ID )
{
    switch (SelectType)
    {
    case GBS_PEN: 
        {
            switch (ID)
            {
            case 0:     return PS_SOLID;
            case 1:     return NULL_OBJECT_BASE + PS_NULL;
            case 2:     return PS_DASH;
            case 3:     return PS_DOT;
            case 4:     return PS_DASHDOT;
            case 5:     return PS_DASHDOTDOT;
            }
            break;
            
        } 
    case GBS_BRUSH:
        {
            switch (ID)
            {
            case 0:     return BS_SOLID;
            case 1:     return BS_NULL;
            case 2:     return HATCH_BRUSH_BASE + HS_HORIZONTAL;
            case 3:     return HATCH_BRUSH_BASE + HS_VERTICAL;
            case 4:     return HATCH_BRUSH_BASE + HS_FDIAGONAL;
            case 5:     return HATCH_BRUSH_BASE + HS_BDIAGONAL;
            case 6:     return HATCH_BRUSH_BASE + HS_CROSS;
            case 7:     return HATCH_BRUSH_BASE + HS_DIAGCROSS;
            }
            break;
        }
    }
    return -1;
}

int CGraphic::GetSelectionById( _GBS SelectType, int sel )
{
    switch (SelectType)
    {
    case GBS_PEN: 
        {
            switch (sel)
            {
            case PS_SOLID:          return 0;
            case NULL_OBJECT_BASE + PS_NULL: return 1;
            case PS_DASH:           return 2;
            case PS_DOT:            return 3;
            case PS_DASHDOT:        return 4;
            case PS_DASHDOTDOT:     return 5;
            }
            break;
        }
    case GBS_BRUSH:
        {
            switch (sel)
            {
            case BS_SOLID:  return 0;
            case BS_NULL:   return 1;
            case HATCH_BRUSH_BASE + HS_HORIZONTAL:  return 2;
            case HATCH_BRUSH_BASE + HS_VERTICAL:    return 3;
            case HATCH_BRUSH_BASE + HS_FDIAGONAL:   return 4;
            case HATCH_BRUSH_BASE + HS_BDIAGONAL:   return 5;
            case HATCH_BRUSH_BASE + HS_CROSS:       return 6;
            case HATCH_BRUSH_BASE + HS_DIAGCROSS:   return 7;
            }
            break;
        }
    }
    return -1;
}

LPCTSTR CGraphic::GetPenStyleById( int ID, BOOL bConvert /*= TRUE*/ )
{
    int id = ID;
    if (bConvert)
    {
        id = GetIdBySelection(GBS_PEN, ID);
    }
    if (IF_NULL_OBJECT(id))
    {
        id &= ~NULL_OBJECT_BASE;
        if (id == PS_NULL)      return _T("透明");
    }
    else
    {
        switch (id)
        {
        case PS_SOLID:          return _T("实线");
        case PS_DASH:           return _T("虚线 (-------)");
        case PS_DOT:            return _T("点   (.......)");
        case PS_DASHDOT:        return _T("虚点1(_._._._)");
        case PS_DASHDOTDOT:     return _T("虚点2(_.._.._)");
        }
    }
    ASSERT(!"PS> ID falut!"); return NULL;
}

LPCTSTR CGraphic::GetBrushStyleById( int ID, BOOL bConvert /*= TRUE*/ )
{
    int id = ID;
    if (bConvert)
    {
        id = GetIdBySelection(GBS_BRUSH, ID);
    }
    if (!IF_HATCH_BRUSH(id))
    {
        if (id == BS_NULL)      return _T("透明");
        if (id == BS_SOLID)     return _T("实心");
    }
    else
    {
        id &= ~HATCH_BRUSH_BASE;
        switch (id)
        {
        case HS_HORIZONTAL:     return _T("水平线 (-------)");
        case HS_VERTICAL:       return _T("垂直线 (|||||||)");
        case HS_FDIAGONAL:      return _T("右斜线 (\\\\\\\\\\\\\\)");
        case HS_BDIAGONAL:      return _T("左斜线 (///////)");
        case HS_CROSS:          return _T("网格线 (+++++++)");
        case HS_DIAGCROSS:      return _T("斜纹线 (xxxxxxx)");
        }
    }
    ASSERT(!"BS> ID falut!"); return NULL;
}

BOOL CGraphic::PtInRectTest( CPoint& pt )
{
    CRect rt;
    rt.SetRect(m_pt1, m_pt2);
    rt.NormalizeRect();
    return rt.PtInRect(pt);
}

void CGraphic::GetRect( CRect& rt )
{

}

int CGraphic::GetPts() const
{
    return 2;
}

BOOL CGraphic::EnableBrush() const
{
    return TRUE;
}

void CGraphic::UpdateData( GraphicMember* pSrc, BOOL bSave /*= TRUE */ )
{
    ASSERT(pSrc);
    if (bSave)
    {
        m_DrawType = pSrc->m_DrawType;
        m_pt1 = pSrc->m_pt1;
        m_pt2 = pSrc->m_pt2;
        m_modifiedTime = pSrc->m_modifiedTime;
        m_bHidden = pSrc->m_bHidden;
        m_lpszName = pSrc->m_lpszName;
    }
    else
    {
        pSrc->m_DrawType = m_DrawType;
        pSrc->m_pt1 = m_pt1;
        pSrc->m_pt2 = m_pt2;
        pSrc->m_modifiedTime = m_modifiedTime;
        pSrc->m_createTime = m_createTime;
        pSrc->m_bHidden = m_bHidden;
        pSrc->m_lpszName = m_lpszName;
    }
}

void CGraphic::CreateGdiObjectFromId( _GBS GdiType, int ID, CGdiObject* object, int width, int color )
{
    ASSERT_VALID(object);
    int id = ID;
    switch (GdiType)
    {
    case GBS_PEN:
        ASSERT(object->IsKindOf(RUNTIME_CLASS(CPen)));
        if (IF_NULL_OBJECT(id))
        {
            id &= ~NULL_OBJECT_BASE;
            if (id == PS_NULL)      { static_cast<CPen*>(object)->Attach(GetStockObject(NULL_PEN)); return; }
        }
        else
        {
            switch (id)
            {
            case PS_SOLID:          
            case PS_DASH:
            case PS_DOT:
            case PS_DASHDOT:
            case PS_DASHDOTDOT:
                static_cast<CPen*>(object)->CreatePen(id, width, color); return;
            }
        } 
        break;
    case GBS_BRUSH:
        ASSERT(object->IsKindOf(RUNTIME_CLASS(CBrush)));
        if (!IF_HATCH_BRUSH(id))
        {
            if (id == BS_NULL)      { static_cast<CBrush*>(object)->Attach(GetStockObject(NULL_BRUSH)); return; }
            if (id == BS_SOLID)     { static_cast<CBrush*>(object)->CreateSolidBrush(color); return; }
        }
        else
        {
            id &= ~HATCH_BRUSH_BASE;
            switch (id)
            {
            case HS_HORIZONTAL:
            case HS_VERTICAL:
            case HS_FDIAGONAL:
            case HS_BDIAGONAL:
            case HS_CROSS:
            case HS_DIAGCROSS:
                static_cast<CBrush*>(object)->CreateHatchBrush(id, color); return;
            }
        }
        break;
    }
    ASSERT(!"CreateGdiObjectFromId Error");
}

LPCTSTR CGraphic::HitSizingTest( CPoint& pt, BOOL& bResult, LONG** PtX /*= NULL*/, LONG** PtY /*= NULL */ )
{
    return NULL;
}

void CGraphic::GraphicHitSizingTest( LONG& x, LONG& y, int inf, CPoint& pt, BOOL& bResult,
    LONG** X /*= NULL*/, LONG** Y /*= NULL*/ )
{
    static CRect rt;
    rt.SetRect(x, y, x, y);
    rt.InflateRect(inf, inf);
    if (rt.PtInRect(pt))
    {
        if (X) *X = &x;
        if (Y) *Y = &y;
        bResult = TRUE;
    }
}


CGraphicLog::CGraphicLog( CObArray* arr ): m_dones(0), m_undos(0)
{
    ASSERT_VALID(arr);
    m_parr = arr;
}

CGraphicLog::~CGraphicLog()
{
    Clear();
}

CString CGraphicLog::GraphicOperation::Trace()
{
    CString str;
    LPCTSTR OP;
    switch (oper)
    {
    case GOS_ADD: OP = _T("ADD"); break;
    case GOS_DELETE: OP = _T("DELETE"); break;
    case GOS_UPDATE: OP = _T("UPDATE"); break;
    default: OP = _T("UNKNOWN");
    }
    str.Format(_T("[%s] - [%p] - [%s] - [%s] - [%d]"), OP, pGraphic, pGraphic->GetName(), pGraphic->m_lpszName, index);
    return str;
}

BOOL CGraphicLog::CanUndo() const
{
    // 可以撤消吗
    return ( m_undos == 0 ) ? FALSE : TRUE;
}

BOOL CGraphicLog::CanDo() const
{
    // 可以恢复吗
    return ( m_dones == 0 ) ? FALSE : TRUE;
}

void CGraphicLog::Undo()
{
    // * * * 这里会修改引用计数和操作对象数组 * * *

    if (m_undos == 0)
    {
        return;
    }
    TRACE("LOG UNDO ------\n");
    m_undos--;
    _GO go = m_listUndo.GetHead();
    CGraphic* pOb = NULL;
    switch (go.oper)
    {
    case GOS_ADD:
        // 撤消添加的，所以为删除操作
        // 将其从图像数组中移除，引用计数减一
        m_parr->RemoveAt(go.index);

        // 撤消列表中本操作记录删除（用完了删除），引用计数减一
        // 这时要保存恢复操作，要恢复撤消添加
        // 所以在listDone里要保存添加操作，引用计数加一        
        // 总之引用计数减一
        Delete(go.pGraphic);
        DoneOper(GOS_ADD, go.pGraphic, go.index);
        break;
    case GOS_DELETE:
        // 撤消删除的，所以为添加操作
        // 将其移动到图像数组中相应位置，引用计数不变
        m_parr->InsertAt(go.index, go.pGraphic);

        // 撤消之前的对象要保存（移动）到恢复列表中，引用计数不变
        // 对象恢复到原始数组，引用计数加一
        AddRef(go.pGraphic);
        DoneOper(GOS_DELETE, go.pGraphic, go.index);
        break;
    case GOS_UPDATE:
        // 撤消更改，现数组中对象要恢复成撤消之前的
        pOb = Convert_To_Graphic(m_parr->GetAt(go.index));
        m_parr->ElementAt(go.index) = go.pGraphic;

        // 所以原对象被保存（移动）进恢复列表，引用计数不变
        // 新对象从撤消操作记录列表中移动进对象数组，引用计数不变
        // 总之引用计数不变
        DoneOper(GOS_UPDATE, pOb, go.index);
        break;
    default: ASSERT(!"operation fault!");
    }
    m_listUndo.RemoveHead();
}

void CGraphicLog::DoneOper( GOS oper, CGraphic* p, int index )
{
    // 在这里不修改引用计数

    if (m_dones == MAX_SAVE)
    {
        ASSERT(!m_listDone.IsEmpty());
        Delete(m_listDone.GetTail().pGraphic);
        m_listDone.RemoveTail();
    }
    else
    {
        m_dones++;
    }
    _GO go;
    go.index = index;
    go.oper = oper;
    go.pGraphic = p;

    TRACE("LOG DONE %s / UN: %d DN: %d REF: %d\n", go.Trace(), m_undos, m_dones, m_refs[go.pGraphic]);
    m_listDone.AddHead(go);
}

void CGraphicLog::Done()
{
    // * * * 这里会修改引用计数和操作对象数组 * * *

    // 恢复操作遵循oper指令

    if (m_dones == 0)
    {
        return;
    }
    TRACE("LOG DONE ------\n");
    m_dones--;
    _GO go = m_listDone.GetHead();
    CGraphic* pOb = NULL;
    switch (go.oper)
    {
    case GOS_ADD:
        // 添加操作
        m_parr->InsertAt(go.index, go.pGraphic);
        
        // 从保存列表移动至目标数组，引用计数不变
        // 添加撤消操作，引用计数加一
        // 总之引用计数加一
        AddRef(go.pGraphic);
        Operator(GOS_ADD, go.pGraphic, go.index, FALSE);
        break;
    case GOS_DELETE:
        // 删除操作
        m_parr->RemoveAt(go.index);

        // 原数组中其被删除，恢复列表删除，引用计数减二
        // 唯一保存在撤消列表中，引用计数加一
        // 总之引用计数减一
        Delete(go.pGraphic);
        Operator(GOS_DELETE, go.pGraphic, go.index, FALSE);
        break;
    case GOS_UPDATE:
        // 更改操作
        pOb = Convert_To_Graphic(m_parr->GetAt(go.index));
        m_parr->ElementAt(go.index) = go.pGraphic;

        // go.pGraphic 恢复列表->目标数组，引用计数不变
        // pOb 目标数组->恢复列表，引用计数不变
        Operator(GOS_UPDATE, pOb, go.index, FALSE);
        break;
    default: ASSERT(!"operation fault!");
    }
    m_listDone.RemoveHead();
}

void CGraphicLog::Operator( GOS oper, CGraphic* p, int index, BOOL bClear /*= TRUE*/ )
{
    ASSERT_VALID(p);

    // 每次操作之后，记忆的恢复操作应该全部清除
    // 使用者操作时，参数bClear为真
    // 撤消操作时，bClear为假
    if (bClear) ClearDone();
    
    if (bClear)
    {
        // * * * 这里会修改引用计数和操作对象数组 * * *

        // 凡是将对象从m_obArray（*m_parr）移出至（listUndo），那么不增加引用
        switch (oper)
        {
        case GOS_ADD:
            // 使用本类的Add(CGraphic*)添加对象并初始化引用计数
            ASSERT(!Add(p));
            // 因为撤消列表里要保存添加操作，所以引用计数加一
            AddRef(p);
            // 这样引用计数为二
            break;
        case GOS_DELETE:
            // 将其从原数组中移除（不是删除）
            m_parr->RemoveAt(index);
            break;
        case GOS_UPDATE:
            // 更改操作，这时要保存原对象（更改前的）
            // 但是修改后的对象是最新创建的，没有引用计数
            // 所以还得初始化引用计数
            // 此时p为新建备份
            ASSERT(!AddRef(p));
            break;
        default: ASSERT(!"Operation fault!");
        }
    }

    if (m_undos == MAX_SAVE)
    {
        // 如果撤消列表已经满，自动删除列尾
        ASSERT(!m_listUndo.IsEmpty());
        Delete(m_listUndo.GetTail().pGraphic);
        m_listUndo.RemoveTail();
    }
    else
    {
        m_undos++;
    }
    _GO go;
    go.index = index;
    go.oper = oper;
    go.pGraphic = p;
    TRACE("LOG OPER %d %s / UN: %d DN: %d REF: %d\n", bClear, go.Trace(), m_undos, m_dones, m_refs[go.pGraphic]);

    // 添加撤消记录
    m_listUndo.AddHead(go);
}

void CGraphicLog::ClearDone()
{
    if (!CanDo()) return;
    TRACE("$ Clear DONE start - - >>>>>>\n");
    POSITION pos = m_listDone.GetHeadPosition();
    _GO go;
    while (pos)
    {
        go = m_listDone.GetNext(pos);
        TRACE("LOG CLEAR DONE DELETE %s / UN: %d DN: %d REF: %d\n", go.Trace(), m_undos, m_dones, m_refs[go.pGraphic] - 1);
        Delete(go.pGraphic);
    }
    m_listDone.RemoveAll();
    m_dones = 0;
    TRACE("$ Clear DONE end - - <<<<<<\n");
}

void CGraphicLog::ClearUndo()
{
    if (!CanUndo()) return;
    TRACE("$ Clear UNDO start - - >>>>>>\n");
    POSITION pos = m_listUndo.GetHeadPosition();
    _GO go;
    while (pos)
    {
        go = m_listUndo.GetNext(pos);
        TRACE("LOG CLEAR DONE DELETE %s / UN: %d DN: %d REF: %d\n", go.Trace(), m_undos, m_dones, m_refs[go.pGraphic] - 1);
        Delete(go.pGraphic);
    }
    m_listUndo.RemoveAll();
    m_undos = 0;
    TRACE("$ Clear UNDO end- - <<<<<<\n");
}

void CGraphicLog::ClearArray()
{
    TRACE("$ Clear GRAPHIC Object Array start - - >>>>>>\n");
    POSITION pos = m_refs.GetStartPosition();
    CGraphic* pOb; int ref;
    while (pos)
    {
        m_refs.GetNextAssoc(pos, pOb, ref);
        TRACE("LOG CLEAR ARRAY - %p - [%s] - [%s] - REF: %d\n", pOb, pOb->GetName(), pOb->m_lpszName, ref - 1);
        Delete(pOb);
    }
    TRACE("$ Clear GRAPHIC Object Array end - - <<<<<<\n");
}

void CGraphicLog::Clear()
{
    TRACE("$ Clear LOG start - - >>>>\n");
    // 清除恢复操作
    ClearDone();
    // 清除撤消操作
    ClearUndo();
    // 这时，引用表中应该都是仅有的引用计数为一的对象
    // 如果Trace都是0，那么表明没有问题
    ClearArray();
    TRACE("$ Clear LOG end - - <<<<\n");
}

void CGraphicLog::Delete( CGraphic* pOb )
{
    // 删除操作，当且仅当引用计数为1时（无其他引用）删除
    ASSERT_VALID(pOb);
    int ref;
    if (m_refs.Lookup(pOb, ref))
    {
        ASSERT(ref >= 1);
        if (ref == 1)
        {
			for (int i = 0; i < m_parr->GetSize(); i++)
			{
				if (m_parr->GetAt(i) == (CObject*)pOb)
				{
					TRACE("Graphic Delete ID: %d, ADDR: %p In Main Array\n", i, pOb);
					m_parr->RemoveAt(i);
					break;
				}
			}
            delete pOb;
            m_refs.RemoveKey(pOb);
            return;
        }
        m_refs[pOb] = ref - 1;
    }
    else
    {
        ASSERT(!"Object not found!");
    }
}

BOOL CGraphicLog::AddRef( CGraphic* pOb )
{
    // 增加引用计数
    ASSERT_VALID(pOb);
    int ref;
    if (m_refs.Lookup(pOb, ref))
    {
        m_refs[pOb] = ref + 1;
        return TRUE;
    }
    else
    {
        m_refs[pOb] = 1;
        return FALSE;
    }

    // 假如是初始化引用计数，那么返回FALSE
}

BOOL CGraphicLog::Add( CGraphic* pOb )
{
    // 新建对象后的必须操作
    // 向数组中新增对象
    // 初始化引用计数
    ASSERT_VALID(pOb);
    m_parr->Add(pOb);
    return AddRef(pOb);
}

BOOL CGraphicLog::Add( CGraphic* pOb, int ID )
{
    // 只在序列化读取时，将所有图形的引用计数初始化为1
    // m_parr之前必须调用SetSize（这样快）
    ASSERT_VALID(pOb);
    m_parr->ElementAt(ID) = pOb;
    return AddRef(pOb);
}

IMPLEMENT_SERIAL( CLine, CGraphic, 1 )
CLine::CLine(): CGraphic(LINE)
{
    
}

LPCTSTR CLine::GetName() const
{
    return _T("直线");
}

void CLine::Draw( CDC* pDC )
{
    ASSERT_VALID(pDC);
    CPen pen;
    CreateGdiObjectFromId(GBS_PEN, m_iPenStyle, &pen, m_iPenWidth, m_clrPen);
    pDC->SelectObject(&pen);
    pDC->MoveTo(m_pt1);
    pDC->LineTo(m_pt2);
}

void CLine::DrawSelectedEdge( CDC* pDC )
{
    ASSERT_VALID(pDC);
    CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
    pDC->SelectObject(&pen);
    CRect rt;
    int inf = INF_WIDTH + m_iPenWidth;
    GraphicDrawSelectedEdge(pDC, m_pt1, inf);
    GraphicDrawSelectedEdge(pDC, m_pt2, inf);
}

void CLine::HitTest( CPoint& pt, BOOL& bResult )
{
    if (!PtInRectTest(pt)) return;
    LineHitTest(m_pt1, m_pt2, pt, bResult);
}

BOOL CLine::EnableBrush() const
{
    return FALSE;
}

void CLine::GetRect( CRect& rt )
{
    rt.SetRect(m_pt1, m_pt2);
    rt.NormalizeRect();
    LONG inf = m_iPenWidth + INF_WIDTH + 1;
    rt.InflateRect(inf, inf);    
}

void CLine::UpdateData( GraphicMember* pSrc, BOOL bSave /*= TRUE */ )
{
    CGraphic::UpdateData(pSrc, bSave);
    if (bSave)
    {
        m_clrPen = pSrc->m_clrPen;
        m_iPenStyle = pSrc->m_iPenStyle;
        m_iPenWidth = pSrc->m_iPenWidth;
    }
    else
    {
        pSrc->m_clrPen = m_clrPen;
        pSrc->m_iPenStyle = m_iPenStyle;
        pSrc->m_iPenWidth = m_iPenWidth;
    }
}

void CLine::Serialize( CArchive &ar )
{
    CGraphic::Serialize(ar);
    if (ar.IsStoring())
    {
        ar << m_clrPen << m_iPenStyle << m_iPenWidth;
    }
    else
    {
        ar >> m_clrPen >> m_iPenStyle >> m_iPenWidth;
    }
}

LPCTSTR CLine::HitSizingTest( CPoint& pt, BOOL& bResult, LONG** PtX /*= NULL*/, LONG** PtY /*= NULL */ )
{
    int inf = DRAG_INF_WIDTH + m_iPenWidth;

    GraphicHitSizingTest(m_pt1.x, m_pt1.y, inf, pt, bResult, PtX, PtY);
    if (bResult) return MAKEINTRESOURCE(IDC_SIZEALL);
    GraphicHitSizingTest(m_pt2.x, m_pt2.y, inf, pt, bResult, PtX, PtY);
    if (bResult) return MAKEINTRESOURCE(IDC_SIZEALL);

    return NULL;
}

IMPLEMENT_SERIAL( CRectangle, CGraphic, 1 )
CRectangle::CRectangle(): CGraphic(RECTANGLE)
{
    
}

LPCTSTR CRectangle::GetName() const
{
    return _T("矩形");
}

void CRectangle::Draw( CDC* pDC )
{
    ASSERT_VALID(pDC);
    CGraphic::Draw(pDC);
    CPen pen;
    CreateGdiObjectFromId(GBS_PEN, m_iPenStyle, &pen, m_iPenWidth, m_clrPen);
    pDC->SelectObject(&pen);
    CBrush br;
    CreateGdiObjectFromId(GBS_BRUSH, m_iBrushStyle, &br, 0, m_clrBrush);
    pDC->SelectObject(&br);
    pDC->Rectangle(m_pt1.x, m_pt1.y, m_pt2.x, m_pt2.y);
}

void CRectangle::DrawSelectedEdge( CDC* pDC )
{
    ASSERT_VALID(pDC);
    CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
    CRect rt;
    int inf = INF_WIDTH + m_iPenWidth;
    pDC->SelectObject(&pen);
    CPoint pt;
    CPoint ct;
    ct.x = ( m_pt1.x + m_pt2.x ) / 2;
    ct.y = ( m_pt1.y + m_pt2.y ) / 2;

    GraphicDrawSelectedEdge(pDC, m_pt1, inf);
    GraphicDrawSelectedEdge(pDC, m_pt2, inf);
    pt.x = m_pt2.x; pt.y = m_pt1.y;
    GraphicDrawSelectedEdge(pDC, pt, inf);
    pt.x = m_pt1.x; pt.y = m_pt2.y;
    GraphicDrawSelectedEdge(pDC, pt, inf);

    if (m_pt2.x - m_pt1.x < inf + INF_WIDTH || m_pt2.y - m_pt1.y < inf + INF_WIDTH)
    {
        return;
    }
    
    pt.x = ct.x; pt.y = m_pt1.y;
    GraphicDrawSelectedEdge(pDC, pt, inf);
    pt.x = ct.x; pt.y = m_pt2.y;
    GraphicDrawSelectedEdge(pDC, pt, inf);
    pt.x = m_pt1.x; pt.y = ct.y;
    GraphicDrawSelectedEdge(pDC, pt, inf);
    pt.x = m_pt2.x; pt.y = ct.y;
    GraphicDrawSelectedEdge(pDC, pt, inf);
}

void CRectangle::HitTest( CPoint& pt, BOOL& bResult )
{
    if (!PtInRectTest(pt)) return;

    static CPoint pt1, pt2;
    pt1.x = m_pt1.x; pt1.y = m_pt2.y; pt2.x = m_pt2.x; pt2.y = m_pt1.y;

    LineHitTest(m_pt1, pt1, pt, bResult); if (bResult) return;
    LineHitTest(m_pt1, pt2, pt, bResult); if (bResult) return;
    LineHitTest(m_pt2, pt1, pt, bResult); if (bResult) return;
    LineHitTest(m_pt2, pt2, pt, bResult);
}

void CRectangle::GetRect( CRect& rt )
{
    rt.SetRect(m_pt1, m_pt2);
    rt.NormalizeRect();
    LONG inf = m_iPenWidth + INF_WIDTH + 1;
    rt.InflateRect(inf, inf);    
}

void CRectangle::UpdateData( GraphicMember* pSrc, BOOL bSave /*= TRUE */ )
{
    CGraphic::UpdateData(pSrc, bSave);
    if (bSave)
    {
        m_clrPen = pSrc->m_clrPen;
        m_iPenStyle = pSrc->m_iPenStyle;
        m_iPenWidth = pSrc->m_iPenWidth;
        m_clrBrush = pSrc->m_clrBrush;
        m_iBrushStyle = pSrc->m_iBrushStyle;
    }
    else
    {
        pSrc->m_clrPen = m_clrPen;
        pSrc->m_iPenStyle = m_iPenStyle;
        pSrc->m_iPenWidth = m_iPenWidth;
        pSrc->m_clrBrush = m_clrBrush;
        pSrc->m_iBrushStyle = m_iBrushStyle;
    }
}

void CRectangle::Serialize( CArchive &ar )
{
    CGraphic::Serialize(ar);
    if (ar.IsStoring())
    {
        ar << m_clrPen << m_iPenStyle << m_iPenWidth << m_clrBrush << m_iBrushStyle;
    }
    else
    {
        ar >> m_clrPen >> m_iPenStyle >> m_iPenWidth >> m_clrBrush >> m_iBrushStyle;
    }
}

LPCTSTR CRectangle::HitSizingTest( CPoint& pt, BOOL& bResult, LONG** PtX /*= NULL*/, LONG** PtY /*= NULL */ )
{
    int inf = DRAG_INF_WIDTH + m_iPenWidth;
    
    BOOL bTrans = TRUE;
    if ((m_pt1.x > m_pt2.x && m_pt1.y < m_pt2.y) || (m_pt1.x < m_pt2.x && m_pt1.y > m_pt2.y))
    {
        bTrans = FALSE;
    }
    GraphicHitSizingTest(m_pt1.x, m_pt1.y, inf, pt, bResult, PtX, PtY);
    if (bResult) return bTrans ? IDC_SIZENWSE : IDC_SIZENESW;
    GraphicHitSizingTest(m_pt2.x, m_pt2.y, inf, pt, bResult, PtX, PtY);
    if (bResult) return bTrans ? IDC_SIZENWSE : IDC_SIZENESW;
    GraphicHitSizingTest(m_pt1.x, m_pt2.y, inf, pt, bResult, PtX, PtY);
    if (bResult) return bTrans ? IDC_SIZENESW : IDC_SIZENWSE;
    GraphicHitSizingTest(m_pt2.x, m_pt1.y, inf, pt, bResult, PtX, PtY);
    if (bResult) return bTrans ? IDC_SIZENESW : IDC_SIZENWSE;

    LONG X = ( m_pt1.x + m_pt2.x ) / 2;
    LONG Y = ( m_pt1.y + m_pt2.y ) / 2;
    GraphicHitSizingTest(X, m_pt1.y, inf, pt, bResult, NULL, PtY);
    if (bResult) return IDC_SIZENS;
    GraphicHitSizingTest(X, m_pt2.y, inf, pt, bResult, NULL, PtY);
    if (bResult) return IDC_SIZENS;
    GraphicHitSizingTest(m_pt1.x, Y, inf, pt, bResult, PtX, NULL);
    if (bResult) return IDC_SIZEWE;
    GraphicHitSizingTest(m_pt2.x, Y, inf, pt, bResult, PtX, NULL);
    if (bResult) return IDC_SIZEWE;

    return NULL;
}

IMPLEMENT_SERIAL( CEllipse, CGraphic, 1 )
CEllipse::CEllipse(): CGraphic(ELLIPSE)
{
    
}

LPCTSTR CEllipse::GetName() const
{
    return _T("椭圆");
}

void CEllipse::Draw( CDC* pDC )
{
    ASSERT_VALID(pDC);
    CGraphic::Draw(pDC);
    CPen pen;
    CreateGdiObjectFromId(GBS_PEN, m_iPenStyle, &pen, m_iPenWidth, m_clrPen);
    pDC->SelectObject(&pen);
    CBrush br;
    CreateGdiObjectFromId(GBS_BRUSH, m_iBrushStyle, &br, 0, m_clrBrush);
    pDC->SelectObject(&br);
    pDC->Ellipse(m_pt1.x, m_pt1.y, m_pt2.x, m_pt2.y);
}

void CEllipse::DrawSelectedEdge( CDC* pDC )
{
    ASSERT_VALID(pDC);
    CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
    CRect rt;
    int inf = INF_WIDTH + m_iPenWidth;
    pDC->SelectObject(&pen);
    CPoint pt;
    CPoint ct;
    ct.x = ( m_pt1.x + m_pt2.x ) / 2;
    ct.y = ( m_pt1.y + m_pt2.y ) / 2;
    
    GraphicDrawSelectedEdge(pDC, m_pt1, inf);
    GraphicDrawSelectedEdge(pDC, m_pt2, inf);
    pt.x = m_pt2.x; pt.y = m_pt1.y;
    GraphicDrawSelectedEdge(pDC, pt, inf);
    pt.x = m_pt1.x; pt.y = m_pt2.y;
    GraphicDrawSelectedEdge(pDC, pt, inf);
    
    if (m_pt2.x - m_pt1.x < inf + INF_WIDTH || m_pt2.y - m_pt1.y < inf + INF_WIDTH)
    {
        return;
    }
    
    pt.x = ct.x; pt.y = m_pt1.y;
    GraphicDrawSelectedEdge(pDC, pt, inf);
    pt.x = ct.x; pt.y = m_pt2.y;
    GraphicDrawSelectedEdge(pDC, pt, inf);
    pt.x = m_pt1.x; pt.y = ct.y;
    GraphicDrawSelectedEdge(pDC, pt, inf);
    pt.x = m_pt2.x; pt.y = ct.y;
    GraphicDrawSelectedEdge(pDC, pt, inf);
}

void CEllipse::HitTest( CPoint& pt, BOOL& bResult )
{
    /*
        椭圆的长轴长 A |m_pt1.x - m_pt2.x| 和 B |m_pt1.y - m_pt2.y|
        方程 4*(X*X*A*A+Y*Y*B*B)=A*A+B*B
    */

    if (!PtInRectTest(pt)) return;

    CPoint S; // 中心点
    S.x = ( m_pt1.x + m_pt2.x ) / 2;
    S.y = ( m_pt1.y + m_pt2.y ) / 2;
    CSize L; // 半长短轴长
    L = m_pt2 - S;
    CPoint P; // 相对坐标
    P = pt - S;
    LONG abs_ = (LONG) abs(P.x*P.x*L.cy*L.cy+P.y*P.y*L.cx*L.cx-L.cx*L.cx*L.cy*L.cy);
    float miss = (float) abs_ / (L.cx * L.cy * L.cx * L.cy);
    bResult = ( (float) fabsf(miss) < SELECT_MISS_ELLIPSE ) ? TRUE : FALSE;
}

void CEllipse::GetRect( CRect& rt )
{
    rt.SetRect(m_pt1, m_pt2);
    rt.NormalizeRect();
    LONG inf = m_iPenWidth + INF_WIDTH + 1;
    rt.InflateRect(inf, inf);
}

void CEllipse::UpdateData( GraphicMember* pSrc, BOOL bSave /*= TRUE */ )
{
    CGraphic::UpdateData(pSrc, bSave);
    if (bSave)
    {
        m_clrPen = pSrc->m_clrPen;
        m_iPenStyle = pSrc->m_iPenStyle;
        m_iPenWidth = pSrc->m_iPenWidth;
        m_clrBrush = pSrc->m_clrBrush;
        m_iBrushStyle = pSrc->m_iBrushStyle;
    }
    else
    {
        pSrc->m_clrPen = m_clrPen;
        pSrc->m_iPenStyle = m_iPenStyle;
        pSrc->m_iPenWidth = m_iPenWidth;
        pSrc->m_clrBrush = m_clrBrush;
        pSrc->m_iBrushStyle = m_iBrushStyle;
    }
}

void CEllipse::Serialize( CArchive &ar )
{
    CGraphic::Serialize(ar);
    if (ar.IsStoring())
    {
        ar << m_clrPen << m_iPenStyle << m_iPenWidth << m_clrBrush << m_iBrushStyle;
    }
    else
    {
        ar >> m_clrPen >> m_iPenStyle >> m_iPenWidth >> m_clrBrush >> m_iBrushStyle;
    }
}

LPCTSTR CEllipse::HitSizingTest( CPoint& pt, BOOL& bResult, LONG** PtX /*= NULL*/, LONG** PtY /*= NULL */ )
{
    int inf = DRAG_INF_WIDTH + m_iPenWidth;
        
    LONG X = ( m_pt1.x + m_pt2.x ) / 2;
    LONG Y = ( m_pt1.y + m_pt2.y ) / 2;
    GraphicHitSizingTest(X, m_pt1.y, inf, pt, bResult, NULL, PtY);
    if (bResult) return IDC_SIZENS;
    GraphicHitSizingTest(X, m_pt2.y, inf, pt, bResult, NULL, PtY);
    if (bResult) return IDC_SIZENS;
    GraphicHitSizingTest(m_pt1.x, Y, inf, pt, bResult, PtX, NULL);
    if (bResult) return IDC_SIZEWE;
    GraphicHitSizingTest(m_pt2.x, Y, inf, pt, bResult, PtX, NULL);
    if (bResult) return IDC_SIZEWE;
    
    return NULL;
}

IMPLEMENT_SERIAL(CCurve, CGraphic, 1)
CCurve::CCurve(): CGraphic(CURVE)
{
    
}

LPCTSTR CCurve::GetName() const
{
    return _T("曲线");
}

void CCurve::Draw( CDC* pDC )
{
    ASSERT_VALID(pDC);
    CPen pen;
    CreateGdiObjectFromId(GBS_PEN, m_iPenStyle, &pen, m_iPenWidth, m_clrPen);
    pDC->SelectObject(&pen);
    int size = m_pts.GetSize();

    CPoint pt;
    pDC->MoveTo(m_pt1);
    for (int i = 0; i < size; i++)
    {
        pt = m_pts[i] + m_pt1;
        pDC->LineTo(pt);
    }
    pDC->LineTo(m_pt2);
}

void CCurve::Serialize( CArchive &ar )
{
    CGraphic::Serialize(ar);
    int size = 0;
    if ( ar.IsStoring() )
    {
        size = m_pts.GetSize();
        ar << m_clrPen << m_iPenStyle << m_iPenWidth << size;
        if (size == 0)
        {
            return;
        }
        for (int i = 0; i < size; i++)
        {
            ar << m_pts[i];
        }
    }
    else
    {
        ar >> m_clrPen >> m_iPenStyle >> m_iPenWidth >> size;
        if (size == 0)
        {
            return;
        }
        m_pts.SetSize(size);
        for (int i = 0; i < size; i++)
        {
            ar >> m_pts[i];
        }
    }
}

void CCurve::DrawSelectedEdge( CDC* pDC )
{
    ASSERT_VALID(pDC);
    CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
    pDC->SelectObject(&pen);
    CRect rt;
    CPoint pt;
    int inf = INF_WIDTH + m_iPenWidth;
    GraphicDrawSelectedEdge(pDC, m_pt1, inf);
    for (int i = 0; i < m_pts.GetSize(); i+=3)
    {
        pt = m_pts[i] + m_pt1;
        GraphicDrawSelectedEdge(pDC, pt, inf);
    }
    GraphicDrawSelectedEdge(pDC, m_pt2, inf);
}

void CCurve::HitTest( CPoint& pt, BOOL& bResult )
{
    CPoint a, b;
    a = m_pt1;
    for (int i = 0; i < m_pts.GetSize(); i++)
    {
        b = m_pts[i] + m_pt1;
        LineHitTest(a, b, pt, bResult); if (bResult) return;
        a = b;
    }
    b = m_pt2;
    LineHitTest(a, b, pt, bResult);    
}

void CCurve::GetRect( CRect& rt )
{
    rt.SetRect(m_pt1, m_pt2);
    rt.NormalizeRect();
    CPoint pt;
    for (int i = 0; i < m_pts.GetSize(); i++)
    {
        pt = m_pts[i] + m_pt1;
        if (!rt.PtInRect(pt))
        {
            if (pt.x < rt.left)     rt.left = pt.x;
            if (pt.y < rt.top)      rt.top = pt.y;
            if (pt.x > rt.right)    rt.right = pt.x;
            if (pt.y > rt.bottom)   rt.bottom = pt.y;
        }
    }
    LONG inf = m_iPenWidth + INF_WIDTH + 1;
    rt.InflateRect(inf, inf);
}

int CCurve::GetPts() const
{
    return CGraphic::GetPts() + m_pts.GetSize();
}

BOOL CCurve::EnableBrush() const
{
    return FALSE;
}

void CCurve::UpdateData( GraphicMember* pSrc, BOOL bSave /*= TRUE */ )
{
    CGraphic::UpdateData(pSrc, bSave);
    if (bSave)
    {
        m_clrPen = pSrc->m_clrPen;
        m_iPenStyle = pSrc->m_iPenStyle;
        m_iPenWidth = pSrc->m_iPenWidth;
        m_pts.Copy(pSrc->m_pts);
    }
    else
    {
        pSrc->m_clrPen = m_clrPen;
        pSrc->m_iPenStyle = m_iPenStyle;
        pSrc->m_iPenWidth = m_iPenWidth;
        pSrc->m_pts.Copy(m_pts);
    }
}

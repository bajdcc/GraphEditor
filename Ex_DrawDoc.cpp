// Ex_DrawDoc.cpp : implementation of the CEx_DrawDoc class
//

#include "stdafx.h"
#include "Ex_Draw.h"

#include "Ex_DrawDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEx_DrawDoc

IMPLEMENT_DYNCREATE(CEx_DrawDoc, CDocument)

BEGIN_MESSAGE_MAP(CEx_DrawDoc, CDocument)
	//{{AFX_MSG_MAP(CEx_DrawDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEx_DrawDoc construction/destruction

CEx_DrawDoc::CEx_DrawDoc(): m_log(&m_obArray)
{
	// TODO: add one-time construction code here
    m_sel = -1;
    m_bShowTab = FALSE;
    m_bShowHidden = FALSE;
    m_createID = 0;
}

CEx_DrawDoc::~CEx_DrawDoc()
{
}

BOOL CEx_DrawDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CEx_DrawDoc serialization

void CEx_DrawDoc::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        // TODO: add storing code here
        ar << m_createID;
        int size = m_obArray.GetSize();
        ar << size;
        for (int i = 0; i < size; i++)
        {
            ar << m_obArray[i];
        }
        TRACE("------------- Store %d .\n", size);
    }
    else
    {
        // TODO: add loading code here
        ar >> m_createID;
        int size = 0;
        ar >> size;
        if (size == 0)
        {
            return;
        }
        m_obArray.SetSize(size);
        CGraphic* pOb;
        for (int i = 0; i < size; i++)
        {
            ar >> pOb;
            ASSERT(!m_log.Add(pOb, i));
        }
        TRACE("-------------- Load %d .\n", size);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CEx_DrawDoc diagnostics

#ifdef _DEBUG
void CEx_DrawDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CEx_DrawDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEx_DrawDoc commands

void CEx_DrawDoc::DeleteContents() 
{
	// TODO: Add your specialized code here and/or call the base class
    m_log.Clear();
	CDocument::DeleteContents();
}

void CEx_DrawDoc::WriteLog( CGraphicLog::GOS gos, CGraphic* P, int index )
{
    m_log.Operator(gos, P, index);
    Trace();
    UpdateAllViews(NULL, 0x6000);
}

void CEx_DrawDoc::Done()
{
    if (m_log.CanDo())
    {
        m_log.Done();
        TRACE(">>>>>> DONE >>>>>>\n");
        Trace();
        m_sel = -1;
        UpdateAllViews(NULL, 0x4000);
        UpdateAllViews(NULL, 0x4001);
        UpdateAllViews(NULL, 0x5000);
        UpdateAllViews(NULL, 0x6000);
    }
}

void CEx_DrawDoc::Undo()
{
    if (m_log.CanUndo())
    {
        m_log.Undo();
        TRACE(">>>>>> UNDO >>>>>>\n");
        Trace();
        m_sel = -1;
        UpdateAllViews(NULL, 0x4000);
        UpdateAllViews(NULL, 0x4001);
        UpdateAllViews(NULL, 0x5000);
        UpdateAllViews(NULL, 0x6000);
    }
}

void CEx_DrawDoc::Trace()
{
    TRACE("---------------- LOG CONTEXT BEGIN\n");
    
    TRACE(">>>>>> LOG ARRAY\n");
    for (int i = 0; i < m_obArray.GetSize(); i++)
    {
        CGraphic* P = Convert_To_Graphic(m_obArray[i]);
		ASSERT_VALID(P);
        TRACE("--- [%p] -- [%s] -- [%d] - REF %d\n", P, P->m_lpszName, i, m_log.m_refs[P]);
    }
    
    POSITION pos;
    CGraphicLog::GraphicOperation go;
    
    TRACE(">>>>>> LOG UNDO\n");
    pos = m_log.m_listUndo.GetHeadPosition();
    while (pos)
    {
        go = m_log.m_listUndo.GetNext(pos);
        TRACE("--- %s -- REF %d\n", go.Trace(), m_log.m_refs[go.pGraphic]);
    }
    
    TRACE(">>>>>> LOG DONE\n");
    pos = m_log.m_listDone.GetHeadPosition();
    while (pos)
    {
        go = m_log.m_listDone.GetNext(pos);
        TRACE("--- %s -- REF %d\n", go.Trace(), m_log.m_refs[go.pGraphic]);
    }
    
    TRACE("---------------- LOG CONTEXT END\n\n");
}

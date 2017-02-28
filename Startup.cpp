// Startup.cpp : implementation file
//

#include "stdafx.h"
#include "Ex_Draw.h"
#include "Startup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Startup dialog


CStartupDlg::CStartupDlg(CWnd* pParent /*=NULL*/)
: CDialog(CStartupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(Startup)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    m_cnt = 0;
}


void CStartupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Startup)
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStartupDlg, CDialog)
	//{{AFX_MSG_MAP(Startup)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_NCDESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Startup message handlers

void CStartupDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 0x10)
	{
        m_cnt++;
        m_progress.SetPos(m_cnt);
        if (m_cnt == 100)
        {
            KillTimer(0x10);
            DestroyWindow();
            m_pParentWnd->EnableWindow(TRUE);
            static_cast<CEx_DrawApp*>(AfxGetApp())->m_event.SetEvent();
        }
        if (m_cnt == 70)
        {
            m_pParentWnd->ShowWindow(SW_NORMAL);
            m_pParentWnd->UpdateWindow();
            m_pParentWnd->EnableWindow(FALSE);
        }
	}

	CDialog::OnTimer(nIDEvent);
}

BOOL CStartupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    SetTimer(0x10, 10, NULL);
    m_progress.SetRange(0, 100);
    m_progress.SetPos(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

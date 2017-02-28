#if !defined(AFX_STARTUP_H__5630B29B_C547_4AFE_99AC_CC00F01EF94D__INCLUDED_)
#define AFX_STARTUP_H__5630B29B_C547_4AFE_99AC_CC00F01EF94D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxmt.h>
// Startup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Startup dialog

class CStartupDlg : public CDialog
{
// Construction
public:
	CStartupDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Startup)
    enum { IDD = IDD_STARTUP };
	CProgressCtrl	m_progress;
	//}}AFX_DATA

    CEvent  m_event;
    int     m_cnt;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Startup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Startup)
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STARTUP_H__5630B29B_C547_4AFE_99AC_CC00F01EF94D__INCLUDED_)

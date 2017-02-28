// Ex_Draw.h : main header file for the EX_DRAW application
//

#if !defined(AFX_EX_DRAW_H__7387CED8_C46F_4C5E_9DE1_275FFF355626__INCLUDED_)
#define AFX_EX_DRAW_H__7387CED8_C46F_4C5E_9DE1_275FFF355626__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "Startup.h"

/////////////////////////////////////////////////////////////////////////////
// CEx_DrawApp:
// See Ex_Draw.cpp for the implementation of this class
//

class CEx_DrawApp : public CWinApp
{
public:
	CEx_DrawApp();

    CStartupDlg* dlg;
    CEvent m_event;

    static UINT WaitingForDlgDestroyProc(LPVOID);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEx_DrawApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CEx_DrawApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EX_DRAW_H__7387CED8_C46F_4C5E_9DE1_275FFF355626__INCLUDED_)

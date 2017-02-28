#if !defined(AFX_HELPDLG_H__4DB4B6F2_0465_47AA_93F5_1F47186F48F0__INCLUDED_)
#define AFX_HELPDLG_H__4DB4B6F2_0465_47AA_93F5_1F47186F48F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HelpDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHelpDlg dialog

class CHelpDlg : public CDialog
{
// Construction
public:
	CHelpDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHelpDlg)
	enum { IDD = IDD_DIALOG_HELP };
	CStatic	m_stat_help;
	CEdit	m_edit_help;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHelpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    CFont m_captionFont;
    CFont m_txtFont;

	// Generated message map functions
	//{{AFX_MSG(CHelpDlg)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HELPDLG_H__4DB4B6F2_0465_47AA_93F5_1F47186F48F0__INCLUDED_)

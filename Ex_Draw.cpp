// Ex_Draw.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Ex_Draw.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "Ex_DrawDoc.h"
#include "Ex_DrawView.h"
#include "Ex_Ctrl.h"
#include "HelpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEx_DrawApp

BEGIN_MESSAGE_MAP(CEx_DrawApp, CWinApp)
	//{{AFX_MSG_MAP(CEx_DrawApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEx_DrawApp construction

CEx_DrawApp::CEx_DrawApp(): m_event(FALSE, TRUE)
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
    dlg = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CEx_DrawApp object

CEx_DrawApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CEx_DrawApp initialization

BOOL CEx_DrawApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_EX_DRATYPE,
		RUNTIME_CLASS(CEx_DrawDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CEx_DrawView));
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

    AfxBeginThread(WaitingForDlgDestroyProc, NULL);
    dlg = new CStartupDlg(m_pMainWnd);
    dlg->Create((UINT)IDD_STARTUP);
    dlg->CenterWindow();
    dlg->ShowWindow(SW_SHOW);

	// The main window has been initialized, so show and update it.
// 	pMainFrame->ShowWindow(m_nCmdShow);
// 	pMainFrame->UpdateWindow();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CEx_DrawApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

UINT CEx_DrawApp::WaitingForDlgDestroyProc( LPVOID )
{
    WaitForSingleObject(static_cast<CEx_DrawApp*>(AfxGetApp())->m_event, INFINITE);
    Sleep(20);
    delete static_cast<CEx_DrawApp*>(AfxGetApp())->dlg;
    static_cast<CEx_DrawApp*>(AfxGetApp())->dlg = NULL;
    Sleep(2000);
    CHelpDlg dlg;
    dlg.DoModal();
    return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CEx_DrawApp message handlers


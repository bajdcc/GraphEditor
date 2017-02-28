; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CEx_Ctrl
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ex_draw.h"
LastPage=0

ClassCount=10
Class1=CChildFrame
Class2=ConfigDlg
Class3=CEx_Ctrl
Class4=CEx_DrawApp
Class5=CAboutDlg
Class6=CEx_DrawDoc
Class7=CEx_DrawView
Class8=CMainFrame
Class9=Startup

ResourceCount=9
Resource1=IDR_EX_CTRL_TMPL
Resource2=IDR_TOOLS
Resource3=IDD_STARTUP
Resource4=IDD_ABOUTBOX
Resource5=IDD_EX_CTRL_FORM
Resource6=IDR_EX_DRATYPE
Resource7=IDR_MAINFRAME
Resource8=IDD_CONFIG
Class10=CHelpDlg
Resource9=IDD_DIALOG_HELP

[CLS:CChildFrame]
Type=0
BaseClass=CMDIChildWnd
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp

[CLS:ConfigDlg]
Type=0
BaseClass=CDialog
HeaderFile=ConfigDlg.h
ImplementationFile=ConfigDlg.cpp

[CLS:CEx_Ctrl]
Type=0
BaseClass=CFormView
HeaderFile=Ex_Ctrl.h
ImplementationFile=Ex_Ctrl.cpp
Filter=D
VirtualFilter=VWC
LastObject=ID_OP_PREV

[CLS:CEx_DrawApp]
Type=0
BaseClass=CWinApp
HeaderFile=Ex_Draw.h
ImplementationFile=Ex_Draw.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=Ex_Draw.cpp
ImplementationFile=Ex_Draw.cpp
LastObject=IDOK

[CLS:CEx_DrawDoc]
Type=0
BaseClass=CDocument
HeaderFile=Ex_DrawDoc.h
ImplementationFile=Ex_DrawDoc.cpp

[CLS:CEx_DrawView]
Type=0
BaseClass=CScrollView
HeaderFile=Ex_DrawView.h
ImplementationFile=Ex_DrawView.cpp
LastObject=CEx_DrawView
Filter=C
VirtualFilter=VWC

[CLS:CMainFrame]
Type=0
BaseClass=CMDIFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp

[CLS:Startup]
Type=0
HeaderFile=Startup.h
ImplementationFile=Startup.cpp

[DLG:IDD_CONFIG]
Type=1
Class=ConfigDlg
ControlCount=24
Control1=IDC_CHANGECOLOR_,button,1342242816
Control2=IDC_COLOR_,static,1342308352
Control3=IDC_COLOR_V_,static,1342308352
Control4=IDC_CHANGECOLOR2_,button,1342242816
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_SELPS_,combobox,1344339971
Control8=IDC_STATIC,static,1342308352
Control9=IDC_SELNAME,edit,1350631552
Control10=IDC_SPINPW_,msctls_updown32,1342177590
Control11=IDC_COLOR2_,static,1342308352
Control12=IDC_COLOR_V2_,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_SELBS_,combobox,1344339971
Control15=IDC_STATIC,button,1342177287
Control16=IDC_STATIC,button,1342177287
Control17=IDC_STATIC,button,1342177287
Control18=IDC_STATIC,static,1342308352
Control19=IDOK,button,1342242816
Control20=IDCANCLE,button,1342242816
Control21=IDC_STATIC,static,1342177296
Control22=IDC_SELPW_,edit,1350639744
Control23=IDC_EDIT_GRAPH,edit,1484849280
Control24=IDC_CFG_HIDDEN,button,1342242819

[DLG:IDD_EX_CTRL_FORM]
Type=1
Class=CEx_Ctrl
ControlCount=16
Control1=IDC_CHANGECOLOR,button,1342242816
Control2=IDC_COLOR,static,1342308352
Control3=IDC_COLOR_V,static,1342308352
Control4=IDC_CHANGECOLOR2,button,1342242816
Control5=IDC_SELGRAPH,combobox,1344339971
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_SELPS,combobox,1344339971
Control9=IDC_STATIC,static,1342308352
Control10=IDC_SELPW,edit,1350639744
Control11=IDC_SPINPW,msctls_updown32,1342177590
Control12=IDC_COLOR2,static,1342308352
Control13=IDC_COLOR_V2,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_SELBS,combobox,1344339971
Control16=IDC_GRAPH_LIST,listbox,1352728833

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_STARTUP]
Type=1
Class=Startup
ControlCount=2
Control1=IDC_STATIC,static,1342308352
Control2=IDC_PROGRESS1,msctls_progress32,1350565888

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[TB:IDR_TOOLS]
Type=1
Class=?
Command1=ID_TOOL_SEL
Command2=ID_TOOL_LINE
Command3=ID_TOOL_RECT
Command4=ID_TOOL_ELLIPSE
Command5=ID_TOOL_CURVE
Command6=ID_OP_PREV
Command7=ID_OP_NEXT
CommandCount=7

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_EX_DRATYPE]
Type=1
Class=CEx_DrawView
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_EDIT_UNDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_VIEW_TOOLBAR
Command16=ID_VIEW_STATUS_BAR
Command17=ID_WINDOW_CASCADE
Command18=ID_WINDOW_TILE_HORZ
Command19=ID_WINDOW_ARRANGE
Command20=ID_CALL_HELP
Command21=ID_APP_ABOUT
Command22=ID_SHOW_TAB
Command23=ID_SHOW_HIDDEN
Command24=ID_SAVE_AS_BITMAP
CommandCount=24

[MNU:IDR_EX_CTRL_TMPL]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_EDIT_UNDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_VIEW_TOOLBAR
Command16=ID_VIEW_STATUS_BAR
Command17=ID_WINDOW_NEW
Command18=ID_WINDOW_CASCADE
Command19=ID_WINDOW_TILE_HORZ
Command20=ID_WINDOW_ARRANGE
Command21=ID_APP_ABOUT
CommandCount=21

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[DLG:IDD_DIALOG_HELP]
Type=1
Class=CHelpDlg
ControlCount=4
Control1=IDC_EDIT_HELP,edit,1353779332
Control2=IDOK,button,1342242816
Control3=IDC_STATIC_HLP,static,1342308353
Control4=IDC_STATIC,static,1342308354

[CLS:CHelpDlg]
Type=0
HeaderFile=HelpDlg.h
ImplementationFile=HelpDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CHelpDlg
VirtualFilter=dWC


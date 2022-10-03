; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CFinalDlg
LastTemplate=CPropertyPage
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "setup.h"
LastPage=0

ClassCount=8
Class1=CSetupApp

ResourceCount=6
Resource1=IDD_DESTINATION_DLG
Resource2=IDD_BROWSE_DLG
Resource3=IDD_MAIN_DLG
Resource4=IDD_CONFIRM_DLG
Class2=CDestinationDlg
Class3=CLicenseDlg
Class4=CMainDlg
Class5=CProgramFolderDlg
Class6=CBrowseDlg
Resource5=IDD_FINAL_DLG
Class7=CConfirmDlg
Class8=CFinalDlg
Resource6=IDD_PROGRAM_FOLDER_DLG

[CLS:CSetupApp]
Type=0
BaseClass=CWinApp
HeaderFile=Setup.h
ImplementationFile=Setup.cpp

[DLG:IDD_DESTINATION_DLG]
Type=1
Class=CDestinationDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177294
Control2=IDC_BROWSE_BUTTON,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_DESTINATION,static,1342308352
Control5=IDC_STATIC,button,1342177287

[DLG:IDD_PROGRAM_FOLDER_DLG]
Type=1
Class=CProgramFolderDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177294
Control2=IDC_STATIC,static,1342308352
Control3=IDC_PROGAM_FOLDER_FLD,edit,1350631552
Control4=IDC_FOLDER_BROWSE_BTN,button,1342242816

[DLG:IDD_MAIN_DLG]
Type=1
Class=CMainDlg
ControlCount=2
Control1=IDC_STATIC,static,1342177294
Control2=IDC_WELCOME_TEXT,static,1342308352

[CLS:CDestinationDlg]
Type=0
HeaderFile=DestinationDlg.h
ImplementationFile=DestinationDlg.cpp
BaseClass=CPropertyPage
Filter=D
VirtualFilter=idWC
LastObject=IDC_BROWSE_BUTTON

[CLS:CLicenseDlg]
Type=0
HeaderFile=LicenseDlg.h
ImplementationFile=LicenseDlg.cpp
BaseClass=CPropertyPage
Filter=D
VirtualFilter=idWC
LastObject=CLicenseDlg

[CLS:CMainDlg]
Type=0
HeaderFile=MainDlg.h
ImplementationFile=MainDlg.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=IDC_INSTALLING_TEXT
VirtualFilter=idWC

[CLS:CProgramFolderDlg]
Type=0
HeaderFile=ProgramFolderDlg.h
ImplementationFile=ProgramFolderDlg.cpp
BaseClass=CPropertyPage
Filter=D
VirtualFilter=idWC
LastObject=CProgramFolderDlg

[DLG:IDD_BROWSE_DLG]
Type=1
Class=CBrowseDlg
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_DIRECTORY_TREE,SysTreeView32,1350631431
Control5=IDC_STATIC,static,1342308352
Control6=IDC_PATH,edit,1350631552

[CLS:CBrowseDlg]
Type=0
HeaderFile=BrowseDlg.h
ImplementationFile=BrowseDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_DIRECTORY_TREE

[DLG:IDD_CONFIRM_DLG]
Type=1
Class=CConfirmDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177294
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_SETUP_INFO,edit,1350568068

[DLG:IDD_FINAL_DLG]
Type=1
Class=CFinalDlg
ControlCount=2
Control1=IDC_FINAL_TEXT,static,1342308352
Control2=IDC_STATIC,static,1342177294

[CLS:CConfirmDlg]
Type=0
HeaderFile=ConfirmDlg.h
ImplementationFile=ConfirmDlg.cpp
BaseClass=CPropertyPage
Filter=D
VirtualFilter=idWC
LastObject=CConfirmDlg

[CLS:CFinalDlg]
Type=0
HeaderFile=FinalDlg.h
ImplementationFile=FinalDlg.cpp
BaseClass=CPropertyPage
Filter=D
VirtualFilter=idWC
LastObject=IDC_FINAL_TEXT


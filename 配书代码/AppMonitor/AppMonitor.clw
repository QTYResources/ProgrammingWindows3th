; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CAppMonitorDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "AppMonitor.h"

ClassCount=3
Class1=CAppMonitorApp
Class2=CAppMonitorDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_APPMONITOR_DIALOG

[CLS:CAppMonitorApp]
Type=0
HeaderFile=AppMonitor.h
ImplementationFile=AppMonitor.cpp
Filter=N

[CLS:CAppMonitorDlg]
Type=0
HeaderFile=AppMonitorDlg.h
ImplementationFile=AppMonitorDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_BTNBROWSE

[CLS:CAboutDlg]
Type=0
HeaderFile=AppMonitorDlg.h
ImplementationFile=AppMonitorDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_APPMONITOR_DIALOG]
Type=1
Class=CAppMonitorDlg
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDITAPP,edit,1484849280
Control4=IDC_BTNBROWSE,button,1342242816
Control5=IDC_STATIC,static,1342308352


# Microsoft Developer Studio Project File - Name="06Meminfo" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=06Meminfo - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "06Meminfo.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "06Meminfo.mak" CFG="06Meminfo - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "06Meminfo - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "06Meminfo - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "06Meminfo - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "06Meminfo - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MT /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "06Meminfo - Win32 Release"
# Name "06Meminfo - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\COMMON\AFXSTATE.CPP
# End Source File
# Begin Source File

SOURCE=..\COMMON\AFXTLS.CPP
# End Source File
# Begin Source File

SOURCE=..\COMMON\APPCORE.CPP
# End Source File
# Begin Source File

SOURCE=..\COMMON\APPINIT.CPP
# End Source File
# Begin Source File

SOURCE=..\COMMON\CMDTARG.CPP
# End Source File
# Begin Source File

SOURCE=..\COMMON\DUMPOUT.CPP
# End Source File
# Begin Source File

SOURCE=..\COMMON\MAP_PP.CPP
# End Source File
# Begin Source File

SOURCE=.\Meminfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Meminfo.rc
# End Source File
# Begin Source File

SOURCE=..\COMMON\OBJCORE.CPP
# End Source File
# Begin Source File

SOURCE=..\COMMON\PLEX.CPP
# End Source File
# Begin Source File

SOURCE=..\COMMON\THRDCORE.CPP
# End Source File
# Begin Source File

SOURCE=..\COMMON\WINCORE.CPP
# End Source File
# Begin Source File

SOURCE=..\COMMON\WINMAIN.CPP
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\COMMON\_AFX.H
# End Source File
# Begin Source File

SOURCE=..\COMMON\_AFXCOLL.H
# End Source File
# Begin Source File

SOURCE=..\COMMON\_AFXIMPL.H
# End Source File
# Begin Source File

SOURCE=..\COMMON\_AFXMSG_.H
# End Source File
# Begin Source File

SOURCE=..\COMMON\_AFXPLEX_.H
# End Source File
# Begin Source File

SOURCE=..\COMMON\_AFXSTAT_.H
# End Source File
# Begin Source File

SOURCE=..\COMMON\_AFXTLS_.H
# End Source File
# Begin Source File

SOURCE=..\COMMON\_AFXWIN.H
# End Source File
# Begin Source File

SOURCE=.\Meminfo.h
# End Source File
# Begin Source File

SOURCE=..\COMMON\WINHAND_.H
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\status.ico
# End Source File
# End Group
# End Target
# End Project

# Microsoft Developer Studio Project File - Name="net_demo" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=net_demo - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "net_demo.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "net_demo.mak" CFG="net_demo - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "net_demo - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "net_demo - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "net_demo - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "net_demo - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "net_demo - Win32 Release"
# Name "net_demo - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AlarmOut.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAlarm.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAnalyze.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgChannelSet.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCustomOsd.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDeviceInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgIPCFeature.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLog.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMask.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMaskAlarm.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMotion.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgNet.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOsd.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPtz.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgQuality.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRecordCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteDo.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteSet.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSearchDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSerialPort.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSubQuality.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVlost.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgWorkSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\net_demo.cpp
# End Source File
# Begin Source File

SOURCE=.\net_demo.rc
# End Source File
# Begin Source File

SOURCE=.\net_demoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PlaybackDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AlarmOut.h
# End Source File
# Begin Source File

SOURCE=.\DlgAlarm.h
# End Source File
# Begin Source File

SOURCE=.\DlgAnalyze.h
# End Source File
# Begin Source File

SOURCE=.\DlgChannelSet.h
# End Source File
# Begin Source File

SOURCE=.\DlgCustomOsd.h
# End Source File
# Begin Source File

SOURCE=.\DlgDeviceInfo.h
# End Source File
# Begin Source File

SOURCE=.\DlgIPCFeature.h
# End Source File
# Begin Source File

SOURCE=.\DlgLog.h
# End Source File
# Begin Source File

SOURCE=.\DlgMask.h
# End Source File
# Begin Source File

SOURCE=.\DlgMaskAlarm.h
# End Source File
# Begin Source File

SOURCE=.\DlgMotion.h
# End Source File
# Begin Source File

SOURCE=.\DlgNet.h
# End Source File
# Begin Source File

SOURCE=.\DlgOsd.h
# End Source File
# Begin Source File

SOURCE=.\DlgPtz.h
# End Source File
# Begin Source File

SOURCE=.\DlgQuality.h
# End Source File
# Begin Source File

SOURCE=.\DlgRecordCtrl.h
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteDo.h
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteSet.h
# End Source File
# Begin Source File

SOURCE=.\DlgSearchDevice.h
# End Source File
# Begin Source File

SOURCE=.\DlgSerialPort.h
# End Source File
# Begin Source File

SOURCE=.\DlgSubQuality.h
# End Source File
# Begin Source File

SOURCE=.\DlgVlost.h
# End Source File
# Begin Source File

SOURCE=.\DlgWorkSheet.h
# End Source File
# Begin Source File

SOURCE=.\howellnetsdk.h
# End Source File
# Begin Source File

SOURCE=.\HwNetSdk.h
# End Source File
# Begin Source File

SOURCE=.\net_demo.h
# End Source File
# Begin Source File

SOURCE=.\net_demoDlg.h
# End Source File
# Begin Source File

SOURCE=.\net_video_test.h
# End Source File
# Begin Source File

SOURCE=.\play_def.h
# End Source File
# Begin Source File

SOURCE=.\PlaybackDialog.h
# End Source File
# Begin Source File

SOURCE=.\protocol_type.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\focus_mask.bmp
# End Source File
# Begin Source File

SOURCE=.\motion_mask.bmp
# End Source File
# Begin Source File

SOURCE=.\res\net_demo.ico
# End Source File
# Begin Source File

SOURCE=.\res\net_demo.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\PlaybackDialog.htm
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

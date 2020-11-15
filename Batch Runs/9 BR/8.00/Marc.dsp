# Microsoft Developer Studio Project File - Name="Marc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Marc - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Marc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Marc.mak" CFG="Marc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Marc - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Marc - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Marc - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib "C:\Program Files\Marc\lib\marccppapi.lib" /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Marc - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib "C:\Program Files\Marc\lib\marccppapi.lib" /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Marc - Win32 Release"
# Name "Marc - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Marc.cpp
# End Source File
# Begin Source File

SOURCE=.\Marc.rc
# End Source File
# Begin Source File

SOURCE=.\MyAction.cpp
# End Source File
# Begin Source File

SOURCE=.\MyBatAgent.cpp
# End Source File
# Begin Source File

SOURCE=.\MyBattery.cpp
# End Source File
# Begin Source File

SOURCE=.\MyDirection.cpp
# End Source File
# Begin Source File

SOURCE=.\MyDoor.cpp
# End Source File
# Begin Source File

SOURCE=.\MyExchange.cpp
# End Source File
# Begin Source File

SOURCE=.\MyGlobal.cpp
# End Source File
# Begin Source File

SOURCE=.\MyGutter.cpp
# End Source File
# Begin Source File

SOURCE=.\MyMap.cpp
# End Source File
# Begin Source File

SOURCE=.\MyMission.cpp
# End Source File
# Begin Source File

SOURCE=.\MyPackages.cpp
# End Source File
# Begin Source File

SOURCE=.\MyStock.cpp
# End Source File
# Begin Source File

SOURCE=.\MyTelePort.cpp
# End Source File
# Begin Source File

SOURCE=.\MyTug.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Marc.h
# End Source File
# Begin Source File

SOURCE=.\MyAction.h
# End Source File
# Begin Source File

SOURCE=.\MyBatAgent.h
# End Source File
# Begin Source File

SOURCE=.\MyBattery.h
# End Source File
# Begin Source File

SOURCE=.\MyDirection.h
# End Source File
# Begin Source File

SOURCE=.\MyDoor.h
# End Source File
# Begin Source File

SOURCE=.\MyExchange.h
# End Source File
# Begin Source File

SOURCE=.\MyGlobal.h
# End Source File
# Begin Source File

SOURCE=.\MyGutter.h
# End Source File
# Begin Source File

SOURCE=.\MyMap.h
# End Source File
# Begin Source File

SOURCE=.\MyMission.h
# End Source File
# Begin Source File

SOURCE=.\MyPackages.h
# End Source File
# Begin Source File

SOURCE=.\MyStock.h
# End Source File
# Begin Source File

SOURCE=.\MyTelePort.h
# End Source File
# Begin Source File

SOURCE=.\MyTug.h
# End Source File
# Begin Source File

SOURCE=.\MyTypes.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Marc.ico
# End Source File
# Begin Source File

SOURCE=.\small.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

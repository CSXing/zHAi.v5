# Microsoft Developer Studio Project File - Name="��լ��v5" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=��լ��v5 - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "��լ��v5.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "��լ��v5.mak" CFG="��լ��v5 - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "��լ��v5 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "zHAi5_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "zHAi5_EXPORTS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib Ws2_32.lib /nologo /dll /machine:I386
# Begin Target

# Name "��լ��v5 - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Ade32.cpp
# End Source File
# Begin Source File

SOURCE=.\Ade32.hpp
# End Source File
# Begin Source File

SOURCE=.\APIHook.cpp
# End Source File
# Begin Source File

SOURCE=.\Bind.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcScreen.cpp
# End Source File
# Begin Source File

SOURCE=.\Client.cpp
# End Source File
# Begin Source File

SOURCE=.\Color.cpp
# End Source File
# Begin Source File

SOURCE=.\console.cpp
# End Source File
# Begin Source File

SOURCE=.\cPatch.cpp
# End Source File
# Begin Source File

SOURCE=.\cSoundHook.cpp
# End Source File
# Begin Source File

SOURCE=.\cVars.cpp
# End Source File
# Begin Source File

SOURCE=.\DefineCmds.cpp
# End Source File
# Begin Source File

SOURCE=.\Drawing.cpp
# End Source File
# Begin Source File

SOURCE=.\EventHook.cpp
# End Source File
# Begin Source File

SOURCE=.\Font.cpp
# End Source File
# Begin Source File

SOURCE=.\Main.cpp
# End Source File
# Begin Source File

SOURCE=.\Menu.cpp
# End Source File
# Begin Source File

SOURCE=.\Opengl.cpp
# End Source File
# Begin Source File

SOURCE=.\misc\parsemsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Players.cpp
# End Source File
# Begin Source File

SOURCE=.\Radar.cpp
# End Source File
# Begin Source File

SOURCE=.\misc\sprites.cpp
# End Source File
# Begin Source File

SOURCE=.\StringFinder.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeHandling.cpp
# End Source File
# Begin Source File

SOURCE=.\zEngines.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\APIHook.h
# End Source File
# Begin Source File

SOURCE=.\Bind.h
# End Source File
# Begin Source File

SOURCE=.\CalcScreen.h
# End Source File
# Begin Source File

SOURCE=.\Client.h
# End Source File
# Begin Source File

SOURCE=.\Color.h
# End Source File
# Begin Source File

SOURCE=.\Console.h
# End Source File
# Begin Source File

SOURCE=.\cPatch.h
# End Source File
# Begin Source File

SOURCE=.\cSoundHook.h
# End Source File
# Begin Source File

SOURCE=.\cVars.h
# End Source File
# Begin Source File

SOURCE=.\DefineCmds.h
# End Source File
# Begin Source File

SOURCE=.\Drawing.h
# End Source File
# Begin Source File

SOURCE=.\EventHook.h
# End Source File
# Begin Source File

SOURCE=.\Font.h
# End Source File
# Begin Source File

SOURCE=.\Menu.h
# End Source File
# Begin Source File

SOURCE=.\Opengl.h
# End Source File
# Begin Source File

SOURCE=.\misc\parsemsg.h
# End Source File
# Begin Source File

SOURCE=.\Players.h
# End Source File
# Begin Source File

SOURCE=.\Radar.h
# End Source File
# Begin Source File

SOURCE=.\misc\sprites.h
# End Source File
# Begin Source File

SOURCE=.\StringFinder.h
# End Source File
# Begin Source File

SOURCE=.\TimeHandling.h
# End Source File
# Begin Source File

SOURCE=.\XorStr.h
# End Source File
# Begin Source File

SOURCE=.\zEngines.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\zHAi.rc
# End Source File
# End Group
# End Target
# End Project

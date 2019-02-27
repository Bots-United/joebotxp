# Microsoft Developer Studio Project File - Name="joebottest" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=joebottest - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "JoeBotTest.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "JoeBotTest.mak" CFG="joebottest - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "joebottest - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "joebottest - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "joebottest - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /G5 /W3 /GX /O2 /Ob2 /I "..\dlls" /I "..\engine" /I "..\common" /I "..\pm_shared" /I "..\\" /I "../NNSim/" /I "../NNSim/SOM/" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FAs /FR /YX /FD /c
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "joebottest - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\dlls" /I "..\engine" /I "..\common" /I "..\pm_shared" /I "..\\" /I "../NNSim/" /I "../NNSim/SOM/" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "joebottest - Win32 Release"
# Name "joebottest - Win32 Debug"
# Begin Source File

SOURCE=..\DLLS\AStar.h
# End Source File
# Begin Source File

SOURCE=..\dlls\AStarBase.cpp
# End Source File
# Begin Source File

SOURCE=..\DLLS\AStarBase.h
# End Source File
# Begin Source File

SOURCE=..\dlls\AStarBaseGoal.cpp
# End Source File
# Begin Source File

SOURCE=..\dlls\AStarMachine.cpp
# End Source File
# Begin Source File

SOURCE=..\DLLS\AStarMachine.h
# End Source File
# Begin Source File

SOURCE=..\dlls\AStarNode.cpp
# End Source File
# Begin Source File

SOURCE=..\DLLS\AStarNode.h
# End Source File
# Begin Source File

SOURCE=..\dlls\AStarNodeContainer.cpp
# End Source File
# Begin Source File

SOURCE=..\DLLS\AStarNodeContainer.h
# End Source File
# Begin Source File

SOURCE=..\dlls\BitField.cpp
# End Source File
# Begin Source File

SOURCE=..\DLLS\BitField.h
# End Source File
# Begin Source File

SOURCE=.\HPTime.cpp
# End Source File
# Begin Source File

SOURCE=.\HPTime.h
# End Source File
# Begin Source File

SOURCE=.\joebottest.cpp
# End Source File
# Begin Source File

SOURCE=..\dlls\Map.cpp
# End Source File
# Begin Source File

SOURCE=..\DLLS\Map.h
# End Source File
# Begin Source File

SOURCE=..\dlls\MemoryPool.h
# End Source File
# Begin Source File

SOURCE=..\dlls\Path.cpp
# End Source File
# Begin Source File

SOURCE=..\DLLS\Path.h
# End Source File
# Begin Source File

SOURCE=..\DLLS\PQueue.h
# End Source File
# Begin Source File

SOURCE=..\DLLS\PQueueHash.h
# End Source File
# Begin Source File

SOURCE=..\dlls\Way.cpp
# End Source File
# Begin Source File

SOURCE=..\DLLS\Way.h
# End Source File
# Begin Source File

SOURCE=..\dlls\Waypoint.cpp
# End Source File
# Begin Source File

SOURCE=..\DLLS\Waypoint.h
# End Source File
# Begin Source File

SOURCE=..\dlls\WaypointGraph.cpp
# End Source File
# Begin Source File

SOURCE=..\DLLS\WaypointGraph.h
# End Source File
# Begin Source File

SOURCE=..\dlls\WPStat.cpp
# End Source File
# Begin Source File

SOURCE=..\DLLS\WPStat.h
# End Source File
# End Target
# End Project

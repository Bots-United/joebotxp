# Microsoft Developer Studio Project File - Name="JoeBot" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=JoeBot - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "JoeBot.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "JoeBot.mak" CFG="JoeBot - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "JoeBot - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "JoeBot - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "JoeBot - Win32 DebugNM" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/SDKSrc/Public/dlls", NVGBAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "JoeBot - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G6 /W3 /GX /Zi /O2 /Ob2 /I "..\dlls" /I "..\engine" /I "..\common" /I "..\pm_shared" /I "..\\" /I "../NNSim/" /I "../NNSim/SOM/" /D "NDEBUG" /D "_WIN32" /D "WIN32" /D "_WINDOWS" /D "__JOEBOTXPDLL" /D "__HLENGINE" /FAs /Fr /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib gdi32.lib winspool.lib comdlg32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib user32.lib advapi32.lib winmm.lib /nologo /subsystem:windows /dll /map /debug /machine:I386 /def:".\JoeBotXP.def" /out:".\Release/JoeBotXP.dll"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
TargetPath=.\Release\JoeBotXP.dll
SOURCE="$(InputPath)"
PostBuild_Cmds=copy                                   $(TargetPath)                                   c:\spiele\half-life\joebot	copy                                   $(TargetPath)                                   m:\half-life\joebot
# End Special Build Tool

!ELSEIF  "$(CFG)" == "JoeBot - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\JoeBot___Win"
# PROP BASE Intermediate_Dir ".\JoeBot___Win"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /MTd /W3 /Gi /GR /GX /ZI /Od /I "..\dlls" /I "..\engine" /I "..\common" /I "..\pm_shared" /I "..\\" /I "../NNSim/" /I "../NNSim/SOM/" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "__JOEBOTXPDLL" /D "__HLENGINE" /FAs /FR /Fp".\Debug/JoeBotXP.pch" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i "..\engine" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 user32.lib advapi32.lib winmm.lib /nologo /subsystem:windows /dll /map /debug /machine:I386 /def:".\JoebotXP.def" /out:".\Debug/JoeBotXP.dll"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
TargetPath=.\Debug\JoeBotXP.dll
SOURCE="$(InputPath)"
PostBuild_Cmds=copy                                   $(TargetPath)                                   c:\spiele\half-life\joebot	copy                                   $(TargetPath)                                   m:\half-life\joebot
# End Special Build Tool

!ELSEIF  "$(CFG)" == "JoeBot - Win32 DebugNM"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "JoeBot___Win32_DebugNM"
# PROP BASE Intermediate_Dir "JoeBot___Win32_DebugNM"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "JoeBot___Win32_DebugNM"
# PROP Intermediate_Dir "JoeBot___Win32_DebugNM"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /W3 /GX /Zi /O2 /Ob2 /I "..\dlls" /I "..\engine" /I "..\common" /I "..\pm_shared" /I "..\\" /I "../NNSim/" /I "../NNSim/SOM/" /D "NDEBUG" /D "_WIN32" /D "WIN32" /D "_WINDOWS" /D "__JOEBOTXPDLL" /D "__HLENGINE" /FAs /Fr /YX /FD /c
# ADD CPP /nologo /G6 /W3 /Gm /GR /GX /ZI /I "..\dlls" /I "..\engine" /I "..\common" /I "..\pm_shared" /I "..\\" /I "../NNSim/" /I "../NNSim/SOM/" /D "NDEBUG" /D "_WIN32" /D "WIN32" /D "_WINDOWS" /D "__JOEBOTXPDLL" /D "__HLENGINE" /FAs /Fr /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /map /debug /machine:I386 /def:".\JoeBotXP.def" /out:".\Release/JoeBotXP.dll"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib gdi32.lib winspool.lib comdlg32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib user32.lib advapi32.lib winmm.lib /nologo /subsystem:windows /dll /map /debug /machine:I386 /def:".\JoeBotXP.def" /out:".\Release/JoeBotXP.dll"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
TargetPath=.\Release\JoeBotXP.dll
SOURCE="$(InputPath)"
PostBuild_Cmds=copy                                   $(TargetPath)                                   c:\spiele\half-life\joebot	copy                                   $(TargetPath)                                   m:\half-life\joebot
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "JoeBot - Win32 Release"
# Name "JoeBot - Win32 Debug"
# Name "JoeBot - Win32 DebugNM"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\Action.cpp
# End Source File
# Begin Source File

SOURCE=.\AStarBase.cpp
# End Source File
# Begin Source File

SOURCE=.\AStarBaseGoal.cpp
# End Source File
# Begin Source File

SOURCE=.\AStarGoalHide.cpp
# End Source File
# Begin Source File

SOURCE=.\AStarGoalMod.cpp
# End Source File
# Begin Source File

SOURCE=.\AStarGoalPFled.cpp
# End Source File
# Begin Source File

SOURCE=.\AStarMachine.cpp
# End Source File
# Begin Source File

SOURCE=.\AStarNode.cpp
# End Source File
# Begin Source File

SOURCE=.\AStarNodeContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\AStarTaskManager.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseBot.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTeamData.cpp
# End Source File
# Begin Source File

SOURCE=.\Behaviour.cpp
# End Source File
# Begin Source File

SOURCE=.\BitField.cpp
# End Source File
# Begin Source File

SOURCE=.\bot.cpp
# End Source File
# Begin Source File

SOURCE=.\bot_client.cpp
# End Source File
# Begin Source File

SOURCE=.\BotCS.cpp
# End Source File
# Begin Source File

SOURCE=.\BotDOD.cpp
# End Source File
# Begin Source File

SOURCE=.\BotHLDM.cpp
# End Source File
# Begin Source File

SOURCE=.\BV_CS.CPP
# End Source File
# Begin Source File

SOURCE=.\BV_HLDM.CPP
# End Source File
# Begin Source File

SOURCE=.\Command.cpp
# End Source File
# Begin Source File

SOURCE=.\Commandfunc.cpp
# End Source File
# Begin Source File

SOURCE=.\dll.cpp
# End Source File
# Begin Source File

SOURCE=.\Emotion.cpp
# End Source File
# Begin Source File

SOURCE=.\engine.cpp
# End Source File
# Begin Source File

SOURCE=.\Entity.cpp
# End Source File
# Begin Source File

SOURCE=.\Game.cpp
# End Source File
# Begin Source File

SOURCE=.\GameCS.cpp
# End Source File
# Begin Source File

SOURCE=.\GameDOD.cpp
# End Source File
# Begin Source File

SOURCE=.\GameHLDM.cpp
# End Source File
# Begin Source File

SOURCE=.\Goal.cpp
# End Source File
# Begin Source File

SOURCE=.\GoalFinder.cpp
# End Source File
# Begin Source File

SOURCE=.\h_export.cpp
# End Source File
# Begin Source File

SOURCE=.\HPTime.cpp
# End Source File
# Begin Source File

SOURCE=.\InterestingEntity.cpp
# End Source File
# Begin Source File

SOURCE=.\linkfunc.cpp
# End Source File
# Begin Source File

SOURCE=.\Log.cpp
# End Source File
# Begin Source File

SOURCE=.\Map.cpp
# End Source File
# Begin Source File

SOURCE=.\namefunc.cpp
# End Source File
# Begin Source File

SOURCE=.\Names.cpp
# End Source File
# Begin Source File

SOURCE=.\Observer.cpp
# End Source File
# Begin Source File

SOURCE=.\ObserverItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Path.cpp
# End Source File
# Begin Source File

SOURCE=.\Perceipt.cpp
# End Source File
# Begin Source File

SOURCE=.\Perception.cpp
# End Source File
# Begin Source File

SOURCE=.\Player.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerCS.cpp
# End Source File
# Begin Source File

SOURCE=.\PQNearWaypoint.cpp
# End Source File
# Begin Source File

SOURCE=.\Profiler.cpp
# End Source File
# Begin Source File

SOURCE=.\Radio.cpp
# End Source File
# Begin Source File

SOURCE=.\RespCurve.cpp
# End Source File
# Begin Source File

SOURCE=.\util.cpp
# End Source File
# Begin Source File

SOURCE=.\Way.cpp
# End Source File
# Begin Source File

SOURCE=.\Waypoint.cpp
# End Source File
# Begin Source File

SOURCE=.\WaypointGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\WeaponDefs.cpp
# End Source File
# Begin Source File

SOURCE=.\Weapons.cpp
# End Source File
# Begin Source File

SOURCE=.\WeaponsCS.cpp
# End Source File
# Begin Source File

SOURCE=.\WeaponsDOD.cpp
# End Source File
# Begin Source File

SOURCE=.\WeaponsHLDM.cpp
# End Source File
# Begin Source File

SOURCE=.\WPStat.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\Action.h
# End Source File
# Begin Source File

SOURCE=.\AStar.h
# End Source File
# Begin Source File

SOURCE=.\AStarBase.h
# End Source File
# Begin Source File

SOURCE=.\AStarBaseGoal.h
# End Source File
# Begin Source File

SOURCE=.\AStarGoalHide.h
# End Source File
# Begin Source File

SOURCE=.\AStarGoalMod.h
# End Source File
# Begin Source File

SOURCE=.\AStarGoalPFled.h
# End Source File
# Begin Source File

SOURCE=.\AStarMachine.h
# End Source File
# Begin Source File

SOURCE=.\AStarNode.h
# End Source File
# Begin Source File

SOURCE=.\AStarNodeContainer.h
# End Source File
# Begin Source File

SOURCE=.\AStarTaskManager.h
# End Source File
# Begin Source File

SOURCE=.\BaseBot.h
# End Source File
# Begin Source File

SOURCE=.\basemonster.h
# End Source File
# Begin Source File

SOURCE=.\BaseTeamData.h
# End Source File
# Begin Source File

SOURCE=.\Behaviour.h
# End Source File
# Begin Source File

SOURCE=.\BitField.h
# End Source File
# Begin Source File

SOURCE=.\bot.h
# End Source File
# Begin Source File

SOURCE=.\bot_client.h
# End Source File
# Begin Source File

SOURCE=.\bot_modid.h
# End Source File
# Begin Source File

SOURCE=.\BotCS.h
# End Source File
# Begin Source File

SOURCE=.\BotDOD.h
# End Source File
# Begin Source File

SOURCE=.\BotHLDM.h
# End Source File
# Begin Source File

SOURCE=.\BV_CS.H
# End Source File
# Begin Source File

SOURCE=.\BV_HLDM.H
# End Source File
# Begin Source File

SOURCE=.\Callback.h
# End Source File
# Begin Source File

SOURCE=.\Command.h
# End Source File
# Begin Source File

SOURCE=.\Commandfunc.h
# End Source File
# Begin Source File

SOURCE=.\custom_time.h
# End Source File
# Begin Source File

SOURCE=.\Emotion.h
# End Source File
# Begin Source File

SOURCE=.\engine.h
# End Source File
# Begin Source File

SOURCE=.\enginecallback.h
# End Source File
# Begin Source File

SOURCE=.\Entity.h
# End Source File
# Begin Source File

SOURCE=.\extdll.h
# End Source File
# Begin Source File

SOURCE=.\Game.h
# End Source File
# Begin Source File

SOURCE=.\GameCS.h
# End Source File
# Begin Source File

SOURCE=.\GameDOD.h
# End Source File
# Begin Source File

SOURCE=.\GameHLDM.h
# End Source File
# Begin Source File

SOURCE=.\globaldefs.h
# End Source File
# Begin Source File

SOURCE=.\globalfunc.h
# End Source File
# Begin Source File

SOURCE=.\globalvars.h
# End Source File
# Begin Source File

SOURCE=.\Goal.h
# End Source File
# Begin Source File

SOURCE=.\GoalFinder.h
# End Source File
# Begin Source File

SOURCE=.\HPTime.h
# End Source File
# Begin Source File

SOURCE=.\InterestingEntity.h
# End Source File
# Begin Source File

SOURCE=.\LListElem.h
# End Source File
# Begin Source File

SOURCE=.\Log.h
# End Source File
# Begin Source File

SOURCE=.\Map.h
# End Source File
# Begin Source File

SOURCE=.\MemoryPool.h
# End Source File
# Begin Source File

SOURCE=.\monsterevent.h
# End Source File
# Begin Source File

SOURCE=.\Names.h
# End Source File
# Begin Source File

SOURCE=.\Observer.h
# End Source File
# Begin Source File

SOURCE=.\ObserverItem.h
# End Source File
# Begin Source File

SOURCE=.\Path.h
# End Source File
# Begin Source File

SOURCE=.\Perceipt.h
# End Source File
# Begin Source File

SOURCE=.\Perception.h
# End Source File
# Begin Source File

SOURCE=.\Player.h
# End Source File
# Begin Source File

SOURCE=.\PlayerCS.h
# End Source File
# Begin Source File

SOURCE=.\PQNearWaypoint.h
# End Source File
# Begin Source File

SOURCE=.\PQueue.h
# End Source File
# Begin Source File

SOURCE=.\PQueueHash.h
# End Source File
# Begin Source File

SOURCE=.\Profiler.h
# End Source File
# Begin Source File

SOURCE=.\Radio.h
# End Source File
# Begin Source File

SOURCE=.\RespCurve.h
# End Source File
# Begin Source File

SOURCE=.\vector.h
# End Source File
# Begin Source File

SOURCE=.\Way.h
# End Source File
# Begin Source File

SOURCE=.\Waypoint.h
# End Source File
# Begin Source File

SOURCE=.\WaypointGraph.h
# End Source File
# Begin Source File

SOURCE=.\WeaponDefs.h
# End Source File
# Begin Source File

SOURCE=.\Weapons.h
# End Source File
# Begin Source File

SOURCE=.\WeaponsCS.h
# End Source File
# Begin Source File

SOURCE=.\WeaponsDOD.h
# End Source File
# Begin Source File

SOURCE=.\WeaponsHLDM.h
# End Source File
# Begin Source File

SOURCE=.\WPStat.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\notes.txt
# End Source File
# Begin Source File

SOURCE=..\docs\progress.txt
# End Source File
# End Target
# End Project

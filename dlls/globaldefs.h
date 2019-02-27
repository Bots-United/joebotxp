#ifndef __GLOBALDEFS_H
#define __GLOBALDEFS_H

//#ifdef _DEBUG
#define DEBUGMESSAGES
//#endif

#define CRASH *((long*)0)=0

#define LADDER_UNKNOWN  0
#define LADDER_UP       1
#define LADDER_DOWN     2

#define WANDER_LEFT  1
#define WANDER_RIGHT 2

#define BOT_PITCH_SPEED 20
#define BOT_YAW_SPEED 20

#define RESPAWN_IDLE             1
#define RESPAWN_NEED_TO_RESPAWN  2
#define RESPAWN_IS_RESPAWNING    3

// game start messages for TFC...
#define MSG_TFC_IDLE          1
#define MSG_TFC_TEAM_SELECT   2
#define MSG_TFC_CLASS_SELECT  3

// game start messages for CS...
#define MSG_CS_IDLE         1
#define MSG_CS_TEAM_SELECT  2
#define MSG_CS_CT_SELECT    3
#define MSG_CS_T_SELECT     4

// game start messages for OpFor...
#define MSG_OPFOR_IDLE          1
#define MSG_OPFOR_TEAM_SELECT   2
#define MSG_OPFOR_CLASS_SELECT  3

// game start messages for FrontLineForce...
#define MSG_FLF_IDLE            1
#define MSG_FLF_TEAM_SELECT     2
#define MSG_FLF_CLASS_SELECT    3
#define MSG_FLF_PISTOL_SELECT   4
#define MSG_FLF_WEAPON_SELECT   5
#define MSG_FLF_RIFLE_SELECT    6
#define MSG_FLF_SHOTGUN_SELECT  7
#define MSG_FLF_SUBMACHINE_SELECT   8
#define MSG_FLF_HEAVYWEAPONS_SELECT 9


#define TFC_CLASS_CIVILIAN  0
#define TFC_CLASS_SCOUT     1
#define TFC_CLASS_SNIPER    2
#define TFC_CLASS_SOLDIER   3
#define TFC_CLASS_DEMOMAN   4
#define TFC_CLASS_MEDIC     5
#define TFC_CLASS_HWGUY     6
#define TFC_CLASS_PYRO      7
#define TFC_CLASS_SPY       8
#define TFC_CLASS_ENGINEER  9



// game start messages for DOD...
#define MSG_DOD_IDLE					1
#define MSG_DOD_TEAM_SELECT				2
#define MSG_DOD_CLASS_SELECT_AX			3
#define MSG_DOD_CLASS_SELECT_AL			4
#define MSG_DOD_CLASS_SELECT_AX_PARA	5
#define MSG_DOD_CLASS_SELECT_AL_PARA	6
#define MSG_DOD_WPN_SELECT_SERGEANT		7
#define MSG_DOD_WPN_SELECT_MACHINE		8
#define MSG_DOD_WPN_SELECT_GRENADIER	9
#define MSG_DOD_WPN_SELECT_RIFLEMAN		10
#define MSG_DOD_WPN_SELECT_FG42			11

#define DOD_CLASS_ASSAULTINF		1
#define DOD_CLASS_LIGHTINF  		2
#define DOD_CLASS_SNIPER    		3
#define DOD_CLASS_HEAVYWEAPONS  	4
#define DOD_CLASS_LEICHTINF			5
#define DOD_CLASS_STURMTRUP			6
#define DOD_CLASS_GERMANSNIPER		7
#define DOD_CLASS_HEAVYWEAPONSGER	8


#define BOT_SKIN_LEN 32
#define BOT_NAME_LEN 32

#endif


// JoeBot
// by Johannes Lampel
// Johannes.Lampel@gmx.de
// http://joebot.counter-strike.de

//
// HPB_bot - botman's High Ping Bastard bot
//
// (http://planethalflife.com/botman/)
//
// bot.h
//

#ifndef BOT_H
#define BOT_H

//#define __LOG

#include "limits.h"
#include "Waypoint.h"
#include "Weapons.h"

// stuff for Win32 vs. Linux builds

#ifndef __linux__

typedef int (FAR *GETENTITYAPI)(DLL_FUNCTIONS *, int);
typedef int (FAR *GETNEWDLLFUNCTIONS)(NEW_DLL_FUNCTIONS *, int *);
typedef void (DLLEXPORT *GIVEFNPTRSTODLL)(enginefuncs_t *, globalvars_t *);
typedef void (FAR *LINK_ENTITY_FUNC)(entvars_t *);

#else

#include <dlfcn.h>
#define GetProcAddress dlsym

typedef int BOOL;

typedef int (*GETENTITYAPI)(DLL_FUNCTIONS *, int);
typedef int (*GETNEWDLLFUNCTIONS)(NEW_DLL_FUNCTIONS *, int *);
typedef void (*GIVEFNPTRSTODLL)(enginefuncs_t *, globalvars_t *);
typedef void (*LINK_ENTITY_FUNC)(entvars_t *);

#endif

edict_t *CREATE_FAKE_CLIENT( const char *netname );
char *GET_INFOBUFFER( edict_t *e );
char *GET_INFO_KEY_VALUE( char *infobuffer, char *key );
void SET_CLIENT_KEY_VALUE( int clientIndex, char *infobuffer, char *key, char *value );
//extern void player( entvars_t *pev );

// define some function prototypes...
BOOL ClientConnect( edict_t *pEntity, const char *pszName,
				   const char *pszAddress, char szRejectReason[ 128 ] );
void ClientPutInServer( edict_t *pEntity );
void ClientCommand( edict_t *pEntity );

void FakeClientCommand(edict_t *pBot, char *arg1, char *arg2, char *arg3);

const char *Cmd_Args( void );
const char *Cmd_Argv( int argc );
int Cmd_Argc( void );

//extern char szTemp[100];

#include "bot_modid.h"

#include "Radio.h"

#include "globaldefs.h"

// new UTIL.CPP functions...
//edict_t *UTIL_FindEntityInSphere( edict_t *pentStart, const Vector &vecCenter, float flRadius );
//edict_t *UTIL_FindEntityByString( edict_t *pentStart, const char *szKeyword, const char *szValue );
//edict_t *UTIL_FindEntityByClassname( edict_t *pentStart, const char *szName );
//edict_t *UTIL_FindEntityByTargetname( edict_t *pentStart, const char *szName );


void UTIL_SayText( const char *pText, edict_t *pEdict );
int UTIL_GetTeam(edict_t *pEntity);
bool UTIL_IsVIP(edict_t *pEntity);
//bot_t *UTIL_GetBotPointer(edict_t *pEdict);
bool IsAlive(edict_t *pEdict);
bool TEq(float f1,float f2,float fD);

//bool FVisibleEx(bot_t *pBot,const Vector &vecOrigin, edict_t *pEdict );
Vector Center(edict_t *pEdict);
Vector GetGunPosition(edict_t *pEdict);
void UTIL_SelectItem(edict_t *pEdict, char *item_name);
Vector VecBModelOrigin(edict_t *pEdict);
bool UpdateSounds(edict_t *pEdict, edict_t *pPlayer);
void UTIL_ShowMenu( edict_t *pEdict, int slots, int displaytime, bool needmore, char *pText );

#include "globalvars.h"
#include "globalfunc.h"

#endif // BOT_H

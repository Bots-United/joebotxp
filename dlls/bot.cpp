// JoeBot
// by Johannes Lampel
// Johannes.Lampel@gmx.de
// http://joebot.counter-strike.de

//
// HPB bot - botman's High Ping Bastard bot
//
// (http://planethalflife.com/botman/)
//
// bot.cpp
//

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "weaponinfo.h"

#include "bot.h"
#include "BaseBot.h"
#include "bot_modid.h"

char szTemp[200];

#include <sys/types.h>
#include <sys/stat.h>

static FILE *fp;

int   need_init = 1;

#define VALVE_MAX_SKINS    10
#define GEARBOX_MAX_SKINS  20

// indicate which models are currently used for random model allocation
bool valve_skin_used[VALVE_MAX_SKINS] = {
   FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE};

bool gearbox_skin_used[GEARBOX_MAX_SKINS] = {
   FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
   FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE};

// store the names of the models...
char *valve_bot_skins[VALVE_MAX_SKINS] = {
   "barney", "gina", "gman", "gordon", "helmet",
   "hgrunt", "recon", "robo", "scientist", "zombie"};

char *gearbox_bot_skins[GEARBOX_MAX_SKINS] = {
   "barney", "beret", "cl_suit", "drill", "fassn", "gina", "gman",
   "gordon", "grunt", "helmet", "hgrunt", "massn", "otis", "recon",
   "recruit", "robo", "scientist", "shepard", "tower", "zombie"};

#ifndef __linux__
extern HINSTANCE h_Library;
#else
extern void * h_Library;
#endif

edict_t *CREATE_FAKE_CLIENT( const char *netname )
{
	return (*g_engfuncs.pfnCreateFakeClient)( netname );
}

char *GET_INFOBUFFER( edict_t *e )
{
	return (*g_engfuncs.pfnGetInfoKeyBuffer)( e );
}

char *GET_INFO_KEY_VALUE( char *infobuffer, char *key )
{
	return (g_engfuncs.pfnInfoKeyValue( infobuffer, key ));
}

void SET_CLIENT_KEY_VALUE( int clientIndex, char *infobuffer, char *key, char *value )
{
	(*g_engfuncs.pfnSetClientKeyValue)( clientIndex, infobuffer, key, value );
}

//extern "C"
//{
// this is the LINK_ENTITY_TO_CLASS function that creates a player (bot)
//void player(entvars_t *pev);
//}

void player( entvars_t *pev )
{
	static LINK_ENTITY_FUNC otherClassName = NULL;
	if (otherClassName == NULL)
		otherClassName = (LINK_ENTITY_FUNC)GetProcAddress(h_Library, "player");
	if (otherClassName != NULL){
		(*otherClassName)(pev);
	}
}

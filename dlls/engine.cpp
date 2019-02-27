// JoeBot
// by Johannes Lampel
// Johannes.Lampel@gmx.de
// http://joebot.counter-strike.de

//
// HPB bot - botman's High Ping Bastard bot
//
// (http://planethalflife.com/botman/)
//
// engine.cpp
//

#include "extdll.h"
#include "util.h"

#include "bot.h"
#include "bot_client.h"
#include "engine.h"

#include "bot_modid.h"

#include "BaseBot.h"
#include "Game.h"
#include "Action.h"

#include <iostream>
using namespace std;

extern enginefuncs_t g_engfuncs;
//extern bot_t bots[32];
extern int mod_id;

edict_t *g_pClientCallEdict;

int debug_engine = 0;

void (*botMsgFunction)(void *, int,int) = NULL;
int botMsgIndex;

// messages created in RegUserMsg which will be "caught"
int message_VGUI = 0;
int message_ShowMenu = 0;
int message_WeaponList = 0;
int message_CurWeapon = 0;
int message_AmmoX = 0;
int message_WeapPickup = 0;
int message_AmmoPickup = 0;
int message_ItemPickup = 0;
int message_Health = 0;
int message_Battery = 0;	// Armor
int message_Damage = 0;
int message_Money = 0;		// for Counter-Strike
int message_StatusI = 0;
int message_Roundtime = 0;
int message_TextMsg = 0;
int message_StatusV = 0;
int message_SetFOV = 0;
int message_ScreenFade = 0;
int message_DeathMsg = 0;
int message_Stamina = 0;		// dod
int message_Slowed = 0;			// dod
int message_Speed = 0;
int message_Bleeding = 0;
int message_Flashlight = 0;
int message_Object = 0;			// dod
int message_SayText = 0;
int message_TeamInfo = 0;

static FILE *fp;

#if _DEBUG
#define DEBUGENGINE
#endif

int pfnPrecacheModel(char* s)
{
#ifdef DEBUGENGINE
	//if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnPrecacheModel: %s\n",s); fclose(fp); }
#endif
	return (*g_engfuncs.pfnPrecacheModel)(s);
}
int pfnPrecacheSound(char* s)
{
#ifdef DEBUGENGINE
	//if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnPrecacheSound: %s\n",s); fclose(fp); }
#endif
	return (*g_engfuncs.pfnPrecacheSound)(s);
}
void pfnSetModel(edict_t *e, const char *m)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnSetModel: edict=%x %s\n",e,m); fclose(fp); }
#endif
	(*g_engfuncs.pfnSetModel)(e, m);
}
int pfnModelIndex(const char *m)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnModelIndex: %s\n",m); fclose(fp); }
	return (*g_engfuncs.pfnModelIndex)(m);
}
int pfnModelFrames(int modelIndex)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnModelFrames:\n"); fclose(fp); }
#endif
	return (*g_engfuncs.pfnModelFrames)(modelIndex);
}
void pfnSetSize(edict_t *e, const float *rgflMin, const float *rgflMax)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnSetSize: %x\n",e); fclose(fp); }
#endif
	(*g_engfuncs.pfnSetSize)(e, rgflMin, rgflMax);
}
void pfnChangeLevel(char* s1, char* s2)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnChangeLevel:\n"); fclose(fp); }
#endif
	
	// kick any bot off of the server after time/frag limit...
	for (int index = 0; index < g_pGame->m_iMaxCurrentPlayer; index++)
	{
		if (g_pGame->getBot(index))  // is this slot used?
		{
			char cmd[40];
			
			sprintf(cmd, "kick \"%s\"\n", g_pGame->getBot(index)->m_name);
			
			g_pGame->SBInfo[index].respawn_state = RESPAWN_NEED_TO_RESPAWN;
			
			SERVER_COMMAND(cmd);  // kick the bot using (kick "name")
		}
	}
	
	(*g_engfuncs.pfnChangeLevel)(s1, s2);
}
void pfnGetSpawnParms(edict_t *ent)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetSpawnParms:\n"); fclose(fp); }
#endif
	(*g_engfuncs.pfnGetSpawnParms)(ent);
}
void pfnSaveSpawnParms(edict_t *ent)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnSaveSpawnParms:\n"); fclose(fp); }
#endif
	(*g_engfuncs.pfnSaveSpawnParms)(ent);
}
float pfnVecToYaw(const float *rgflVector)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnVecToYaw:\n"); fclose(fp); }
	return (*g_engfuncs.pfnVecToYaw)(rgflVector);
}
void pfnVecToAngles(const float *rgflVectorIn, float *rgflVectorOut)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnVecToAngles:\n"); fclose(fp); }
	(*g_engfuncs.pfnVecToAngles)(rgflVectorIn, rgflVectorOut);
}
void pfnMoveToOrigin(edict_t *ent, const float *pflGoal, float dist, int iMoveType)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnMoveToOrigin:\n"); fclose(fp); }
#endif
	(*g_engfuncs.pfnMoveToOrigin)(ent, pflGoal, dist, iMoveType);
}
void pfnChangeYaw(edict_t* ent)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnChangeYaw:\n"); fclose(fp); }
	(*g_engfuncs.pfnChangeYaw)(ent);
}
void pfnChangePitch(edict_t* ent)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnChangePitch:\n"); fclose(fp); }
	(*g_engfuncs.pfnChangePitch)(ent);
}
edict_t* pfnFindEntityByString(edict_t *pEdictStartSearchAfter, const char *pszField, const char *pszValue)
{
	if (!strcmp(pszValue, "info_map_parameters") && (mod_id == CSTRIKE_DLL)){
		//a new round in cs has started
		g_pGame->onRoundStart();
	}
	
	//if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnFindEntityByString: %s\n",pszValue); fclose(fp); }
	return (*g_engfuncs.pfnFindEntityByString)(pEdictStartSearchAfter, pszField, pszValue);
}
int pfnGetEntityIllum(edict_t* pEnt)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetEntityIllum:\n"); fclose(fp); }
#endif
	return (*g_engfuncs.pfnGetEntityIllum)(pEnt);
}
edict_t* pfnFindEntityInSphere(edict_t *pEdictStartSearchAfter, const float *org, float rad)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnFindEntityInSphere:\n"); fclose(fp); }
#endif
	return (*g_engfuncs.pfnFindEntityInSphere)(pEdictStartSearchAfter, org, rad);
}
edict_t* pfnFindClientInPVS(edict_t *pEdict)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnFindClientInPVS:\n"); fclose(fp); }
#endif
	return (*g_engfuncs.pfnFindClientInPVS)(pEdict);
}
edict_t* pfnEntitiesInPVS(edict_t *pplayer)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnEntitiesInPVS:\n"); fclose(fp); }
#endif
	return (*g_engfuncs.pfnEntitiesInPVS)(pplayer);
}
void pfnMakeVectors(const float *rgflVector)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnMakeVectors:\n"); fclose(fp); }
	(*g_engfuncs.pfnMakeVectors)(rgflVector);
}
void pfnAngleVectors(const float *rgflVector, float *forward, float *right, float *up)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnAngleVectors:\n"); fclose(fp); }
	(*g_engfuncs.pfnAngleVectors)(rgflVector, forward, right, up);
}
edict_t* pfnCreateEntity(void)
{
	edict_t *pent = (*g_engfuncs.pfnCreateEntity)();
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCreateEntity: %x\n",pent); fclose(fp); }
#endif
	return pent;
}
void pfnRemoveEntity(edict_t* e)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnRemoveEntity: %x\n",e); fclose(fp); }
#ifdef DEBUGENGINE
	if (debug_engine)
	{
		fp=fopen("bot.txt","a");
		fprintf(fp,"pfnRemoveEntity: %x\n",e);
		if (e->v.model != 0)
			fprintf(fp," model=%s\n", STRING(e->v.model));
		fclose(fp);
	}
#endif
	
	(*g_engfuncs.pfnRemoveEntity)(e);
}
edict_t* pfnCreateNamedEntity(int className)
{
	edict_t *pent = (*g_engfuncs.pfnCreateNamedEntity)(className);
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCreateNamedEntity: edict=%x name=%s\n",pent,STRING(className)); fclose(fp); }
#endif
	return pent;
}
void pfnMakeStatic(edict_t *ent)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnMakeStatic:\n"); fclose(fp); }
#endif
	(*g_engfuncs.pfnMakeStatic)(ent);
}
int pfnEntIsOnFloor(edict_t *e)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnEntIsOnFloor:\n"); fclose(fp); }
#endif
	return (*g_engfuncs.pfnEntIsOnFloor)(e);
}
int pfnDropToFloor(edict_t* e)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnDropToFloor:\n"); fclose(fp); }
#endif
	return (*g_engfuncs.pfnDropToFloor)(e);
}
int pfnWalkMove(edict_t *ent, float yaw, float dist, int iMode)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnWalkMove:\n"); fclose(fp); }
#endif
	return (*g_engfuncs.pfnWalkMove)(ent, yaw, dist, iMode);
}
void pfnSetOrigin(edict_t *e, const float *rgflOrigin)
{
#ifdef DEBUGENGINE
	//if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnSetOrigin:\n"); fclose(fp); }
#endif
	(*g_engfuncs.pfnSetOrigin)(e, rgflOrigin);
}
void pfnEmitSound(edict_t *entity, int channel, const char *sample, /*int*/float volume, float attenuation, int fFlags, int pitch)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnEmitSound:%i;%s\n",channel,sample); fclose(fp); }
#endif
	(*g_engfuncs.pfnEmitSound)(entity, channel, sample, volume, attenuation, fFlags, pitch);
}
void pfnEmitAmbientSound(edict_t *entity, float *pos, const char *samp, float vol, float attenuation, int fFlags, int pitch)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnEmitAmbientSound:\n"); fclose(fp); }
#endif
	(*g_engfuncs.pfnEmitAmbientSound)(entity, pos, samp, vol, attenuation, fFlags, pitch);
}
void pfnTraceLine(const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnTraceLine:\n"); fclose(fp); }
	(*g_engfuncs.pfnTraceLine)(v1, v2, fNoMonsters, pentToSkip, ptr);
}
void pfnTraceToss(edict_t* pent, edict_t* pentToIgnore, TraceResult *ptr)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnTraceToss:\n"); fclose(fp); }
	(*g_engfuncs.pfnTraceToss)(pent, pentToIgnore, ptr);
}
int pfnTraceMonsterHull(edict_t *pEdict, const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnTraceMonsterHull:\n"); fclose(fp); }
	return (*g_engfuncs.pfnTraceMonsterHull)(pEdict, v1, v2, fNoMonsters, pentToSkip, ptr);
}
void pfnTraceHull(const float *v1, const float *v2, int fNoMonsters, int hullNumber, edict_t *pentToSkip, TraceResult *ptr)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnTraceHull:\n"); fclose(fp); }
	(*g_engfuncs.pfnTraceHull)(v1, v2, fNoMonsters, hullNumber, pentToSkip, ptr);
}
void pfnTraceModel(const float *v1, const float *v2, int hullNumber, edict_t *pent, TraceResult *ptr)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnTraceModel:\n"); fclose(fp); }
	(*g_engfuncs.pfnTraceModel)(v1, v2, hullNumber, pent, ptr);
}
const char *pfnTraceTexture(edict_t *pTextureEntity, const float *v1, const float *v2 )
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnTraceTexture:\n"); fclose(fp); }
	return (*g_engfuncs.pfnTraceTexture)(pTextureEntity, v1, v2);
}
void pfnTraceSphere(const float *v1, const float *v2, int fNoMonsters, float radius, edict_t *pentToSkip, TraceResult *ptr)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnTraceSphere:\n"); fclose(fp); }
	(*g_engfuncs.pfnTraceSphere)(v1, v2, fNoMonsters, radius, pentToSkip, ptr);
}
void pfnGetAimVector(edict_t* ent, float speed, float *rgflReturn)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetAimVector:\n"); fclose(fp); }
	(*g_engfuncs.pfnGetAimVector)(ent, speed, rgflReturn);
}
void pfnServerCommand(char* str)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnServerCommand: %s\n",str); fclose(fp); }
#endif
	(*g_engfuncs.pfnServerCommand)(str);
}
void pfnServerExecute(void)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnServerExecute:\n"); fclose(fp); }
#endif
	(*g_engfuncs.pfnServerExecute)();
}
void pfnClientCommand(edict_t* pEdict, char* szFmt, ...)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnClientCommand=%s\n",szFmt); fclose(fp); }
#endif
	
	/*if (!(pEdict->v.flags & FL_FAKECLIENT))
	{
	char tempFmt[256];
	va_list argp;
	va_start(argp, szFmt);
	vsprintf(tempFmt, szFmt, argp);
	(*g_engfuncs.pfnClientCommand)(pEdict, tempFmt);
	va_end(argp);
}*/
	return;
}
void pfnParticleEffect(const float *org, const float *dir, float color, float count)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnParticleEffect:\n"); fclose(fp); }
	(*g_engfuncs.pfnParticleEffect)(org, dir, color, count);
}
void pfnLightStyle(int style, char* val)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnLightStyle:\n"); fclose(fp); }
	(*g_engfuncs.pfnLightStyle)(style, val);
}
int pfnDecalIndex(const char *name)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnDecalIndex:\n"); fclose(fp); }
	return (*g_engfuncs.pfnDecalIndex)(name);
}
int pfnPointContents(const float *rgflVector)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnPointContents:\n"); fclose(fp); }
	return (*g_engfuncs.pfnPointContents)(rgflVector);
}
void pfnMessageBegin(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed)
{
	if (gpGlobals->deathmatch)
	{
		int index = -1;
#ifdef DEBUGENGINE
		if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnMessageBegin: edict=%x dest=%d type=%d\n",ed,msg_dest,msg_type); fclose(fp); }
#endif
		
		if (ed)
		{
			index = g_pGame->isBot(ed);
			
			// is this message for a bot?
			if (index != -1)
			{
				botMsgFunction = NULL;  // no msg function until known otherwise
				botMsgIndex = index;    // index of bot receiving message

				g_pClientCallEdict = ed;
				
				if (mod_id == VALVE_DLL)
				{
					if (msg_type == message_WeaponList)
						botMsgFunction = BotClient_Valve_WeaponList;
					else if (msg_type == message_CurWeapon)
						botMsgFunction = BotClient_Valve_CurrentWeapon;
					else if (msg_type == message_AmmoX)
						botMsgFunction = BotClient_Valve_AmmoX;
					else if (msg_type == message_AmmoPickup)
						botMsgFunction = BotClient_Valve_AmmoPickup;
					else if (msg_type == message_WeapPickup)
						botMsgFunction = BotClient_Valve_WeaponPickup;
					else if (msg_type == message_ItemPickup)
						botMsgFunction = BotClient_Valve_ItemPickup;
					else if (msg_type == message_Health)
						botMsgFunction = BotClient_Valve_Health;
					else if (msg_type == message_Battery)
						botMsgFunction = BotClient_Valve_Battery;
					else if (msg_type == message_Damage)
						botMsgFunction = BotClient_Valve_Damage;
					else if (msg_type == message_SetFOV)
						botMsgFunction = BotClient_Valve_SetFOV;
				}
				else if (mod_id == CSTRIKE_DLL)
				{
					if (msg_type == message_VGUI)
						botMsgFunction = BotClient_CS_VGUI;
					else if (msg_type == message_ShowMenu)
						botMsgFunction = BotClient_CS_ShowMenu;
					else if (msg_type == message_WeaponList)
						botMsgFunction = BotClient_CS_WeaponList;
					else if (msg_type == message_CurWeapon)
						botMsgFunction = BotClient_CS_CurrentWeapon;
					else if (msg_type == message_AmmoX)
						botMsgFunction = BotClient_CS_AmmoX;
					else if (msg_type == message_WeapPickup)
						botMsgFunction = BotClient_CS_WeaponPickup;
					else if (msg_type == message_AmmoPickup)
						botMsgFunction = BotClient_CS_AmmoPickup;
					else if (msg_type == message_ItemPickup)
						botMsgFunction = BotClient_CS_ItemPickup;
					else if (msg_type == message_Health)
						botMsgFunction = BotClient_CS_Health;
					else if (msg_type == message_Battery)
						botMsgFunction = BotClient_CS_Battery;
					else if (msg_type == message_Damage)
						botMsgFunction = BotClient_CS_Damage;
					else if (msg_type == message_Money)
						botMsgFunction = BotClient_CS_Money;
					else if (msg_type == message_StatusI)
						botMsgFunction = BotClient_CS_StatusI;
					else if (msg_type == message_Roundtime)
						botMsgFunction = BotClient_CS_Roundtime;
						/*else if (msg_type == message_StatusV)
					botMsgFunction = BotClient_CS_StatusV;*/
					else if (msg_type == message_SetFOV)
						botMsgFunction = BotClient_CS_SetFOV;
					else if (msg_type == message_ScreenFade)
						botMsgFunction = BotClient_CS_ScreenFade;
					else if (msg_type == message_TextMsg)
						botMsgFunction = BotClient_CS_TextMsg;
					else if (msg_type == message_Flashlight)
						botMsgFunction = BotClient_CS_Flashlight;
					else if (msg_type == message_SayText)
						botMsgFunction = BotClient_CS_SayText;
					else if (msg_type == message_TeamInfo)
						botMsgFunction = BotClient_CS_TeamInfo;
				}
				else if (mod_id == DOD_DLL)
				{
					if (msg_type == message_VGUI)
						botMsgFunction = BotClient_DOD_VGUI;
					else if (msg_type == message_WeaponList)
						botMsgFunction = BotClient_DOD_WeaponList;
					else if (msg_type == message_CurWeapon)
						botMsgFunction = BotClient_DOD_CurrentWeapon;
					else if (msg_type == message_AmmoX)
						botMsgFunction = BotClient_DOD_AmmoX;
					else if (msg_type == message_AmmoPickup)
						botMsgFunction = BotClient_DOD_AmmoPickup;
					else if (msg_type == message_WeapPickup)
						botMsgFunction = BotClient_DOD_WeaponPickup;
					else if (msg_type == message_ItemPickup)
						botMsgFunction = BotClient_DOD_ItemPickup;
					else if (msg_type == message_Health)
						botMsgFunction = BotClient_DOD_Health;
					else if (msg_type == message_Battery)
						botMsgFunction = BotClient_DOD_Battery;
					else if (msg_type == message_Damage)
						botMsgFunction = BotClient_DOD_Damage;
					else if (msg_type == message_ScreenFade)
						botMsgFunction = BotClient_DOD_ScreenFade;
					else if (msg_type == message_Stamina)
						botMsgFunction = BotClient_DOD_Stamina;
					else if (msg_type == message_Slowed)
						botMsgFunction = BotClient_DOD_Slowed;
					else if (msg_type == message_DeathMsg)
						botMsgFunction = BotClient_DOD_DeathMsg;
					else if (msg_type == message_Speed)
						botMsgFunction = BotClient_DOD_Speed;
					else if (msg_type == message_Speed)
						botMsgFunction = BotClient_DOD_Bleeding;
					else if (msg_type == message_Object)
						botMsgFunction = BotClient_DOD_Object;
					else if (msg_type == message_SetFOV)
						botMsgFunction = BotClient_DOD_SetFOV;
					else if (msg_type == message_TeamInfo)
						botMsgFunction = BotClient_CS_TeamInfo;
				}
				
				/*else if (mod_id == GEARBOX_DLL)
				{
				if (msg_type == message_WeaponList)
				botMsgFunction = BotClient_Gearbox_WeaponList;
				else if (msg_type == message_CurWeapon)
				botMsgFunction = BotClient_Gearbox_CurrentWeapon;
				else if (msg_type == message_AmmoX)
				botMsgFunction = BotClient_Gearbox_AmmoX;
				else if (msg_type == message_AmmoPickup)
				botMsgFunction = BotClient_Gearbox_AmmoPickup;
				else if (msg_type == message_WeapPickup)
				botMsgFunction = BotClient_Gearbox_WeaponPickup;
				else if (msg_type == message_ItemPickup)
				botMsgFunction = BotClient_Gearbox_ItemPickup;
				else if (msg_type == message_Health)
				botMsgFunction = BotClient_Gearbox_Health;
				else if (msg_type == message_Battery)
				botMsgFunction = BotClient_Gearbox_Battery;
				else if (msg_type == message_Damage)
				botMsgFunction = BotClient_Gearbox_Damage;
				}*/
         }
		 else{
			 // it's no a bot, but maybe we wanna know what this is, too ?!

			 botMsgFunction = NULL;  // no msg function until known otherwise
			 botMsgIndex = -1;    // no index of bot receiving message
			 g_pClientCallEdict = ed;

			 if (mod_id == VALVE_DLL)
			 {
				 if (msg_type == message_Damage)
					 botMsgFunction = BotClient_Valve_Damage;
				 else if (msg_type == message_CurWeapon)
					 botMsgFunction = BotClient_Valve_CurrentWeapon;
			 }
			 else if (mod_id == CSTRIKE_DLL)
			 {
				 if (msg_type == message_Damage)
					 botMsgFunction = BotClient_CS_Damage;
				 else if (msg_type == message_CurWeapon)
					 botMsgFunction = BotClient_CS_CurrentWeapon;
			 }
			 else if (mod_id == DOD_DLL)
			 {
				 if (msg_type == message_Damage)
					 botMsgFunction = BotClient_DOD_Damage;
				 else if (msg_type == message_CurWeapon)
					 botMsgFunction = BotClient_DOD_CurrentWeapon;
			 }
		 }
      }
	  else{	// message to all
		  botMsgFunction = NULL;  // no msg function until known otherwise
		  botMsgIndex = -1;       // index of bot receiving message (none)

		  g_pClientCallEdict = ed;
		  
		  if (mod_id == CSTRIKE_DLL)
		  {
			  if (msg_type == message_DeathMsg)
				  botMsgFunction = BotClient_CS_DeathMsg;
			  else if (msg_type == message_TextMsg)
				  botMsgFunction = BotClient_CS_TextMsg;
		  }
		  else if (mod_id == DOD_DLL)
		  {
		  }
	  }
   }
   
   (*g_engfuncs.pfnMessageBegin)(msg_dest, msg_type, pOrigin, ed);
}

void pfnMessageEnd(void)
{
	if (gpGlobals->deathmatch)
	{
#ifdef DEBUGENGINE
		if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnMessageEnd:\n"); fclose(fp); }
#endif
		
		// if this message is for a bot, call the client message function...
		if (botMsgFunction)
			(*botMsgFunction)((void *)0, botMsgIndex,_CLIENT_END);
		
		// clear out the bot message function pointer...
		botMsgFunction = NULL;
	}
	
	(*g_engfuncs.pfnMessageEnd)();
}

void pfnWriteByte(int iValue)
{
	if (gpGlobals->deathmatch)
	{
#ifdef DEBUGENGINE
		if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnWriteByte: %d\n",iValue); fclose(fp); }
#endif
		
		// if this message is for a bot, call the client message function...
		if (botMsgFunction)
			(*botMsgFunction)((void *)&iValue, botMsgIndex,_CLIENT_BYTE);
	}
	
	(*g_engfuncs.pfnWriteByte)(iValue);
}
void pfnWriteChar(int iValue)
{
	if (gpGlobals->deathmatch)
	{
#ifdef DEBUGENGINE
		if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnWriteChar: %d\n",iValue); fclose(fp); }
#endif
		
		// if this message is for a bot, call the client message function...
		if (botMsgFunction)
			(*botMsgFunction)((void *)&iValue, botMsgIndex,_CLIENT_CHAR);
	}
	
	(*g_engfuncs.pfnWriteChar)(iValue);
}
void pfnWriteShort(int iValue)
{
	if (gpGlobals->deathmatch)
	{
#ifdef DEBUGENGINE
		if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"prnWriteShort: %d\n",iValue); fclose(fp); }
#endif
		
		// if this message is for a bot, call the client message function...
		if (botMsgFunction)
			(*botMsgFunction)((void *)&iValue, botMsgIndex,_CLIENT_SHORT);
	}
	
	(*g_engfuncs.pfnWriteShort)(iValue);
}
void pfnWriteLong(int iValue)
{
	if (gpGlobals->deathmatch)
	{
#ifdef DEBUGENGINE
		if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnWriteLong: %d\n",iValue); fclose(fp); }
#endif
		
		// if this message is for a bot, call the client message function...
		if (botMsgFunction)
			(*botMsgFunction)((void *)&iValue, botMsgIndex,_CLIENT_LONG);
	}
	
	(*g_engfuncs.pfnWriteLong)(iValue);
}
void pfnWriteAngle(float flValue)
{
	if (gpGlobals->deathmatch)
	{
#ifdef DEBUGENGINE
		if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnWriteAngle: %f\n",flValue); fclose(fp); }
#endif
		
		// if this message is for a bot, call the client message function...
		if (botMsgFunction)
			(*botMsgFunction)((void *)&flValue, botMsgIndex,_CLIENT_ANGLE);
	}
	
	(*g_engfuncs.pfnWriteAngle)(flValue);
}
void pfnWriteCoord(float flValue)
{
	if (gpGlobals->deathmatch)
	{
#ifdef DEBUGENGINE
		if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnWriteCoord: %f\n",flValue); fclose(fp); }
#endif
		
		// if this message is for a bot, call the client message function...
		if (botMsgFunction)
			(*botMsgFunction)((void *)&flValue, botMsgIndex,_CLIENT_COORD);
	}
	
	(*g_engfuncs.pfnWriteCoord)(flValue);
}
void pfnWriteString(const char *sz)
{
	if (gpGlobals->deathmatch)
	{
#ifdef DEBUGENGINE
		if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnWriteString: %s\n",sz); fclose(fp); }
#endif
		
		// if this message is for a bot, call the client message function...
		if (botMsgFunction)
			(*botMsgFunction)((void *)sz, botMsgIndex,_CLIENT_STRING);
	}
	
	(*g_engfuncs.pfnWriteString)(sz);
}
void pfnWriteEntity(int iValue)
{
	if (gpGlobals->deathmatch)
	{
#ifdef DEBUGENGINE
		if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnWriteEntity: %d\n",iValue); fclose(fp); }
#endif
		
		// if this message is for a bot, call the client message function...
		if (botMsgFunction)
			(*botMsgFunction)((void *)&iValue, botMsgIndex,_CLIENT_ENTITY);
	}
	
	(*g_engfuncs.pfnWriteEntity)(iValue);
}
void pfnCVarRegister(cvar_t *pCvar)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCVarRegister:\n"); fclose(fp); }
#endif
	(*g_engfuncs.pfnCVarRegister)(pCvar);
}
float pfnCVarGetFloat(const char *szVarName)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCVarGetFloat: %s\n",szVarName); fclose(fp); }
	return (*g_engfuncs.pfnCVarGetFloat)(szVarName);
}
const char* pfnCVarGetString(const char *szVarName)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCVarGetString:\n"); fclose(fp); }
	return (*g_engfuncs.pfnCVarGetString)(szVarName);
}
void pfnCVarSetFloat(const char *szVarName, float flValue)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCVarSetFloat:\n"); fclose(fp); }
	(*g_engfuncs.pfnCVarSetFloat)(szVarName, flValue);
}
void pfnCVarSetString(const char *szVarName, const char *szValue)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCVarSetString:\n"); fclose(fp); }
	(*g_engfuncs.pfnCVarSetString)(szVarName, szValue);
}
void* pfnPvAllocEntPrivateData(edict_t *pEdict, long cb)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnPvAllocEntPrivateData:\n"); fclose(fp); }
#endif
	return (*g_engfuncs.pfnPvAllocEntPrivateData)(pEdict, cb);
}
void* pfnPvEntPrivateData(edict_t *pEdict)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnPvEntPrivateData:\n"); fclose(fp); }
#endif
	return (*g_engfuncs.pfnPvEntPrivateData)(pEdict);
}
void pfnFreeEntPrivateData(edict_t *pEdict)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnFreeEntPrivateData:\n"); fclose(fp); }
#endif
	(*g_engfuncs.pfnFreeEntPrivateData)(pEdict);
}
const char* pfnSzFromIndex(int iString)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnSzFromIndex:\n"); fclose(fp); }
#endif
	return (*g_engfuncs.pfnSzFromIndex)(iString);
}
int pfnAllocString(const char *szValue)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnAllocString:\n"); fclose(fp); }
#endif
	return (*g_engfuncs.pfnAllocString)(szValue);
}
entvars_t* pfnGetVarsOfEnt(edict_t *pEdict)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetVarsOfEnt:\n"); fclose(fp); }
#endif
	return (*g_engfuncs.pfnGetVarsOfEnt)(pEdict);
}
edict_t* pfnPEntityOfEntOffset(int iEntOffset)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnPEntityOfEntOffset:\n"); fclose(fp); }
	return (*g_engfuncs.pfnPEntityOfEntOffset)(iEntOffset);
}
int pfnEntOffsetOfPEntity(const edict_t *pEdict)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnEntOffsetOfPEntity: %x\n",pEdict); fclose(fp); }
	return (*g_engfuncs.pfnEntOffsetOfPEntity)(pEdict);
}
int pfnIndexOfEdict(const edict_t *pEdict)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnIndexOfEdict: %x\n",pEdict); fclose(fp); }
	return (*g_engfuncs.pfnIndexOfEdict)(pEdict);
}
edict_t* pfnPEntityOfEntIndex(int iEntIndex)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnPEntityOfEntIndex:\n"); fclose(fp); }
	return (*g_engfuncs.pfnPEntityOfEntIndex)(iEntIndex);
}
edict_t* pfnFindEntityByVars(entvars_t* pvars)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnFindEntityByVars:\n"); fclose(fp); }
	return (*g_engfuncs.pfnFindEntityByVars)(pvars);
}
void* pfnGetModelPtr(edict_t* pEdict)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetModelPtr: %x\n",pEdict); fclose(fp); }
	return (*g_engfuncs.pfnGetModelPtr)(pEdict);
}
int pfnRegUserMsg(const char *pszName, int iSize)
{
	int msg;
	
	msg = (*g_engfuncs.pfnRegUserMsg)(pszName, iSize);
	
	if (gpGlobals->deathmatch)
	{
#ifdef DEBUGENGINE
		if (debug_engine) {
			fp=fopen("bot.txt","a");if(fp) fprintf(fp,"pfnRegUserMsg: pszName=%s msg=%d\n",pszName,msg); fclose(fp);
		}
#endif
		
		if (mod_id == VALVE_DLL)
		{
			if (strcmp(pszName, "WeaponList") == 0)
			{
				message_WeaponList = msg;
			}
			else if (strcmp(pszName, "CurWeapon") == 0)
			{
				message_CurWeapon = msg;
			}
			else if (strcmp(pszName, "AmmoX") == 0)
			{
				message_AmmoX = msg;
			}
			else if (strcmp(pszName, "AmmoPickup") == 0)
			{
				message_AmmoPickup = msg;
			}
			else if (strcmp(pszName, "WeapPickup") == 0)
			{
				message_WeapPickup = msg;
			}
			else if (strcmp(pszName, "ItemPickup") == 0)
			{
				message_ItemPickup = msg;
			}
			else if (strcmp(pszName, "Health") == 0)
			{
				message_Health = msg;
			}
			else if (strcmp(pszName, "Battery") == 0)
			{
				message_Battery = msg;
			}
			else if (strcmp(pszName, "Damage") == 0)
			{
				message_Damage = msg;
			}
			else if (strcmp(pszName, "SetFOV") == 0){
				message_SetFOV = msg;
			}
		}
		/*else if (mod_id == TFC_DLL)
		{
		if (strcmp(pszName, "VGUIMenu") == 0)
		{
		message_VGUI = msg;
		}
		else if (strcmp(pszName, "WeaponList") == 0)
		{
		message_WeaponList = msg;
		}
		else if (strcmp(pszName, "CurWeapon") == 0)
		{
		message_CurWeapon = msg;
		}
		else if (strcmp(pszName, "AmmoX") == 0)
		{
		message_AmmoX = msg;
		}
		else if (strcmp(pszName, "AmmoPickup") == 0)
		{
		message_AmmoPickup = msg;
		}
		else if (strcmp(pszName, "WeapPickup") == 0)
		{
		message_WeapPickup = msg;
		}
		else if (strcmp(pszName, "ItemPickup") == 0)
		{
		message_ItemPickup = msg;
		}
		else if (strcmp(pszName, "Health") == 0)
		{
		message_Health = msg;
		}
		else if (strcmp(pszName, "Battery") == 0)
		{
		message_Battery = msg;
		}
		else if (strcmp(pszName, "Damage") == 0)
		{
		message_Damage = msg;
		}
		}
		*/else if (mod_id == CSTRIKE_DLL)
		{
			if (strcmp(pszName, "VGUIMenu") == 0)
			{
				message_VGUI = msg;
			}
			else if (strcmp(pszName, "ShowMenu") == 0)
			{
				message_ShowMenu = msg;
			}
			else if (strcmp(pszName, "WeaponList") == 0)
			{
				message_WeaponList = msg;
			}
			else if (strcmp(pszName, "CurWeapon") == 0){
				message_CurWeapon = msg;
			}
			else if (strcmp(pszName, "AmmoX") == 0){
				message_AmmoX = msg;
			}
			else if (strcmp(pszName, "AmmoPickup") == 0){
				message_AmmoPickup = msg;
			}
			else if (strcmp(pszName, "WeapPickup") == 0){
				message_WeapPickup = msg;
			}
			else if (strcmp(pszName, "ItemPickup") == 0){
				message_ItemPickup = msg;
			}
			else if (strcmp(pszName, "Health") == 0){
				message_Health = msg;
			}
			else if (strcmp(pszName, "Battery") == 0){
				message_Battery = msg;
			}
			else if (strcmp(pszName, "Damage") == 0){
				message_Damage = msg;
			}
			else if (strcmp(pszName, "Money") == 0){
				message_Money = msg;
			}
			else if (strcmp(pszName, "StatusIcon") == 0){
				message_StatusI = msg;
			}
			else if (strcmp(pszName, "RoundTime") == 0){
				message_Roundtime = msg;
			}
			else if (strcmp(pszName, "SetFOV") == 0){
				message_SetFOV = msg;
			}
			else if (strcmp(pszName, "ScreenFade") == 0){
				message_ScreenFade = msg;
			}
			else if (strcmp(pszName, "TextMsg") == 0){
				message_TextMsg = msg;
			}
			else if (strcmp(pszName, "DeathMsg") == 0){
				message_DeathMsg = msg;
			}
			else if (strcmp(pszName, "TeamInfo") == 0){
				message_TeamInfo = msg;
			}
			else if (strcmp(pszName, "SayText") == 0)
				message_SayText = msg;
				/*else if (strcmp(pszName, "StatusValue") == 0)
				{
				message_StatusV = msg;
		}*/
		}
		else if (mod_id == DOD_DLL)
		{
			if (strcmp(pszName, "VGUIMenu") == 0)
				message_VGUI = msg;
			else if (strcmp(pszName, "WeaponList") == 0)
				message_WeaponList = msg;
			else if (strcmp(pszName, "CurWeapon") == 0)
				message_CurWeapon = msg;
			else if (strcmp(pszName, "AmmoX") == 0)
				message_AmmoX = msg;
			else if (strcmp(pszName, "AmmoPickup") == 0)
				message_AmmoPickup = msg;
			else if (strcmp(pszName, "WeapPickup") == 0)
				message_WeapPickup = msg;
			else if (strcmp(pszName, "ItemPickup") == 0)
				message_ItemPickup = msg;
			else if (strcmp(pszName, "Health") == 0)
				message_Health = msg;
			else if (strcmp(pszName, "Battery") == 0)
				message_Battery = msg;
			else if (strcmp(pszName, "Damage") == 0)
				message_Damage = msg;
			else if (strcmp(pszName, "DeathMsg") == 0)
				message_DeathMsg = msg;
			else if (strcmp(pszName, "ScreenFade") == 0)
				message_ScreenFade = msg;
			else if (strcmp(pszName, "Stamina") == 0)
				message_Stamina = msg;
			else if (strcmp(pszName, "Slowed") == 0)
				message_Slowed = msg;
			else if (strcmp(pszName, "Speed") == 0)
				message_Speed = msg;
			else if (strcmp(pszName, "Bleeding") == 0)
				message_Bleeding = msg;
			else if (strcmp(pszName, "Object") == 0)
				message_Object = msg;
			else if (strcmp(pszName, "SetFOV") == 0)
				message_SetFOV = msg;
			else if (strcmp(pszName, "TeamInfo") == 0){
				message_TeamInfo = msg;
			}
			
		}
		/*else if (mod_id == GEARBOX_DLL)
		{
		if (strcmp(pszName, "WeaponList") == 0)
		{
		message_WeaponList = msg;
		}
		else if (strcmp(pszName, "CurWeapon") == 0)
		{
		message_CurWeapon = msg;
		}
		else if (strcmp(pszName, "AmmoX") == 0)
		{
		message_AmmoX = msg;
		}
		else if (strcmp(pszName, "AmmoPickup") == 0)
		{
		message_AmmoPickup = msg;
		}
		else if (strcmp(pszName, "WeapPickup") == 0)
		{
		message_WeapPickup = msg;
		}
		else if (strcmp(pszName, "ItemPickup") == 0)
		{
		message_ItemPickup = msg;
		}
		else if (strcmp(pszName, "Health") == 0)
		{
		message_Health = msg;
		}
		else if (strcmp(pszName, "Battery") == 0)
		{
		message_Battery = msg;
		}
		else if (strcmp(pszName, "Damage") == 0)
		{
		message_Damage = msg;
		}
		}*/
   }
   
   return msg;
}
void pfnAnimationAutomove(const edict_t* pEdict, float flTime)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnAnimationAutomove:\n"); fclose(fp); }
	(*g_engfuncs.pfnAnimationAutomove)(pEdict, flTime);
}
void pfnGetBonePosition(const edict_t* pEdict, int iBone, float *rgflOrigin, float *rgflAngles )
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetBonePosition:\n"); fclose(fp); }
	(*g_engfuncs.pfnGetBonePosition)(pEdict, iBone, rgflOrigin, rgflAngles);
}
unsigned long pfnFunctionFromName( const char *pName )
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnFunctionFromName:\n"); fclose(fp); }
#endif
	return (*g_engfuncs.pfnFunctionFromName)(pName);
}
const char *pfnNameForFunction( unsigned long function )
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnNameForFunction:\n"); fclose(fp); }
#endif
	return (*g_engfuncs.pfnNameForFunction)(function);
}
void pfnClientPrintf( edict_t* pEdict, PRINT_TYPE ptype, const char *szMsg )
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnClientPrintf:\n"); fclose(fp); }
#endif
	if (!(pEdict->v.flags & FL_FAKECLIENT))
		(*g_engfuncs.pfnClientPrintf)(pEdict, ptype, szMsg);
}
void pfnServerPrint( const char *szMsg )
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnServerPrint:\n"); fclose(fp); }
#endif
	(*g_engfuncs.pfnServerPrint)(szMsg);
}
void pfnGetAttachment(const edict_t *pEdict, int iAttachment, float *rgflOrigin, float *rgflAngles )
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetAttachment:\n"); fclose(fp); }
	(*g_engfuncs.pfnGetAttachment)(pEdict, iAttachment, rgflOrigin, rgflAngles);
}
void pfnCRC32_Init(CRC32_t *pulCRC)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCRC32_Init:\n"); fclose(fp); }
#endif
	(*g_engfuncs.pfnCRC32_Init)(pulCRC);
}
void pfnCRC32_ProcessBuffer(CRC32_t *pulCRC, void *p, int len)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCRC32_ProcessBuffer:\n"); fclose(fp); }
#endif
	(*g_engfuncs.pfnCRC32_ProcessBuffer)(pulCRC, p, len);
}
void pfnCRC32_ProcessByte(CRC32_t *pulCRC, unsigned char ch)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCRC32_ProcessByte:\n"); fclose(fp); }
#endif
	(*g_engfuncs.pfnCRC32_ProcessByte)(pulCRC, ch);
}
CRC32_t pfnCRC32_Final(CRC32_t pulCRC)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCRC32_Final:\n"); fclose(fp); }
#endif
	return (*g_engfuncs.pfnCRC32_Final)(pulCRC);
}
long pfnRandomLong(long lLow, long lHigh)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnRandomLong: lLow=%d lHigh=%d\n",lLow,lHigh); fclose(fp); }
	return (*g_engfuncs.pfnRandomLong)(lLow, lHigh);
}
float pfnRandomFloat(float flLow, float flHigh)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnRandomFloat:\n"); fclose(fp); }
	return (*g_engfuncs.pfnRandomFloat)(flLow, flHigh);
}
void pfnSetView(const edict_t *pClient, const edict_t *pViewent )
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnSetView:\n"); fclose(fp); }
#endif
	(*g_engfuncs.pfnSetView)(pClient, pViewent);
}
float pfnTime( void )
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnTime:\n"); fclose(fp); }
#endif
	return (*g_engfuncs.pfnTime)();
}
void pfnCrosshairAngle(const edict_t *pClient, float pitch, float yaw)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCrosshairAngle:\n"); fclose(fp); }
#endif
	(*g_engfuncs.pfnCrosshairAngle)(pClient, pitch, yaw);
}
byte *pfnLoadFileForMe(char *filename, int *pLength)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnLoadFileForMe: filename=%s\n",filename); fclose(fp); }
#endif
	return (*g_engfuncs.pfnLoadFileForMe)(filename, pLength);
}
void pfnFreeFile(void *buffer)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnFreeFile:\n"); fclose(fp); }
#endif
	(*g_engfuncs.pfnFreeFile)(buffer);
}
void pfnEndSection(const char *pszSectionName)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnEndSection:\n"); fclose(fp); }
#endif
	(*g_engfuncs.pfnEndSection)(pszSectionName);
}
int pfnCompareFileTime(char *filename1, char *filename2, int *iCompare)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCompareFileTime:\n"); fclose(fp); }
#endif
	return (*g_engfuncs.pfnCompareFileTime)(filename1, filename2, iCompare);
}
void pfnGetGameDir(char *szGetGameDir)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetGameDir:\n"); fclose(fp); }
#endif
	(*g_engfuncs.pfnGetGameDir)(szGetGameDir);
}
void pfnCvar_RegisterVariable(cvar_t *variable)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCvar_RegisterVariable:\n"); fclose(fp); }
#endif
	(*g_engfuncs.pfnCvar_RegisterVariable)(variable);
}
void pfnFadeClientVolume(const edict_t *pEdict, int fadePercent, int fadeOutSeconds, int holdTime, int fadeInSeconds)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnFadeClientVolume:\n"); fclose(fp); }
#endif
	(*g_engfuncs.pfnFadeClientVolume)(pEdict, fadePercent, fadeOutSeconds, holdTime, fadeInSeconds);
}
void pfnSetClientMaxspeed(const edict_t *pEdict, float fNewMaxspeed){
	int index = -1;
	index = g_pGame->isBot(pEdict);
	if(index !=-1){
		g_pGame->getBot(index)->m_pAction->setMaxSpeed(fNewMaxspeed);
		if(mod_id == DOD_DLL){
			g_pGame->getBot(index)->m_pAction->setMaxSpeed(g_pGame->getBot(index)->m_pAction->getMaxSpeed()/2);		// cause the other is sprinting speed
		}
	}
	
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnSetClientMaxspeed: edict=%x %f\n",pEdict,fNewMaxspeed); fclose(fp); }
#endif
	(*g_engfuncs.pfnSetClientMaxspeed)(pEdict, fNewMaxspeed);
}
edict_t * pfnCreateFakeClient(const char *netname)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCreateFakeClient:\n"); fclose(fp); }
#endif
	return (*g_engfuncs.pfnCreateFakeClient)(netname);
}
void pfnRunPlayerMove(edict_t *fakeclient, const float *viewangles, float forwardmove, float sidemove, float upmove, unsigned short buttons, byte impulse, byte msec )
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnRunPlayerMove:\n"); fclose(fp); }
#endif
	(*g_engfuncs.pfnRunPlayerMove)(fakeclient, viewangles, forwardmove, sidemove, upmove, buttons, impulse, msec);
}
int pfnNumberOfEntities(void)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnNumberOfEntities:\n"); fclose(fp); }
#endif
	return (*g_engfuncs.pfnNumberOfEntities)();
}
char* pfnGetInfoKeyBuffer(edict_t *e)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetInfoKeyBuffer:\n"); fclose(fp); }
#endif
	return (*g_engfuncs.pfnGetInfoKeyBuffer)(e);
}
char* pfnInfoKeyValue(char *infobuffer, char *key)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnInfoKeyValue: %s %s\n",infobuffer,key); fclose(fp); }
#endif
	return (*g_engfuncs.pfnInfoKeyValue)(infobuffer, key);
}
void pfnSetKeyValue(char *infobuffer, char *key, char *value)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnSetKeyValue: %s %s\n",key,value); fclose(fp); }
#endif
	(*g_engfuncs.pfnSetKeyValue)(infobuffer, key, value);
}
void pfnSetClientKeyValue(int clientIndex, char *infobuffer, char *key, char *value)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnSetClientKeyValue: %s %s\n",key,value); fclose(fp); }
#endif
	(*g_engfuncs.pfnSetClientKeyValue)(clientIndex, infobuffer, key, value);
}
int pfnIsMapValid(char *filename)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnIsMapValid:\n"); fclose(fp); }
#endif
	return (*g_engfuncs.pfnIsMapValid)(filename);
}
void pfnStaticDecal( const float *origin, int decalIndex, int entityIndex, int modelIndex )
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnStaticDecal:\n"); fclose(fp); }
#endif
	(*g_engfuncs.pfnStaticDecal)(origin, decalIndex, entityIndex, modelIndex);
}
int pfnPrecacheGeneric(char* s)
{
#ifdef DEBUGENGINE
	//if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnPrecacheGeneric: %s\n",s); fclose(fp); }
#endif
	return (*g_engfuncs.pfnPrecacheGeneric)(s);
}
int pfnGetPlayerUserId(edict_t *e )
{
	if (gpGlobals->deathmatch)
	{
#ifdef DEBUGENGINE
		if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetPlayerUserId: %x\n",e); fclose(fp); }
#endif
		
		if (mod_id == GEARBOX_DLL)
		{
			// is this edict a bot?
			if (g_pGame->isBot( e ) != -1)
				return 0;  // don't return a valid index (so bot won't get kicked)
		}
	}
	
	return (*g_engfuncs.pfnGetPlayerUserId)(e);
}
void pfnBuildSoundMsg(edict_t *entity, int channel, const char *sample, /*int*/float volume, float attenuation, int fFlags, int pitch, int msg_dest, int msg_type, const float *pOrigin, edict_t *ed)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnBuildSoundMsg:\n"); fclose(fp); }
#endif
	(*g_engfuncs.pfnBuildSoundMsg)(entity, channel, sample, volume, attenuation, fFlags, pitch, msg_dest, msg_type, pOrigin, ed);
}
int pfnIsDedicatedServer(void)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnIsDedicatedServer:\n"); fclose(fp); }
#endif
	return (*g_engfuncs.pfnIsDedicatedServer)();
}
cvar_t* pfnCVarGetPointer(const char *szVarName)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCVarGetPointer: %s\n",szVarName); fclose(fp); }
#endif
	return (*g_engfuncs.pfnCVarGetPointer)(szVarName);
}
unsigned int pfnGetPlayerWONId(edict_t *e)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetPlayerWONId: %x\n",e); fclose(fp); }
#endif
	return (*g_engfuncs.pfnGetPlayerWONId)(e);
}

// new stuff for SDK 2.0

void pfnInfo_RemoveKey(char *s, const char *key)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnInfo_RemoveKey:\n"); fclose(fp); }
#endif
	(*g_engfuncs.pfnInfo_RemoveKey)(s, key);
}
const char *pfnGetPhysicsKeyValue(const edict_t *pClient, const char *key)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetPhysicsKeyValue:\n"); fclose(fp); }
#endif
	return (*g_engfuncs.pfnGetPhysicsKeyValue)(pClient, key);
}
void pfnSetPhysicsKeyValue(const edict_t *pClient, const char *key, const char *value)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnSetPhysicsKeyValue:\n"); fclose(fp); }
#endif
	(*g_engfuncs.pfnSetPhysicsKeyValue)(pClient, key, value);
}
const char *pfnGetPhysicsInfoString(const edict_t *pClient)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetPhysicsInfoString:\n"); fclose(fp); }
#endif
	return (*g_engfuncs.pfnGetPhysicsInfoString)(pClient);
}
unsigned short pfnPrecacheEvent(int type, const char *psz)
{
#ifdef DEBUGENGINE
	//if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnPrecacheEvent:\n"); fclose(fp); }
#endif
	return (*g_engfuncs.pfnPrecacheEvent)(type, psz);
}
void pfnPlaybackEvent(int flags, const edict_t *pInvoker, unsigned short eventindex, float delay,
					  float *origin, float *angles, float fparam1,float fparam2, int iparam1, int iparam2, int bparam1, int bparam2)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnPlaybackEvent:\n"); fclose(fp); }
#endif
	(*g_engfuncs.pfnPlaybackEvent)(flags, pInvoker, eventindex, delay, origin, angles, fparam1, fparam2, iparam1, iparam2, bparam1, bparam2);
}
unsigned char *pfnSetFatPVS(float *org)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnSetFatPVS:\n"); fclose(fp); }
#endif
	return (*g_engfuncs.pfnSetFatPVS)(org);
}
unsigned char *pfnSetFatPAS(float *org)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnSetFatPAS:\n"); fclose(fp); }
#endif
	return (*g_engfuncs.pfnSetFatPAS)(org);
}
int pfnCheckVisibility(const edict_t *entity, unsigned char *pset)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCheckVisibility:\n"); fclose(fp); }
	return (*g_engfuncs.pfnCheckVisibility)(entity, pset);
}
void pfnDeltaSetField(struct delta_s *pFields, const char *fieldname)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnDeltaSetField:\n"); fclose(fp); }
	(*g_engfuncs.pfnDeltaSetField)(pFields, fieldname);
}
void pfnDeltaUnsetField(struct delta_s *pFields, const char *fieldname)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnDeltaUnsetField:\n"); fclose(fp); }
	(*g_engfuncs.pfnDeltaUnsetField)(pFields, fieldname);
}
void pfnDeltaAddEncoder(char *name, void (*conditionalencode)( struct delta_s *pFields, const unsigned char *from, const unsigned char *to))
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnDeltaAddEncoder:\n"); fclose(fp); }
	(*g_engfuncs.pfnDeltaAddEncoder)(name, conditionalencode);
}
int pfnGetCurrentPlayer(void)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetCurrentPlayer:\n"); fclose(fp); }
#endif
	return (*g_engfuncs.pfnGetCurrentPlayer)();
}
int pfnCanSkipPlayer(const edict_t *player)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCanSkipPlayer:\n"); fclose(fp); }
#endif
	return (*g_engfuncs.pfnCanSkipPlayer)(player);
}
int pfnDeltaFindField(struct delta_s *pFields, const char *fieldname)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnDeltaFindField:\n"); fclose(fp); }
	return (*g_engfuncs.pfnDeltaFindField)(pFields, fieldname);
}
void pfnDeltaSetFieldByIndex(struct delta_s *pFields, int fieldNumber)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnDeltaSetFieldByIndex:\n"); fclose(fp); }
#endif
	(*g_engfuncs.pfnDeltaSetFieldByIndex)(pFields, fieldNumber);
}
void pfnDeltaUnsetFieldByIndex(struct delta_s *pFields, int fieldNumber)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnDeltaUnsetFieldByIndex:\n"); fclose(fp); }
	(*g_engfuncs.pfnDeltaUnsetFieldByIndex)(pFields, fieldNumber);
}
void pfnSetGroupMask(int mask, int op)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnSetGroupMask:\n"); fclose(fp); }
#endif
	(*g_engfuncs.pfnSetGroupMask)(mask, op);
}
int pfnCreateInstancedBaseline(int classname, struct entity_state_s *baseline)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCreateInstancedBaseline:\n"); fclose(fp); }
#endif
	return (*g_engfuncs.pfnCreateInstancedBaseline)(classname, baseline);
}
void pfnCvar_DirectSet(struct cvar_s *var, char *value)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCvar_DirectSet:\n"); fclose(fp); }
#endif
	(*g_engfuncs.pfnCvar_DirectSet)(var, value);
}
void pfnForceUnmodified(FORCE_TYPE type, float *mins, float *maxs, const char *filename)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnForceUnmodified:\n"); fclose(fp); }
#endif
	(*g_engfuncs.pfnForceUnmodified)(type, mins, maxs, filename);
}
void pfnGetPlayerStats(const edict_t *pClient, int *ping, int *packet_loss)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetPlayerStats:\n"); fclose(fp); }
#endif
	(*g_engfuncs.pfnGetPlayerStats)(pClient, ping, packet_loss);
}

void pfnAddServerCommand(char *cmd_name, void (*function)(void))
{
#ifdef DEBUGENGINE
   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnAddServerCommand: %s %x\n",cmd_name,function); fclose(fp); }
#endif
   (*g_engfuncs.pfnAddServerCommand)(cmd_name, function);
}

// hl1108

qboolean pfnVoice_GetClientListening(int iReceiver, int iSender) { 
	return (*g_engfuncs.pfnVoice_GetClientListening)(iReceiver, iSender); 
} 
qboolean pfnVoice_SetClientListening(int iReceiver, int iSender, qboolean bListen) { 
	return (*g_engfuncs.pfnVoice_SetClientListening)(iReceiver, iSender, bListen); 
} 

// hl1109

const char *pfnGetPlayerAuthID(edict_t *e){
	return (*g_engfuncs.pfnGetPlayerAuthID)(e); 
}

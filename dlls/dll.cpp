// JoeBot
// by Johannes Lampel
// Johannes.Lampel@gmx.de
// http://joebot.counter-strike.de

//
// HPB bot - botman's High Ping Bastard bot
//
// (http://planethalflife.com/botman/)
//
// dll.cpp
//

#include "extdll.h"
#include "enginecallback.h"
#include "util.h"
#include "cbase.h"
#include "entity_state.h"

#include "bot.h"

#include "bot_modid.h"

#include <time.h>
#include <iostream>
using namespace std;

#include "Command.h"
#include "Game.h"
#include "BaseBot.h"
#include "globalvars.h"

#include "util.h"

#include "AStarBase.h"
#include "AStarTaskManager.h"
#include "Action.h"
#include "PerceptionBot.h"
#include "Behaviour.h"

#include "Observer.h"
CObserver g_Observer;

#include "HPTime.h"

#define _PAUSE_TIME 1

#include "Names.h"

#include "BV_CS.h"
#include "BV_HLDM.h"

#include "BWP.h"

extern GETENTITYAPI other_GetEntityAPI;
extern GETNEWDLLFUNCTIONS other_GetNewDLLFunctions;
extern enginefuncs_t g_engfuncs;
extern globalvars_t  *gpGlobals;
extern char *g_argv;

bool g_debuginfo = false;

#ifdef DEBUGENGINE
extern int debug_engine;
#endif

long g_lGlobalCount = 0;

static FILE *fp;
extern float globalFrameTime;

DLL_FUNCTIONS other_gFunctionTable;
DLL_GLOBAL const Vector g_vecZero = Vector(0,0,0);

int mod_id = -1;
int m_spriteTexture = 0;
int isFakeClientCommand = 0;
int fake_arg_count;
int g_iMinBots = -1;
int g_iMaxBots = -1;
bool g_GameRules = FALSE;
//edict_t *clients[32];
//edict_t *listenserver_edict = NULL;

bool g_bJoinWHumanRES = false;
bool g_bJoinWHumanMAX = false;

bool g_bShowProfile = false;
bool g_bShowWPStat = true;

bool g_show_waypoints = true;

int flf_bug_fix;  // for FLF 1.1 capture point bug
int flf_bug_check;  // for FLF 1.1 capture point bug

edict_t *pEdictLastJoined;

cvar_t sv_bot = {"joebot",""};

char g_szWelcomeMsg[200];
char _JOEBOTVERSION[80];
char _JOEBOTVERSIONWOOS[80]= "XP::B006";
bool g_bDedicatedWelcome = false;

bool g_bMyBirthday;
long g_lAge;

void BotNameInit(void);

bool bInitInfo = true;

#if _DEBUG
#define DEBUGENGINE
#endif
extern int debug_engine;

#include "Map.h"

char g_szProfileOutput[10000];

bool MyBirthday(void){
	time_t now = time(NULL);
	tm *tm_now = localtime(&now);
	
	if(tm_now->tm_mon == 6
		&&tm_now->tm_mday==8){			// 8.7.
		g_lAge = tm_now->tm_year - 82;
		return true;
	}
	return false;
}

void GameDLLInit( void ){
	long lschl;
	cout << "JoeBOT : CPU at apprx. " << long(g_HPTime.getFrequency() / 10000)/100.f << " MHz"<<endl;

	CVAR_REGISTER (&sv_bot);
	
#ifdef __linux__
	sprintf(_JOEBOTVERSION,"%s (linux)",_JOEBOTVERSIONWOOS);
#else
	sprintf(_JOEBOTVERSION,"%s (win32)",_JOEBOTVERSIONWOOS);
#endif
#ifdef _DEBUG
	strcat(_JOEBOTVERSION," DBGV");
#endif
	
	sprintf(g_szWelcomeMsg,"----- JoeBot XP %s by @$3.1415rin { http://joebot.bots-united.org }; -----\n\0",_JOEBOTVERSION);
	
	memset(weapon_defs,0,sizeof(bot_weapon_t) * MAX_WEAPONS);
	
	// is the engine's random number generator initialized nicely ? - well, fu, we'll do it ourself
	for(lschl = g_HPTime.getTime()&255;lschl;lschl--){
		g_lAge = RANDOM_LONG(0,100);		// use lAge as global var, so the compiler won't optimize it away
	}

	g_Names.load("bot_names.txt");

	srand(g_HPTime.getTime());
	
	(*other_gFunctionTable.pfnGameInit)();
}

int DispatchSpawn( edict_t *pent )
{
	//	int index;
	
	if (gpGlobals->deathmatch)
	{
		char *pClassname = (char *)STRING(pent->v.classname);
		
#ifdef DEBUGENGINE
		if (debug_engine)
		{
			fp=fopen("bot.txt","a");
			fprintf(fp, "DispatchSpawn: %x %s\n",pent,pClassname);
			if (pent->v.model != 0)
				fprintf(fp, " model=%s\n",STRING(pent->v.model));
			fclose(fp);
		}
#endif
		
		if (!strcmp(pClassname, "worldspawn"))
		{
			/*PRECACHE_SOUND("weapons/xbow_hit1.wav");		// waypoint add
			PRECACHE_SOUND("weapons/mine_activate.wav");	// waypoint delete
			PRECACHE_SOUND("common/wpn_hudoff.wav");		// path add/delete start
			PRECACHE_SOUND("common/wpn_hudon.wav");			// path add/delete done
			PRECACHE_SOUND("common/wpn_moveselect.wav");	// path add/delete cancel
			PRECACHE_SOUND("common/wpn_denyselect.wav");	// path add/delete error
			PRECACHE_SOUND("player/sprayer.wav");*/

			//listenserver_edict = 0;
			
			m_spriteTexture = PRECACHE_MODEL( "sprites/lgtning.spr");
			
			g_GameRules = TRUE;

			g_pGame->onMapStart();
		}
	}
	
	return (*other_gFunctionTable.pfnSpawn)(pent);
}

void DispatchThink( edict_t *pent )
{
	(*other_gFunctionTable.pfnThink)(pent);
}

void DispatchUse( edict_t *pentUsed, edict_t *pentOther )
{
	(*other_gFunctionTable.pfnUse)(pentUsed, pentOther);
}

void DispatchTouch( edict_t *pentTouched, edict_t *pentOther )
{
	(*other_gFunctionTable.pfnTouch)(pentTouched, pentOther);
}

void DispatchBlocked( edict_t *pentBlocked, edict_t *pentOther )
{
	(*other_gFunctionTable.pfnBlocked)(pentBlocked, pentOther);
}

void DispatchKeyValue( edict_t *pentKeyvalue, KeyValueData *pkvd )
{
	/*static edict_t *temp_pent;
	static int flag_index;*/
	
	//   fp=fopen("bot.txt","a"); fprintf(fp, "DispatchKeyValue: %x %s=%s\n",pentKeyvalue,pkvd->szKeyName,pkvd->szValue); fclose(fp);
	
	(*other_gFunctionTable.pfnKeyValue)(pentKeyvalue, pkvd);
}

void DispatchSave( edict_t *pent, SAVERESTOREDATA *pSaveData )
{
	(*other_gFunctionTable.pfnSave)(pent, pSaveData);
}

int DispatchRestore( edict_t *pent, SAVERESTOREDATA *pSaveData, int globalEntity )
{
	return (*other_gFunctionTable.pfnRestore)(pent, pSaveData, globalEntity);
}

void DispatchObjectCollsionBox( edict_t *pent )
{
	(*other_gFunctionTable.pfnSetAbsBox)(pent);
}

void SaveWriteFields( SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount )
{
	(*other_gFunctionTable.pfnSaveWriteFields)(pSaveData, pname, pBaseData, pFields, fieldCount);
}

void SaveReadFields( SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount )
{
	(*other_gFunctionTable.pfnSaveReadFields)(pSaveData, pname, pBaseData, pFields, fieldCount);
}

void SaveGlobalState( SAVERESTOREDATA *pSaveData )
{
	(*other_gFunctionTable.pfnSaveGlobalState)(pSaveData);
}

void RestoreGlobalState( SAVERESTOREDATA *pSaveData )
{
	(*other_gFunctionTable.pfnRestoreGlobalState)(pSaveData);
}

void ResetGlobalState( void )
{
	(*other_gFunctionTable.pfnResetGlobalState)();
}

BOOL ClientConnect( edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[ 128 ]  )
{ 
	if (gpGlobals->deathmatch){
#ifdef DEBUGENGINE
		if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp, "ClientConnect: pent=%x name=%s\n",pEntity,pszName); fclose(fp); }
#endif
		
		// check if this client is the listen server client
		if (!strcmp(pszAddress, "loopback"))
		{
			// save the edict of the listen server client...
			g_pGame->m_pListenserverEdict = pEntity;

			//g_pGame->addPlayer(pEntity); // hackhack : isnt clientputinserver called for listenserver#s ?
			
			//bot_cfg_pause_time = gpGlobals->time + .5f;
		}
		
		// check if this is NOT a bot joining the server...
		if (strcmp(pszAddress, "127.0.0.1"))
		{
			// don't try to add bots for 60 seconds, give client time to get added
			//bot_check_time = gpGlobals->time + 60.0;
			//bot_check_time = gpGlobals->time + _PAUSE_TIME*2;
			
			/*for (i=0; i < 32; i++)
			{
				if (bots[i])  // count the number of bots in use
					count++;
			}*/
			
			// if there are currently more than the minimum number of bots running
			// then kick one of the bots off the server...
			/*if ((count > min_bots) && (min_bots != -1))
			{
				for (i=0; i < 32; i++){
					if (bots[i]){  // is this slot used?
						sprintf(szTemp, "kick \"%s\"\n", STRING(bots[i]->m_pPlayer->getEntity()->v.netname));
						
						SERVER_COMMAND(szTemp);  // kick the bot using (kick "name")
						
						break;
					}
				}
			}*/
		}
		pEdictLastJoined = pEntity;
	}
	
	return (*other_gFunctionTable.pfnClientConnect)(pEntity, pszName, pszAddress, szRejectReason);
}

void ClientDisconnect( edict_t *pEntity ){
	if (gpGlobals->deathmatch)
	{
#ifdef DEBUGENGINE
		if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp, "ClientDisconnect: %x\n",pEntity); fclose(fp); }
#endif
		// now you can remove the player from the game ( otherwise GetBotIndex won't work )
		g_pGame->removePlayer(pEntity);
	}
	
	(*other_gFunctionTable.pfnClientDisconnect)(pEntity);
}

void ClientKill( edict_t *pEntity )
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp, "ClientKill: %x\n",pEntity); fclose(fp); }
#endif
	(*other_gFunctionTable.pfnClientKill)(pEntity);
}

void Endround(void){
	int i;
	for(i=0;i<32;i++){
		if(g_pGame->getBot(i)){
			if(g_pGame->getBot(i)->isAlive()){
				g_pGame->getBot(i)->getEntity()->v.frags++;
				ClientKill(g_pGame->getBot(i)->getEntity());
			}
		}
	}
}

void ClientPutInServer( edict_t *pEntity )
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp, "ClientPutInServer: %x\n",pEntity); fclose(fp); }
#endif
	
	/*int i = 0;
	
	while ((i < 32) && (clients[i] != NULL))
		i++;
	
	if (i < 32)
		clients[i] = pEntity;  // store this clients edict in the clients array*/

	g_pGame->addPlayer(pEntity);
	
	(*other_gFunctionTable.pfnClientPutInServer)(pEntity);
}

void ClientCommand( edict_t *pEntity )
{
	//	int i;
	// only allow custom commands if deathmatch mode and NOT dedicated server and
	// client sending command is the listen server client...
	
	if ((gpGlobals->deathmatch) && (!IS_DEDICATED_SERVER()) &&
		(pEntity == g_pGame->m_pListenserverEdict))
	{
		const char *pcmd = Cmd_Argv(0);
		const char *arg1 = Cmd_Argv(1);
		const char *arg2 = Cmd_Argv(2);
		const char *arg3 = Cmd_Argv(3);
		const char *arg4 = Cmd_Argv(4);
		//		char msg[80];
#ifdef DEBUGENGINE
		if (debug_engine)
		{
			fp=fopen("bot.txt","a"); fprintf(fp,"ClientCommand: %s",pcmd);
			if ((arg1 != NULL) && (*arg1 != 0))
				fprintf(fp," %s", arg1);
			if ((arg2 != NULL) && (*arg2 != 0))
				fprintf(fp," %s", arg2);
			if ((arg3 != NULL) && (*arg3 != 0))
				fprintf(fp," %s", arg3);
			if ((arg4 != NULL) && (*arg4 != 0))
				fprintf(fp," %s", arg4);
			fprintf(fp, "\n");
			fclose(fp);
		}
#endif
		
		if(g_Commands.Exec(pEntity,CM_CONSOLE,pcmd,arg1,arg2,arg3,arg4))
			return;
	}
	
	(*other_gFunctionTable.pfnClientCommand)(pEntity);
}

void ClientUserInfoChanged( edict_t *pEntity, char *infobuffer )
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt", "a"); fprintf(fp, "ClientUserInfoChanged: pEntity=%x infobuffer=%s\n", pEntity, infobuffer); fclose(fp); }
#endif
	
	(*other_gFunctionTable.pfnClientUserInfoChanged)(pEntity, infobuffer);
}

void ServerActivate( edict_t *pEdictList, int edictCount, int clientMax )
{
	(*other_gFunctionTable.pfnServerActivate)(pEdictList, edictCount, clientMax);
}

void ServerDeactivate( void )
{
	g_pGame->onMapEnd();

	(*other_gFunctionTable.pfnServerDeactivate)();
}

void PlayerPreThink( edict_t *pEntity )
{
	(*other_gFunctionTable.pfnPlayerPreThink)(pEntity);
}

void PlayerPostThink( edict_t *pEntity )
{
	//if(listenserver_edict&&bots[0])BotDebug(VARS(listenserver_edict),bots[0]->m_pPlayer->getOrigin(),"hallo");
	(*other_gFunctionTable.pfnPlayerPostThink)(pEntity);
}

void getInfo(char *szDebugText,edict_t *pInfo){
	if(pInfo){
		char szTemp [1000];
		char szPT[][80] = {"PT_NONE",
			"PT_ENEMY",
			"PT_TMATE",
			"PT_DAMAGE",
			"PT_ENTITY",
			"PT_SOUND",
			"PT_KILL",
			"PT_ACTION",
			"PT_COMMAND"};
		sprintf(szTemp,"%s %s- %i\n",CEntity(pInfo).isAlive()?"":"DEAD ",STRING(pInfo->v.netname),pInfo->v.health);
		strcat(szDebugText,szTemp);
		
		/*
		sprintf(szTemp,"(%.0f,%.0f,%.0f)\n",pInfo->v.v_angle.x,pInfo->v.v_angle.y,pInfo->v.v_angle.z);
		strcat(szDebugText,szTemp);*/
		if(pInfo->v.button & IN_FORWARD){
			sprintf(szTemp,"IN_FORWARD\n");
			strcat(szDebugText,szTemp);
		}
		int iBI = g_pGame->isBot(pInfo);
		if(iBI == -1){
			sprintf(szTemp,"\nthis isn't a bot\n");
			strcat(szDebugText,szTemp);
		}
		else{
			sprintf(szTemp,"bot related stuff: %i\n",g_pGame->getBot(iBI)->m_pEMTeam->m_iVisCount/*,g_pGame->getBot(iBI)->m_bot_team*/);
			strcat(szDebugText,szTemp);
			
			sprintf(szTemp,"Weapon Cl%i - ID%i\n",g_pGame->getBot(iBI)->m_pWeapons->m_iCurrentWClip,g_pGame->getBot(iBI)->m_pWeapons->getCWeaponID());
			strcat(szDebugText,szTemp);
			
			sprintf(szTemp,"Way %iN %iCi AC%i/C%i - Sq%i\n",g_pGame->getBot(iBI)->m_iNearestWP,g_pGame->getBot(iBI)->m_pMovePath->m_iCurrWP,g_pGame->getBot(iBI)->m_pMovePath->m_CurrWay.getCurrentIndex(),g_pGame->getBot(iBI)->m_pMovePath->m_CurrWay.size(),g_pGame->getBot(iBI)->m_iSquad);
			strcat(szDebugText,szTemp);
			
			sprintf(szTemp,"Vel%.0f/W%.0f\n",g_pGame->getBot(iBI)->getVelocity().length(),g_pGame->getBot(iBI)->m_pAction->getSpeed());
			strcat(szDebugText,szTemp);
			
			sprintf(szTemp,"Percs : %i/%i\n",g_pGame->getBot(iBI)->m_pPerception->m_LPerceptions.size()
				,g_pGame->getBot(iBI)->m_pPerception->m_LLTMem.size());
			strcat(szDebugText,szTemp);
			list<CPerceipt*>::iterator iter;
			for(iter = g_pGame->getBot(iBI)->m_pPerception->m_LPerceptions.begin();iter != g_pGame->getBot(iBI)->m_pPerception->m_LPerceptions.end();iter++){
				sprintf(szTemp,"%s,",szPT[(*iter)->m_lType]);
				if((*iter)->m_lType !=CPerceipt::PT_ENTITY)	// just write an "E" for each entity to rpevent overflow
					strcat(szDebugText,szTemp);
				else
					strcat(szDebugText,"E");
			}
			sprintf(szTemp,"\nGoals : %i\n",g_pGame->getBot(iBI)->m_LGoals.size());
			strcat(szDebugText,szTemp);
			sprintf(szTemp,"Action : %s(%2i)\n",g_pGame->getBot(iBI)->m_LGoals.m_pActionWinner?g_pGame->getBot(iBI)->m_LGoals.m_pActionWinner->m_pBehaviour->getName():"none",
				g_pGame->getBot(iBI)->m_LGoals.m_pActionWinner?g_pGame->getBot(iBI)->m_LGoals.m_pActionWinner->m_iImportance:-1);
			strcat(szDebugText,szTemp);
			
			sprintf(szTemp,"Look : %s(%2i)\n",g_pGame->getBot(iBI)->m_LGoals.m_pLookWinner?g_pGame->getBot(iBI)->m_LGoals.m_pLookWinner->m_pBehaviour->getName():"none",
				g_pGame->getBot(iBI)->m_LGoals.m_pLookWinner?g_pGame->getBot(iBI)->m_LGoals.m_pLookWinner->m_iImportance:-1);
			strcat(szDebugText,szTemp);
			
			sprintf(szTemp,"Move : %s(%2i)\n",g_pGame->getBot(iBI)->m_LGoals.m_pMoveWinner?g_pGame->getBot(iBI)->m_LGoals.m_pMoveWinner->m_pBehaviour->getName():"none",
				g_pGame->getBot(iBI)->m_LGoals.m_pMoveWinner?g_pGame->getBot(iBI)->m_LGoals.m_pMoveWinner->m_iImportance:-1);
			strcat(szDebugText,szTemp);
			
			sprintf(szTemp,"\nLMoveWP : %s(%i,%i,%i)\n",g_pGame->getBot(iBI)->m_LGoals.m_pLMoveBv?g_pGame->getBot(iBI)->m_LGoals.m_pLMoveBv->getName():"none",
				g_pGame->getBot(iBI)->m_LGoals.m_iLMovePriority,g_pGame->getBot(iBI)->m_LGoals.m_pLMoveBv?g_pGame->getBot(iBI)->m_LGoals.m_pLMoveBv->m_iExecuteFSMS:-1,
				(g_pGame->getBot(iBI)->m_pMovePath->hasWay()&&g_Map.m_Waypoints.isValid(g_pGame->getBot(iBI)->m_pMovePath->m_CurrWay.getDestination()))?g_pGame->getBot(iBI)->m_pMovePath->m_CurrWay.getDestination():-1);
			strcat(szDebugText,szTemp);

			//sprintf(szTemp,"C%i",(int)g_pGame->getBot(iBI)->m_pPerception->m_LCommands.size());
			sprintf(szTemp,"C %f",(int)g_pGame->getBot(iBI)->m_pCamp->m_fCamp - g_pGame->getTime());
			strcat(szDebugText,szTemp);
		}
	}
	else{
		strcat(szDebugText,"\n\nno edict to display information about\n");
	}
}

char szDebugText[10000];

void ShowInfo(void){
	//return;
	//cout << g_pGame->getMaxCurrentPlayer() << endl;
#ifndef _DEBUG
	if(!g_bShowProfile && !g_bShowWPStat){
		return;
	}
#endif
	if(!IS_DEDICATED_SERVER()){
		if(g_pGame->m_pListenserverEdict.getEntity()){
			static float fNext = 0;
			static float fNextDiff = .2;

			if(fNext < g_pGame->getTime()){
				fNextDiff = .42f;
				fNext = g_pGame->getTime() + .4f;

				g_debuginfo = true;
			}
			else{
				if(g_pGame->getTime()<30 && fNext > 31)
					fNext = 0;
				g_debuginfo = false;
				return;
			}

			edict_t *pInfo = 0;

			szDebugText[0] = 0;
			if(!g_bShowProfile && !(g_bShowWPStat && g_Map.m_Waypoints.m_bDisplay)){
#ifdef _DEBUG
				float fMin = 10;
				char szTemp[200];

				sprintf(szTemp,"time: %.0f\n",g_pGame->getTime());
				strcat(szDebugText,szTemp);
				sprintf(szTemp,"Players : %i\n",g_pGame->getMaxCurrentPlayer());
				strcat(szDebugText,szTemp);
				sprintf(szTemp,"ASMP : %li %3li %li/%li\n",g_ASN_MPool.m_lCAlloc,g_ASN_MPool.m_lCAlloc-g_ASN_MPool.m_lCFree,g_ASN_MPool.m_lNextFree,g_ASN_MPool.m_lCMax);
				strcat(szDebugText,szTemp);
				sprintf(szTemp,"ASTM : #%li/%li\n",g_AStarTasks.getCurrentTotalTasks(),g_AStarTasks.getTotalTasks());
				strcat(szDebugText,szTemp);
				pInfo = 0;
				pInfo = UTIL_GetNearestPlayer(g_pGame->m_pListenserverEdict.getEntity(),-1,fMin,true,true);
				fMin = 100000;
				if(!pInfo){
					pInfo = UTIL_GetNearestPlayer(g_pGame->m_pListenserverEdict.getEntity(),-1,fMin,true,false);
				}
				if(pInfo){
					getInfo(szDebugText,pInfo);
				}
#endif
			}
			else if(g_bShowProfile){
				//sprintf(szDebugText,"%li %li\n",g_lGlobalCount,g_pGame->m_iMaxCurrentPlayer);
				*szDebugText = 0;
				strcat(szDebugText,g_szProfileOutput);
			}
			else if(g_bShowWPStat && g_Map.m_Waypoints.m_bDisplay){
				if(!g_pGame->m_pListenserverEdict.getEntity())
					return;

				int iNWP = g_Map.m_Waypoints.getNearest(g_pGame->m_pListenserverEdict.getOrigin());
				if(iNWP == -1)
					return;

				char szTemp[80];
				sprintf(szDebugText,"WPStat Info (%i/%i)\n\n",iNWP,g_Map.m_Waypoints.m_iNumWaypoints);
				sprintf(szTemp,"iVWP    : %i\n",g_Map.m_Waypoints[iNWP].m_iVisibleWaypoints);strcat(szDebugText,szTemp);
				sprintf(szTemp,"fAvDv  : %.0f\n",g_Map.m_Waypoints[iNWP].m_fAvDistVWP);strcat(szDebugText,szTemp);
				sprintf(szTemp,"D/Dd    : %li/%li\n",g_Map.m_Waypoints[iNWP].m_lTotalDamage,g_Map.m_Waypoints[iNWP].m_lTotalDamaged);strcat(szDebugText,szTemp);
				sprintf(szTemp,"traffic : %li\n",g_Map.m_Waypoints[iNWP].m_lTraffic);strcat(szDebugText,szTemp);

				g_pGame->drawBeam(0,g_pGame->m_pListenserverEdict.getEntity(),g_pGame->m_pListenserverEdict.getOrigin(),g_Map.m_Waypoints[iNWP].m_VOrigin,2,0,Vector(255,255,255),255,0,fNextDiff*10);
				if(!g_Map.m_Waypoints.m_bDisplay)
					g_pGame->drawBeam(0,g_pGame->m_pListenserverEdict.getEntity(),g_Map.m_Waypoints[iNWP].m_VOrigin + Vector(0,0,20),g_Map.m_Waypoints[iNWP].m_VOrigin - Vector(0,0,20)
					,10,0,Vector(255,255,255),127,0,fNextDiff*10);
			}

			hudtextparms_t message_params;
			static float fLastTime=0;

			if(!pInfo)
				pInfo = g_pGame->m_pListenserverEdict.getEntity();

			message_params.x = 0;
			message_params.y = 1;
			message_params.effect = 0;
			message_params.r1 = 0;
			message_params.g1 = 255;
			message_params.b1 = 255;
			message_params.a1 = 1;
			message_params.r2 = message_params.r1;
			message_params.g2 = message_params.g1;
			message_params.b2 = message_params.b1;
			message_params.a2 = 1;
			message_params.fadeinTime = 0.f;
			message_params.fadeoutTime = 0.f;
			message_params.holdTime = fNextDiff;
			message_params.fxTime = 0.f;
			message_params.channel = 1;

			fLastTime = g_pGame->getTime();

			long lNL = UTIL_CountLines(szDebugText);
			lNL = 17 - lNL;
			for(;lNL>0;lNL--){
				strcat(szDebugText,"\n");
			}
			szDebugText[500] = 0;
			szDebugText[501] = 0;

			UTIL_ShowText(g_pGame->m_pListenserverEdict.getEntity(),message_params,szDebugText);
		}
	}
}

void StartFrame( void )
{
	g_HPTime.markTick();
	g_Profiler.startProfile();

	g_lBeam = 0;
	
	g_pGame->startFrame();

	*g_szProfileOutput = 0;
	g_Profiler.profileDumpOutputToBuffer(g_szProfileOutput);
   
	(*other_gFunctionTable.pfnStartFrame)();
}

void ParmsNewLevel( void )
{
	(*other_gFunctionTable.pfnParmsNewLevel)();
}

void ParmsChangeLevel( void )
{
	(*other_gFunctionTable.pfnParmsChangeLevel)();
}

const char *GetGameDescription( void )
{
	return (*other_gFunctionTable.pfnGetGameDescription)();
}

void PlayerCustomization( edict_t *pEntity, customization_t *pCust )
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt", "a"); fprintf(fp, "PlayerCustomization: %x\n",pEntity); fclose(fp); }
#endif
	
	(*other_gFunctionTable.pfnPlayerCustomization)(pEntity, pCust);
}

void SpectatorConnect( edict_t *pEntity )
{
	(*other_gFunctionTable.pfnSpectatorConnect)(pEntity);
}

void SpectatorDisconnect( edict_t *pEntity )
{
	(*other_gFunctionTable.pfnSpectatorDisconnect)(pEntity);
}

void SpectatorThink( edict_t *pEntity )
{
	(*other_gFunctionTable.pfnSpectatorThink)(pEntity);
}

void Sys_Error( const char *error_string )
{
	(*other_gFunctionTable.pfnSys_Error)(error_string);
}

void PM_Move ( struct playermove_s *ppmove, int server )
{
	(*other_gFunctionTable.pfnPM_Move)(ppmove, server);
}

void PM_Init ( struct playermove_s *ppmove )
{
	(*other_gFunctionTable.pfnPM_Init)(ppmove);
}

char PM_FindTextureType( char *name )
{
	return (*other_gFunctionTable.pfnPM_FindTextureType)(name);
}

void SetupVisibility( edict_t *pViewEntity, edict_t *pClient, unsigned char **pvs, unsigned char **pas )
{
	(*other_gFunctionTable.pfnSetupVisibility)(pViewEntity, pClient, pvs, pas);
}

void UpdateClientData ( const struct edict_s *ent, int sendweapons, struct clientdata_s *cd )
{
	(*other_gFunctionTable.pfnUpdateClientData)(ent, sendweapons, cd);
}

int AddToFullPack( struct entity_state_s *state, int e, edict_t *ent, edict_t *host, int hostflags, int player, unsigned char *pSet )
{
	return (*other_gFunctionTable.pfnAddToFullPack)(state, e, ent, host, hostflags, player, pSet);
}

void CreateBaseline( int player, int eindex, struct entity_state_s *baseline, struct edict_s *entity, int playermodelindex, vec3_t player_mins, vec3_t player_maxs )
{
	(*other_gFunctionTable.pfnCreateBaseline)(player, eindex, baseline, entity, playermodelindex, player_mins, player_maxs);
}

void RegisterEncoders( void )
{
	(*other_gFunctionTable.pfnRegisterEncoders)();
}

int GetWeaponData( struct edict_s *player, struct weapon_data_s *info )
{
	return (*other_gFunctionTable.pfnGetWeaponData)(player, info);
}

void CmdStart( const edict_t *player, const struct usercmd_s *cmd, unsigned int random_seed )
{
	(*other_gFunctionTable.pfnCmdStart)(player, cmd, random_seed);
}

void CmdEnd ( const edict_t *player )
{
	(*other_gFunctionTable.pfnCmdEnd)(player);
}

int ConnectionlessPacket( const struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size )
{
	return (*other_gFunctionTable.pfnConnectionlessPacket)(net_from, args, response_buffer, response_buffer_size);
}

int GetHullBounds( int hullnumber, float *mins, float *maxs )
{
	return (*other_gFunctionTable.pfnGetHullBounds)(hullnumber, mins, maxs);
}

void CreateInstancedBaselines( void )
{
	(*other_gFunctionTable.pfnCreateInstancedBaselines)();
}

int InconsistentFile( const edict_t *player, const char *filename, char *disconnect_message )
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt", "a"); fprintf(fp, "InconsistentFile: %x filename=%s\n",player,filename); fclose(fp); }
#endif
	
	return (*other_gFunctionTable.pfnInconsistentFile)(player, filename, disconnect_message);
}

int AllowLagCompensation( void )
{
	return (*other_gFunctionTable.pfnAllowLagCompensation)();
}


DLL_FUNCTIONS gFunctionTable =
{
	GameDLLInit,               //pfnGameInit
		DispatchSpawn,             //pfnSpawn
		DispatchThink,             //pfnThink
		DispatchUse,               //pfnUse
		DispatchTouch,             //pfnTouch
		DispatchBlocked,           //pfnBlocked
		DispatchKeyValue,          //pfnKeyValue
		DispatchSave,              //pfnSave
		DispatchRestore,           //pfnRestore
		DispatchObjectCollsionBox, //pfnAbsBox
		
		SaveWriteFields,           //pfnSaveWriteFields
		SaveReadFields,            //pfnSaveReadFields
		
		SaveGlobalState,           //pfnSaveGlobalState
		RestoreGlobalState,        //pfnRestoreGlobalState
		ResetGlobalState,          //pfnResetGlobalState
		
		ClientConnect,             //pfnClientConnect
		ClientDisconnect,          //pfnClientDisconnect
		ClientKill,                //pfnClientKill
		ClientPutInServer,         //pfnClientPutInServer
		ClientCommand,             //pfnClientCommand
		ClientUserInfoChanged,     //pfnClientUserInfoChanged
		ServerActivate,            //pfnServerActivate
		ServerDeactivate,          //pfnServerDeactivate
		
		PlayerPreThink,            //pfnPlayerPreThink
		PlayerPostThink,           //pfnPlayerPostThink
		
		StartFrame,                //pfnStartFrame
		ParmsNewLevel,             //pfnParmsNewLevel
		ParmsChangeLevel,          //pfnParmsChangeLevel
		
		GetGameDescription,        //pfnGetGameDescription    Returns string describing current .dll game.
		PlayerCustomization,       //pfnPlayerCustomization   Notifies .dll of new customization for player.
		
		SpectatorConnect,          //pfnSpectatorConnect      Called when spectator joins server
		SpectatorDisconnect,       //pfnSpectatorDisconnect   Called when spectator leaves the server
		SpectatorThink,            //pfnSpectatorThink        Called when spectator sends a command packet (usercmd_t)
		
		Sys_Error,                 //pfnSys_Error          Called when engine has encountered an error
		
		PM_Move,                   //pfnPM_Move
		PM_Init,                   //pfnPM_Init            Server version of player movement initialization
		PM_FindTextureType,        //pfnPM_FindTextureType
		
		SetupVisibility,           //pfnSetupVisibility        Set up PVS and PAS for networking for this client
		UpdateClientData,          //pfnUpdateClientData       Set up data sent only to specific client
		AddToFullPack,             //pfnAddToFullPack
		CreateBaseline,            //pfnCreateBaseline        Tweak entity baseline for network encoding, allows setup of player baselines, too.
		RegisterEncoders,          //pfnRegisterEncoders      Callbacks for network encoding
		GetWeaponData,             //pfnGetWeaponData
		CmdStart,                  //pfnCmdStart
		CmdEnd,                    //pfnCmdEnd
		ConnectionlessPacket,      //pfnConnectionlessPacket
		GetHullBounds,             //pfnGetHullBounds
		CreateInstancedBaselines,  //pfnCreateInstancedBaselines
		InconsistentFile,          //pfnInconsistentFile
		AllowLagCompensation,      //pfnAllowLagCompensation
};

#ifdef __BORLANDC__
int EXPORT GetEntityAPI( DLL_FUNCTIONS *pFunctionTable, int interfaceVersion )
#else
extern "C" EXPORT int GetEntityAPI( DLL_FUNCTIONS *pFunctionTable, int interfaceVersion )
#endif
{
	// check if engine's pointer is valid and version is correct...
	
	if ( !pFunctionTable || interfaceVersion != INTERFACE_VERSION )
		return FALSE;
	
	// pass engine callback function table to engine...
	memcpy( pFunctionTable, &gFunctionTable, sizeof( DLL_FUNCTIONS ) );
	
	// pass other DLLs engine callbacks to function table...
	if (!(*other_GetEntityAPI)(&other_gFunctionTable, INTERFACE_VERSION))
	{
		return FALSE;  // error initializing function table!!!
	}
	
	return TRUE;
}


#ifdef __BORLANDC__
int EXPORT GetNewDLLFunctions( NEW_DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion )
#else
extern "C" EXPORT int GetNewDLLFunctions( NEW_DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion )
#endif
{
	if (other_GetNewDLLFunctions == NULL)
		return FALSE;
	
	// pass other DLLs engine callbacks to function table...
	if (!(*other_GetNewDLLFunctions)(pFunctionTable, interfaceVersion))
	{
		return FALSE;  // error initializing function table!!!
	}
	
	return TRUE;
}


void FakeClientCommand(edict_t *pBot, char *arg1, char *arg2, char *arg3)
{
	int length;
	
	memset(g_argv, 0, sizeof(g_argv));
	
	isFakeClientCommand = 1;
	
	if ((arg1 == NULL) || (*arg1 == 0))
		return;
	
	if ((arg2 == NULL) || (*arg2 == 0))
	{
		length = sprintf(&g_argv[0], "%s", arg1);
		fake_arg_count = 1;
	}
	else if ((arg3 == NULL) || (*arg3 == 0))
	{
		length = sprintf(&g_argv[0], "%s %s", arg1, arg2);
		fake_arg_count = 2;
	}
	else
	{
		length = sprintf(&g_argv[0], "%s %s %s", arg1, arg2, arg3);
		fake_arg_count = 3;
	}
	
	g_argv[length] = 0;  // null terminate just in case
	
	strcpy(&g_argv[64], arg1);
	
	if (arg2)
		strcpy(&g_argv[128], arg2);
	
	if (arg3)
		strcpy(&g_argv[192], arg3);
	
	// allow the MOD DLL to execute the ClientCommand...
	ClientCommand(pBot);
	
	isFakeClientCommand = 0;
}


const char *Cmd_Args( void )
{
	if (isFakeClientCommand)
	{
		return &g_argv[0];
	}
	else
	{
		return (*g_engfuncs.pfnCmd_Args)();
	}
}


const char *Cmd_Argv( int argc )
{
	if (isFakeClientCommand)
	{
		if (argc == 0)
		{
			return &g_argv[64];
		}
		else if (argc == 1)
		{
			return &g_argv[128];
		}
		else if (argc == 2)
		{
			return &g_argv[192];
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		return (*g_engfuncs.pfnCmd_Argv)(argc);
	}
}


int Cmd_Argc( void )
{
	if (isFakeClientCommand)
	{
		return fake_arg_count;
	}
	else
	{
		return (*g_engfuncs.pfnCmd_Argc)();
	}
}

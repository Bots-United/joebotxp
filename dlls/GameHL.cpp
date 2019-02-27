// CGameHL.cpp: implementation of the CGameHL class.
//
//////////////////////////////////////////////////////////////////////

#include "GameHL.h"
#include "entity_state.h"
#include "Map.h"
#include "bot.h"
#include "BaseBot.h"
#include "Observer.h"
#include "InterestingEntity.h"
#include "Names.h"
#include "Command.h"
#include "BaseTeam.h"

#include "PerceptionBot.h"

#include "Log.h"

#include <algorithm>
#include <iostream>
using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define _PAUSE_TIME 1
#define _MAXCFGLINESPERFRAME 5

CGame *g_pGame = new CGameHL;	// we need to put an instance here, cause otherwise we'd sometimes get crashes. if here would be 0, we would need additional if statements, which would be work, therefore it isnt done.

extern bool g_show_waypoints;
extern bool g_GameRules;
extern bool g_bJoinWHumanRES;
extern bool g_bJoinWHumanMAX;
extern int g_iMaxBots;

void UpdateClientData(const struct edict_s *ent, int sendweapons, struct clientdata_s *cd);
void ShowInfo(void);

CGameHL::CGameHL()
{
	memset(m_ppPlayer,0,sizeof(CPlayer*)*32);
	reset();
	
	m_fRoundStart = 0;
	m_fCheckServerCmd = 0;
	m_fClientUpdateTime = 0;
	m_fPreviousTime = -1;
	m_fBotCfgPauseTime = 0;
	m_fBotCheckTime = 2;
	m_fRespawnTime = 0;
	m_pBotCfgfp = 0;
	m_bNeed2OpenCfg = TRUE;
	m_bSpawnTimeReset = FALSE;

	m_iNumBots = 0;
	m_iPrevNumBots = 0;

	m_LTeamData.clear();
	
	for (int i=0; i<32; i++){
		SBInfo[i].bot_class = 0;
		SBInfo[i].bot_skill = 90;
		SBInfo[i].bot_team = 5;
		SBInfo[i].kick_time = 0;
		*SBInfo[i].name = 0;
		SBInfo[i].respawn_state = RESPAWN_IDLE;
		*SBInfo[i].skin = 0;
	}
}

CGameHL::~CGameHL(){
	// delete list of interesting entities is already done by CGame
}

void CGameHL::debugMsg(int iDebugLevel,const char *szFmt,...){
	char szText[256];
	va_list argp;
	va_start(argp, szFmt);
	vsprintf(szText, szFmt, argp);
	va_end(argp);

	if(IS_DEDICATED_SERVER()){
		if(szText)	cout << szText;
		cout.flush();
	}
	else
	{
		if(iDebugLevel){
			// check if this should be displayed

			if(!(iDebugLevel & m_lMsgDebug))		// if this shouldnt be displayed
				return;		// go back
		}

		if(m_pListenserverEdict)
			ClientPrint(VARS(m_pListenserverEdict),HUD_PRINTNOTIFY,szText,0,0,0,0);
	}
}

void CGameHL::userMsg(CEntity client, const char *szFmt,...){
	char szText[256];
	va_list argp;
	va_start(argp, szFmt);
	vsprintf(szText, szFmt, argp);
	va_end(argp);

	if(IS_DEDICATED_SERVER() && client.getEntity() == 0){
		if(szText)	cout << "JoeBOT :: "<<szText<<endl;
	}
	else
		if(client.getEntity())
			ClientPrint(VARS(client.getEntity()),HUD_PRINTNOTIFY,szText,0,0,0,0);
		else if(m_pListenserverEdict)
			ClientPrint(VARS(m_pListenserverEdict.getEntity()),HUD_PRINTNOTIFY,szText,0,0,0,0);
}

bool CGameHL::drawBeam(int iDebugLevel, CEntity Entity, const Vector &start, const Vector &end, int width,int noise, const Vector &color, int brightness, int speed,int iLife){
	if(!Entity.getEntity())				// if nothing is specified, take the listenserver entity
		Entity = m_pListenserverEdict;
	if(g_lBeam > 100 || !Entity.getEntity())
		return 1;

	if(iDebugLevel){
		// check if this should be displayed

		if(!(iDebugLevel & m_lBeamDebug))		// if this shouldnt be displayed
			return 0;		// go back
	}

	g_lBeam++;
	
	MESSAGE_BEGIN(MSG_ONE, SVC_TEMPENTITY, NULL, Entity.getEntity());
	WRITE_BYTE( TE_BEAMPOINTS);
	WRITE_COORD(start.x);
	WRITE_COORD(start.y);
	WRITE_COORD(start.z);
	WRITE_COORD(end.x);
	WRITE_COORD(end.y);
	WRITE_COORD(end.z);
	WRITE_SHORT( m_spriteTexture );
	WRITE_BYTE( 1 ); // framestart
	WRITE_BYTE( 10 ); // framerate
	WRITE_BYTE( iLife ); // life in 0.1's
	WRITE_BYTE( width ); // width
	WRITE_BYTE( noise );  // noise
	
	WRITE_BYTE( color.x );   // r
	WRITE_BYTE( color.y );   // g
	WRITE_BYTE( color.z );   // b
	
	WRITE_BYTE( brightness );   // brightness
	WRITE_BYTE( speed );    // speed
	MESSAGE_END();
	
	return 0;
}

bool CGameHL::createBot(const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	edict_t *BotEnt;
	CBaseBot *pBot;
	char c_skin[BOT_SKIN_LEN+1];
	char c_name[BOT_NAME_LEN+1];
	int skill = 100;
	//int index;
	//int i, j, length;
	const CNamesItem *pName;
	
	if(pName = g_Names.getName())
		strcpy( c_name, pName->m_szName );
	else
		strcpy( c_name, "bot" );
	
	BotEnt = CREATE_FAKE_CLIENT( c_name );
	
	if (FNullEnt( BotEnt ))
	{
		userMsg( m_pListenserverEdict, "Max. Players reached.  Can't create bot!\n");
	}
	else
	{
		char ptr[128];  // allocate space for message from ClientConnect
		char *infobuffer;
		int clientIndex;
		int iPlayerIndex;
		//CPlayer *pPlayerSlot;

		if (IS_DEDICATED_SERVER())
			printf("Creating bot...\n");
		else 
			userMsg( m_pListenserverEdict, "Creating bot...\n");
		
		//ppBot = getFreeSlot();
		
		/*if (!ppBot)
		{
			userMsg( m_pListenserverEdict, "Can't create bot!\n");
			return false;
		}*/
		
		// create the player entity by calling MOD's player function
		// (from LINK_ENTITY_TO_CLASS for player object)
		
		player( VARS(BotEnt) );
		
		infobuffer = GET_INFOBUFFER( BotEnt );
		clientIndex = ENTINDEX( BotEnt );
		
		SET_CLIENT_KEY_VALUE( clientIndex, infobuffer, "model", "gina" );
		
		ClientConnect( BotEnt, c_name, "127.0.0.1", ptr );
		
		// Pieter van Dijk - use instead of DispatchSpawn() - Hip Hip Hurray!
		ClientPutInServer( BotEnt );

		iPlayerIndex = isPlayer(BotEnt);

		//ppBot = &( (((CBaseBot**)m_ppPlayer)[iPlayerIndex]) );
		delete m_ppPlayer[iPlayerIndex];		// delete player, cause there we wanna put some bot

		if (iPlayerIndex == -1)
		{
			userMsg( m_pListenserverEdict, "Can't create bot!\n");
			return false;
		}
		
		BotEnt->v.flags |= FL_FAKECLIENT;
		
		// initialize all the variables for this bot...
		
		m_ppPlayer[iPlayerIndex] = pBot = allocBot();
		pBot->setEntity(BotEnt);
		
		//pBot->m_pPlayer = g_pGame->getPlayer(g_pGame->isPlayer(BotEnt));
		
		if(!pBot){
			userMsg( m_pListenserverEdict, "Can't create bot! - no player slots in CGameHL\n");
			return false;
		}
		
		pBot->m_respawn_state = RESPAWN_IDLE;
		pBot->m_create_time = g_pGame->getTime();
		pBot->m_name[0] = 0;  // name not set by server yet

		strcpy(pBot->m_skin, c_skin);
		
		pBot->m_not_started = 1;  // hasn't joined game yet
		
		pBot->m_start_action = 0;  // not needed for non-team MODs
		
		
		//      BotSpawnInit(pBot);
		
		pBot->m_need_to_initialize = FALSE;  // don't need to initialize yet
		
		BotEnt->v.idealpitch = BotEnt->v.v_angle.x;
		BotEnt->v.ideal_yaw = BotEnt->v.v_angle.y;
		BotEnt->v.pitch_speed = BOT_PITCH_SPEED;
		BotEnt->v.yaw_speed = BOT_YAW_SPEED;
		
		pBot->m_iSkill = skill;  // 0 based for array indexes
		
		pBot->m_iJoinTeam = atoi(arg1);
		pBot->m_iClass = -1;
		
		pBot->init();

		//pBot->m_pPlayer->m_pBot = pBot;		// set bot pointer
	}
	
	onBotCreation();
	return true;
}

CBaseBot *CGameHL::allocBot(void){
	return new CBaseBot;
}

CPlayer *CGameHL::allocPlayer(void){
	return new CPlayer;
}

// team data handling

CBaseTeam *CGameHL :: allocTeamData(void){
	return 0;		// since this is no team game by default, alloc no data :D
}

// startframe

void CGameHL::startFrame(void){
	g_Profiler.frame();
	m_lThought = 0;
	PROFILE("frame");
	//g_Log.process();

	if (gpGlobals->deathmatch)
	{
		/*if(g_AStarTasks.m_LTasks.size() < 50){
			AStarBase *pAStar = new AStarMachine<AStarGoalMod>;		// create a direct path

			pAStar->setDestination(RANDOM_LONG(0,g_Map.m_Waypoints.m_iNumWaypoints));
			pAStar->setStart(RANDOM_LONG(0,g_Map.m_Waypoints.m_iNumWaypoints));

			g_AStarTasks.addTask(pAStar,5,0,0);
		}*/
		int i, index, bot_index;
		unsigned int ui;
		char msg[256];
		int count;
		//check_server_cmd = 0;
		
		// if a new map has started then (MUST BE FIRST IN StartFrame)...
		checkMapChange();

		ShowInfo();

		// draw waypoints
		if(m_pListenserverEdict){
			if(g_show_waypoints){
				g_Map.m_Waypoints.drawTo(m_pListenserverEdict);
			}
		}
		
		updateClientData();

		g_Observer.observe();

		g_AStarTasks.run();

		count = 0;

		// let the teams think
		for(ui=0; ui < m_LTeamData.size(); ui++){
			m_LTeamData[ui]->think();
		}

		// let the bots think
		for (bot_index = 0; bot_index < m_iMaxCurrentPlayer; bot_index++){
			if (getBot(bot_index)){  // is this slot used ?
				//if(m_pListenserverEdict)g_pGame->userMsg( VARS(m_pListenserverEdict), HUD_PRINTNOTIFY, "debug_engine enabled!\n");
				if(getBot(bot_index)->think()){
					if(m_lThought){
						// if we had already a 'thought' this frame, add a little offset for the next think process
						// so we hope not to have another time more than one 'think' in one frame
						getBot(bot_index)->m_fNextThink += .025f*(float)m_lThought;		// .1f is normal diff.
					}
					m_lThought ++;
				}
				
				count++;
			}
		}
	
		if (count > m_iNumBots)
			m_iNumBots = count;
		
			/*for (player_index = 1; player_index <= gpGlobals->maxClients; player_index++)
			{
			pPlayer = INDEXENT(player_index);
			
			  if (pPlayer && !pPlayer->free)
			  {
			  if ((g_waypoint_on) && FBitSet(pPlayer->v.flags, FL_CLIENT))
			  {
			  WaypointThink(pPlayer);
			  }
			  }
		}*/
	
		if (g_GameRules)
		{
			if (m_bNeed2OpenCfg)  // have we opened bot.cfg file yet?
			{
				char filename[256];
				char mapname[64];
				
				m_bNeed2OpenCfg = FALSE;  // only do this once!!!
				
				// check if mapname_bot.cfg file exists...
				
				//strcpy(mapname, STRING(gpGlobals->mapname));
				strcpy(mapname, "bot.cfg");
				UTIL_BuildFileName(filename,"joebot","bot.cfg");
				
				//UTIL_BuildFileName(filename, "maps", mapname);
				
				if (!*filename||((m_pBotCfgfp = fopen(filename, "r")) != NULL))
				{
					sprintf(msg, "JoeBOT: Executing %s\n", filename);
					ALERT( at_console, msg );
					printf(msg);
				}
				else
				{
					UTIL_BuildFileName(filename, "bot.cfg", NULL);
					
					sprintf(msg, "JoeBOT: Executing %s\n", filename);
					ALERT( at_console, msg );
					
					m_pBotCfgfp = fopen(filename, "r");
					
					if (m_pBotCfgfp == NULL)
						ALERT( at_console, "bot.cfg file not found\n" );
				}
				
				if (IS_DEDICATED_SERVER())
					m_fBotCfgPauseTime = getTime() + 2.0;
				else
					m_fBotCfgPauseTime = getTime() + 20.0;
			}
			
			if (!IS_DEDICATED_SERVER() && !m_bSpawnTimeReset)
			{
				if (m_pListenserverEdict != NULL)
				{
					if (CEntity(m_pListenserverEdict).isAlive())
					{
						m_bSpawnTimeReset = TRUE;
						
						if (m_fRespawnTime >= 1.0)
							m_fRespawnTime = min(m_fRespawnTime, getTime() + 1.f);
						
						if (m_fBotCfgPauseTime >= 1.0)
							m_fBotCfgPauseTime = min(m_fBotCfgPauseTime, getTime() + 1.f);
					}
				}
			}
			
			if ((m_pBotCfgfp) &&
				(m_fBotCfgPauseTime >= 1.0) && (m_fBotCfgPauseTime <= getTime()))
			{
				// process bot.cfg file options...
				processBotCfgFile();
			}
			
		}    
		
		// are we currently respawning bots and is it time to spawn one yet?
		if (/*(respawn_time > 1.0) && */(m_fRespawnTime <= getTime()))
		{
			/*int index = 0;
			
			// find bot needing to be respawned...
			while ((index < 32) &&
				(SBInfo[index].respawn_state != RESPAWN_NEED_TO_RESPAWN))
				index++;
			
			if (index < 32)
			{
				if(!g_bJoinWHumanRES || (g_bJoinWHumanRES && UTIL_HumansInGame() != 0)){
					SBInfo[index].respawn_state = RESPAWN_IS_RESPAWNING;
					if(m_pBots[index]) delete m_pBots[index];      // free up this slot
					m_pBots[index] = 0;
					
					// respawn 1 bot then wait a while (otherwise engine crashes)
					if ((mod_id == VALVE_DLL) ||
						((mod_id == GEARBOX_DLL)))
					{
						char c_skill[2],c_team[2];
						
						sprintf(c_skill, "%d", SBInfo[index].bot_skill);
						sprintf(c_team, "%d", SBInfo[index].bot_team);
						
						g_pGame->createBot( c_team,SBInfo[index].skin, SBInfo[index].name, c_skill);
					}
					else
					{
						char c_skill[5];
						char c_team[5];
						char c_class[5];
						
						//cout << " ------------------- respawning after map change - wanting to respawn" << endl;
						
						sprintf(c_skill, "%i", SBInfo[index].bot_skill);
						sprintf(c_team, "%i", SBInfo[index].bot_team);
						sprintf(c_class, "%i", SBInfo[index].bot_class);
						
						if ((mod_id == TFC_DLL) || (mod_id == GEARBOX_DLL))
							g_pGame->createBot( NULL, NULL, SBInfo[index].name, c_skill);
						else
							g_pGame->createBot( c_team, c_class, SBInfo[index].name, c_skill);
					}
					
					m_fRespawnTime = getTime() + .5;  // set next respawn time
					
					m_fBotCheckTime = getTime() + _PAUSE_TIME - .1;
				}
				else
					m_fRespawnTime = getTime() + .5;
			}
			else
			{
				m_fRespawnTime = 0.0;
			}*/
			
			// check if time to see if a bot needs to be created...
			if (m_fBotCheckTime < getTime())
			{
				int count = 0;
				//int counthumans = UTIL_HumansInGame();
				
				m_fBotCheckTime = getTime() + 1.5;		//hackhack
				
				for (i = 0; i < 32; i++)
				{
					if (m_ppPlayer[i] != NULL)
						count++;
				}
				
				// if there are currently less than the maximum number of "players"
				// then add another bot using the default skill level...
				if ((count < g_iMaxBots) && (g_iMaxBots != -1))
				{
					//cout << " ------------------- creating bot due to max_bots" << endl;
					if(!g_bJoinWHumanMAX){
						g_pGame->createBot(NULL, NULL, NULL, NULL);
					}
					else{
						if ( UTIL_HumansInGame() != 0){
							g_pGame->createBot(NULL, NULL, NULL, NULL);
						}
					}
				}
			}
		}  
		
		// if time to check for server commands then do so...
		if ((m_fCheckServerCmd <= getTime()) && IS_DEDICATED_SERVER())
		{
			m_fCheckServerCmd = getTime() + 1.0;
			
			char *cvar_bot = (char *)CVAR_GET_STRING( "joebot" );
			
			if ( cvar_bot && cvar_bot[0] )
			{
				char cmd_line[80];
				char *cmd, *arg1, *arg2, *arg3, *arg4, *arg5, *arg6;
				
				strcpy(cmd_line, cvar_bot);
				
				index = 0;
				cmd = cmd_line;
				//cout << cmd << endl;
				arg1 = arg2 = arg3 = arg4 = arg5 = arg6 = NULL;
				
				// skip to blank or end of string...
				while ((cmd_line[index] != '|') && (cmd_line[index] != 0))
					index++;
				
				if (cmd_line[index] == '|')
				{
					cmd_line[index++] = 0;
					arg1 = &cmd_line[index];
					
					// skip to blank or end of string...
					while ((cmd_line[index] != '|') && (cmd_line[index] != 0))
						index++;
					
					if (cmd_line[index] == '|')
					{
						cmd_line[index++] = 0;
						arg2 = &cmd_line[index];
						
						// skip to blank or end of string...
						while ((cmd_line[index] != '|') && (cmd_line[index] != 0))
							index++;
						
						if (cmd_line[index] == '|')
						{
							cmd_line[index++] = 0;
							arg3 = &cmd_line[index];
							
							// skip to blank or end of string...
							while ((cmd_line[index] != '|') && (cmd_line[index] != 0))
								index++;
							
							if (cmd_line[index] == '|')
							{
								cmd_line[index++] = 0;
								arg4 = &cmd_line[index];
								
								// skip to blank or end of string...
								while ((cmd_line[index] != '|') && (cmd_line[index] != 0))
									index++;
								
								if (cmd_line[index] == '|')
								{
									cmd_line[index++] = 0;
									arg5 = &cmd_line[index];
									
									// skip to blank or end of string...
									while ((cmd_line[index] != '|') && (cmd_line[index] != 0))
										index++;
									
									if (cmd_line[index] == '|')
									{
										cmd_line[index++] = 0;
										arg6 = &cmd_line[index];
									}
								}
							}
						}
					}
				}
				g_Commands.Exec(0,CM_DEDICATED,cmd,arg1,arg2,arg3,arg4);
				
				CVAR_SET_STRING("joebot", "");
			}
		}
		
		m_fPreviousTime = getTime();
   }
}

void CGameHL::checkMapChange(void){
	if ((getTime() + 0.1) < m_fPreviousTime)
	{
		char filename[256];
		char mapname[64];
		char msg[80];
		int count;
		int index;
		
		m_fCheckServerCmd = 0.0;  // reset at start of map
		
		// check if mapname_bot.cfg file exists...
		
		//strcpy(mapname, STRING(gpGlobals->mapname));
		strcpy(mapname, "bot.cfg");
		UTIL_BuildFileName(filename,"joebot","bot.cfg");
		
		//UTIL_BuildFileName(filename, "maps", mapname);
		
		if ((m_pBotCfgfp = fopen(filename, "r")) != NULL)
		{
			sprintf(msg, "JoeBOT: Executing %s\n", filename);
			ALERT( at_console, msg );
			printf(msg);
			
			/*for (index = 0; index < 32; index++)
			{
			m_pBots[index].is_used = FALSE;
			m_pBots[index].respawn_state = 0;
			m_pBots[index].kick_time = 0.0;
		}*/
			if (IS_DEDICATED_SERVER())
				m_fBotCfgPauseTime = getTime() + _PAUSE_TIME;
			else
				m_fBotCfgPauseTime = getTime() + 20.0;
		}
		/*else
		{*/
		count = 0;
		
		// mark the bots as needing to be respawned...
		for (index = 0; index < 32; index++)
		{
			if (count >= m_iPrevNumBots)
			{
				//m_pBots[index].is_used = FALSE;
				SBInfo[index].respawn_state = 0;
				SBInfo[index].kick_time = 0.0;
			}
			
			/*if (m_pBots[index])  // is this slot used?
			{
				SBInfo[index].respawn_state = RESPAWN_NEED_TO_RESPAWN;
				count++;
			}*/
			
			// check for any bots that were very recently kicked...
			if ((SBInfo[index].kick_time + 5.0) > m_fPreviousTime)
			{
				SBInfo[index].respawn_state = RESPAWN_NEED_TO_RESPAWN;
				count++;
			}
			else
				SBInfo[index].kick_time = 0.0;  // reset to prevent false spawns later
		}
		
		// set the respawn time
		if (IS_DEDICATED_SERVER())
			m_fRespawnTime = getTime() + _PAUSE_TIME;
		else
			m_fRespawnTime = getTime() + _PAUSE_TIME;
		//}
		
		m_fClientUpdateTime = getTime() + 10.0;  // start updating client data again
		
		//bot_check_time = getTime() + 30.0;
		m_fBotCheckTime = getTime() + _PAUSE_TIME*2;
	}
}

void CGameHL::updateClientData(void){
	if (m_fClientUpdateTime <= getTime()){
		m_fClientUpdateTime = getTime() + 1.0;
		clientdata_s cd;
		int i;
		
		for (i=0; i < 32; i++)
		{
			if (getBot(i))
			{
				memset(&cd, 0, sizeof(cd));
				
				UpdateClientData( getBot(i)->getEntity(), 1, &cd );
				
				// see if a weapon was dropped...
				if (getBot(i)->m_pWeapons->m_iWeapons != cd.weapons)
				{
					getBot(i)->m_pWeapons->m_iWeapons = cd.weapons;
				}
			}
		}
	}
}

void CGameHL::traceLine( const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, IGNORE_GLASS ignoreGlass, CEntity pentIgnore, TraceResult *ptr ){
	TRACE_LINE( vecStart, vecEnd, (igmon == ignore_monsters ? TRUE : FALSE) | (ignoreGlass?0x100:0), pentIgnore, ptr );
}

void CGameHL::traceLine( const Vector &vecStart, const Vector &vecEnd, CEntity pentIgnore, TraceResult *ptr ){
	TRACE_LINE( vecStart, vecEnd, TRUE /* let's ignore monsters */, pentIgnore, ptr );
}

void CGameHL::traceHull( const Vector &vecStart, const Vector &vecEnd, HULLS iHullNumber, edict_t *pentIgnore, TraceResult *ptr ){
	TRACE_HULL( vecStart, vecEnd, TRUE /* let's ignore monsters */,iHullNumber, pentIgnore, ptr );
}

void CGameHL::traceHull( const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, IGNORE_GLASS ignoreGlass,HULLS iHullNumber, edict_t *pentIgnore, TraceResult *ptr ){
	TRACE_HULL( vecStart, vecEnd, (igmon == ignore_monsters ? TRUE : FALSE) | (ignoreGlass?0x100:0),iHullNumber, pentIgnore, ptr );
}

bool CGameHL::fVisibleEx(Vector v_start, Vector v_end, bool bignore_doors, bool bignore_breakables, bool bignore_monsters, bool bignore_glass,CEntity pEdictIgnore){
	TraceResult tr1,tr2;
	char	szDoor[] = "func_door",
			szBreakable[] = "func_breakable";
	
	g_pGame->traceLine(v_start,v_end,bignore_monsters?ignore_monsters:dont_ignore_monsters,bignore_glass?ignore_glass:dont_ignore_glass,pEdictIgnore,&tr1);
	//UTIL_TraceHull(v_start,v_end,bignore_monsters?ignore_monsters:dont_ignore_monsters,bignore_glass?ignore_glass:dont_ignore_glass,iHull,pEdictIgnore,&tr1);

	if(tr1.flFraction == 1.f)
		return true;

	Vector v_dir = (v_end - v_start).normalize();
	
	if(tr1.pHit){
		if(bignore_doors
			&&!strncmp(STRING(tr1.pHit->v.classname),szDoor,9)){
			// it's a door
			// trace vice versa
			g_pGame->traceLine(v_end,v_start,bignore_monsters?ignore_monsters:dont_ignore_monsters,bignore_glass?ignore_glass:dont_ignore_glass,pEdictIgnore,&tr2);
			
			if(tr1.pHit == tr2.pHit)		// the same door
				return true;

			return fVisibleEx(tr1.vecEndPos + v_dir*.001,tr2.vecEndPos - v_dir * .001,bignore_doors,bignore_breakables,bignore_monsters,bignore_glass,pEdictIgnore);
		}
		else if(bignore_breakables
			&&!strncmp(STRING(tr1.pHit->v.classname),szBreakable,14)){
			// it's a breakable
			if(tr1.pHit->v.takedamage == DAMAGE_YES){		// can be destroyed
				g_pGame->traceLine(v_end,v_start,bignore_monsters?ignore_monsters:dont_ignore_monsters,bignore_glass?ignore_glass:dont_ignore_glass,pEdictIgnore,&tr2);
				//UTIL_TraceHull(v_end,v_start,bignore_monsters?ignore_monsters:dont_ignore_monsters,bignore_glass?ignore_glass:dont_ignore_glass,iHull,pEdictIgnore,&tr2);
				if(tr1.pHit == tr2.pHit)		// the same breakable
					return true;

				return fVisibleEx(tr1.vecEndPos + v_dir*.001,tr2.vecEndPos - v_dir * .001,bignore_doors,bignore_breakables,bignore_monsters,bignore_glass,pEdictIgnore);
			}
			else
				return false;
		}
		else
			return false;
	}
	else
		return false;
}
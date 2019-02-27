// CGame.cpp: implementation of the CGame class.
//
//////////////////////////////////////////////////////////////////////

#include "Game.h"
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

extern bool g_show_waypoints;
extern bool g_GameRules;
extern bool g_bJoinWHumanRES;
extern bool g_bJoinWHumanMAX;
extern int g_iMaxBots;

void UpdateClientData(const struct edict_s *ent, int sendweapons, struct clientdata_s *cd);
void ShowInfo(void);

char g_szDebugIdent[][80] = {
	"system",		// DEBUG_SYSTEM
	"game",			// DEBUG_GAME
	"team",			// DEBUG_TEAM
	"squad",		// DEBUG_SQUAD
	"bot",			// DEBUG_BOT
	"nav",			// DEBUG_NAV
	"action",		// DEBUG_ACTION
	"\0"
};

CGame::CGame(){
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

	m_lBeamDebug = 0;		// no beams to show yet
	///m_lMsgDebug = 0;
}

CGame::~CGame(){
	// delete list of interesting enteties
	for_each(m_LIEntities.begin(),m_LIEntities.end(),Destroy<CIEntity *>());
	for_each(m_LTeamData.begin(),m_LTeamData.end(),Destroy<CBaseTeam *>());
}

void CGame::reset(void){
	int i;
	for(i=0; i < 32; i++){
		if(m_ppPlayer[i]){
			removePlayer(m_ppPlayer[i]->getEntity());
		}
	}
	m_iMaxCurrentPlayer = 0;
	//m_iMaxCurrentBot = 0;
	
	m_bTeamplayChecked = false;
}

bool CGame::addPlayer(edict_t *pEntity){
	int i,ifree = -1;
	
	i = isPlayer(pEntity);
	
	if(i==-1){
		ifree=0;
		while(m_ppPlayer[ifree]
			&&ifree<32)
			ifree++;
	}
	else
		return true;		// this player is already here
	
	if(ifree<=31){
		m_ppPlayer[ifree] = allocPlayer();
		m_ppPlayer[ifree]->setEntity(pEntity);
		if(ifree==m_iMaxCurrentPlayer)
			m_iMaxCurrentPlayer ++;
		return true;
	}
	else{
		debugMsg(DEBUG_GAME,"some real shit is going on in CGame::addPlayer");
		return false;
	}
}

bool CGame::removePlayer(edict_t *pEntity){
	int i = isPlayer(pEntity),
		iB;
	unsigned int ui;

	if(i ==-1){
		debugMsg(DEBUG_GAME,"cannot remove player that's not existing in CGame::removePlayer");
		return false;
	}
	
	if(i<32){
		// tell the teams about it, the team in which the player was will find out itself if this call has to be taken seriously :D
		for(ui = 0; ui < m_LTeamData.size(); ui++){
			m_LTeamData[ui]->removeMember(i);
		}
		if((iB = isBot(m_ppPlayer[i]))!=-1){
			cout << "it's a bot that has to be removed" << endl;
			SBInfo[iB].bot_class = getBot(iB)->m_iClass;
			SBInfo[iB].bot_skill = getBot(iB)->m_iSkill;
			SBInfo[iB].bot_team = getBot(iB)->m_iTeam;
			strcpy(SBInfo[iB].name,getBot(iB)->m_name);
			strcpy(SBInfo[iB].skin,getBot(iB)->m_skin);
			
			SBInfo[iB].kick_time = getTime();  // save the kicked time
			
			//freeBotSlot(iB);
		}
		m_ppPlayer[i]->setEntity(0);
		delete m_ppPlayer[i];
		m_ppPlayer[i] = 0;
		if(i==m_iMaxCurrentPlayer-1)
			m_iMaxCurrentPlayer --;

		// remove perceptions
		int i;
		for(i=0; i<m_iMaxCurrentPlayer; i++){
			if(getBot(i)){
				getBot(i)->m_pPerception->removePerception(pEntity);
			}
		}
		return true;
	}
	else{
		debugMsg(DEBUG_GAME, "some real shit is going on in CGame::removePlayer");
		return false;
	}
}

CPlayer *CGame::preAddPlayer(void){
	int i;
	i=0;
	while(m_ppPlayer[i]
		&&i<32)
		i++;
	
	if(i<32){
		m_ppPlayer[i] = allocPlayer();
		if(i==m_iMaxCurrentPlayer)
			m_iMaxCurrentPlayer ++;
		return m_ppPlayer[i];
	}
	else{
		debugMsg(DEBUG_GAME,"some real shit is going on in CGame::preAddPlayer");
		return 0;
	}
}

int CGame :: isPlayer(CEntity pEntity){
	int i;
	
	i=0;
	while(i<32){
		if(m_ppPlayer[i]
			&& m_ppPlayer[i]->getEntity()==pEntity){
			break;
		}
		i++;
	}
	
	if(i>=32)
		i = -1;
	
	return i;
}

int CGame :: isPlayer(CPlayer *pPlayer){
	int i;
	
	i=0;
	while(i<m_iMaxCurrentPlayer){
		if(m_ppPlayer[i]
			&& m_ppPlayer[i]==pPlayer){
			return i;
		}
		i++;
	}
	
	return -1;
}

bool CGame::getTeamplay(void){
	if(!m_bTeamplayChecked){
		m_bTeamplay = CVAR_GET_FLOAT("mp_teamplay") > 0.0;
		m_bTeamplayChecked = true;
	}
	return m_bTeamplay;
}

int CGame::getMaxCurrentPlayer(void){
	return m_iMaxCurrentPlayer;
}

CPlayer *CGame::getPlayer(int iIndex){
	if(iIndex >=0&&iIndex < 32){
		return m_ppPlayer[iIndex];
	}
	else
		return 0;
}

void CGame::onMapStart(void){
	unsigned int i;
	g_Map.reset();
	g_Map.load();
	g_Observer.reset();
	
	m_bTeamplayChecked = false;
	for(i=0; i < 32; i++){
		delete m_ppPlayer[i];
		m_ppPlayer[i] = 0;
	}

	m_fBotCfgPauseTime = 0.0;
	m_iPrevNumBots = m_iNumBots;
	m_iNumBots = 0;
	m_fBotCheckTime = getTime() + _PAUSE_TIME;
	m_fRespawnTime = 0.0;
	m_bSpawnTimeReset = FALSE;

	for(i = 0; i < m_LTeamData.size(); i++){
		if(m_LTeamData[i])
			m_LTeamData[i]->reset();
	}
}

void CGame::onMapEnd(void){
	g_Map.save();
}

void CGame::onRoundStart(void){
	m_fRoundStart = g_pGame->getTime();

	unsigned int ui;
	for(ui=0; ui < m_LTeamData.size(); ui++){
		m_LTeamData[ui]->onRoundStart();
	}
}

CBaseBot *CGame::allocBot(void){
	return new CBaseBot;
}

void CGame::onBotCreation(void){
	if(!g_Map.m_Waypoints.m_iNumWaypoints){
		g_Map.createBasicWaypoints();
	}
}

long CGame :: weaponClass2ID(const char *szClassname){
	long lschl;
	for(lschl=0;lschl < 32;lschl ++){
		if(!strcmp(szClassname,weapon_defs[lschl].szClassname)){
			return lschl;
		}
	}
	return -1;
}

// team data handling

CBaseTeam *CGame :: allocTeamData(void){
	return 0;		// since this is no team game by default, alloc no data :D
}

CBaseTeam *CGame :: getTeamData(int iTeam){
	if(iTeam < 0)
		return 0;

	if(iTeam < (int)m_LTeamData.size()){
		return m_LTeamData[iTeam];
	}
	else{
		CBaseTeam *pNewTD;
		assert(iTeam >= 0 && iTeam < 4);

		int i = (int)m_LTeamData.size();
		for(; i <= iTeam; i++){
			m_LTeamData.push_back(pNewTD = allocTeamData());

			pNewTD->m_iTeamID = i;
		}

		return m_LTeamData[iTeam];
	}
}

// something to optimize
int CGame::getTeamMembers(int iTeam){
	int i = 0,iCount = 0;
	
	while(i<m_iMaxCurrentPlayer){
		if(getPlayer(i)->getTeam() == iTeam)
			iCount++;

		i++;
	}
	return iCount;
}

// m_pBots handling

CPlayer **CGame :: getFreeSlot(void){
	int i = 0;
	while(i < 32){
		if(!m_ppPlayer[i]){
			if(i >= m_iMaxCurrentPlayer)
				m_iMaxCurrentPlayer ++;
			return &(m_ppPlayer[i]);
		}
		i++;
	}
	return 0;
}

int CGame :: isBot(const edict_t *pEntity){
	int i;
	for(i = 0; i < m_iMaxCurrentPlayer; i++){
		if(m_ppPlayer[i]){
			if(m_ppPlayer[i]->isBot()){
				if(m_ppPlayer[i]->getEntity() == pEntity){
					return i;
				}
			}
		}
	}
	return -1;
}

int CGame :: isBot(const CPlayer *pPlayer){
	int i;
	for(i = 0; i < m_iMaxCurrentPlayer; i++){
		if(m_ppPlayer[i]){
			if(m_ppPlayer[i]->isBot()){
				if(m_ppPlayer[i] == pPlayer){
					return i;
				}
			}
		}
	}
	return -1;
}

int CGame :: isBot(const CBaseBot *pBot){
	int i;
	for(i = 0; i < m_iMaxCurrentPlayer; i++){
		if(m_ppPlayer[i]){
			if(m_ppPlayer[i]->isBot()){
				if(m_ppPlayer[i] == (CPlayer*)pBot){
					return i;
				}
			}
		}
	}
	return -1;
}

void CGame::processBotCfgFile(void)
{
	int iLPF;
	int ch;
	char cmd_line[256];
	int cmd_index;
	static char server_cmd[256];
	char *cmd, *arg1, *arg2, *arg3, *arg4,*arg5, *arg6;
	//	char msg[80];
	
	if (m_fBotCfgPauseTime > getTime())
		return;
	
	for(iLPF = 0;iLPF < _MAXCFGLINESPERFRAME;iLPF++){
		if (m_pBotCfgfp == NULL)
			return;
		
		cmd_index = 0;
		cmd_line[cmd_index] = 0;
		
		ch = fgetc(m_pBotCfgfp);
		
		// skip any leading blanks
		while (ch == ' ')
			ch = fgetc(m_pBotCfgfp);
		
		while ((ch != EOF) && (ch != '\r') && (ch != '\n'))
		{
			if (ch == '\t')  // convert tabs to spaces
				ch = ' ';
			
			cmd_line[cmd_index] = ch;
			
			ch = fgetc(m_pBotCfgfp);
			
			// skip multiple spaces in input file
			while ((cmd_line[cmd_index] == ' ') &&
				(ch == ' '))      
				ch = fgetc(m_pBotCfgfp);
			
			cmd_index++;
		}
		
		if (ch == '\r')  // is it a carriage return?
		{
			ch = fgetc(m_pBotCfgfp);  // skip the linefeed
		}
		
		// if reached end of file, then close it
		if (ch == EOF)
		{
			fclose(m_pBotCfgfp);
			
			m_pBotCfgfp = NULL;
			
			m_fBotCfgPauseTime = 0.0;
		}
		
		cmd_line[cmd_index] = 0;  // terminate the command line
		
		// copy the command line to a server command buffer...
		strcpy(server_cmd, cmd_line);
		strcat(server_cmd, "\n");
		
		cmd_index = 0;
		cmd = cmd_line;
		arg1 = arg2 = arg3 = arg4 = arg5 = arg6 = NULL;
		
		// skip to blank or end of string...
		while ((cmd_line[cmd_index] != ' ') && (cmd_line[cmd_index] != 0))
			cmd_index++;
		
		if (cmd_line[cmd_index] == ' ')
		{
			cmd_line[cmd_index++] = 0;
			arg1 = &cmd_line[cmd_index];
			
			// skip to blank or end of string...
			while ((cmd_line[cmd_index] != ' ') && (cmd_line[cmd_index] != 0))
				cmd_index++;
			
			if (cmd_line[cmd_index] == ' ')
			{
				cmd_line[cmd_index++] = 0;
				arg2 = &cmd_line[cmd_index];
				
				// skip to blank or end of string...
				while ((cmd_line[cmd_index] != ' ') && (cmd_line[cmd_index] != 0))
					cmd_index++;
				
				if (cmd_line[cmd_index] == ' ')
				{
					cmd_line[cmd_index++] = 0;
					arg3 = &cmd_line[cmd_index];
					
					// skip to blank or end of string...
					while ((cmd_line[cmd_index] != ' ') && (cmd_line[cmd_index] != 0))
						cmd_index++;
					
					if (cmd_line[cmd_index] == ' ')
					{
						cmd_line[cmd_index++] = 0;
						arg4 = &cmd_line[cmd_index];
						
						// skip to blank or end of string...
						while ((cmd_line[cmd_index] != ' ') && (cmd_line[cmd_index] != 0))
							cmd_index++;
						
						if (cmd_line[cmd_index] == ' ')
						{
							cmd_line[cmd_index++] = 0;
							arg5 = &cmd_line[cmd_index];
							
							// skip to blank or end of string...
							while ((cmd_line[cmd_index] != ' ') && (cmd_line[cmd_index] != 0))
								cmd_index++;
							
							if (cmd_line[cmd_index] == ' ')
							{
								cmd_line[cmd_index++] = 0;
								arg6 = &cmd_line[cmd_index];
							}
						}
					}
				}
			}
		}
		
		//printf("%s-%s-%s-%s-%s\n",cmd,arg1,arg2,arg3,arg4);
		if ((cmd_line[0] == '#') || (cmd_line[0] == 0))
			continue;  // return if comment or blank line
		
		if(g_Commands.Exec(0,CM_SCRIPT,cmd,arg1,arg2,arg3,arg4))
			continue;
		
		//sprintf(msg, "JoeBOT : executing server command: %s - is the syntax right ?\n", server_cmd);
		//ALERT( at_console, msg );
		
		//if (IS_DEDICATED_SERVER())
		//	printf(msg);
		
		SERVER_COMMAND(server_cmd);
	}
}

bool CGame :: fInViewCone(const Vector &VOrigin, CEntity Edict)
{
	Vector2D V2LOS;
	float    flDot;
	
	//UTIL_MakeVectors ( Edict.getAngles() );
	
	V2LOS = ( VOrigin - Edict.getOrigin() ).make2D();
	V2LOS = V2LOS.normalize();
	
	flDot = DotProduct (V2LOS , Edict.getAngles().angles2vec().make2D() );
	
	if ( flDot > 0.50 )  // 60 degree field of view 
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

bool CGame :: fVisible( const Vector &vecOrigin, CEntity Edict )
{
	TraceResult tr;
	Vector      vecLookerOrigin;
	
	// look through caller's eyes
	vecLookerOrigin = Edict.getOrigin() + Edict.getVOffset();
	
	traceLine(vecLookerOrigin, vecOrigin, ignore_monsters, ignore_glass, Edict, &tr);
	
	if (tr.flFraction != 1.0)
	{
		return FALSE;// Line of sight is not established
	}
	else
	{
		return TRUE;// line of sight is valid.
	}
}

bool CGame::fVisibleEx(Vector v_start, Vector v_end, bool bignore_doors, bool bignore_breakables, bool bignore_monsters, bool bignore_glass,CEntity pEdictIgnore){
	TraceResult tr;
	
	traceLine(v_start, v_end, ignore_monsters, ignore_glass, pEdictIgnore, &tr);
	
	if (tr.flFraction != 1.0)
	{
		return FALSE;// Line of sight is not established
	}
	else
	{
		return TRUE;// line of sight is valid.
	}
}
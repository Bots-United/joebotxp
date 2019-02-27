// GameDOD.cpp: implementation of the CGameDOD class.
//
//////////////////////////////////////////////////////////////////////

#include "GameDOD.h"
#include "BotDOD.h"

#include "Map.h"
#include "bot.h"

#include "InterestingEntity.h"

#include "Names.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameDOD::CGameDOD(){
	// all this pIEntity = new looks useless, and it is, mainly
	// but so you can use the pointer to get the CIEntity::m_iId easily

	CIEntity *pIEntity;

	mod_id = DOD_DLL;
	m_iMod_Id = DOD_DLL;

	m_LIEntities.push_back(pIEntity = new CInterestingLadder("func_ladder"));

	m_LIEntities.push_back(pIEntity = new CIEntity("info_player_allies",true));
	m_LIEntities.push_back(pIEntity = new CIEntity("info_player_axis",true));

	m_LIEntities.push_back(pIEntity = new CIEntity("dod_control_point",true));

	m_LIEntities.push_back(pIEntity = new CInterestingWeapon("grenade",DOD_WEAPON_HANDGRENADE,false));			// allied grenade
	m_LIEntities.push_back(pIEntity = new CInterestingWeapon("grenade2",DOD_WEAPON_STICKGRENADE,false));		// axis grenade

	// create team data ...
	getTeamData(1);
}

CGameDOD::~CGameDOD()
{

}

bool CGameDOD::getTeamplay(void){
	return true;
}

bool CGameDOD::createBot(const char *arg1,const char *arg2,const char *arg3,const char *arg4){
		edict_t *BotEnt;
	CBaseBot *pBot,**ppBot;
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

		ppBot = &( (((CBaseBot**)m_ppPlayer)[iPlayerIndex]) );
		delete m_ppPlayer[iPlayerIndex];		// delete player, cause there we wanna put some bot

		if (!ppBot)
		{
			userMsg( m_pListenserverEdict, "Can't create bot!\n");
			return false;
		}
		
		BotEnt->v.flags |= FL_FAKECLIENT;
		
		// initialize all the variables for this bot...
		
		pBot = *ppBot = allocBot();
		
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
		
		pBot->m_iTeam = -1;
		pBot->m_iClass = -1;

		if ((arg1 != NULL) && (arg1[0] != 0)){
			pBot->m_iTeam = atoi(arg1);
			
			if ((arg2 != NULL) && (arg2[0] != 0)){
				pBot->m_iClass = atoi(arg2);
			}
		}
		
		pBot->init();
	}
	
	onBotCreation();
	return true;
}

CBaseBot *CGameDOD::allocBot(void){
	return new CBotDOD;
}
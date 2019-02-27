// BotCS.cpp: implementation of the CBotCS class.
//
//////////////////////////////////////////////////////////////////////

#include "BotCS.h"

#include "BV_HLDM.h"
#include "BV_CS.h"

#include "Player.h"
#include "ActionHL.h"
#include "PerceptionBot.h"
#include "WeaponsCS.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBotCS::CBotCS(){
	m_bInBuyZone = false;
	m_bHasDefuser = false;
	m_bGlockBurst = false;
	m_fRoundTime = 5;

	m_pAction = new CActionHL;
	m_pPerception = new CPerceptionBot;
	m_pWeapons = new CWeaponsCS;

	m_LpBehaviour.push_back(m_pMovePath = new CBVNP_HLDM_MovePath(this));
	m_LpBehaviour.push_back(m_pCamp = new CBVNP_HLDM_Camp(this));
	//m_pCamp = new CBVNP_HLDM_Camp(this);

	m_LpBehaviour.push_back(new CBVNP_HLDM_Duck(this));
	m_LpBehaviour.push_back(new CBVNP_HLDM_AvoidTK(this));
	m_LpBehaviour.push_back(new CBVNP_HLDM_ShootObstacle(this));
	m_LpBehaviour.push_back(new CBVNP_HLDM_Jump(this));
	m_LpBehaviour.push_back(new CBVNP_HLDM_AvoidStuck(this));
	m_LpBehaviour.push_back(new CBVNP_HLDM_AvoidStuckWP(this));
	m_LpBehaviour.push_back(new CBVNP_HLDM_Ladder(this));
	//m_LpBehaviour.push_back(new CBVNP_HLDM_RunAround(this));
	m_LpBehaviour.push_back(new CBVNP_HLDM_LookAround(this));
	m_LpBehaviour.push_back(new CBVNP_HLDM_Explore(this));
	m_LpBehaviour.push_back(new CBVNP_HLDM_ZoomBack(this));

	m_LpBehaviour.push_back(new CBVNP_CS_Buy(this));
	m_LpBehaviour.push_back(new CBVNP_CS_PlantBomb(this));

	m_LpBehaviour.push_back(new CBV_HLDM_WatchEnemy(this));
	m_LpBehaviour.push_back(new CBV_HLDM_ShootEnemy(this));
	m_LpBehaviour.push_back(new CBV_HLDM_ExplGrenade(this));
	m_LpBehaviour.push_back(new CBV_HLDM_WatchDamage(this));
	m_LpBehaviour.push_back(new CBV_HLDM_Hear(this));
	m_LpBehaviour.push_back(new CBV_HLDM_AdjustLadder(this));
	m_LpBehaviour.push_back(new CBV_HLDM_CheckKill(this));
	m_LpBehaviour.push_back(new CBV_HLDM_SuppFire(this));
	m_LpBehaviour.push_back(new CBV_HLDM_SuppFireAc(this));
	//m_LpBehaviour.push_back(new CBV_HLDM_Run2Sound(this));

	m_LpBehaviour.push_back(new CBV_HLDM_AttackDirect(this));
	m_LpBehaviour.push_back(new CBV_HLDM_AttackIndirect(this));
	m_LpBehaviour.push_back(new CBV_HLDM_Pullback(this));
	m_LpBehaviour.push_back(new CBV_HLDM_HideReload(this));

	m_LpBehaviour.push_back(new CBVLT_HLDM_AttackFollow(this));
	m_LpBehaviour.push_back(new CBVLT_HLDM_AttackSnipe(this));

	//m_LpBehaviour.push_back(new CBV_CS_WatchTMate(this));
	m_LpBehaviour.push_back(new CBV_CS_PickupWeapon(this));
	m_LpBehaviour.push_back(new CBV_CS_UseHostage(this));
	m_LpBehaviour.push_back(new CBV_CS_GotoBomb(this));
	m_LpBehaviour.push_back(new CBV_CS_LookAtBomb(this));
	m_LpBehaviour.push_back(new CBV_CS_DefuseBomb(this));
	m_LpBehaviour.push_back(new CBV_CS_GuardWBBomb(this));

	m_LpBehaviour.push_back(m_pSquadCommand = new CBV_CS_SquadCommand(this));

	m_LpEmotion.push_back(m_pEMTeam = new CEM_HLDM_Team(this));
	m_LpEmotion.push_back(m_pEMEnemy = new CEM_HLDM_Enemy(this));
	m_LpEmotion.push_back(m_pEMLEnemy = new CEM_HLDM_LastEnemy(this));

	//CBaseBot::CBaseBot();
}

CBotCS::~CBotCS(){
}

void CBotCS::menu(void){
	char c_team[32];
	char c_class[32];
	
	// handle menu here ..
	
	if (m_start_action == MSG_CS_TEAM_SELECT){
		m_start_action = MSG_CS_IDLE;  // switch back to idle
		
		if ((m_iJoinTeam != 1) 
			&& (m_iJoinTeam != 2) 
			&& (m_iJoinTeam != 5))
			m_iJoinTeam = -1;
		
		if (m_iJoinTeam == -1)
			m_iJoinTeam = 5;
		
		// select the team the bot wishes to join...
		if (m_iJoinTeam == 1)
			strcpy(c_team, "1");
		else if (m_iJoinTeam == 2)
			strcpy(c_team, "2");
		else
			strcpy(c_team, "5");
		
		fakeClientCommand("menuselect", c_team, NULL);
		
		return;
	}
	
	if (m_start_action == MSG_CS_CT_SELECT)  // counter terrorist
	{
		m_start_action = MSG_CS_IDLE;  // switch back to idle
		
		if ((m_iClass < 1) || (m_iClass > 4))
			m_iClass = -1;  // use random if invalid
		
		if (m_iClass == -1)
			//m_iClass = RANDOM_LONG(1, 4);
			m_iClass = 5;
		
		// select the class the bot wishes to use...
		if (m_iClass == 1)
			strcpy(c_class, "1");
		else if (m_iClass == 2)
			strcpy(c_class, "2");
		else if (m_iClass == 3)
			strcpy(c_class, "3");
		else if (m_iClass == 4)
			strcpy(c_class, "4");
		else
			strcpy(c_class, "5");  // random
		
		fakeClientCommand("menuselect", c_class, NULL);
		
		// bot has now joined the game (doesn't need to be started)
		m_not_started = 0;
		
		return;
	}
	
	if (m_start_action == MSG_CS_T_SELECT)  // terrorist select
	{
		m_start_action = MSG_CS_IDLE;  // switch back to idle
		
		if ((m_iClass < 1) || (m_iClass > 4))
			m_iClass = -1;  // use random if invalid
		
		if (m_iClass == -1)
			//m_iClass = RANDOM_LONG(1, 4);
			m_iClass = 5;
		
		// select the class the bot wishes to use...
		if (m_iClass == 1)
			strcpy(c_class, "1");
		else if (m_iClass == 2)
			strcpy(c_class, "2");
		else if (m_iClass == 3)
			strcpy(c_class, "3");
		else if (m_iClass == 4)
			strcpy(c_class, "4");
		else
			strcpy(c_class, "5");  // random
		
		fakeClientCommand("menuselect", c_class, NULL);
		
		// bot has now joined the game (doesn't need to be started)
		m_not_started = 0;
		
		return;
	}
}

void CBotCS::init(void){
	m_LHostages.clear();		// delete list of used hostages
	m_iTeam = getTeam();

	CBaseBot::init();
}
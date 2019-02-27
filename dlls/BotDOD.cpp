// BotDOD.cpp: implementation of the CBotDOD class.
//
//////////////////////////////////////////////////////////////////////

#include "BotDOD.h"

#include "BV_HLDM.h"
#include "BV_CS.h"

#include "WeaponsDOD.h"

#include "ActionHL.h"
#include "PerceptionBot.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#include <iostream>
using namespace std;

CBotDOD::CBotDOD(){
	m_bBleeding = false;
	m_bSlowed = false;
	m_iStamina = 100;

	m_pAction = new CActionHL;
	m_pPerception = new CPerceptionBot;
	m_pWeapons = new CWeaponsDOD;

	m_LpBehaviour.push_back(m_pMovePath = new CBVNP_HLDM_MovePath(this));
	m_LpBehaviour.push_back(m_pCamp = new CBVNP_HLDM_Camp(this));

	m_LpBehaviour.push_back(new CBVNP_HLDM_Duck(this));
	m_LpBehaviour.push_back(new CBVNP_HLDM_ShootObstacle(this));
	m_LpBehaviour.push_back(new CBVNP_HLDM_AvoidStuck(this));
	m_LpBehaviour.push_back(new CBVNP_HLDM_AvoidStuckWP(this));
	m_LpBehaviour.push_back(new CBVNP_HLDM_Ladder(this));
	m_LpBehaviour.push_back(new CBVNP_HLDM_RunAround(this));
	m_LpBehaviour.push_back(new CBVNP_HLDM_LookAround(this));
	m_LpBehaviour.push_back(new CBVNP_HLDM_Explore(this));
	m_LpBehaviour.push_back(new CBVNP_HLDM_ZoomBack(this));

	m_LpBehaviour.push_back(new CBV_HLDM_WatchEnemy(this));
	m_LpBehaviour.push_back(new CBV_HLDM_ShootEnemy(this));
	m_LpBehaviour.push_back(new CBV_HLDM_ExplGrenade(this));
	m_LpBehaviour.push_back(new CBV_HLDM_WatchDamage(this));
	m_LpBehaviour.push_back(new CBV_HLDM_Hear(this));
	m_LpBehaviour.push_back(new CBV_HLDM_AdjustLadder(this));
	m_LpBehaviour.push_back(new CBV_HLDM_CheckKill(this));
	m_LpBehaviour.push_back(new CBV_HLDM_SuppFireAc(this));

	//m_LpBehaviour.push_back(new CBV_CS_WatchTMate(this));

	m_LpBehaviour.push_back(new CBV_HLDM_AttackDirect(this));
	m_LpBehaviour.push_back(new CBV_HLDM_AttackIndirect(this));
	m_LpBehaviour.push_back(new CBV_HLDM_Pullback(this));
	m_LpBehaviour.push_back(new CBV_HLDM_HideReload(this));

	m_LpBehaviour.push_back(new CBVLT_HLDM_AttackFollow(this));
	m_LpBehaviour.push_back(new CBVLT_HLDM_AttackSnipe(this));

	m_LpBehaviour.push_back(m_pSquadCommand = new CBV_HLDM_SquadCommand(this));

	m_LpEmotion.push_back(m_pEMTeam = new CEM_HLDM_Team(this));
	m_LpEmotion.push_back(m_pEMEnemy = new CEM_HLDM_Enemy(this));
	m_LpEmotion.push_back(m_pEMLEnemy = new CEM_HLDM_LastEnemy(this));
}

CBotDOD::~CBotDOD(){
}

void CBotDOD::menu(void){
	char c_team[32];
	char c_class[32];

	
	if (m_start_action == MSG_DOD_IDLE){
	}
	else if (m_start_action == MSG_DOD_TEAM_SELECT){
		//m_start_action = MSG_DOD_CLASS_SELECT;  // switch back to idle
		m_start_action = MSG_DOD_IDLE;  // switch back to idle
		
		if ((m_iJoinTeam != 1) && (m_iJoinTeam != 2) && (m_iJoinTeam != 5))
			m_iJoinTeam = -1;
		
		// select the team the bot wishes to join...
		if (m_iJoinTeam == 1){strcpy(c_team, "1");
		}
		else if (m_iJoinTeam == 2){
			strcpy(c_team, "2");
		}
		else strcpy(c_team, "5");
		
		fakeClientCommand("jointeam", c_team, NULL);
		return;
	}
	
	else if (m_start_action == MSG_DOD_CLASS_SELECT_AL)
	{
		m_start_action = MSG_DOD_IDLE;  // switch back to idle
		//create_time = gpGlobals->time;  // reset
		// select the class the bot wishes to use...
		if ((m_iClass < 1) || (m_iClass > 5))
			m_iClass = -1;  // use random if invalid

		// select the class the bot wishes to use...
		if (m_iClass == 1)
			strcpy(c_class, "lightinf");
		else if (m_iClass == 2)
			strcpy(c_class, "assaultinf");
		else if (m_iClass == 3)
			strcpy(c_class, "sniper");
		else if (m_iClass == 4)
			strcpy(c_class, "heavyweapons");
		else if (m_iClass == 5)
			strcpy(c_class, "machinegunner");
		else if (m_iClass == -1)
			strcpy(c_class, "randompc");

		fakeClientCommand(c_class, NULL,NULL);
		return;
	}
	else if (m_start_action == MSG_DOD_CLASS_SELECT_AX)
	{
		m_start_action = MSG_DOD_IDLE;  // switch back to idle
		// select the class the bot wishes to use...
		if ((m_iClass < 1) || (m_iClass > 5))
			m_iClass = -1;  // use random if invalid

		// select the class the bot wishes to use...
		if (m_iClass == 1)
			strcpy(c_class, "leichtinf");
		else if (m_iClass == 2)
			strcpy(c_class, "sturmtrup");
		else if (m_iClass == 3)
			strcpy(c_class, "germansniper");
		else if (m_iClass == 4)
			strcpy(c_class, "heavyweaponsger");
		else if (m_iClass == 5)
			strcpy(c_class, "machinegunnerger");
		else if (m_iClass == -1)
			strcpy(c_class, "randompc");

		fakeClientCommand(c_class, NULL,NULL);
		return;
	}

	else if (m_start_action == MSG_DOD_CLASS_SELECT_AL_PARA)
	{
		m_start_action = MSG_DOD_IDLE;  // switch back to idle
		// select the class the bot wishes to use...
		if ((m_iClass < 1) || (m_iClass > 7))
			m_iClass = -1;  // use random if invalid
		
		// select the class the bot wishes to use...
		if (m_iClass == 1)
			strcpy(c_class, "para_garand");
		else if (m_iClass == 2)
			strcpy(c_class, "para_carbine");
		else if (m_iClass == 3)
			strcpy(c_class, "para_thompson");
		else if (m_iClass == 4)
			strcpy(c_class, "para_greasegun");
		else if (m_iClass == 5)
			strcpy(c_class, "para_spring");
		else if (m_iClass == 6)
			strcpy(c_class, "para_bar");
		else if (m_iClass == 7)
			strcpy(c_class, "para_30cal");
		else if (m_iClass == -1)
			strcpy(c_class, "para_random");

		fakeClientCommand(c_class, NULL,NULL);
		
		// bot has now joined the game (doesn't need to be started)
		//not_started = 0;
		return;
	}
	else if (m_start_action == MSG_DOD_CLASS_SELECT_AX_PARA)
	{
		m_start_action = MSG_DOD_IDLE;  // switch back to idle
		// select the class the bot wishes to use...
		if ((m_iClass < 1) || (m_iClass > 8))
			m_iClass = -1;  // use random if invalid

		// select the class the bot wishes to use...
		if (m_iClass == 1)
			strcpy(c_class, "para_kar");
		else if (m_iClass == 2)
			strcpy(c_class, "para_k43");
		else if (m_iClass == 3)
			strcpy(c_class, "para_scopedkar");
		else if (m_iClass == 4)
			strcpy(c_class, "para_mp40");
		else if (m_iClass == 5)
			strcpy(c_class, "para_mp44");
		else if (m_iClass == 6)
			strcpy(c_class, "para_fg42");
		else if (m_iClass == 7)
			strcpy(c_class, "para_mg34");
		else if (m_iClass == 8)
			strcpy(c_class, "para_mg42");
		else if (m_iClass == -1)
			strcpy(c_class, "para_random");

		fakeClientCommand(c_class, NULL,NULL);
		
		return;
	}
	else if (m_start_action == MSG_DOD_WPN_SELECT_SERGEANT)
	{
		if(RANDOM_LONG(0,100) < 50){
			fakeClientCommand("selectwpn1",0,0);
		}
		else
			fakeClientCommand("selectwpn2",0,0);
		m_start_action = MSG_DOD_IDLE;  // switch back to idle
	}
	else if (m_start_action == MSG_DOD_WPN_SELECT_MACHINE)
	{
		if(RANDOM_LONG(0,100) < 50){
			fakeClientCommand("selectwpn1",0,0);
		}
		else
			fakeClientCommand("selectwpn2",0,0);
		m_start_action = MSG_DOD_IDLE;  // switch back to idle
	}
	else if (m_start_action == MSG_DOD_WPN_SELECT_GRENADIER)
	{
		if(RANDOM_LONG(0,100) < 50){
			fakeClientCommand("selectwpn1",0,0);
		}
		else
			fakeClientCommand("selectwpn2",0,0);
		m_start_action = MSG_DOD_IDLE;  // switch back to idle
	}
	else if (m_start_action == MSG_DOD_WPN_SELECT_RIFLEMAN)
	{
		if(RANDOM_LONG(0,100) < 50){
			fakeClientCommand("selectwpn1",0,0);
		}
		else
			fakeClientCommand("selectwpn2",0,0);
		m_start_action = MSG_DOD_IDLE;  // switch back to idle
	}
	else if (m_start_action == MSG_DOD_WPN_SELECT_FG42)
	{
		if(RANDOM_LONG(0,100) < 50){
			fakeClientCommand("selectwpn1",0,0);
		}
		else
			fakeClientCommand("selectwpn2",0,0);
		m_start_action = MSG_DOD_IDLE;  // switch back to idle
	}
}
// BotHLDM.cpp: implementation of the CBotHLDM class.
//
//////////////////////////////////////////////////////////////////////

#include "BotHLDM.h"

#include "BV_HLDM.h"

#include "ActionHL.h"
#include "PerceptionBot.h"
#include "WeaponsHLDM.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBotHLDM::CBotHLDM(){
	m_pAction = new CActionHL;
	m_pPerception = new CPerceptionBot;
	m_pWeapons = new CWeaponsHLDM;

	m_LpBehaviour.push_back(m_pMovePath = new CBVNP_HLDM_MovePath(this));
	m_LpBehaviour.push_back(m_pCamp = new CBVNP_HLDM_Camp(this));

	m_LpBehaviour.push_back(new CBVNP_HLDM_Duck(this));
	m_LpBehaviour.push_back(new CBVNP_HLDM_ShootObstacle(this));
	m_LpBehaviour.push_back(new CBVNP_HLDM_AvoidStuck(this));
	m_LpBehaviour.push_back(new CBVNP_HLDM_AvoidStuckWP(this));
	m_LpBehaviour.push_back(new CBVNP_HLDM_Ladder(this));
	m_LpBehaviour.push_back(new CBVNP_HLDM_RunAround(this));
	m_LpBehaviour.push_back(new CBVNP_HLDM_RunToHealthWP(this));
	m_LpBehaviour.push_back(new CBVNP_HLDM_ZoomBack(this));

	m_LpBehaviour.push_back(new CBV_HLDM_WatchEnemy(this));
	m_LpBehaviour.push_back(new CBV_HLDM_ShootEnemy(this));
	m_LpBehaviour.push_back(new CBV_HLDM_ExplGrenade(this));
	m_LpBehaviour.push_back(new CBV_HLDM_WatchDamage(this));
	m_LpBehaviour.push_back(new CBV_HLDM_Hear(this));
	m_LpBehaviour.push_back(new CBV_HLDM_AdjustLadder(this));
	m_LpBehaviour.push_back(new CBV_HLDM_RunToWeapon(this));
	m_LpBehaviour.push_back(new CBV_HLDM_RunToWeaponbox(this));
	m_LpBehaviour.push_back(new CBV_HLDM_RunToHealth(this));
	m_LpBehaviour.push_back(new CBV_HLDM_CheckKill(this));
	m_LpBehaviour.push_back(new CBV_HLDM_Run2Sound(this));
	m_LpBehaviour.push_back(new CBV_HLDM_SuppFireAc(this));

	m_LpBehaviour.push_back(new CBVLT_HLDM_AttackFollow(this));
	m_LpBehaviour.push_back(new CBVLT_HLDM_AttackSnipe(this));

	m_LpEmotion.push_back(m_pEMTeam = new CEM_HLDM_Team(this));
	m_LpEmotion.push_back(m_pEMEnemy = new CEM_HLDM_Enemy(this));
	m_LpEmotion.push_back(m_pEMLEnemy = new CEM_HLDM_LastEnemy(this));
}

CBotHLDM::~CBotHLDM(){
}

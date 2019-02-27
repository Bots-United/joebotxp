// GameCS.cpp: implementation of the CGameCS class.
//
//////////////////////////////////////////////////////////////////////

#include "GameCS.h"

#include "BotCS.h"
#include "PlayerCS.h"
#include "TeamCS.h"
#include "Observer.h"
#include "InterestingEntity.h"

#include "globalvars.h"

//////////////////////////////////////////////////////////////////////
// Construction / Destruction
//////////////////////////////////////////////////////////////////////

CGameCS::CGameCS(){
	// all this pIEntity = new looks useless, and it is, mainly
	// but so you can use the pointer to get the CIEntity::m_iId easily

	CIEntity *pIEntity;

	mod_id = CSTRIKE_DLL;
	m_iMod_Id = CSTRIKE_DLL;

	m_LIEntities.push_back(pIEntity = new CIEntity("info_player_start",true));		// ct spawn
	m_LIEntities.push_back(pIEntity = new CIEntity("info_player_deathmatch",true));	// te spawn

	m_LIEntities.push_back(pIEntity = new CIEntity("hostage_entity",true));			// hostage
	m_iIEntityIdHostage = pIEntity->m_iId;

	m_LIEntities.push_back(pIEntity = new CIEntity("grenade",false));				// bomb, grenade
	m_iIEntityIdGrenade = pIEntity->m_iId;
	
	m_LIEntities.push_back(pIEntity = new CInterestingLadder("func_ladder"));
	
	//m_LIEntities.push_back(pIEntity = new CInterestingWeapon("armory_entity",-1,true));
	
	//m_LIEntities.push_back(pIEntity = new CInterestingWeaponboxWeapon("weapon_usp",CS_WEAPON_USP,false));
	//m_LIEntities.push_back(pIEntity = new CInterestingWeaponboxWeapon("weapon_glock18",CS_WEAPON_GLOCK18,false));
	//m_LIEntities.push_back(pIEntity = new CInterestingWeaponboxWeapon("weapon_elite",CS_WEAPON_ELITE,false));
	//m_LIEntities.push_back(pIEntity = new CInterestingWeaponboxWeapon("weapon_fiveseven",CS_WEAPON_FIVESEVEN,false));
	//m_LIEntities.push_back(pIEntity = new CInterestingWeaponboxWeapon("weapon_deagle",CS_WEAPON_DEAGLE,false));
	//m_LIEntities.push_back(pIEntity = new CInterestingWeaponboxWeapon("weapon_p228",CS_WEAPON_P228,false));
	m_LIEntities.push_back(pIEntity = new CInterestingWeaponboxWeapon("weapon_scout",CS_WEAPON_SCOUT,false));
	m_LIEntities.push_back(pIEntity = new CInterestingWeaponboxWeapon("weapon_xm1014",CS_WEAPON_XM1014,false));
	m_LIEntities.push_back(pIEntity = new CInterestingWeaponboxWeapon("weapon_c4",CS_WEAPON_C4,false));
	m_LIEntities.push_back(pIEntity = new CInterestingWeaponboxWeapon("weapon_mac10",CS_WEAPON_MAC10,false));
	m_LIEntities.push_back(pIEntity = new CInterestingWeaponboxWeapon("weapon_aug",CS_WEAPON_AUG,false));
	m_LIEntities.push_back(pIEntity = new CInterestingWeaponboxWeapon("weapon_ump45",CS_WEAPON_UMP45,false));
	m_LIEntities.push_back(pIEntity = new CInterestingWeaponboxWeapon("weapon_sg550",CS_WEAPON_SG550,false));
	m_LIEntities.push_back(pIEntity = new CInterestingWeaponboxWeapon("weapon_awp",CS_WEAPON_AWP,false));
	m_LIEntities.push_back(pIEntity = new CInterestingWeaponboxWeapon("weapon_mp5navy",CS_WEAPON_MP5NAVY,false));
	m_LIEntities.push_back(pIEntity = new CInterestingWeaponboxWeapon("weapon_m249",CS_WEAPON_M249,false));
	m_LIEntities.push_back(pIEntity = new CInterestingWeaponboxWeapon("weapon_m3",CS_WEAPON_M3,false));
	m_LIEntities.push_back(pIEntity = new CInterestingWeaponboxWeapon("weapon_m4a1",CS_WEAPON_M4A1,false));
	m_LIEntities.push_back(pIEntity = new CInterestingWeaponboxWeapon("weapon_tmp",CS_WEAPON_TMP,false));
	m_LIEntities.push_back(pIEntity = new CInterestingWeaponboxWeapon("weapon_g3sg1",CS_WEAPON_G3SG1,false));
	m_LIEntities.push_back(pIEntity = new CInterestingWeaponboxWeapon("weapon_sg552",CS_WEAPON_SG552,false));
	m_LIEntities.push_back(pIEntity = new CInterestingWeaponboxWeapon("weapon_ak47",CS_WEAPON_AK47,false));
	m_LIEntities.push_back(pIEntity = new CInterestingWeaponboxWeapon("weapon_p90",CS_WEAPON_P90,false));

	m_LIEntities.push_back(pIEntity = new CInterestingZone("func_hostage_rescue",CWaypoint::FLAG_GOAL));
	m_LIEntities.push_back(pIEntity = new CInterestingZone("func_bomb_target",CWaypoint::FLAG));

	// create team data ...
	getTeamData(1);
}

CGameCS::~CGameCS(){
}

CBaseBot *CGameCS::allocBot(void){
	return new CBotCS;
}

CPlayer *CGameCS::allocPlayer(void){
	return new CPlayerCS;
}

CBaseTeam *CGameCS :: allocTeamData(void){
	return new CTeamCS;
}

bool CGameCS::getTeamplay(void){
	return true;
}

void CGameCS::onRoundStart(void){
	g_Observer.reset();

	int i;
	for(i=0;i<m_iMaxCurrentPlayer;i++){
		if(getBot(i)){
			getBot(i)->m_bNeed2Init = true;
		}
	}
	CGame::onRoundStart();
}

long CGameCS::weaponModel2ID(const char *szModel){
	if(!szModel||!*szModel)
		return -1;
	
	if(!strcmp(szModel,"models/p_mp5.mdl"))
		return CS_WEAPON_MP5NAVY;
	if(!strcmp(szModel,"models/p_m4a1.mdl"))
		return CS_WEAPON_M4A1;
	if(!strcmp(szModel,"models/p_ak47.mdl"))
		return CS_WEAPON_AK47;
	
	if(!strcmp(szModel,"models/p_usp.mdl"))
		return CS_WEAPON_USP;
	if(!strcmp(szModel,"models/p_glock18.mdl"))
		return CS_WEAPON_GLOCK18;
	if(!strcmp(szModel,"models/p_deagle.mdl"))
		return CS_WEAPON_DEAGLE;
	if(!strcmp(szModel,"models/p_elite.mdl"))
		return CS_WEAPON_ELITE;
	if(!strcmp(szModel,"models/p_p228.mdl"))
		return CS_WEAPON_P228;
	if(!strcmp(szModel,"models/p_fiveseven.mdl"))
		return CS_WEAPON_FIVESEVEN;
	
	if(!strcmp(szModel,"models/p_hegrenade.mdl"))
		return CS_WEAPON_HEGRENADE;
	if(!strcmp(szModel,"models/p_flashbang.mdl"))
		return CS_WEAPON_FLASHBANG;
	if(!strcmp(szModel,"models/p_smokegrenade.mdl"))
		return CS_WEAPON_SMOKEGRENADE;
	
	if(!strcmp(szModel,"models/p_awp.mdl"))
		return CS_WEAPON_AWP;
	if(!strcmp(szModel,"models/p_aug.mdl"))
		return CS_WEAPON_AUG;
	if(!strcmp(szModel,"models/p_g3sg1.mdl"))
		return CS_WEAPON_G3SG1;
	if(!strcmp(szModel,"models/p_m249.mdl"))
		return CS_WEAPON_M249;
	if(!strcmp(szModel,"models/p_m3.mdl"))
		return CS_WEAPON_M3;
	if(!strcmp(szModel,"models/p_mac10.mdl"))
		return CS_WEAPON_MAC10;
	if(!strcmp(szModel,"models/p_p90.mdl"))
		return CS_WEAPON_P90;
	if(!strcmp(szModel,"models/p_tmp.mdl"))
		return CS_WEAPON_TMP;
	if(!strcmp(szModel,"models/p_scout.mdl"))
		return CS_WEAPON_SCOUT;
	if(!strcmp(szModel,"models/p_sg552.mdl"))
		return CS_WEAPON_SG552;
	if(!strcmp(szModel,"models/p_xm1014.mdl"))
		return CS_WEAPON_XM1014;
	if(!strcmp(szModel,"models/p_ump45.mdl"))
		return CS_WEAPON_UMP45;
	if(!strcmp(szModel,"models/p_sg550.mdl"))
		return CS_WEAPON_SG550;
	if(!strcmp(szModel,"models/p_knife.mdl"))
		return CS_WEAPON_KNIFE;
	if(!strcmp(szModel,"models/p_c4.mdl"))
		return CS_WEAPON_C4;
	return -1;
}
// CGameHLDM.cpp: implementation of the CGameHLDM class.
//
//////////////////////////////////////////////////////////////////////

#include "GameHLDM.h"

#include "BotHLDM.h"

#include "Weapons.h"
#include "InterestingEntity.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameHLDM::CGameHLDM()
{
	// all this pIEntity = new looks useless, and it is, mainly
	// but so you can use the pointer to get the CIEntity::m_iId easily

	CIEntity *pIEntity;

	mod_id =  VALVE_DLL;
	m_iMod_Id = VALVE_DLL;

	m_LIEntities.push_back(pIEntity = new CInterestingAmmo("ammo_357",-1,true));
	m_LIEntities.push_back(pIEntity = new CInterestingAmmo("ammo_9mmAR",-1,true));
	m_LIEntities.push_back(pIEntity = new CInterestingAmmo("ammo_9mmbox",-1,true));
	m_LIEntities.push_back(pIEntity = new CInterestingAmmo("ammo_9mmclip",-1,true));
	m_LIEntities.push_back(pIEntity = new CInterestingAmmo("ammo_ARgrenades",-1,true));
	m_LIEntities.push_back(pIEntity = new CInterestingAmmo("ammo_buckshot",-1,true));
	m_LIEntities.push_back(pIEntity = new CInterestingAmmo("ammo_crossbow",-1,true));
	m_LIEntities.push_back(pIEntity = new CInterestingAmmo("ammo_egonclip",-1,true));
	m_LIEntities.push_back(pIEntity = new CInterestingAmmo("ammo_gaussclip",-1,true));
	m_LIEntities.push_back(pIEntity = new CInterestingAmmo("ammo_glockclip",-1,true));
	m_LIEntities.push_back(pIEntity = new CInterestingAmmo("ammo_mp5clip",-1,true));
	m_LIEntities.push_back(pIEntity = new CInterestingAmmo("ammo_mp5grenades",-1,true));
	m_LIEntities.push_back(pIEntity = new CInterestingAmmo("ammo_rpgclip",-1,true));

	m_LIEntities.push_back(pIEntity = new CInterestingWeapon("weapon_357",VALVE_WEAPON_357,true));
	m_LIEntities.push_back(pIEntity = new CInterestingWeapon("weapon_9mmAR",VALVE_WEAPON_MP5,true));
	m_LIEntities.push_back(pIEntity = new CInterestingWeapon("weapon_9mmhandgun",VALVE_WEAPON_GLOCK,true));
	m_LIEntities.push_back(pIEntity = new CInterestingWeapon("weapon_crossbow",VALVE_WEAPON_CROSSBOW,true));
	m_LIEntities.push_back(pIEntity = new CInterestingWeapon("weapon_crowbar",VALVE_WEAPON_CROWBAR,true));
	m_LIEntities.push_back(pIEntity = new CInterestingWeapon("weapon_egon",VALVE_WEAPON_EGON,true));
	m_LIEntities.push_back(pIEntity = new CInterestingWeapon("weapon_gauss",VALVE_WEAPON_GAUSS,true));
	m_LIEntities.push_back(pIEntity = new CInterestingWeapon("weapon_handgrenade",VALVE_WEAPON_HANDGRENADE,true));
	m_LIEntities.push_back(pIEntity = new CInterestingWeapon("weapon_hornetgun",VALVE_WEAPON_HORNETGUN,true));
	m_LIEntities.push_back(pIEntity = new CInterestingWeapon("weapon_rpg",VALVE_WEAPON_RPG,true));
	m_LIEntities.push_back(pIEntity = new CInterestingWeapon("weapon_satchel",VALVE_WEAPON_SATCHEL,true));
	m_LIEntities.push_back(pIEntity = new CInterestingWeapon("weapon_shotgun",VALVE_WEAPON_SHOTGUN,true));
	m_LIEntities.push_back(pIEntity = new CInterestingWeapon("weapon_snark",VALVE_WEAPON_SNARK,true));
	m_LIEntities.push_back(pIEntity = new CInterestingWeapon("weapon_tripmine",VALVE_WEAPON_TRIPMINE,true));

	m_LIEntities.push_back(pIEntity = new CInterestingCharger("func_recharge",CIEntity::ARMOR,true));
	m_LIEntities.push_back(pIEntity = new CInterestingCharger("func_healthcharger",CIEntity::HEALTH,true));

	m_LIEntities.push_back(pIEntity = new CInterestingItem("item_battery",CIEntity::ARMOR,true));
	m_LIEntities.push_back(pIEntity = new CInterestingItem("item_healthkit",CIEntity::HEALTH,true));
	m_LIEntities.push_back(pIEntity = new CInterestingItem("item_longjump",CIEntity::NONE,true));

	m_LIEntities.push_back(pIEntity = new CInterestingWeapon("weaponbox",-1,false));
	m_iIEntityIdWBox = pIEntity->m_iId;

	m_LIEntities.push_back(pIEntity = new CIEntity("laser_spot",false));
	m_LIEntities.push_back(pIEntity = new CIEntity("info_player_deathmatch",true));

	m_LIEntities.push_back(pIEntity = new CInterestingLadder("func_ladder"));
}

CGameHLDM::~CGameHLDM()
{
}

CBaseBot *CGameHLDM::allocBot(void){
	return new CBotHLDM;
}
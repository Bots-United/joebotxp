// InterestingEntity.cpp: implementation of the CIEntity class.
//
//////////////////////////////////////////////////////////////////////

#include "InterestingEntity.h"

#include "util.h"

#include "Map.h"
#include "Perceipt.h"

#include <list>
#include <iostream>
using namespace std;

long g_IEntityId = 0;
Vector g_VZero = Vector(0,0,0);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIEntity::CIEntity():m_iId(++g_IEntityId)
{
	m_iType = NONE;
	m_iWeaponId = -1;
	m_iAmmoId = -1;
	m_iContaining = 0;
	m_szClassname[0] = 0;
	m_bStatic = false;
//	m_fDistance = 50;
	m_lWPFlag = 0;			// just use flags for zones

	//cout << g_IEntityId << endl;
}

CIEntity::CIEntity(const char *szClassname,bool bStatic):m_iId(++g_IEntityId)
{
	m_iType = NONE;
	m_iWeaponId = -1;
	m_iAmmoId = -1;
	m_iContaining = 0;
//	m_fDistance = 50;
	m_lWPFlag = 0;			// just use flags for zones

	strcpy(m_szClassname,szClassname);
	m_bStatic = bStatic;

	//cout << g_IEntityId << endl;
}

CIEntity::~CIEntity(){
}

Vector CIEntity::getWPOrigin(CEntity &Entity){
	return Entity.getOrigin();
}

const Vector &CIEntity::getOrigin(CEntity &Entity){
	return Entity.getOrigin();
}

long CIEntity::addWP(CEntity &Entity){
	Vector VOrigin = getWPOrigin(Entity);

	UTIL_TraceToGround(VOrigin);
					
	VOrigin = VOrigin + Vector(0,0,38);

	if(g_Map.m_Waypoints.getNearest(VOrigin,false,false,0,50) == -1)
		g_Map.m_Waypoints.add(VOrigin);

	return 1;
}

bool CIEntity::compare(CEntity &Entity){
	////////////////////////////////////////////////////////////
	// CPerceipt *CIEntity::compare(CEntity &Entity){
	////////////////////////////////////////////////////////////
	// check if that entity matches this IEntity
	////////////////////////////////////////////////////////////

	if(!isValid(Entity))
		return false;

	if(strcmp(m_szClassname,Entity.getClassName()))
		return false;

	return true;
}

void CIEntity::assign(CEntity &Entity, CPerceipt *pNewPerceipt){
	// assigns perception data to perceipt.

	pNewPerceipt->m_fCreate = pNewPerceipt->m_fLUpdate = g_pGame->getTime();
	pNewPerceipt->m_lType = CPerceipt::PT_ENTITY;

	pNewPerceipt->m_pEntity = Entity;
	pNewPerceipt->m_VOrigin = getOrigin(Entity);

	pNewPerceipt->m_pIEntity = this;

	switch(m_iType){
	case WEAPON:
		pNewPerceipt->m_lTypeSpec |= CPerceipt::PTX_ENTITY_WEAPON;
		break;
	case WEAPONWBX:
		pNewPerceipt->m_lTypeSpec |= CPerceipt::PTX_ENTITY_WEAPONWBX;
		break;
	case AMMO:
		pNewPerceipt->m_lTypeSpec |= CPerceipt::PTX_ENTITY_AMMO;
		break;
	case CHARGER:
		switch(m_iContaining){
		case ARMOR:
			pNewPerceipt->m_lTypeSpec |= CPerceipt::PTX_ENTITY_ARMORC;
			break;
		case HEALTH:
			pNewPerceipt->m_lTypeSpec |= CPerceipt::PTX_ENTITY_HEALTHC;
		default:
			break;
		}
		break;
	case ITEM:
		switch(m_iContaining){
		case ARMOR:
			pNewPerceipt->m_lTypeSpec |= CPerceipt::PTX_ENTITY_ARMOR;
			break;
		case HEALTH:
			pNewPerceipt->m_lTypeSpec |= CPerceipt::PTX_ENTITY_HEALTH;
		default:
			break;
		}
		break;
	case LADDER:
		pNewPerceipt->m_lTypeSpec |= CPerceipt::PTX_ENTITY_LADDER;
		break;
	default:
		break;
	};
}

bool CIEntity::isValid(CEntity &Entity){
	if (Entity.getEntity()->v.effects & EF_NODRAW)
		return false;
	return true;
}

/////////////////////////////////////////////////
///   CInteresting Weapon
/////////////////////////////////////////////////

CInterestingWeapon::CInterestingWeapon(){
	m_iType = WEAPON;
}

CInterestingWeapon::CInterestingWeapon(const char *szClassname,int iWeaponID, bool bStatic){
	m_iType = WEAPON;

	strcpy(m_szClassname,szClassname);
	m_iWeaponId = iWeaponID;
	m_bStatic = bStatic;
}

CInterestingWeapon::~CInterestingWeapon(){
}

bool CInterestingWeapon::isValid(CEntity &Entity){
	if(Entity.getEntity()->v.effects & EF_NODRAW){
		// somebody owns this weapon or it hasnt respawned yet
		return false;
	}
	return true;
}

/////////////////////////////////////////////////
///   CInteresting WeaponboxWeapon
/////////////////////////////////////////////////

CInterestingWeaponboxWeapon::CInterestingWeaponboxWeapon(){
	m_iType = WEAPONWBX;
}

CInterestingWeaponboxWeapon::CInterestingWeaponboxWeapon(const char *szClassname,int iWeaponID, bool bStatic){
	m_iType = WEAPONWBX;

	strcpy(m_szClassname,szClassname);
	m_iWeaponId = iWeaponID;
	m_bStatic = bStatic;
}

CInterestingWeaponboxWeapon::~CInterestingWeaponboxWeapon(){
}

bool CInterestingWeaponboxWeapon::isValid(CEntity &Entity){
	if(!Entity.getOwner())		// no owner ... so what's that ? at least no weaponbox
		return false;
	if(strcmp(Entity.getOwner().getClassName(),"weaponbox"))		// it's not in a weaponbox
		return false;

	return true;
}

const Vector &CInterestingWeaponboxWeapon::getOrigin(CEntity &Entity){
	if(!Entity.getOwner())
		return g_VZero;

	return Entity.getOwner().getOrigin();
}

/////////////////////////////////////////////////
///   CInteresting Ammo
/////////////////////////////////////////////////

CInterestingAmmo::CInterestingAmmo(){
	m_iType = AMMO;
}

CInterestingAmmo::CInterestingAmmo(const char *szClassname,int iAmmoID, bool bStatic){
	m_iType = AMMO;

	strcpy(m_szClassname,szClassname);
	m_iAmmoId = iAmmoID;
	m_bStatic = bStatic;
}

CInterestingAmmo::~CInterestingAmmo(){
}

/////////////////////////////////////////////////
///   CInteresting Item
/////////////////////////////////////////////////

CInterestingItem::CInterestingItem(){
	m_iType = ITEM;
}

CInterestingItem::CInterestingItem(const char *szClassname,int iContaining,bool bStatic){
	m_iType = ITEM;

	strcpy(m_szClassname,szClassname);
	m_bStatic = bStatic;
	m_iContaining = iContaining;
}

CInterestingItem::~CInterestingItem(){
}

/////////////////////////////////////////////////
///   CInteresting Charger
/////////////////////////////////////////////////

CInterestingCharger::CInterestingCharger(){
	m_iType = CHARGER;
}

CInterestingCharger::CInterestingCharger(const char *szClassname,int iContaining,bool bStatic){
	m_iType = CHARGER;

	strcpy(m_szClassname,szClassname);
	m_bStatic = bStatic;
	m_iContaining = iContaining;
}

extern edict_t *listenserver_edict;
Vector CInterestingCharger::getWPOrigin(CEntity &Entity){
	Vector VReturn1,VReturn2,VDiff = CrossProduct(Entity.getSize(),Vector(0,0,1)).normalize()*20;
	TraceResult tr1;

	VReturn1 = VReturn2 = Entity.getBModelOrigin();

	VReturn1 = VReturn1 + VDiff;
	VReturn2 = VReturn2 - VDiff;

	g_pGame->traceLine(VReturn1,VReturn1 - Vector(0,0,150),ignore_monsters,ignore_glass,0,&tr1);
	if(tr1.flFraction == 1.f){
		VReturn1 = VReturn2;
	}

	return VReturn1;
}

/////////////////////////////////////////////////
///   CInteresting Ladder
/////////////////////////////////////////////////

CInterestingLadder::CInterestingLadder(){
	m_iType = LADDER;
}

CInterestingLadder::CInterestingLadder(const char *szClassname){
	m_iType = LADDER;

	strcpy(m_szClassname,szClassname);
	m_bStatic = true;
	m_iContaining = -1;
}

long CInterestingLadder::addWP(CEntity &Entity){
	long lCount = 0;

	Vector ladderLeft = Entity.getAbsMin();
	Vector ladderRight = Entity.getAbsMax();
	
	// calculate the normal of the ladder ...
	ladderLeft.z = ladderRight.z;
	Vector ladderNormal = CrossProduct( ladderLeft - ladderRight, Vector(0,0,1) ).normalize();

	Vector VCheck11,VCheck12,
		VTest1,VTest2,
		VDiff = ladderNormal*15.f;
	TraceResult tr1,tr2;

	VTest1 = VTest2 = Entity.getBModelOrigin();

	// test on which side of the ladder we can climb up
	VTest1 = VTest1 + VDiff;		// front
	VTest2 = VTest2 - VDiff;		// back

	VCheck11 = VCheck12 = VTest1;
	//VCheck11.z = Entity.getAbsMin().z+1;
	//VCheck11.z = pEntity->v.absmin.z+1;
	VCheck12.z = Entity.getAbsMax().z;

	// check the upper half of the fucking ladder .... is there a hole we can get thru ?
	g_pGame->traceHull(VCheck12,VCheck11,point_hull,0,&tr1);
	//UTIL_DrawBeam(listenserver_edict,VCheck11,VCheck12,10,0,255,255,255,255,0);
	
	
	if(POINT_CONTENTS(VCheck11)==CONTENTS_SOLID|| 
		tr1.flFraction != 1.f){	// obviously we should try the other side
	//if(tr1.pHit){
		g_pGame->drawBeam(DEBUG_SYSTEM,0,VCheck11,VCheck12,5,0,Vector(255,255,255),255,0,150);

		VCheck11 = VCheck12 = VTest2;
		VCheck12.z = Entity.getAbsMax().z;
	}

	g_pGame->drawBeam(DEBUG_SYSTEM,0,VCheck11,VCheck12,10,0,Vector(255,0,0),255,0,150);
	g_pGame->traceHull(VCheck12,VCheck11 - Vector(0,0,1000),point_hull,0,&tr2);		// head_hull doesnt work, maybe the distance of 15 units to the ladder is to low
	VCheck11 = tr2.vecEndPos;	// VCheck11 is the lowest end of ladder ...

	g_pGame->drawBeam(DEBUG_SYSTEM,0,VCheck11-Vector(200,0,0),VCheck11+Vector(200,0,0),10,10,Vector(0,0,255),255,0,150);
	g_pGame->drawBeam(DEBUG_SYSTEM,0,VCheck11-Vector(0,200,0),VCheck11+Vector(0,200,0),10,10,Vector(0,0,255),255,0,150);

	// now we got the right side of the ladder ...
	// so let's check the waypoints :D
	Vector VWaypoint = VCheck11 + Vector(0,0,38);		// from the bottom to the top
	int iAdded,iLastAdded = -1;

	list<int> LAdded;
	// we add waypoints from the bottom of the ladder to the top and connect them already in this order
	do{
		if(g_Map.m_Waypoints.getNearest(VWaypoint,false,false,0,50) == -1){
			iAdded = g_Map.m_Waypoints.add(VWaypoint);
			g_Map.m_Waypoints[iAdded].m_lFlags |= CWaypoint::LADDER;
			LAdded.push_front(iAdded);
			lCount ++;
			if(iLastAdded != -1){
				if(!g_Map.m_Waypoints[iAdded].isConnectedTo(iLastAdded))
					g_Map.m_Waypoints.connect(iAdded,iLastAdded);
			}
			iLastAdded = iAdded;
		}

		VWaypoint.z += 75;
	}while(VWaypoint.z < VCheck12.z-40);
	
	VWaypoint = VCheck12 + Vector(0,0,38);
	if(g_Map.m_Waypoints.getNearest(VWaypoint,false,false,0,50) == -1){
		iAdded = g_Map.m_Waypoints.add(VWaypoint);
		g_Map.m_Waypoints[iAdded].m_lFlags |= CWaypoint::LADDER;
		lCount ++;
		if(iLastAdded != -1){
			if(!g_Map.m_Waypoints[iAdded].isConnectedTo(iLastAdded))
				g_Map.m_Waypoints.connect(iAdded,iLastAdded);
		}
		iLastAdded = iAdded;
	}

	while(!LAdded.empty()){		// go back the list to connect them in both ways
		iAdded = LAdded.front();
		LAdded.pop_front();
		if(iLastAdded != -1){
			if(!g_Map.m_Waypoints[iAdded].isConnectedTo(iLastAdded))
				g_Map.m_Waypoints.connect(iAdded,iLastAdded);
		}
		iLastAdded = iAdded;
	}

	return lCount;
}

/////////////////////////////////////////////////
///   CInteresting Zone
/////////////////////////////////////////////////

CInterestingZone::CInterestingZone(){
	m_iType = ZONE;
}

CInterestingZone::~CInterestingZone(){
}

CInterestingZone::CInterestingZone(const char *szName,const long lWPFlag){
	m_iType = ZONE;

	strcpy(m_szClassname,szName);
	m_lWPFlag = lWPFlag;
}

bool CInterestingZone::isValid(CEntity &Entity){
    // always valid ... no need to check if it is drawn
	return true;
}

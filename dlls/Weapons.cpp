// Weapons.cpp: implementation of the CWeapons class.
//
//////////////////////////////////////////////////////////////////////

#include "Weapons.h"

#include "util.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction of CWeaponsItem
//////////////////////////////////////////////////////////////////////

CWeaponsItem::CWeaponsItem(){
	m_fDelay = .1f;
	m_bUsable = false;
	m_bPenetratingWalls = false;
	m_iPreference = 0;
	m_iClipSize = 0;
	m_fPauseTime = .1f;

	m_iLoudness=255;
	m_iCost=1000000;			// not set, so you cannot buy it
	m_iTeam = -1;
	m_fBuyProp=0;
	m_pFBuy=0;
}

CWeaponsItem::~CWeaponsItem(){
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction of CWeapons
//////////////////////////////////////////////////////////////////////

CWeapons::CWeapons(){
	m_iWeapons = 0;
	m_pWeapons = 0;
	alloc();

	m_iCurrentWId = -1;
	m_iCurrentWAmmo1 = 0;
	m_iCurrentWAmmo2 = 0;
	m_iCurrentWClip = 0;

	memset(m_piAmmo,0,sizeof(int)*MAX_AMMO_SLOTS);
	memset(m_piAmmoClip,0,sizeof(int)*32);
}

CWeapons::~CWeapons(){
}

void CWeapons::alloc(void){
	delete m_pWeapons;

	m_pWeapons = new CWeaponsItem[32];
}

int CWeapons::getBestWeapon(float fDistance){			// return the best choice of the available weapons for this distance
	int i;
	int iMax = -1;
	float fMax = -1;
	float fTemp;

	for(i = 0; i < 32; i++){
		if(!m_pWeapons[i].m_bUsable)
			continue;

		if(!(m_iWeapons & (1<<i)))		// bot hasnt got this weapon
			continue;
				
		fTemp = getValueWeaponDistance(i,fDistance);

		if(fTemp > fMax){
			fMax = fTemp;
			iMax = i;
		}
	}	

	return iMax;
}

float CWeapons::getValueWeaponDistance(int iWeapon, float fDistance){
	return m_pWeapons[iWeapon].m_Usability.getValue(fDistance);
}

float CWeapons::getBestDistance(int iWeapon){
	// return the ideal distance for this weapon
	return m_pWeapons[iWeapon].m_Usability.getMaximum();
}

float CWeapons::getClipPerc(void){
	return (float)m_iCurrentWClip/(float)getCWeapon()->m_iClipSize;
}

int CWeapons::getClipSize(int iWeapon){
	if(iWeapon >= 0 && iWeapon < 32)
		return m_pWeapons[iWeapon].m_iClipSize;
	else
		return 1;
}

float CWeapons::getBProp(int iWeapon){
if(iWeapon >= 0 && iWeapon < 32)
	return m_pWeapons[iWeapon].m_fBuyProp;
else
return 0;
}

int CWeapons::getLoudness(int iWeapon){
	if(iWeapon >= 0 && iWeapon < 32)
	return m_pWeapons[iWeapon].m_iLoudness;
	else return 200;
}

int CWeapons::getTeam(int iWeapon){
	if(iWeapon >= 0 && iWeapon < 32)
	return m_pWeapons[iWeapon].m_iTeam;
	else
		return 0;
}

int CWeapons::getCost(int iWeapon){
	if(iWeapon >= 0 && iWeapon < 32)
		return m_pWeapons[iWeapon].m_iCost;
	else
		return 100000;
}

CWeaponsItem *CWeapons::getCWeapon(void){
	if(getCWeaponID() != -1){
		return &(m_pWeapons[getCWeaponID()]);
	}
	else		// just return the first weapon :)
		return &(m_pWeapons[0]);
}

void CWeapons::randomizePrefs(int iL,int iH){
	int i;
	for(i=0; i < 32; i++){
		m_pWeapons[i].m_iPreference += RANDOM_LONG(iL,iH);
	}
}
#include "WeaponDefs.h"
#include "Weapons.h"

#include <memory.h>
#include <iostream>
using namespace std;

#include "BaseBot.h"

#include "Player.h"

#define _DELAY_NONE .3

///////////////////////////////////////////////////////////////////////////////////////
// class CWeaponDefs
//
// this class contains hard-coded information about different weapons in different mods
//

CWeaponDefs g_WeaponDefs;

CWeaponDefs :: CWeaponDefs(){
	init();
}

void CWeaponDefs :: init(void){
	memset(m_ipClipSize,0,sizeof(int)*MAX_MOD*32);
	memset(m_iCost,0,sizeof(int)*MAX_MOD*32);
	memset(m_iTeam,0,sizeof(int)*MAX_MOD*32);
	memset(m_fPBuyProb,0,sizeof(float)*MAX_MOD*32);
	memset(m_pFBuy,0,sizeof(void(*)(CBaseBot *))*MAX_MOD*32);

	m_ipClipSize[CSTRIKE_DLL][CS_WEAPON_P228]				= 13;
	m_ipClipSize[CSTRIKE_DLL][CS_WEAPON_UNKNOWN1]			= 0;
	m_ipClipSize[CSTRIKE_DLL][CS_WEAPON_SCOUT]			= 10;
	m_ipClipSize[CSTRIKE_DLL][CS_WEAPON_HEGRENADE]		= 1;
	m_ipClipSize[CSTRIKE_DLL][CS_WEAPON_XM1014]			= 7;
	m_ipClipSize[CSTRIKE_DLL][CS_WEAPON_C4]				= 1000;
	m_ipClipSize[CSTRIKE_DLL][CS_WEAPON_MAC10]			= 30;
	m_ipClipSize[CSTRIKE_DLL][CS_WEAPON_AUG]				= 30;
	m_ipClipSize[CSTRIKE_DLL][CS_WEAPON_SMOKEGRENADE]		= 1;
	m_ipClipSize[CSTRIKE_DLL][CS_WEAPON_ELITE]			= 30;
	m_ipClipSize[CSTRIKE_DLL][CS_WEAPON_UNKNOWN6]			= 0;
	m_ipClipSize[CSTRIKE_DLL][CS_WEAPON_UNKNOWN7]			= 0;
	m_ipClipSize[CSTRIKE_DLL][CS_WEAPON_USP]				= 12;
	m_ipClipSize[CSTRIKE_DLL][CS_WEAPON_GLOCK18]			= 20;
	m_ipClipSize[CSTRIKE_DLL][CS_WEAPON_AWP]				= 10;
	m_ipClipSize[CSTRIKE_DLL][CS_WEAPON_MP5NAVY]			= 30;
	m_ipClipSize[CSTRIKE_DLL][CS_WEAPON_M249]				= 100;
	m_ipClipSize[CSTRIKE_DLL][CS_WEAPON_M3]				= 8;
	m_ipClipSize[CSTRIKE_DLL][CS_WEAPON_M4A1]				= 30;
	m_ipClipSize[CSTRIKE_DLL][CS_WEAPON_TMP]				= 30;
	m_ipClipSize[CSTRIKE_DLL][CS_WEAPON_G3SG1]			= 20;
	m_ipClipSize[CSTRIKE_DLL][CS_WEAPON_FLASHBANG]		= 2;
	m_ipClipSize[CSTRIKE_DLL][CS_WEAPON_DEAGLE]			= 7;
	m_ipClipSize[CSTRIKE_DLL][CS_WEAPON_SG552]			= 30;
	m_ipClipSize[CSTRIKE_DLL][CS_WEAPON_AK47]				= 30;
	m_ipClipSize[CSTRIKE_DLL][CS_WEAPON_KNIFE]			= 1;
	m_ipClipSize[CSTRIKE_DLL][CS_WEAPON_P90]				= 50;
	// cs version 1.0 weapons
	m_ipClipSize[CSTRIKE_DLL][CS_WEAPON_FIVESEVEN]		= 20;
	m_ipClipSize[CSTRIKE_DLL][CS_WEAPON_UMP45]			= 30;
	m_ipClipSize[CSTRIKE_DLL][CS_WEAPON_SG550]			= 30;

	// init func table for buying weapons
	m_pFBuy[CSTRIKE_DLL][CS_WEAPON_P228]		= BotBuy_CS_WEAPON_P228;
	m_pFBuy[CSTRIKE_DLL][CS_WEAPON_SCOUT]		= BotBuy_CS_WEAPON_SCOUT;
	m_pFBuy[CSTRIKE_DLL][CS_WEAPON_HEGRENADE]	= BotBuy_CS_WEAPON_HEGRENADE;
	m_pFBuy[CSTRIKE_DLL][CS_WEAPON_XM1014]		= BotBuy_CS_WEAPON_XM1014;
	m_pFBuy[CSTRIKE_DLL][CS_WEAPON_MAC10]		= BotBuy_CS_WEAPON_MAC10;
	m_pFBuy[CSTRIKE_DLL][CS_WEAPON_AUG]			= BotBuy_CS_WEAPON_AUG;
	m_pFBuy[CSTRIKE_DLL][CS_WEAPON_SMOKEGRENADE]= BotBuy_CS_WEAPON_SMOKEGRENADE;
	m_pFBuy[CSTRIKE_DLL][CS_WEAPON_ELITE]		= BotBuy_CS_WEAPON_ELITE;
	m_pFBuy[CSTRIKE_DLL][CS_WEAPON_FIVESEVEN]	= BotBuy_CS_WEAPON_FIVESEVEN;
	m_pFBuy[CSTRIKE_DLL][CS_WEAPON_UMP45]		= BotBuy_CS_WEAPON_UMP45;
	m_pFBuy[CSTRIKE_DLL][CS_WEAPON_SG550]		= BotBuy_CS_WEAPON_SG550;
	m_pFBuy[CSTRIKE_DLL][CS_WEAPON_USP]			= BotBuy_CS_WEAPON_USP;
	m_pFBuy[CSTRIKE_DLL][CS_WEAPON_GLOCK18]		= BotBuy_CS_WEAPON_GLOCK18;
	m_pFBuy[CSTRIKE_DLL][CS_WEAPON_AWP]			= BotBuy_CS_WEAPON_AWP;
	m_pFBuy[CSTRIKE_DLL][CS_WEAPON_MP5NAVY]		= BotBuy_CS_WEAPON_MP5NAVY;
	m_pFBuy[CSTRIKE_DLL][CS_WEAPON_M249]		= BotBuy_CS_WEAPON_M249;
	m_pFBuy[CSTRIKE_DLL][CS_WEAPON_M3]			= BotBuy_CS_WEAPON_M3;
	m_pFBuy[CSTRIKE_DLL][CS_WEAPON_M4A1]		= BotBuy_CS_WEAPON_M4A1;
	m_pFBuy[CSTRIKE_DLL][CS_WEAPON_TMP]			= BotBuy_CS_WEAPON_TMP;
	m_pFBuy[CSTRIKE_DLL][CS_WEAPON_G3SG1]		= BotBuy_CS_WEAPON_G3SG1;
	m_pFBuy[CSTRIKE_DLL][CS_WEAPON_FLASHBANG]	= BotBuy_CS_WEAPON_FLASHBANG;
	m_pFBuy[CSTRIKE_DLL][CS_WEAPON_DEAGLE]		= BotBuy_CS_WEAPON_DEAGLE;
	m_pFBuy[CSTRIKE_DLL][CS_WEAPON_SG552]		= BotBuy_CS_WEAPON_SG552;
	m_pFBuy[CSTRIKE_DLL][CS_WEAPON_AK47]		= BotBuy_CS_WEAPON_AK47;
	m_pFBuy[CSTRIKE_DLL][CS_WEAPON_P90]			= BotBuy_CS_WEAPON_P90;
	m_pFBuy[CSTRIKE_DLL][CS_WEAPON_HEGRENADE]	= BotBuy_CS_WEAPON_HE;
	m_pFBuy[CSTRIKE_DLL][CS_WEAPON_FLASHBANG]	= BotBuy_CS_WEAPON_FL;
	m_pFBuy[CSTRIKE_DLL][CS_WEAPON_SMOKEGRENADE]= BotBuy_CS_WEAPON_SG;

	for(int ischl=0;ischl < 32;ischl++){
		m_iCost[CSTRIKE_DLL][ischl] = 60000;				// just set evrything highn enough to prevent bot from buy non def stuff
		m_iTeam[CSTRIKE_DLL][ischl] = -1;							// standard is no team
		m_fPBuyProb[CSTRIKE_DLL][ischl] = 0;
	}

	m_iCost[CSTRIKE_DLL][CS_WEAPON_P228]				= 600;
	m_iCost[CSTRIKE_DLL][CS_WEAPON_SCOUT]				= 2750;
	m_iCost[CSTRIKE_DLL][CS_WEAPON_HEGRENADE]			= 300;
	m_iCost[CSTRIKE_DLL][CS_WEAPON_XM1014]			= 3000;
	m_iCost[CSTRIKE_DLL][CS_WEAPON_MAC10]				= 1400;
	m_iCost[CSTRIKE_DLL][CS_WEAPON_AUG]				= 3500;
	m_iCost[CSTRIKE_DLL][CS_WEAPON_SMOKEGRENADE]		= 300;
	m_iCost[CSTRIKE_DLL][CS_WEAPON_ELITE]				= 1000;
	m_iCost[CSTRIKE_DLL][CS_WEAPON_USP]				= 500;
	m_iCost[CSTRIKE_DLL][CS_WEAPON_GLOCK18]			= 400;
	m_iCost[CSTRIKE_DLL][CS_WEAPON_AWP]				= 4750;
	m_iCost[CSTRIKE_DLL][CS_WEAPON_MP5NAVY]			= 1500;
	m_iCost[CSTRIKE_DLL][CS_WEAPON_M249]				= 5750;
	m_iCost[CSTRIKE_DLL][CS_WEAPON_M3]				= 1700;
	m_iCost[CSTRIKE_DLL][CS_WEAPON_M4A1]				= 3100;
	m_iCost[CSTRIKE_DLL][CS_WEAPON_TMP]				= 1250;
	m_iCost[CSTRIKE_DLL][CS_WEAPON_G3SG1]				= 5000;
	m_iCost[CSTRIKE_DLL][CS_WEAPON_FLASHBANG]			= 300;
	m_iCost[CSTRIKE_DLL][CS_WEAPON_DEAGLE]			= 650;
	m_iCost[CSTRIKE_DLL][CS_WEAPON_SG552]				= 3500;
	m_iCost[CSTRIKE_DLL][CS_WEAPON_AK47]				= 2500;
	m_iCost[CSTRIKE_DLL][CS_WEAPON_P90]				= 2350;
	// cs version 1.0 weapons
	m_iCost[CSTRIKE_DLL][CS_WEAPON_FIVESEVEN]			= 750;
	m_iCost[CSTRIKE_DLL][CS_WEAPON_UMP45]				= 1700;
	m_iCost[CSTRIKE_DLL][CS_WEAPON_SG550]				= 4200;

	m_iTeam[CSTRIKE_DLL][CS_WEAPON_SG550]				= 1;		// ct=>1;te=>0
	m_iTeam[CSTRIKE_DLL][CS_WEAPON_SG552]				= 0;
	m_iTeam[CSTRIKE_DLL][CS_WEAPON_TMP]				= 1;
	m_iTeam[CSTRIKE_DLL][CS_WEAPON_M4A1]				= 1;
	m_iTeam[CSTRIKE_DLL][CS_WEAPON_MAC10]				= 0;
	m_iTeam[CSTRIKE_DLL][CS_WEAPON_AK47]				= 0;
	m_iTeam[CSTRIKE_DLL][CS_WEAPON_AUG]				= 1;

	m_fPBuyProb[CSTRIKE_DLL][CS_WEAPON_SCOUT]				= 5;
	m_fPBuyProb[CSTRIKE_DLL][CS_WEAPON_XM1014]			= 5;
	m_fPBuyProb[CSTRIKE_DLL][CS_WEAPON_MAC10]				= 5;
	m_fPBuyProb[CSTRIKE_DLL][CS_WEAPON_AUG]				= 20;
	m_fPBuyProb[CSTRIKE_DLL][CS_WEAPON_AWP]				= 8;
	m_fPBuyProb[CSTRIKE_DLL][CS_WEAPON_MP5NAVY]			= 20;
	m_fPBuyProb[CSTRIKE_DLL][CS_WEAPON_M249]				= 2;
	m_fPBuyProb[CSTRIKE_DLL][CS_WEAPON_M3]				= 3;
	m_fPBuyProb[CSTRIKE_DLL][CS_WEAPON_M4A1]				= 20;
	m_fPBuyProb[CSTRIKE_DLL][CS_WEAPON_TMP]				= 5;
	m_fPBuyProb[CSTRIKE_DLL][CS_WEAPON_G3SG1]				= 1;
	m_fPBuyProb[CSTRIKE_DLL][CS_WEAPON_SG552]				= 20;
	m_fPBuyProb[CSTRIKE_DLL][CS_WEAPON_AK47]				= 20;
	m_fPBuyProb[CSTRIKE_DLL][CS_WEAPON_P90]				= 5;
	// cs version 1.0 weapons
	m_fPBuyProb[CSTRIKE_DLL][CS_WEAPON_UMP45]				= 15;
	m_fPBuyProb[CSTRIKE_DLL][CS_WEAPON_SG550]				= 1;/*
	m_fPBuyProb[CSTRIKE_DLL][CS_WEAPON_SCOUT]				= 10;
	m_fPBuyProb[CSTRIKE_DLL][CS_WEAPON_XM1014]				= 10;
	m_fPBuyProb[CSTRIKE_DLL][CS_WEAPON_MAC10]				= 10;
	m_fPBuyProb[CSTRIKE_DLL][CS_WEAPON_AUG]				= 10;
	m_fPBuyProb[CSTRIKE_DLL][CS_WEAPON_AWP]				= 10;
	m_fPBuyProb[CSTRIKE_DLL][CS_WEAPON_MP5NAVY]			= 10;
	m_fPBuyProb[CSTRIKE_DLL][CS_WEAPON_M249]				= 10;
	m_fPBuyProb[CSTRIKE_DLL][CS_WEAPON_M3]					= 10;
	m_fPBuyProb[CSTRIKE_DLL][CS_WEAPON_M4A1]				= 10;
	m_fPBuyProb[CSTRIKE_DLL][CS_WEAPON_TMP]				= 10;
	m_fPBuyProb[CSTRIKE_DLL][CS_WEAPON_G3SG1]				= 10;
	m_fPBuyProb[CSTRIKE_DLL][CS_WEAPON_SG552]				= 10;
	m_fPBuyProb[CSTRIKE_DLL][CS_WEAPON_AK47]				= 10;
	m_fPBuyProb[CSTRIKE_DLL][CS_WEAPON_P90]				= 10;
	// cs version 1.0 weapons
	m_fPBuyProb[CSTRIKE_DLL][CS_WEAPON_UMP45]				= 10;
	m_fPBuyProb[CSTRIKE_DLL][CS_WEAPON_SG550]				= 10;*/

	float fTemp[32];
	memcpy(fTemp,m_fPBuyProb[CSTRIKE_DLL],sizeof(float) * 32);
	float fSum=0,fLast=0;
	long lschl;
	for(lschl=0;lschl<32;lschl++)
		fSum+=m_fPBuyProb[CSTRIKE_DLL][lschl];
	for(lschl=0;lschl<32;lschl++){
		/*if(fTemp[lschl]<0)
			continue;*/
		m_fPBuyProb[CSTRIKE_DLL][lschl] = fLast + fTemp[lschl] / fSum * 100.0f;
		fLast = m_fPBuyProb[CSTRIKE_DLL][lschl];
		//cout << fLast << endl;
		if(fTemp[lschl] == .0)
			m_fPBuyProb[CSTRIKE_DLL][lschl] = 0;
		//cout << m_fPBuyProb[CSTRIKE_DLL][lschl] << endl;
	}
	
	/* copy stuff for csclassic */
	memcpy(&(m_ipClipSize[CSCLASSIC_DLL]),&(m_ipClipSize[CSTRIKE_DLL]),sizeof(int)*32);
	memcpy(&(m_iCost[CSCLASSIC_DLL]),&(m_iCost[CSTRIKE_DLL]),sizeof(int)*32);
	memcpy(&(m_iTeam[CSCLASSIC_DLL]),&(m_iTeam[CSTRIKE_DLL]),sizeof(int)*32);

	memcpy(&(m_fPBuyProb[CSCLASSIC_DLL]),&(m_fPBuyProb[CSTRIKE_DLL]),sizeof(float)*32);
	/* ------------------------ */
}

CWeaponDefs :: ~CWeaponDefs(){}

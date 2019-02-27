// WeaponsCS.cpp: implementation of the CWeaponsCS class.
//
//////////////////////////////////////////////////////////////////////

#include "WeaponsCS.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#include "BaseBot.h"

// buy functions

void BotBuy_CS_WEAPON_P228(CBaseBot *pBot){
	pBot->fakeClientCommand("buy",0,0);
	pBot->fakeClientCommand("menuselect","1",0);
	pBot->fakeClientCommand("menuselect","4",0);
}

void BotBuy_CS_WEAPON_SCOUT(CBaseBot *pBot){
	pBot->fakeClientCommand("buy",0,0);
	pBot->fakeClientCommand("menuselect","4",0);
	pBot->fakeClientCommand("menuselect","5",0);
}

void BotBuy_CS_WEAPON_HEGRENADE(CBaseBot *pBot){
	pBot->fakeClientCommand("buyequip",0,0);
	pBot->fakeClientCommand("menuselect","4",0);
}

void BotBuy_CS_WEAPON_XM1014(CBaseBot *pBot){
	pBot->fakeClientCommand("buy",0,0);
	pBot->fakeClientCommand("menuselect","2",0);
	pBot->fakeClientCommand("menuselect","2",0);
}

void BotBuy_CS_WEAPON_MAC10(CBaseBot *pBot){
	pBot->fakeClientCommand("buy",0,0);
	pBot->fakeClientCommand("menuselect","3",0);
	pBot->fakeClientCommand("menuselect","4",0);
}

void BotBuy_CS_WEAPON_AUG(CBaseBot *pBot){
	pBot->fakeClientCommand("buy",0,0);
	pBot->fakeClientCommand("menuselect","4",0);
	pBot->fakeClientCommand("menuselect","4",0);
}

void BotBuy_CS_WEAPON_SMOKEGRENADE(CBaseBot *pBot){
	pBot->fakeClientCommand("buyequip",0,0);
	pBot->fakeClientCommand("menuselect","5",0);
}

void BotBuy_CS_WEAPON_ELITE(CBaseBot *pBot){
	pBot->fakeClientCommand("buy",0,0);
	pBot->fakeClientCommand("menuselect","1",0);
	pBot->fakeClientCommand("menuselect","5",0);
}

void BotBuy_CS_WEAPON_FIVESEVEN(CBaseBot *pBot){
	pBot->fakeClientCommand("buy",0,0);
	pBot->fakeClientCommand("menuselect","1",0);
	pBot->fakeClientCommand("menuselect","6",0);
}

void BotBuy_CS_WEAPON_UMP45(CBaseBot *pBot){
	pBot->fakeClientCommand("buy",0,0);
	pBot->fakeClientCommand("menuselect","3",0);
	pBot->fakeClientCommand("menuselect","5",0);
}

void BotBuy_CS_WEAPON_SG550(CBaseBot *pBot){
	pBot->fakeClientCommand("buy",0,0);
	pBot->fakeClientCommand("menuselect","4",0);
	pBot->fakeClientCommand("menuselect","8",0);
}

void BotBuy_CS_WEAPON_USP(CBaseBot *pBot){
	pBot->fakeClientCommand("buy",0,0);
	pBot->fakeClientCommand("menuselect","1",0);
	pBot->fakeClientCommand("menuselect","2",0);
}

void BotBuy_CS_WEAPON_GLOCK18(CBaseBot *pBot){
	pBot->fakeClientCommand("buy",0,0);
	pBot->fakeClientCommand("menuselect","1",0);
	pBot->fakeClientCommand("menuselect","1",0);
}

void BotBuy_CS_WEAPON_AWP(CBaseBot *pBot){
	pBot->fakeClientCommand("buy",0,0);
	pBot->fakeClientCommand("menuselect","4",0);
	pBot->fakeClientCommand("menuselect","6",0);
}

void BotBuy_CS_WEAPON_MP5NAVY(CBaseBot *pBot){
	pBot->fakeClientCommand("buy",0,0);
	pBot->fakeClientCommand("menuselect","3",0);
	pBot->fakeClientCommand("menuselect","1",0);
}

void BotBuy_CS_WEAPON_M249(CBaseBot *pBot){
	pBot->fakeClientCommand("buy",0,0);
	pBot->fakeClientCommand("menuselect","5",0);
	pBot->fakeClientCommand("menuselect","1",0);
}

void BotBuy_CS_WEAPON_M3(CBaseBot *pBot){
	pBot->fakeClientCommand("buy",0,0);
	pBot->fakeClientCommand("menuselect","2",0);
	pBot->fakeClientCommand("menuselect","1",0);
}

void BotBuy_CS_WEAPON_M4A1(CBaseBot *pBot){
	pBot->fakeClientCommand("buy",0,0);
	pBot->fakeClientCommand("menuselect","4",0);
	pBot->fakeClientCommand("menuselect","3",0);
}

void BotBuy_CS_WEAPON_TMP(CBaseBot *pBot){
	pBot->fakeClientCommand("buy",0,0);
	pBot->fakeClientCommand("menuselect","3",0);
	pBot->fakeClientCommand("menuselect","2",0);
}

void BotBuy_CS_WEAPON_G3SG1(CBaseBot *pBot){
	pBot->fakeClientCommand("buy",0,0);
	pBot->fakeClientCommand("menuselect","4",0);
	pBot->fakeClientCommand("menuselect","7",0);
}

void BotBuy_CS_WEAPON_FLASHBANG(CBaseBot *pBot){
	pBot->fakeClientCommand("buyequip",0,0);
	pBot->fakeClientCommand("menuselect","3",0);
}

void BotBuy_CS_WEAPON_DEAGLE(CBaseBot *pBot){
	pBot->fakeClientCommand("buy",0,0);
	pBot->fakeClientCommand("menuselect","1",0);
	pBot->fakeClientCommand("menuselect","3",0);
}

void BotBuy_CS_WEAPON_SG552(CBaseBot *pBot){
	pBot->fakeClientCommand("buy",0,0);
	pBot->fakeClientCommand("menuselect","4",0);
	pBot->fakeClientCommand("menuselect","2",0);
}

void BotBuy_CS_WEAPON_AK47(CBaseBot *pBot){
	pBot->fakeClientCommand("buy",0,0);
	pBot->fakeClientCommand("menuselect","4",0);
	pBot->fakeClientCommand("menuselect","1",0);
}

void BotBuy_CS_WEAPON_P90(CBaseBot *pBot){
	pBot->fakeClientCommand("buy",0,0);
	pBot->fakeClientCommand("menuselect","3",0);
	pBot->fakeClientCommand("menuselect","3",0);
}

void BotBuy_PAmmo(CBaseBot *pBot){
	pBot->fakeClientCommand("buy",0,0);
	pBot->fakeClientCommand("menuselect","6",0);
}

void BotBuy_SAmmo(CBaseBot *pBot){
	pBot->fakeClientCommand("buy",0,0);
	pBot->fakeClientCommand("menuselect","7",0);
}

void BotBuy_CS_WEAPON_HE(CBaseBot *pBot){
	pBot->fakeClientCommand("buyequip",0,0);
	pBot->fakeClientCommand("menuselect","4",0);
}

void BotBuy_CS_WEAPON_FL(CBaseBot *pBot){
	pBot->fakeClientCommand("buyequip",0,0);
	pBot->fakeClientCommand("menuselect","3",0);
}

void BotBuy_CS_WEAPON_SG(CBaseBot *pBot){
	pBot->fakeClientCommand("buyequip",0,0);
	pBot->fakeClientCommand("menuselect","5",0);
}

// CWEAPONS immpl

CWeaponsCS::CWeaponsCS(){
	// argh, somewhen this' really gotta be xmled :D
	CRespCurve RCZoomRifle;
	RCZoomRifle.addEntry(0,0);
	RCZoomRifle.addEntry(2000,3);

	CRespCurve RCZoomSniper;
	RCZoomSniper.addEntry(0,1);
	RCZoomSniper.addEntry(2000,3);

	CRespCurve RCDuckPistol;
	RCDuckPistol.addEntry(0,0);
	RCDuckPistol.addEntry(2000,2);
	RCDuckPistol.addEntry(3000,3);

	CRespCurve RCDuckSubM;
	RCDuckSubM=RCDuckPistol;

	CRespCurve RCDuckRifle;
	RCDuckRifle=RCDuckPistol;

	CRespCurve RCDuckSniper;
	RCDuckSniper.addEntry(0,0);
	RCDuckSniper.addEntry(1500,2);
	RCDuckSniper.addEntry(3000,3);

	CWeaponsItem *p;
	
	p = &(m_pWeapons[CS_WEAPON_P228]);

	p->m_bUsable = true;
	p->m_iClipSize = 13;
	p->m_iCost = 600;
	p->m_iLoudness = 160;
	p->m_pFBuy = BotBuy_CS_WEAPON_P228;
	p->m_Duck = RCDuckPistol;

	p->m_Usability.addEntry(0,20);
	p->m_Usability.addEntry(50,50);
	p->m_Usability.addEntry(1000,40);

	p = &(m_pWeapons[CS_WEAPON_SCOUT]);

	p->m_bUsable = true;
	p->m_bPenetratingWalls = true;
	p->m_fDelay = .75f;
	p->m_iClipSize = 10;
	p->m_fBuyProp = 5;
	p->m_iCost = 2750;
	p->m_iLoudness = 130;
	p->m_pFBuy = BotBuy_CS_WEAPON_SCOUT;
	p->m_Duck = RCDuckSniper;
	p->m_Zoom = RCZoomSniper;

	p->m_Usability.addEntry(0,0);
	p->m_Usability.addEntry(500,40);
	p->m_Usability.addEntry(700,95);

	p->m_iPreference = 1;

	p = &(m_pWeapons[CS_WEAPON_XM1014]);

	p->m_bUsable = true;
	p->m_fDelay = .2f;
	p->m_iClipSize = 7;
	p->m_fBuyProp = 5;
	p->m_iCost = 3000;
	p->m_iLoudness = 200;
	p->m_pFBuy = BotBuy_CS_WEAPON_XM1014;
	p->m_Usability.addEntry(0,100);
	p->m_Usability.addEntry(400,100);
	p->m_Usability.addEntry(600,0);

	p->m_iPreference = 1;

	p = &(m_pWeapons[CS_WEAPON_MAC10]);

	p->m_bUsable = true;
	p->m_iClipSize = 30;
	p->m_fBuyProp = 5;
	p->m_iCost = 1400;
	p->m_iTeam = 0;
	p->m_iLoudness = 180;
	p->m_pFBuy = BotBuy_CS_WEAPON_MAC10;
	p->m_Duck = RCDuckSubM;

	p->m_Usability.addEntry(0,60);
	p->m_Usability.addEntry(400,75);
	p->m_Usability.addEntry(1000,20);

	p->m_iPreference = -1;

	p->m_fPauseTime = .7f;

	p = &(m_pWeapons[CS_WEAPON_AUG]);

	p->m_bUsable = true;
	p->m_bPenetratingWalls = true;
	p->m_iClipSize = 30;
	p->m_fBuyProp = 20;
	p->m_iCost = 3500;
	p->m_iTeam = 1;
	p->m_iLoudness = 230;
	p->m_pFBuy = BotBuy_CS_WEAPON_AUG;
	p->m_Duck = RCDuckRifle;
	p->m_Zoom = RCZoomRifle;

	p->m_Usability.addEntry(0,10);
	p->m_Usability.addEntry(300,50);
	p->m_Usability.addEntry(600,85);

	p->m_iPreference = 2;

	p->m_fPauseTime = .7f;

	p = &(m_pWeapons[CS_WEAPON_ELITE]);

	p->m_bUsable = true;
	p->m_iClipSize = 30;
	p->m_iCost = 1000;
	p->m_iTeam = 0;
	p->m_iLoudness = 180;
	p->m_pFBuy = BotBuy_CS_WEAPON_ELITE;
	p->m_Duck = RCDuckPistol;

	p->m_Usability.addEntry(0,20);
	p->m_Usability.addEntry(50,50);
	p->m_Usability.addEntry(1000,10);
	p->m_Usability.addEntry(1200,00);

	p->m_iPreference = 2;

	p->m_fPauseTime = .4f;

	p = &(m_pWeapons[CS_WEAPON_FIVESEVEN]);

	p->m_bUsable = true;
	p->m_iClipSize = 20;
	p->m_iCost = 750;
	p->m_iTeam = 1;
	p->m_iLoudness = 160;
	p->m_pFBuy = BotBuy_CS_WEAPON_FIVESEVEN;
	p->m_Duck = RCDuckPistol;

	p->m_Usability.addEntry(0,20);
	p->m_Usability.addEntry(5,50);
	p->m_Usability.addEntry(1000,30);
	p->m_iPreference = 1;

	p = &(m_pWeapons[CS_WEAPON_UMP45]);

	p->m_bUsable = true;
	p->m_iClipSize = 30;
	p->m_fBuyProp = 10;
	p->m_iCost = 1700;
	p->m_iLoudness = 180;
	p->m_pFBuy = BotBuy_CS_WEAPON_UMP45;
	p->m_Duck = RCDuckSubM;

	p->m_Usability.addEntry(0,50);
	p->m_Usability.addEntry(600,80);
	p->m_Usability.addEntry(1200,60);

	p->m_fPauseTime = .4f;

	p = &(m_pWeapons[CS_WEAPON_SG550]);

	p->m_bUsable = true;
	p->m_bPenetratingWalls = true;
	p->m_fDelay = .4f;
	p->m_iClipSize = 30;
	p->m_fBuyProp = 1;
	p->m_iCost = 4200;
	p->m_iTeam = 1;
	p->m_iLoudness = 230;
	p->m_pFBuy = BotBuy_CS_WEAPON_SG550;
	p->m_Duck = RCDuckSniper;
	p->m_Zoom = RCZoomSniper;
	
	p->m_Usability.addEntry(300,0);
	p->m_Usability.addEntry(500,50);
	p->m_Usability.addEntry(700,95);

	p->m_iPreference = 1;

	p->m_fPauseTime = .5f;

	p = &(m_pWeapons[CS_WEAPON_USP]);

	p->m_bUsable = true;
	p->m_iClipSize = 12;
	p->m_iCost = 500;
	p->m_iLoudness = 170;
	p->m_pFBuy = BotBuy_CS_WEAPON_USP;
	p->m_Duck = RCDuckPistol;

	p->m_Usability.addEntry(0,20);
	p->m_Usability.addEntry(50,50);
	p->m_Usability.addEntry(1000,35);

	p = &(m_pWeapons[CS_WEAPON_GLOCK18]);

	p->m_bUsable = true;
	p->m_iClipSize = 20;
	p->m_iCost = 400;
	p->m_iLoudness = 170;
	p->m_pFBuy = BotBuy_CS_WEAPON_GLOCK18;
	p->m_Duck = RCDuckPistol;

	p->m_Usability.addEntry(0,20);
	p->m_Usability.addEntry(50,50);
	p->m_Usability.addEntry(1000,35);

	p = &(m_pWeapons[CS_WEAPON_AWP]);

	p->m_bUsable = true;
	p->m_iClipSize = 10;
	p->m_fBuyProp = 8;
	p->m_iCost = 4750;
	p->m_iLoudness = 255;
	p->m_pFBuy = BotBuy_CS_WEAPON_AWP;
	p->m_bPenetratingWalls = true;
	p->m_fDelay = 1.25f;
	p->m_Duck = RCDuckSniper;
	p->m_Zoom = RCZoomSniper;

	p->m_Usability.addEntry(250,0);
	p->m_Usability.addEntry(500,40);
	p->m_Usability.addEntry(700,100);

	p->m_iPreference = 2;

	p = &(m_pWeapons[CS_WEAPON_MP5NAVY]);

	p->m_bUsable = true;
	p->m_iClipSize = 30;
	p->m_fBuyProp = 20;
	p->m_iCost = 1500;
	p->m_iLoudness = 180;
	p->m_pFBuy = BotBuy_CS_WEAPON_MP5NAVY;
	p->m_Duck = RCDuckSubM;

	p->m_Usability.addEntry(0,50);
	p->m_Usability.addEntry(450,75);
	p->m_Usability.addEntry(1000,50);

	p->m_fPauseTime = .4f;

	p = &(m_pWeapons[CS_WEAPON_M249]);

	p->m_bUsable = true;
	p->m_iClipSize = 100;
	p->m_fBuyProp = 3;
	p->m_iCost = 5750;
	p->m_iLoudness = 240;
	p->m_pFBuy = BotBuy_CS_WEAPON_M249;

	p->m_Usability.addEntry(0,60);
	p->m_Usability.addEntry(400,80);
	p->m_Usability.addEntry(1000,30);

	p->m_iPreference = 2;
	p->m_fPauseTime = .7f;

	p = &(m_pWeapons[CS_WEAPON_M3]);

	p->m_bUsable = true;
	p->m_fDelay = .9f;
	p->m_iClipSize = 8;
	p->m_fBuyProp = 10;
	p->m_iCost = 1700;
	p->m_iLoudness = 220;
	p->m_pFBuy = BotBuy_CS_WEAPON_M3;

	p->m_Usability.addEntry(0,100);
	p->m_Usability.addEntry(400,100);
	p->m_Usability.addEntry(600,0);

	p = &(m_pWeapons[CS_WEAPON_M4A1]);

	p->m_bUsable = true;
	p->m_bPenetratingWalls = true;
	p->m_iClipSize = 30;
	p->m_fBuyProp = 10;
	p->m_iCost = 3100;
	p->m_iTeam = 1;
	p->m_iLoudness = 230;
	p->m_pFBuy = BotBuy_CS_WEAPON_M4A1;
	p->m_Duck = RCDuckRifle;

	p->m_Usability.addEntry(0,50);
	p->m_Usability.addEntry(400,75);
	p->m_Usability.addEntry(1000,70);

	p->m_iPreference = 20;

	p->m_fPauseTime = .5f;

	p = &(m_pWeapons[CS_WEAPON_TMP]);

	p->m_bUsable = true;
	p->m_iClipSize = 30;
	p->m_fBuyProp = 5;
	p->m_iCost = 1250;
	p->m_iTeam = 1;
	p->m_iLoudness = 150;
	p->m_pFBuy = BotBuy_CS_WEAPON_TMP;
	p->m_Duck = RCDuckSubM;

	p->m_Usability.addEntry(0,40);
	p->m_Usability.addEntry(400,80);
	p->m_Usability.addEntry(1000,40);

	p->m_iPreference = -1;

	p->m_fPauseTime = .5f;

	p = &(m_pWeapons[CS_WEAPON_G3SG1]);

	p->m_bUsable = true;
	p->m_bPenetratingWalls = true;
	p->m_fDelay = .4f;
	p->m_iClipSize = 20;
	p->m_fBuyProp = 1;
	p->m_iCost = 5000;
	p->m_iTeam = 0;
	p->m_iLoudness = 230;
	p->m_pFBuy = BotBuy_CS_WEAPON_G3SG1;
	p->m_Duck = RCDuckSniper;
	p->m_Zoom = RCZoomSniper;

	p->m_Usability.addEntry(300,0);
	p->m_Usability.addEntry(500,40);
	p->m_Usability.addEntry(700,95);

	p->m_iPreference = 2;

	p = &(m_pWeapons[CS_WEAPON_DEAGLE]);

	p->m_bUsable = true;
	p->m_bPenetratingWalls = true;
	p->m_fDelay = .3f;
	p->m_iClipSize = 7;
	p->m_iCost = 650;
	p->m_iLoudness = 255;
	p->m_pFBuy = BotBuy_CS_WEAPON_DEAGLE;
	p->m_Duck = RCDuckPistol;

	p->m_Usability.addEntry(0,30);
	p->m_Usability.addEntry(50,50);
	p->m_Usability.addEntry(1000,55);

	p->m_fPauseTime = .4f;

	p = &(m_pWeapons[CS_WEAPON_SG552]);

	p->m_bUsable = true;
	p->m_bPenetratingWalls = true;
	p->m_iClipSize = 30;
	p->m_fBuyProp = 20;
	p->m_iCost = 3500;
	p->m_iTeam = 0;
	p->m_iLoudness = 230;
	p->m_pFBuy = BotBuy_CS_WEAPON_SG552;
	p->m_Duck = RCDuckRifle;
	p->m_Zoom = RCZoomRifle;

	p->m_Usability.addEntry(0,40);
	p->m_Usability.addEntry(400,70);
	p->m_Usability.addEntry(1000,80);

	p->m_iPreference = 2;

	p->m_fPauseTime = .5f;

	p = &(m_pWeapons[CS_WEAPON_AK47]);

	p->m_bUsable = true;
	p->m_bPenetratingWalls = true;
	p->m_iClipSize = 30;
	p->m_fBuyProp = 20;
	p->m_iCost = 2500;
	p->m_iTeam = 0;
	p->m_iLoudness = 255;
	p->m_pFBuy = BotBuy_CS_WEAPON_AK47;
	p->m_Duck = RCDuckRifle;

	p->m_Usability.addEntry(0,60);
	p->m_Usability.addEntry(400,70);
	p->m_Usability.addEntry(1000,60);

	p->m_iPreference = 1;

	p->m_fPauseTime = .5f;

	p = &(m_pWeapons[CS_WEAPON_KNIFE]);

	p->m_bUsable = true;
	p->m_iClipSize = 1;
	
	p->m_iCost = 100000;
	
	p->m_iLoudness = 0;
	p->m_pFBuy = 0;

	p->m_Usability.addEntry(0,60);
	p->m_Usability.addEntry(50,50);
	p->m_Usability.addEntry(80,0);

	p = &(m_pWeapons[CS_WEAPON_P90]);

	p->m_bUsable = true;
	p->m_iClipSize = 50;
	p->m_fBuyProp = 10;
	p->m_iCost = 2350;
	
	p->m_iLoudness = 180;
	p->m_pFBuy = BotBuy_CS_WEAPON_P90;
	p->m_Duck = RCDuckSubM;

	p->m_Usability.addEntry(0,55);
	p->m_Usability.addEntry(400,80);
	p->m_Usability.addEntry(1000,40);

	p->m_fPauseTime = .6f;

	p = &(m_pWeapons[CS_WEAPON_HEGRENADE]);
	p->m_iLoudness = 10;
	p->m_pFBuy = BotBuy_CS_WEAPON_HEGRENADE;
	p->m_Usability.addEntry(0,0);
	p->m_Usability.addEntry(400,0);
	p->m_Usability.addEntry(1000,90);
	p->m_Usability.addEntry(1350,0);

	p = &(m_pWeapons[CS_WEAPON_FLASHBANG]);
	p->m_iLoudness = 10;
	p->m_pFBuy = BotBuy_CS_WEAPON_FLASHBANG;

	p = &(m_pWeapons[CS_WEAPON_SMOKEGRENADE]);
	p->m_iLoudness = 10;
	p->m_pFBuy = BotBuy_CS_WEAPON_SMOKEGRENADE;


	randomizePrefs(-1,1);		// some differences between bots are always welcome
}

CWeaponsCS::~CWeaponsCS(){
}

long CWeaponsCS :: isSecondary(long lbit){
	if(lbit & 1<<CS_WEAPON_USP){
		return CS_WEAPON_USP;
	}
	else if(lbit & 1<<CS_WEAPON_GLOCK18){
		return CS_WEAPON_GLOCK18;
	}
	else if(lbit & 1<<CS_WEAPON_DEAGLE){
		return CS_WEAPON_DEAGLE;
	}
	else if(lbit & 1<<CS_WEAPON_ELITE){
		return CS_WEAPON_ELITE;
	}
	else if(lbit & 1<<CS_WEAPON_P228){
		return CS_WEAPON_P228;
	}
	else if(lbit & 1<<CS_WEAPON_FIVESEVEN){		// cs10
		return CS_WEAPON_FIVESEVEN;
	}
	
	return 0;
}

long CWeaponsCS :: isPrimary(long lbit){
	if(lbit & 1<<CS_WEAPON_AK47){
		return CS_WEAPON_AK47;
	}
	if(lbit & 1<<CS_WEAPON_AWP){
		return CS_WEAPON_AWP;
	}
	else if(lbit & 1<<CS_WEAPON_AUG){
		return CS_WEAPON_AUG;
	}
	else if(lbit & 1<<CS_WEAPON_G3SG1){
		return CS_WEAPON_G3SG1;
	}
	else if(lbit & 1<<CS_WEAPON_M249){
		return CS_WEAPON_M249;
	}
	else if(lbit & 1<<CS_WEAPON_M3){
		return CS_WEAPON_M3;
	}
	else if(lbit & 1<<CS_WEAPON_M4A1){
		return CS_WEAPON_M4A1;
	}
	else if(lbit & 1<<CS_WEAPON_MAC10){
		return CS_WEAPON_MAC10;
	}
	else if(lbit & 1<<CS_WEAPON_MP5NAVY){
		return CS_WEAPON_MP5NAVY;
	}
	else if(lbit & 1<<CS_WEAPON_P90){
		return CS_WEAPON_P90;
	}
	else if(lbit & 1<<CS_WEAPON_TMP){
		return CS_WEAPON_TMP;
	}
	else if(lbit & 1<<CS_WEAPON_SCOUT){
		return CS_WEAPON_SCOUT;
	}
	else if(lbit & 1<<CS_WEAPON_SG552){
		return CS_WEAPON_SG552;
	}
	else if(lbit & 1<<CS_WEAPON_XM1014){
		return CS_WEAPON_XM1014;
	}
	else if(lbit & 1<<CS_WEAPON_UMP45){	// cs10
		return CS_WEAPON_UMP45;
	}
	else if(lbit & 1<<CS_WEAPON_SG550){
		return CS_WEAPON_SG550;
	}
	return 0;
}

long CWeaponsCS :: isShotgun(long lbit){
	if(lbit & 1<<CS_WEAPON_XM1014){
		return CS_WEAPON_XM1014;
	}
	else if(lbit & 1<<CS_WEAPON_M3){
		return CS_WEAPON_M3;
	}
	return 0;
}

long CWeaponsCS :: isSubM(long lbit){
	if(lbit & 1<<CS_WEAPON_UMP45){
		return CS_WEAPON_UMP45;
	}
	else if(lbit & 1<<CS_WEAPON_MAC10){
		return CS_WEAPON_MAC10;
	}
	else if(lbit & 1<<CS_WEAPON_MP5NAVY){
		return CS_WEAPON_MP5NAVY;
	}
	else if(lbit & 1<<CS_WEAPON_P90){
		return CS_WEAPON_P90;
	}
	else if(lbit & 1<<CS_WEAPON_TMP){
		return CS_WEAPON_TMP;
	}
	
	return 0;
}

long CWeaponsCS :: isMachineGun(long lbit){
	if(lbit & 1<<CS_WEAPON_M249){
		return CS_WEAPON_M249;
	}
	else
		return 0;
}

long CWeaponsCS :: isRifle(long lbit){
	if(lbit & 1<<CS_WEAPON_AK47){
		return CS_WEAPON_AK47;
	}
	else if(lbit & 1<<CS_WEAPON_AUG){
		return CS_WEAPON_AUG;
	}
	else if(lbit & 1<<CS_WEAPON_M4A1){
		return CS_WEAPON_M4A1;
	}
	return 0;
}

long CWeaponsCS :: isSniper(long lbit){				// return weapons, who allow you to zoom
	if(lbit & 1<<CS_WEAPON_AUG){
		return CS_WEAPON_AUG;
	}
	else if(lbit & 1<<CS_WEAPON_AWP){
		return CS_WEAPON_AWP;
	}
	else if(lbit & 1<<CS_WEAPON_G3SG1){
		return CS_WEAPON_G3SG1;
	}
	else if(lbit & 1<<CS_WEAPON_SCOUT){
		return CS_WEAPON_SCOUT;
	}
	else if(lbit & 1<<CS_WEAPON_SG552){
		return CS_WEAPON_SG552;
	}
	else if(lbit & 1<<CS_WEAPON_SG550){	// cs 10
		return CS_WEAPON_SG550;
	}
	return 0;
}

long CWeaponsCS :: isGrenade(long lbit){
	if(lbit & 1<<CS_WEAPON_HEGRENADE){
		return CS_WEAPON_HEGRENADE;
	}
	else if(lbit & 1<<CS_WEAPON_FLASHBANG){
		return CS_WEAPON_FLASHBANG;
	}
	else if(lbit & 1<<CS_WEAPON_SMOKEGRENADE){
		return CS_WEAPON_SMOKEGRENADE;
	}
	return 0;
}

long CWeaponsCS :: isExplGrenade(long lbit){
	if(lbit & 1<<CS_WEAPON_HEGRENADE){
		return CS_WEAPON_HEGRENADE;
	}
	return 0;
}

long CWeaponsCS :: isKnife(long lbit){
	if(lbit & 1<<CS_WEAPON_KNIFE){
		return CS_WEAPON_KNIFE;
	}
	else return 0;
}

// WeaponsDOD.cpp: implementation of the CWeaponsDOD class.
//
//////////////////////////////////////////////////////////////////////

#include "WeaponsDOD.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWeaponsDOD::CWeaponsDOD()
{
	/*
	1 weapon_amerknife -1
	2 weapon_gerknife -1
	3 weapon_colt 7
	4 weapon_luger 8
	5 weapon_garand 8
	6 weapon_scopedkar 5
	7 weapon_thompson 30
	8 weapon_mp44 30
	9 weapon_spring 5
	10 weapon_kar 5
	11 weapon_bar 20
	12 weapon_mp40 32
	13 weapon_handgrenade -1
	14 weapon_stickgrenade -1
	15 weapon_stickgrenade_ex -1
	16 weapon_handgrenade_ex -1
	17 weapon_mg42 250
	18 weapon_30cal 150
	19 weapon_spade -1
	20 weapon_m1carbine 15
	21 weapon_mg34 75
	22 weapon_greasegun 30
	23 weapon_fg42 20
	24 weapon_k43 10
	*/
	CWeaponsItem *p;
	
	// weapons for DOD
	p = &(m_pWeapons[DOD_WEAPON_AMERKNIFE]);
	
	p->m_bUsable = true;
	p->m_iClipSize = -1;

	p->m_Usability.addEntry(0,70);
	p->m_Usability.addEntry(100,0);
	
	p = &(m_pWeapons[DOD_WEAPON_GERKNIFE]);
	
	p->m_bUsable = true;
	p->m_iClipSize = -1;

	p->m_Usability.addEntry(0,70);
	p->m_Usability.addEntry(100,0);
	
	p = &(m_pWeapons[DOD_WEAPON_COLT]);
	
	p->m_bUsable = true;
	p->m_iClipSize = 7;

	p->m_Usability.addEntry(0,20);
	p->m_Usability.addEntry(500,40);
	p->m_Usability.addEntry(1000,20);
	
	p = &(m_pWeapons[DOD_WEAPON_LUGER]);
	
	p->m_bUsable = true;
	p->m_iClipSize = 8;

	p->m_Usability.addEntry(0,30);
	p->m_Usability.addEntry(500,50);

	p = &(m_pWeapons[DOD_WEAPON_GARAND]);
	
	p->m_bUsable = true;
	p->m_iClipSize = 8;

	p->m_Usability.addEntry(0,30);
	p->m_Usability.addEntry(500,50);
	
	p = &(m_pWeapons[DOD_WEAPON_SCOPEDKAR]);
	
	p->m_bUsable = true;
	p->m_iClipSize = 5;

	p->m_Usability.addEntry(0,5);
	p->m_Usability.addEntry(500,50);
	p->m_Usability.addEntry(1000,80);
	
	p = &(m_pWeapons[DOD_WEAPON_THOMPSON]);
	
	p->m_bUsable = true;
	p->m_iClipSize = 30;

	p->m_Usability.addEntry(0,30);
	p->m_Usability.addEntry(500,50);
	
	p = &(m_pWeapons[DOD_WEAPON_MP44]);
	
	p->m_bUsable = true;
	p->m_iClipSize = 30;

	p->m_Usability.addEntry(0,30);
	p->m_Usability.addEntry(500,50);
	
	p = &(m_pWeapons[DOD_WEAPON_SPRING]);
	
	p->m_bUsable = true;
	p->m_iClipSize = 5;

	p->m_Usability.addEntry(0,5);
	p->m_Usability.addEntry(500,50);
	p->m_Usability.addEntry(1000,80);
	
	p = &(m_pWeapons[DOD_WEAPON_KAR]);
	
	p->m_bUsable = true;
	p->m_iClipSize = 5;

	p->m_Usability.addEntry(0,30);
	p->m_Usability.addEntry(500,50);
	
	p = &(m_pWeapons[DOD_WEAPON_BAR]);
	
	p->m_bUsable = true;
	p->m_iClipSize = 20;

	p->m_Usability.addEntry(0,30);
	p->m_Usability.addEntry(500,50);
	
	p = &(m_pWeapons[DOD_WEAPON_MP40]);
	
	p->m_bUsable = true;
	p->m_iClipSize = 30;

	p->m_Usability.addEntry(0,30);
	p->m_Usability.addEntry(500,50);
	
	p = &(m_pWeapons[DOD_WEAPON_MG42]);
	
	p->m_bUsable = true;
	p->m_iClipSize = 250;

	p->m_Usability.addEntry(0,30);
	p->m_Usability.addEntry(500,50);
	
	p = &(m_pWeapons[DOD_WEAPON_30CAL]);
	
	p->m_bUsable = true;
	p->m_iClipSize = 150;

	p->m_Usability.addEntry(0,30);
	p->m_Usability.addEntry(500,50);
	
	p = &(m_pWeapons[DOD_WEAPON_SPADE]);
	
	p->m_bUsable = true;
	p->m_iClipSize = -1;

	p->m_Usability.addEntry(0,70);
	p->m_Usability.addEntry(100,0);
	
	p = &(m_pWeapons[DOD_WEAPON_M1CARBINE]);
	
	p->m_bUsable = true;
	p->m_iClipSize = 15;

	p->m_Usability.addEntry(0,30);
	p->m_Usability.addEntry(750,70);
	
	p = &(m_pWeapons[DOD_WEAPON_MG34]);
	
	p->m_bUsable = true;
	p->m_iClipSize = 75;

	p->m_Usability.addEntry(0,30);
	p->m_Usability.addEntry(500,80);

	p = &(m_pWeapons[DOD_WEAPON_GREASEGUN]);
	
	p->m_bUsable = true;
	p->m_iClipSize = 30;

	p->m_Usability.addEntry(0,30);
	p->m_Usability.addEntry(500,50);

	p = &(m_pWeapons[DOD_WEAPON_FG42]);
	
	p->m_bUsable = true;
	p->m_iClipSize = 20;

	p->m_Usability.addEntry(0,30);
	p->m_Usability.addEntry(500,60);

	p = &(m_pWeapons[DOD_WEAPON_K43]);
	
	p->m_bUsable = true;
	p->m_iClipSize = 10;

	p->m_Usability.addEntry(0,30);
	p->m_Usability.addEntry(500,65);

	randomizePrefs(-1,1);		// some differences between bots are always welcome
}

CWeaponsDOD::~CWeaponsDOD()
{

}

long CWeaponsDOD :: isSecondary(long lbit){
	
	if(lbit & 1<<DOD_WEAPON_LUGER){
		return DOD_WEAPON_LUGER;
	}
	else if(lbit & 1<<DOD_WEAPON_COLT){
		return DOD_WEAPON_COLT;
	}
	return 0;
}

long CWeaponsDOD :: isPrimary(long lbit){
	
	if(lbit & 1<<DOD_WEAPON_KAR){
		return DOD_WEAPON_KAR;
	}
	else if(lbit & 1<<DOD_WEAPON_MP40){
		return DOD_WEAPON_MP40;
	}
	else if(lbit & 1<<DOD_WEAPON_MP44){
		return DOD_WEAPON_MP44;
	}
	else if(lbit & 1<<DOD_WEAPON_SCOPEDKAR){
		return DOD_WEAPON_SCOPEDKAR;
	}
	else if(lbit & 1<<DOD_WEAPON_GARAND){
		return DOD_WEAPON_GARAND;
	}
	else if(lbit & 1<<DOD_WEAPON_THOMPSON){
		return DOD_WEAPON_THOMPSON;
	}
	else if(lbit & 1<<DOD_WEAPON_SPRING){
		return DOD_WEAPON_SPRING;
	}
	else if(lbit & 1<<DOD_WEAPON_BAR){
		return DOD_WEAPON_BAR;
	}
	return 0;
}

long CWeaponsDOD :: isShotgun(long lbit){
	// there are no shotguns in dod
	return 0;
}

long CWeaponsDOD :: isSubM(long lbit){
	if(lbit & 1<<DOD_WEAPON_MP40){
		return DOD_WEAPON_MP40;
	}
	else if(lbit & 1<<DOD_WEAPON_MP44){
		return DOD_WEAPON_MP44;
	}
	else if(lbit & 1<<DOD_WEAPON_THOMPSON){
		return DOD_WEAPON_THOMPSON;
	}
	else if(lbit & 1<<DOD_WEAPON_BAR){
		return DOD_WEAPON_BAR;
	}
	else if(lbit & 1<<DOD_WEAPON_M1CARBINE){
		return DOD_WEAPON_M1CARBINE;
	}
	return 0;
}

long CWeaponsDOD :: isMachineGun(long lbit){
	if(lbit & 1<<DOD_WEAPON_30CAL){
		return DOD_WEAPON_30CAL;
	}
	else if(lbit & 1<<DOD_WEAPON_MG34){
		return DOD_WEAPON_MG34;
	}
	else if(lbit & 1<<DOD_WEAPON_MG42){
		return DOD_WEAPON_MG42;
	}
	else
		return 0;
}

long CWeaponsDOD :: isRifle(long lbit){
	return 0;
}

long CWeaponsDOD :: isKnife(long lbit){
	if(lbit & 1<<DOD_WEAPON_AMERKNIFE){
		return DOD_WEAPON_AMERKNIFE;
	}
	else if(lbit & 1<<DOD_WEAPON_GERKNIFE){
		return DOD_WEAPON_GERKNIFE;
	}
	else if(lbit & 1<<DOD_WEAPON_SPADE){
		return DOD_WEAPON_SPADE;
	}
	else
		return 0;
}

long CWeaponsDOD :: isSniper(long lbit){				// return weapons, who allow you to zoom
	if(lbit & 1<<DOD_WEAPON_SCOPEDKAR){
		return DOD_WEAPON_SCOPEDKAR;
	}
	else if(lbit & 1<<DOD_WEAPON_SPRING){
		return DOD_WEAPON_SPRING;
	}
	return 0;
}

long CWeaponsDOD :: isGrenade(long lbit){
	return 0;
}
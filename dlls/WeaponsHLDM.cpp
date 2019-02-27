// WeaponsHLDM.cpp: implementation of the CWeaponsHLDM class.
//
//////////////////////////////////////////////////////////////////////

#include "WeaponsHLDM.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWeaponsHLDM::CWeaponsHLDM()
{
	CWeaponsItem *p;

	p = &(m_pWeapons[VALVE_WEAPON_CROWBAR]);

	p->m_bUsable = true;
	p->m_iClipSize = 1;
	p->m_Usability.addEntry(0,100);
	p->m_Usability.addEntry(50,90);
	p->m_Usability.addEntry(80,0);

	p = &(m_pWeapons[VALVE_WEAPON_GLOCK]);

	p->m_bUsable = true;
	p->m_iClipSize = 20;

	p->m_Usability.addEntry(0,20);
	p->m_Usability.addEntry(300,40);
	p->m_Usability.addEntry(1000,0);

	p = &(m_pWeapons[VALVE_WEAPON_357]);

	p->m_bUsable = true;

	p->m_Usability.addEntry(0,30);
	p->m_Usability.addEntry(300,50);
	p->m_Usability.addEntry(1000,30);

	p = &(m_pWeapons[VALVE_WEAPON_MP5]);

	p->m_bUsable = true;
	p->m_iClipSize = 50;

	p->m_Usability.addEntry(0,40);
	p->m_Usability.addEntry(300,40);
	p->m_Usability.addEntry(1000,20);

	p = &(m_pWeapons[VALVE_WEAPON_CROSSBOW]);

	p->m_bUsable = true;
	p->m_iClipSize = 5;

	p->m_Usability.addEntry(0,0);
	p->m_Usability.addEntry(300,50);
	p->m_Usability.addEntry(1000,50);

	p = &(m_pWeapons[VALVE_WEAPON_SHOTGUN]);

	p->m_bUsable = true;
	p->m_iClipSize = 8;

	p->m_Usability.addEntry(0,90);
	p->m_Usability.addEntry(300,90);
	p->m_Usability.addEntry(400,0);

	p = &(m_pWeapons[VALVE_WEAPON_RPG]);

	p->m_bUsable = true;
	p->m_iClipSize = 1;

	p->m_Usability.addEntry(300,0);
	p->m_Usability.addEntry(600,80);
	p->m_Usability.addEntry(1000,90);

	p = &(m_pWeapons[VALVE_WEAPON_GAUSS]);

	p->m_bUsable = true;
	p->m_iClipSize = 100;

	p->m_Usability.addEntry(0,15);
	p->m_Usability.addEntry(600,80);
	p->m_Usability.addEntry(1200,40);

	p = &(m_pWeapons[VALVE_WEAPON_EGON]);

	p->m_bUsable = true;
	p->m_iClipSize = 100;

	p->m_Usability.addEntry(0,50);

	p = &(m_pWeapons[VALVE_WEAPON_HORNETGUN]);

	p->m_bUsable = true;
	p->m_iClipSize = 8;

	p->m_Usability.addEntry(0,15);
	p->m_Usability.addEntry(600,80);
	p->m_Usability.addEntry(1200,0);

	p = &(m_pWeapons[VALVE_WEAPON_SNARK]);

	p->m_bUsable = true;
	p->m_iClipSize = 20;

	p->m_Usability.addEntry(0,5);
	p->m_Usability.addEntry(600,4);
	p->m_Usability.addEntry(1000,0);
}

CWeaponsHLDM::~CWeaponsHLDM()
{

}

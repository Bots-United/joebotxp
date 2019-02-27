#ifndef __CWEAPONDEFS_H
#define __CWEAPONDEFS_H

#include "bot_modid.h"
///////////////////////////////////////////////////////////////////////////////////////
// class CWeaponDefs
//
// this class contains hard-coded information about different weapons in different mods
//

class CBaseBot;

class CWeaponDefs{
public:
	CWeaponDefs();
	void init(void);
	~CWeaponDefs();

	int m_ipClipSize[MAX_MOD][32];
	int m_iCost[MAX_MOD][32];
	int m_iTeam[MAX_MOD][32];
	float m_fPBuyProb[MAX_MOD][32];
	float m_fLoudness[MAX_MOD][32];
	void (*m_pFBuy[MAX_MOD][32])(CBaseBot *);
protected:
private:
};

extern CWeaponDefs g_WeaponDefs;

#endif //__CWEAPONDEFS_H
#ifndef __BV_CS_H
#define __BV_CS_H

#include "Behaviour.h"
#include "Emotion.h"
#include "Callback.h"
#include "extdll.h"	// for Vector def
#include <string.h>

#include "BV_HLDM.h"

class CBV_CS_SquadCommand:public CBV_HLDM_SquadCommand{
public:
	CBV_CS_SquadCommand(CBaseBot *pBot):CBV_HLDM_SquadCommand(pBot){}

	virtual void execute(CGoal *);
};

class CBVNP_CS_Buy:public CBehaviour{
public:
	CBVNP_CS_Buy(CBaseBot *pBot):CBehaviour(pBot){strcpy(m_szName,"CBVNP_CS_Buy");
		percept(CBehaviour::P_NONE);}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void execute(CGoal *);
};

class CBV_CS_GuardWBBomb:public CBehaviour,CCallback_onPathCreation,CCallback_onReachedDestination{
public:
	CBV_CS_GuardWBBomb(CBaseBot *pBot):CBehaviour(pBot){strcpy(m_szName,"CBV_CS_GuardWBBomb");}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void initialize(CGoal *);
	virtual void execute(CGoal *);

	void onPathCreation(AStarBase *p);
	void onReachedDestination(CPlayer *);
};

class CBV_CS_PickupWeapon:public CBehaviour,CCallback_onPathCreation,CCallback_onReachedDestination{
public:
	CBV_CS_PickupWeapon(CBaseBot *pBot):CBehaviour(pBot),m_fLastPickup(0){strcpy(m_szName,"CBV_CS_PickupWeapon");}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void execute(CGoal *);
	virtual void initialize(CGoal *);
	void onPathCreation(AStarBase *p);
	void onReachedDestination(CPlayer *);

	int getWeaponValue(int);

	float m_fLastPickup;
	bool m_bDrop;		// should the current weapon be dropped when we arrive at the target ?
};

class CBV_CS_UseHostage:public CBehaviour{
public:
	CBV_CS_UseHostage(CBaseBot *pBot):CBehaviour(pBot),m_fLastPickup(0){strcpy(m_szName,"CBV_CS_UseHostage");}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void execute(CGoal *);

	float m_fLastPickup;
};

class CBV_CS_DefuseBomb:public CBehaviour{
public:
	CBV_CS_DefuseBomb(CBaseBot *pBot):CBehaviour(pBot){strcpy(m_szName,"CBV_CS_DefuseBomb");}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void execute(CGoal *);
};

class CBV_CS_GotoBomb:public CBehaviour,CCallback_onPathCreation{
public:
	CBV_CS_GotoBomb(CBaseBot *pBot):CBehaviour(pBot){strcpy(m_szName,"CBV_CS_GotoBomb");}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void initialize(CGoal *);
	virtual void onPathCreation(AStarBase *);
};

class CBV_CS_LookAtBomb:public CBehaviour{
public:
	CBV_CS_LookAtBomb(CBaseBot *pBot):CBehaviour(pBot){strcpy(m_szName,"CBV_CS_LookAtBomb");}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void execute(CGoal *);
};

class CBVNP_CS_PlantBomb:public CBehaviour{
public:
	CBVNP_CS_PlantBomb(CBaseBot *pBot):CBehaviour(pBot),m_fLastPickup(0){strcpy(m_szName,"CBVNP_CS_PlantBomb");
		percept(CBehaviour::P_NONE);}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void execute(CGoal *);

	float m_fLastPickup;
};

class CBV_CS_WatchTMate:public CBehaviour{
public:
	CBV_CS_WatchTMate(CBaseBot *pBot):CBehaviour(pBot){strcpy(m_szName,"CBV_CS_WatchTMate");}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void execute(CGoal *);
};

#endif __BV_CS_H
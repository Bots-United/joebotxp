#ifndef __BV_HLDM_H
#define __BV_HLDM_H

#include "Behaviour.h"
#include "Emotion.h"
#include "Way.h"
#include "Callback.h"
#include "Perceipt.h"

#include <string.h>

// handling of Squad _and_ team commands
class CBV_HLDM_SquadCommand:public CBehaviour,CCallback_onPathCreation,CCallback_onReachedDestination{
public:
	CBV_HLDM_SquadCommand(CBaseBot *pBot):CBehaviour(pBot){strcpy(m_szName,"CBV_HLDM_SquadCommand");
		percept(CBehaviour::P_Command|CBehaviour::P_NONE);}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void execute(CGoal *);
	virtual void initialize(CGoal *);
	virtual void deinitialize(CGoal *);
	virtual void reset(void);

	virtual void onReachedDestination(CPlayer *);
	virtual void onPathCreation(AStarBase *);

	CPerceipt m_LastCommand;
};

class CBVNP_HLDM_Camp:public CBehaviour{
public:
	CBVNP_HLDM_Camp(CBaseBot *pBot):CBehaviour(pBot),m_fCamp(0)
		{strcpy(m_szName,"CBVNP_HLDM_Camp");
		percept(CBehaviour::P_NONE);}

	virtual void deinitialize(CGoal *);
	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void execute(CGoal *);
	virtual void reset(void){m_fCamp = 0;CBehaviour::reset();}

	void camp(float);				// camp until absolute time
	void campO(float);				// camp for offset time
	bool isCamping(void);			// is the bot already camping ?
	void stopCamping(void);			// stop camping - NOW !

	float m_fCamp;			// store here until when we wanna camp
};

class CBVNP_HLDM_Duck:public CBehaviour{
public:
	CBVNP_HLDM_Duck(CBaseBot *pBot):CBehaviour(pBot){strcpy(m_szName,"CBVNP_HLDM_Duck");
		percept(CBehaviour::P_NONE);}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void execute(CGoal *);
};

class CBVNP_HLDM_ZoomBack:public CBehaviour{
public:
	CBVNP_HLDM_ZoomBack(CBaseBot *pBot):CBehaviour(pBot){strcpy(m_szName,"CBVNP_HLDM_ZoomBack");
		percept(CBehaviour::P_NONE);}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void execute(CGoal *);
};

class CBVNP_HLDM_Jump:public CBehaviour{
public:
	CBVNP_HLDM_Jump(CBaseBot *pBot):CBehaviour(pBot){strcpy(m_szName,"CBVNP_HLDM_Jump");
		percept(CBehaviour::P_NONE);}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void execute(CGoal *);
};

class CBVNP_HLDM_LookAround:public CBehaviour{
public:
	CBVNP_HLDM_LookAround(CBaseBot *pBot):CBehaviour(pBot){strcpy(m_szName,"CBVNP_HLDM_LookAround");
		percept(CBehaviour::P_NONE);}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void execute(CGoal *);
	//virtual void reset(void);
};

class CBVNP_HLDM_AvoidStuck:public CBehaviour{
public:
	CBVNP_HLDM_AvoidStuck(CBaseBot *pBot):CBehaviour(pBot),m_lCStuck(0){strcpy(m_szName,"CBVNP_HLDM_AvoidStuck");
		percept(CBehaviour::P_NONE);}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void execute(CGoal *);

	long m_lCStuck;					// counting how often the bot's stuck
};

// avoid getting stuck while reaching just one waypoint
class CBVNP_HLDM_AvoidStuckWP:public CBehaviour{
public:
	CBVNP_HLDM_AvoidStuckWP(CBaseBot *pBot):CBehaviour(pBot){strcpy(m_szName,"CBVNP_HLDM_AvoidStuckWP");
		percept(CBehaviour::P_NONE);}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void execute(CGoal *);
};

class CBVNP_HLDM_Ladder:public CBehaviour{
public:
	CBVNP_HLDM_Ladder(CBaseBot *pBot):CBehaviour(pBot){strcpy(m_szName,"CBVNP_HLDM_Ladder");
		percept(CBehaviour::P_NONE);}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void execute(CGoal *);
	virtual void deinitialize(void);
};

class CBVNP_HLDM_RunAround:public CBehaviour,CCallback_onPathCreation{
public:
	CBVNP_HLDM_RunAround(CBaseBot *pBot):CBehaviour(pBot){strcpy(m_szName,"CBVNP_HLDM_RunAround");
		percept(CBehaviour::P_NONE);}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void onPathCreation(AStarBase *);
	virtual void initialize(CGoal *);
};

class CBVNP_HLDM_GoCamping:public CBehaviour,CCallback_onPathCreation,CCallback_onReachedDestination{
public:
	CBVNP_HLDM_GoCamping(CBaseBot *pBot):CBehaviour(pBot){strcpy(m_szName,"CBVNP_HLDM_GoCamping");
		percept(CBehaviour::P_NONE);}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void onPathCreation(AStarBase *);
	virtual void initialize(CGoal *);
	virtual void onReachedDestination(CPlayer *);
};

class CBV_HLDM_Run2Sound:public CBehaviour,CCallback_onPathCreation{
public:
	CBV_HLDM_Run2Sound(CBaseBot *pBot):CBehaviour(pBot){strcpy(m_szName,"CBV_HLDM_Run2Sound");}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void onPathCreation(AStarBase *);
	virtual void initialize(CGoal *);
};

class CBVNP_HLDM_RunToHealthWP:public CBehaviour,CCallback_onPathCreation{
public:
	CBVNP_HLDM_RunToHealthWP(CBaseBot *pBot):CBehaviour(pBot){strcpy(m_szName,"CBVNP_HLDM_RunToHealthWP");
		percept(CBehaviour::P_NONE);}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void onPathCreation(AStarBase *);
	virtual void initialize(CGoal *);
};

class CBV_HLDM_RunToWeapon:public CBehaviour,CCallback_onPathCreation{
public:
	CBV_HLDM_RunToWeapon(CBaseBot *pBot):CBehaviour(pBot){strcpy(m_szName,"CBV_HLDM_RunToWeapon");}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void onPathCreation(AStarBase *);
	virtual void initialize(CGoal *);
};

class CBV_HLDM_CheckKill:public CBehaviour{
public:
	CBV_HLDM_CheckKill(CBaseBot *pBot):CBehaviour(pBot){strcpy(m_szName,"CBV_HLDM_CheckKill");}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void execute(CGoal *);
};

class CBV_HLDM_RunToHealth:public CBehaviour,CCallback_onPathCreation{
public:
	CBV_HLDM_RunToHealth(CBaseBot *pBot):CBehaviour(pBot){strcpy(m_szName,"CBV_HLDM_RunToHealth");}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void onPathCreation(AStarBase *);
	virtual void initialize(CGoal *);
};

class CBV_HLDM_RunToWeaponbox:public CBehaviour,CCallback_onPathCreation{
public:
	CBV_HLDM_RunToWeaponbox(CBaseBot *pBot):CBehaviour(pBot){strcpy(m_szName,"CBV_HLDM_RunToWeaponbox");}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void onPathCreation(AStarBase *);
	virtual void initialize(CGoal *);

	Vector VWBox;
};

class CBV_HLDM_AdjustLadder:public CBehaviour{
public:
	CBV_HLDM_AdjustLadder(CBaseBot *pBot):CBehaviour(pBot){strcpy(m_szName,"CBV_HLDM_AdjustLadder");}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void execute(CGoal *);
};

class CBV_HLDM_WatchEnemy:public CBehaviour{
public:
	CBV_HLDM_WatchEnemy(CBaseBot *pBot):CBehaviour(pBot){strcpy(m_szName,"CBV_HLDM_WatchEnemy");}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void execute(CGoal *);
};

class CBV_HLDM_ShootEnemy:public CBehaviour{
public:
	CBV_HLDM_ShootEnemy(CBaseBot *pBot):CBehaviour(pBot),
		m_fLastWeaponCheck(0),
		m_fLastEnemyPerc(0){strcpy(m_szName,"CBV_HLDM_ShootEnemy");}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void execute(CGoal *);

	float m_fLastEnemyPerc;
	float m_fLastWeaponCheck;
};

class CBVNP_HLDM_ShootObstacle:public CBehaviour{
public:
	CBVNP_HLDM_ShootObstacle(CBaseBot *pBot):CBehaviour(pBot){strcpy(m_szName,"CBVNP_HLDM_ShootObstacle");}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void execute(CGoal *);
};

class CBV_HLDM_WatchDamage:public CBehaviour{
public:
	CBV_HLDM_WatchDamage(CBaseBot *pBot):CBehaviour(pBot){strcpy(m_szName,"CBV_HLDM_WatchDamage");}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void execute(CGoal *);
};

class CBV_HLDM_Hear:public CBehaviour{
public:
	CBV_HLDM_Hear(CBaseBot *pBot):CBehaviour(pBot){strcpy(m_szName,"CBV_HLDM_Hear");}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void execute(CGoal *);
};

class CBVNP_HLDM_AvoidTK:public CBehaviour{
public:
	CBVNP_HLDM_AvoidTK(CBaseBot *pBot):CBehaviour(pBot){
		strcpy(m_szName,"CBVNP_HLDM_AvoidTK");
		percept(CBehaviour::P_NONE);}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void execute(CGoal *);
};

class CBV_HLDM_ExplGrenade:public CBehaviour{		// check if there is the possibility to throw an explosive grenade
public:
	CBV_HLDM_ExplGrenade(CBaseBot *pBot):CBehaviour(pBot){strcpy(m_szName,"CBV_HLDM_ExplGrenade");}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void execute(CGoal *);

	virtual void reset(void){m_fLastCheck = 0;CBehaviour::reset();}

	float m_fLastCheck;
};

class CBV_HLDM_SuppFire:public CBehaviour{			// determine if there is the need to create some PTX_ACTION_SUPPFIRE
public:
	CBV_HLDM_SuppFire(CBaseBot *pBot):CBehaviour(pBot){strcpy(m_szName,"CBV_HLDM_SuppFire");}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void execute(CGoal *);

	virtual void reset(void){m_fLastCheck = 0;CBehaviour::reset();}

	float m_fLastCheck;
};

class CBV_HLDM_SuppFireAc:public CBehaviour{			// shoot if there is an PTX_ACTION_SUPPFIRE
public:
	CBV_HLDM_SuppFireAc(CBaseBot *pBot):CBehaviour(pBot){strcpy(m_szName,"CBV_HLDM_SuppFireAc");}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void execute(CGoal *);
};

class CBVNP_HLDM_MovePath:public CBehaviour{
public:
	CBVNP_HLDM_MovePath(CBaseBot *pBot):CBehaviour(pBot){strcpy(m_szName,"CBVNP_HLDM_MovePath");
		percept(CBehaviour::P_NONE);}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void execute(CGoal *);
	virtual void getResult(AStarBase *p=0,CCallback_onReachedDestination *pDReached=0);
	virtual void reset(void);
	virtual void initPath(void);
	bool hasWay(void){return m_CurrWay.size()>0;}

	CCallback_onReachedDestination *m_pDReached;

	int m_iState;		// are currently running towards CurrWP (0) or do we wanna go to m_VGoal (1)
	CWay m_CurrWay;

	bool m_bMove2Origin;	// if the bot should not only follow this path to the end, but also go to VGoal
	Vector m_VGoal;			// vector to move to

	int m_iCurrWP;
	int m_iLastWP;
	int m_iTargetWP;
	float m_fLastDist;
	float m_fLastVisitedWP;
	bool m_bSetTarget;
};

class CBVNP_HLDM_Explore:public CBehaviour,CCallback_onPathCreation{
public:
	CBVNP_HLDM_Explore(CBaseBot *pBot):CBehaviour(pBot),m_fLastCheck(0),m_iChecks(10)
		{strcpy(m_szName,"CBVNP_HLDM_Explore");
		percept(CBehaviour::P_NONE);}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void execute(CGoal *);
	//virtual void initialize(CGoal *);
	virtual void onPathCreation(AStarBase *);

	float m_fLastCheck;
	int m_iChecks;
};

class CEM_HLDM_LastEnemy:public CEmotion{
public:
	CEM_HLDM_LastEnemy(CBaseBot *pBot):CEmotion(pBot){strcpy(m_szName,"CEM_HLDM_LastEnemy");}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void preprocess(void);
	virtual void postprocess(void);

	float m_fTime;
	Vector m_VOrigin;
};

class CEM_HLDM_Team:public CEmotion{
public:
	CEM_HLDM_Team(CBaseBot *pBot):CEmotion(pBot){strcpy(m_szName,"CEM_HLDM_Team");}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void preprocess(void);
	virtual void postprocess(void);

	int m_iCount;
	Vector m_VAvPos;
	Vector m_VAvVel;
	int m_iVisCount;
	Vector m_VVisAvPos;
	Vector m_VVisAvVel;
};

class CEM_HLDM_Enemy:public CEM_HLDM_Team{
public:
	CEM_HLDM_Enemy(CBaseBot *pBot):CEM_HLDM_Team(pBot){strcpy(m_szName,"CEM_HLDM_Enemy");}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
};

class CBV_HLDM_AttackDirect:public CBehaviour,CCallback_onPathCreation{
public:
	CBV_HLDM_AttackDirect(CBaseBot *pBot):CBehaviour(pBot){strcpy(m_szName,"CBV_HLDM_AttackDirect");}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void execute(CGoal *);
	virtual void onPathCreation(AStarBase *);
	virtual void initialize(CGoal *);
};

class CBV_HLDM_AttackIndirect:public CBehaviour,CCallback_onPathCreation{
public:
	CBV_HLDM_AttackIndirect(CBaseBot *pBot):CBehaviour(pBot){strcpy(m_szName,"CBV_HLDM_AttackIndirect");}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	//virtual void execute(CGoal *);
	virtual void onPathCreation(AStarBase *);
	virtual void initialize(CGoal *);
};

class CBV_HLDM_Pullback:public CBehaviour,CCallback_onPathCreation{
public:
	CBV_HLDM_Pullback(CBaseBot *pBot):CBehaviour(pBot){strcpy(m_szName,"CBV_HLDM_Pullback");}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void onPathCreation(AStarBase *);
	virtual void initialize(CGoal *);
};

class CBV_HLDM_HideReload:public CBehaviour,CCallback_onPathCreation,CCallback_onReachedDestination{
public:
	CBV_HLDM_HideReload(CBaseBot *pBot):CBehaviour(pBot){strcpy(m_szName,"CBV_HLDM_HideReload");}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	//virtual void execute(CGoal *);
	virtual void onPathCreation(AStarBase *);
	virtual void initialize(CGoal *);
	virtual void onReachedDestination(CPlayer *);
};

class CBVLT_HLDM_AttackFollow:public CBehaviour,CCallback_onPathCreation{
public:
	CBVLT_HLDM_AttackFollow(CBaseBot *pBot):CBehaviour(pBot){strcpy(m_szName,"CBVLT_HLDM_AttackFollow");
		percept(CBehaviour::P_LTMem);}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void execute(CGoal *);
	virtual void onPathCreation(AStarBase *);
	virtual void initialize(CGoal *);
};

class CBVLT_HLDM_AttackSnipe:public CBehaviour,CCallback_onPathCreation,CCallback_onReachedDestination{
public:
	CBVLT_HLDM_AttackSnipe(CBaseBot *pBot):CBehaviour(pBot){strcpy(m_szName,"CBVLT_HLDM_AttackSnipe");
		percept(CBehaviour::P_LTMem);}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void execute(CGoal *);
	virtual void onPathCreation(AStarBase *);
	virtual void initialize(CGoal *);
	virtual void onReachedDestination(CPlayer *);
};

#endif

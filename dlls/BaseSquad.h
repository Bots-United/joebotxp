#ifndef __BASESQUAD_H
#define __BASESQUAD_H

#include "PerceptionSquad.h"
#include "GoalFinder.h"
#include "Callback.h"
#include "AStarMachine.h"
#include "AStarGoalMod.h"
#include "Callback.h"
#include "Permission.h"

#include <algorithm>
#include <vector>
using namespace std;

class CBaseTeam;
class CBaseBot;
class CSBehaviour;

// permissions to be handled on squad level
enum squad_permissions{
};

class CBaseSquadMember{
public:
	CBaseSquadMember(){
		m_iPlayerIndex = -1;
		m_iGoalWP = -1;
		m_lCommandFlag=0;
		m_iOldGoalWP = -1;
		m_pOnReachedDestination = 0;
		m_pAStarMachine = 0;
		m_fCommandSent = -1;
	}
	CBaseSquadMember(int iPlayer,int iGoalWP=-1,long lCommandFlag =0){
		m_iPlayerIndex = iPlayer;
		m_iGoalWP = iGoalWP;
		m_lCommandFlag = lCommandFlag;
		m_iOldGoalWP = -1;
		m_pOnReachedDestination = 0;
		m_pAStarMachine = 0;
		m_fCommandSent = -1;
	}
	
	int m_iPlayerIndex;			// index of player
	int m_iOldGoalWP;			// the waypoint we were running to last time
	int m_iGoalWP;				// if you wanna go the bot to a new wp, set this variable
	long m_lCommandFlag;		// flag for GOTODEF command
	float m_fCommandSent;		// time when a command to go to some place was sent
	AStarBase *m_pAStarMachine;	// pointer to AStarMachine
	CCallback_onReachedDestination *m_pOnReachedDestination;	// do we need some OnReachedDestination callback ?
	CWay m_WSquadWay;

	enum FLAGS{
		SF_DEFENSIVE = 1<<1,
		SF_AGGRESSIVE = 1<<2,
	};
};

class CBaseSquad:public CCallback_onPathCreation,public CPermission
{
	friend class CBaseTeam;
public:
	CBaseSquad(void);
	virtual ~CBaseSquad(void);

	virtual void onRoundStart(void);		// round start, i.e. you gotta create new squads
	virtual void think(void);
	virtual void getMembers(void);

	void calcMidPos(void);

	int hasWeapon(long);			// has somebody this weapon ? ( bitflags, not index )

	int isMember(int);
	virtual bool addMember(int);
	virtual bool removeMember(int);
	int countMembersAlive(void);
	CPlayer *getMemberAlive(void);

	virtual void tellIncomingMessage(CBaseBot *pBot,int iPlayerIndex, int iType, int iOtherPlayerIndex=-1, int iNWP=-1);

	/* pathfinding - coordination */

	void thinkPaths(void);		// look if we gotta find a new path
	virtual void onPathCreation(AStarBase *p);

	float m_fPenaltyforSameWay;	// the penalty for running the same way like a squadmate
	bool m_bASMinWork;			// are we waiting for a AStarMachine ?

	/* end : pathfinding - coordination */

	vector<CBaseSquadMember> m_LMembers;
	int m_iSquadID;						// ID of this squad
	int m_iMembers;						// how many members should we have ?
	CBaseTeam *m_pTeam;					// to which team do we belong ?

	Vector m_VMiddle;		// VMiddle of squad
	float m_fSigmaDist;		// sigma of distance - if this is below 300-400 we can say the squad is together

	float m_fNextThink;		// when has this squad to think the next time ?
	float m_fThinkPer;		// how long is the period between 'thinking'

	CPerception *m_pPerception;		// our perception system
	CGoalFinder m_LGoals;						// goals 'produced' by behaviours
	list<CSBehaviour*>	m_LpBehaviour;			// behaviours
};

#endif //__BASESQUAD_H


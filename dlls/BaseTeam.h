// BaseTeam.h: interface for the CBaseTeam class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__CBaseTeam_H)
#define __CBaseTeam_H

#include "Waypoint.h"
#include "Radio.h"
#include "BaseSquad.h"
#include "Permission.h"

#include <vector>
#include <list>
using namespace std;

enum team_permissions{			// enum of permissions requestable from team
	TP_PICKUPWEAPON = 0,
	TP_THROWGRENADE
};

class CBaseBot;

class CBaseTeam : public CPermission
{
public:
	CBaseTeam();
	virtual ~CBaseTeam();

	virtual void think(void);					// "think"
	virtual void reset(void);					// reset
	virtual void onRoundStart(void);			// call on round start
	virtual void createSquads(void);			// create some squads
	virtual CBaseSquad *allocSquad(void);		// allocate a squad
	virtual bool addMe2Squad(int);				// add a player to squad
	virtual bool addMe2BiggestSquad(int);		// add a player to the biggest squad

	int countMembersAlive();					// who many members of this team are still alive ?

	virtual bool removeMember(int);				// remove a member

	// squad management
	CBaseSquad *getSquad(int);					// return the pointer to a squad
	vector<CBaseSquad *> m_LpSquads;			// list of squads

	// team messages
	virtual void processMessage(CBaseBot *pBot,int iPlayerIndex, int iType, int iOtherPlayerIndex=-1, int iNWP=-1);

	// those messages are mainly those already implemented as radio commands in CS
	void tellEnemySpotted(CBaseBot *pBot, int iEnemyPlayerIndex, int iEnemyNWP);
	void tellEnemyDown(CBaseBot *pBot, int iEnemyPlayerIndex, int iEnemyNWP);
	void tellSniperSpotted(CBaseBot *pBot, int iEnemyPlayerIndex, int iEnemyNWP);
	void tellSectorClear(CBaseBot *pBot);
	void tellGoGoGo(CBaseBot *pBot);
	void tellCoverMe(CBaseBot *pBot, int iDestination);
	void tellInPosition(CBaseBot *pBot, int iPositionWP);
	void tellNeedBackup(CBaseBot *pBot, int iPositionWP);
	void tellFallBack(CBaseBot *pBot);
	void tellStickTogether(CBaseBot *pBot);
	void tellGetInPosition(CBaseBot *pBot);
	void tellAffirmative(CBaseBot *pBot);
	void tellNegative(CBaseBot *pBot);
	void tellDied(CBaseBot *pBot);
	void tellRespawned(CBaseBot *pBot);
	void tellEngagingEnemy(CBaseBot *pBot,int iEnemyPlayerIndex,int iEnemyNWP);

	// misc other commands
	void tellPickupWeapon(CBaseBot *pBot);		// we wanna pick up a weapon somewhere ... well, let's tell about the team, otherwise they'll try to do the same and things will start getting fucked up

	float m_pfSeen[_MAX_WAYPOINTS];				// last time we seen this waypoint ( as a team )
	float m_ppfLastMessageSent[32][RC_MAX];		// last time this message has been sent by player
	float m_pfTime4Message[RC_MAX];				// minimum time difference in between 2 radio commands of one type
	float m_pfRadioUsed[32];					// last time a bot used the radio

	int m_iTeamID;				// ID of this team

	list <CPlayer *> m_LpPlayersNotAssigned;

	// no need for a perception system here since the perceipts of the squads are gonna used
	float m_fNextBThink;		// when has this team to think the next time about redistributing behaviours ? should normally be each 10s, but maybe squads should also be able to engage such a process by themselves ?
	float m_fNextThink;		// normal 'thinking'
	CGoalFinder m_LGoals;						// goals 'produced' by behaviours
	list <CSBehaviour *> m_LpSquadBehaviours;	// list of behaviours

	float m_fThinkPer;
	float m_fBThinkPer;
};

#endif // !defined(__CBaseTeam_H)

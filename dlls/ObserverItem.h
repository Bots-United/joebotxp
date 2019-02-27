// ObserverItem.h: interface for the CObserverItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__OBSERVERITEM_H)
#define __OBSERVERITEM_H

#include "Player.h"

#define OI_LocRecLength 40

#define PLAYER_FATAL_FALL_SPEED 1024		// approx 60 feet
#define PLAYER_MAX_SAFE_FALL_SPEED 580		// approx 20 feet 

class CObserverItemRec{
public:
	CObserverItemRec(){}
	~CObserverItemRec(){}

	Vector m_VOrigin;		// origin
	float m_fHeight;		// heigt above ground
};

class CObserverItem  
{
public:
	CObserverItem();
	virtual ~CObserverItem();

	void observe(CPlayer *);
	void reset(void);

	int  placeWaypoint(Vector,long lFlags = 0);
	void addNearPath(void);

	bool preProcess(void);			// do some stuff which may be need by other functions, determining height, nearest wp , ...
	void record(void);				// reocrd locations
	bool checkDistances(void);		// check if it's far enough away from other waypoints to place a waypoint
	bool checkLadders(void);		// check for ladders
	bool checkCorners(void);		// check if you are running around a corner
	bool postProcess(void);			// do some cleanup

	bool checkRec4Connection(int,int);	// check if a unreachable path can be avoided by placing a waypoint somewhere in between, therewith going aside the problematic location. this is based on the recorded data

	bool m_bFCheck;					// full ckeck ?!

	edict_t *m_pEntity;
	CPlayer *m_pPlayer;

	int m_iLastAddedWP;				// last added wp
	int m_iNearestWP;				// nearest waypoints
	float m_fNearestDistance;		// distance

	int m_iLastNearWP;				// last near wp, i.e. the last waypoint within 50 units
	int m_iLastLastNearWP;			// the near wp before this one
	float m_fLastNearDistance;		// distance

	bool m_bOnLadder;				// is player on ladder ?

	Vector m_VLastOrigin;			// origin when last checkin all that stuff

	CObserverItemRec m_pRec[OI_LocRecLength];	// recorded locations
	int m_iRecItems;				// number of recorded items

	float m_fHeight;		// height above ground
};

#endif // !defined(__OBSERVERITEM_H)


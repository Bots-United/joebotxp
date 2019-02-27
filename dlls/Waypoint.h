// CWaypoint.h: interface for the CWaypoint classes
//
//////////////////////////////////////////////////////////////////////

#if !defined(__CWaypoint_H)
#define __CWaypoint_H

#include "extdll.h"

#include "BitField.h"

#include <assert.h>

#define WP_VERSION 6

#define _MAX_WAYPOINTS	4096
#define _MAX_PATHS		8

struct WPHeader_t{
	char szText[32];
	char szMapname[32];
	long lVersion;
};

#include "Path.h"
#include "WPStat.h"

class CWaypoint:public CPath,public CWPStatItem
{
public:
	CWaypoint();
	virtual ~CWaypoint();

	void resetPaths(void);

	bool drawTo(edict_t *);

	void resetStat(void){CPath::resetStat();CWPStatItem::resetStat();}

	void addVisibleWP(CWaypoint *);

	virtual bool removeLastConnection(void);	// removes the last connection
	virtual bool removeConnectionTo(int);		// removes a connection to a certain waypoint

	void init();

	void save(FILE *);
	void saveXML(FILE *,int);
	void load(FILE *);

	void copyStat(const CWaypoint &);

	Vector m_VOrigin;
	long m_lFlags;

	int m_iVisibleWaypoints;
	float m_fAvDistVWP;			// average distance to visible waypoints

	float m_fDisplayNext;		// last time this CWaypoint has been displayed

	enum Types{
		DELETED		= (1<<31),

		TEAM		= ((1<<0) + (1<<1)),
		TEAM_SPEC	= (1<<2),
		HEALTH		= (1<<3),
		WEAPON		= (1<<4),
		AMMO		= (1<<5),
		ARMOR		= (1<<6),
		LADDER		= (1<<7),
		CROUCH		= (1<<8),
		FLAG		= (1<<9),
		FLAG_GOAL	= (1<<10),
		MIDAIR		= (1<<11)
	};
};

#define UD_WAYPOINT_CHANGED 1

#include "WaypointGraph.h"

#endif // !defined(__CWaypoint_H)

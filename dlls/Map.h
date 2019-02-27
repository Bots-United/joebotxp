// CMap.h: interface for the CMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__MAP_H)
#define __MAP_H

#include "Waypoint.h"
#include "WPStat.h"

class CMap  
{
public:
	CMap();
	virtual ~CMap();

	void reset(void);								// reset all the map related data
	long createBasicWaypoints(void);				// create basic waypoints liek ladder wps etc.

	static bool getReachable(const Vector &,const Vector &);// are two locatiosn reachable ?

	bool load(const char *szPCMapname = 0);			// load all the map related data
	bool save(const char *szPCMapname = 0);			// save all the map related data

	CWaypointGraph m_Waypoints;						// my favorite data, the waypoints
	//CWPStat m_WPStat;								// stat data
};

extern CMap g_Map;

#endif // !defined(__MAP_H)

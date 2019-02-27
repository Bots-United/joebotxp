#ifndef __WAYPOINT_GRAPH
#define __WAYPOINT_GRAPH

#include <list>
#include <vector>
using namespace std;

class CWaypointGraph{
public:
	CWaypointGraph();
	virtual ~CWaypointGraph();

	virtual void update(int, int);
	void reset(void);

	int add(const Vector &);
	void remove(int);
	bool connect(int, int);
	bool disconnect(int, int);
	void drawTo(edict_t *);
	void checkAutoPath(int);

	bool getVisible(int,int);
	void vis_recalc(int);

	int getHashcode(const Vector &V){
		int ix = (((int)V.x + 4096) & 0x007F80) >> 7; 
		int iy = (((int)V.y + 4096) & 0x007F80) >> 1; 
		return ix + iy; 
	}
	void addToHashTable(int);
	bool removeFromHashtable(int);

	int getNearest(const Vector &,bool bVisible = false, bool bReachable = false, float fMin = 0, float fMax = 100000,long lFlags = 0);		// finds nearest waypoint using hash table ( for info about the limitations, see the def f this func )
	int getNearesto(const Vector &,bool bVisible = false, bool bReachable = false, float fMin = 0, float fMax = 100000,long lFlags = 0);	// finds nearest waypoint without using the hash table
	int getNearestB(const Vector &,CBitField *, bool bVisible = false, bool bReachable = false, float fMin = 0, float fMax = 100000,long lFlags = 0);	// finds nearest waypoint without using the hash table

	int getMaximum(CBitField *, int , float fFactor = 1, float fOffset = 0);

	void setVisibleMask(CBitField *,int);
	void broadenMask(CBitField *,int iDepth = 1);
	void setDistanceMask(CBitField *pMask,float fFactor, float fBDistance, Vector &VOrigin);

	virtual bool save(const char *);
	virtual bool saveXML(const char *);
	virtual bool load(const char *);

	bool isValid(int iWP){
		return ( (iWP >= 0) && ( iWP < m_iNumWaypoints ) );
	}

	CWaypoint &operator[](long lIndex){
		assert(lIndex >= 0 && lIndex < _MAX_WAYPOINTS);
		return ((CWaypoint &)(m_pWaypoints[lIndex]));
	}

	// waypoint statistic data member functions

	void addDamage(Vector &,Vector &,int);
	void addKill(Vector &);
	void addTraffic(int);
	void resetStat(void);

	// data

	CWaypoint m_pWaypoints[_MAX_WAYPOINTS];
	int m_iNumWaypoints;

	CBitField *m_ppVisTable[_MAX_WAYPOINTS];
	float m_fMaxDistance2Display;

	// display options
	bool m_bDisplay;
	bool m_bDisplayWaypoints;
	bool m_bDisplayPaths;
	vector<int> m_pLHash[4096];			// hashtable buckets for fast nearest waypoint lookup

	enum WG{
		WG_SEARCH_AVDISTVWP=0,	// Average Distance to visible waypoints
		WG_SEARCH_IVWP,			// Number of visible waypoints
		WG_SEARCH_KILLTIME,		// time of a kill
		WG_SEARCH_DAMAGE,		// damage done from waypoint
		WG_SEARCH_DAMAGED,		// damage done to waypoint
		WG_SEARCH_TRAFFIC		// traffic here
	};

	// statistic data
	long m_lMaxTotalDamage;
};

#endif


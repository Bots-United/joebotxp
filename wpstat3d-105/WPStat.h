// CWPStat.h: interface for the CWPStat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__CWPStat_H)
#define __CWPStat_H

#include "extdll.h"

#define _CWPSTATITEM_KILLTIMESTORAGE 4

class CWPStatItem{
public:
	CWPStatItem();
	virtual ~CWPStatItem();

	void resetStat(void);
	void addKill(void);
	void addDamage(Vector2D &,int);			// if this waypoint is the origin for 'damage'
	void addDamaged(Vector2D &,int);		// if this waypoint is 'damaged'

	virtual void save(FILE *);
	virtual void load(FILE *);

	// in game collected data
	long m_plDDamage[8];			// directed damage ( from E/NE/N/NW/W/SW/S/SE )
	long m_lTotalDamage;			// total damage

	long m_plDDamaged[8];			// directed damaged by ( from E/NE/N/NW/W/SW/S/SE )
	long m_lTotalDamaged;			// total damaged

	int m_iNKill;
	float m_pfKillTime[_CWPSTATITEM_KILLTIMESTORAGE];		// time of kill
	long m_lTraffic;				// traffic at this wp

	// terrain analysis data
	// misc
protected:
	static float getAngle(const Vector2D &);
};

#define _WPSTAT_VERSION 5

/*class CWPStat  
{
public:
	CWPStat();
	virtual ~CWPStat();

	void reset(void);

	bool save(const char *);
	bool load(const char *);

	virtual void update(int,int);

	void addDamage(Vector &,Vector &,int);
	void addKill(Vector &);
	void addTraffic(int);

	CWPStatItem &operator[](long);

	CWPStatItem m_pSData[_MAX_WAYPOINTS];
	long m_lMaxTotalDamage;
};*/

#endif // !defined(__CWPStat_H)

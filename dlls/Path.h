#ifndef __PATH_H
#define __PATH_H

#include "stdio.h"

#include "PathStat.h"

class CPath{
	friend class CWaypoint;
	friend class CPath_iterator;
public:
	CPath();
	virtual ~CPath();

	bool addConnectionTo(int,CPathStat *pPStat = 0);			// adds a connection to a certain waypoint
	bool isConnectedTo(int);			// returns true if this is connected to that waypoint
	int getConnectionCount(void);		// returns the number of connections
	int getConnection(int);				// returns the waypoint index of a certain path pointing to

	void resetStat(void);

	//virtual bool removeLastConnection(void);	// removes the last connection
	//virtual bool removeConnectionTo(int);		// removes a connection to a certain waypoint
	virtual int getLastConnection(void);		// returns the index of the waypoint the last path is connecting to

	int increasedFailedEntry(int);

	void load(FILE *fhd);				// loads this paths
	void save(FILE *fhd);				// saves this paths ( just the count, then saveCPaths is called )
	void saveXML(FILE *fhd);
	void savePathsXML(FILE *);
	void savePaths(FILE *);				// saves this paths
	void reset(void);					// remvoe all paths

	int *getFailedEntry(int);

	int m_piPathTo[_MAX_PATHS];
	CPath *m_pNext;

	CPathStat m_pStats[_MAX_PATHS];
};

class CPath_iterator{
public:
	CPath_iterator();
	~CPath_iterator();

	CPath_iterator & operator =(CPath &PPath){
		m_pPath = & PPath;
		m_iIndex = 0;

		return ((CPath_iterator &)(*this));
	}
	CPath_iterator & operator ++(){
		if(m_iIndex < _MAX_PATHS - 1){
			m_iIndex ++;
		}
		else{
			if(m_pPath)
				m_pPath = m_pPath->m_pNext;
			m_iIndex = 0;
		}
		return ((CPath_iterator &)(*this));
	}
	int getTo(void)const{
		if(m_pPath)
			return m_pPath->m_piPathTo[m_iIndex];
		else
			return -1;
	}
	const CPathStat *getStat(void)const{
		if(m_pPath)
			return &(m_pPath->m_pStats[m_iIndex]);
		else
			return 0;
	}

	CPath *m_pPath;
	int m_iIndex;
};

#endif


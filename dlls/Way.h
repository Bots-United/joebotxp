// Way.h: interface for the CWay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__WAY_H)
#define __WAY_H

#include "Waypoint.h"
#include "BitField.h"

class CWay  
{
	friend class AStarBase;
public:
	CWay();
	virtual ~CWay();

	int find(int);				// return the index of a given waypoint in this way. -1 if unsucessful
	int getLast(void);			// get last waypoint before the current one
	int getCurrent(void);		// get current waypoint index
	int advance(void);			// advance to next waypoint and return current index
	int getOffset(int);			// get a waypoint at a given offset to m_iActCount - returns -1 if invalid
	int getStart(void){return m_piIndices[0];}
	int getDestination(void);	// get the last waypoint in this way
	float calcLength(CWaypointGraph *pGraph,int iOffset = 0);	// just calculate the 
	void reset(void){m_iCount = 0;m_iActCount=0;}		// reset the way ... no entries, no current wp

	void setMask(CWaypointGraph *pGraph,CBitField *pBitField,bool bBroad = false,bool bFull=true);			// set all bits according to the waypoints of this way - if broad is true, then also set bits of the waypoints connected to the way
	bool isPosInWay(CWaypointGraph *pGraph,const Vector &,int,const float fDot = .6f);	// check if a location is within the cones around the 'bones' of the way, defined by the dot product specified as parameter

	int operator[](long lIndex){assert(lIndex < m_iCount && lIndex > -1);return m_piIndices[lIndex];}
	int size(void){return m_iCount;}
	int left(void){return m_iCount - m_iActCount-1;}	// returns the number of waypoints left in this path
	int getCurrentIndex(void){return m_iActCount;}		// returns the index for the waypoint index field of this way
//protected:
	int m_piIndices[_MAX_WAYPOINTS];
	int m_iCount;
	int m_iActCount;
};

#endif // !defined(__WAY_H)

// AStarBase.h: interface for the AStarBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__ASTARBASE_H)
#define __ASTARBASE_H

#include <stack>		// stl stack class
using namespace std;

#include "PQueue.h"
#include "PQueueHash.h"
#include "AStarNodeContainer.h"
#include "Way.h"
#include "MemoryPool.h"

//#define _LOGRESULTS

extern CMemoryPool<AStarNode,256> g_ASN_MPool;

class AStarBase : public stack<AStarNode *>
{
public:
	AStarBase();
	virtual ~AStarBase();

	virtual bool runAStar(void) = 0;

	AStarNode *newNode(int iWaypoint = -1){
		//m_pLastNode->m_pNext = new AStarNode(iWaypoint);
		m_pLastNode->m_pNext = g_ASN_MPool.get();
		m_pLastNode->m_pNext->m_iWaypoint = iWaypoint;
		m_pLastNode->m_pNext->m_Children.reset();
		m_pLastNode->m_pNext->m_pNext = 0;
		m_pLastNode->m_pNext->m_pParent = 0;
		
		m_pLastNode = m_pLastNode->m_pNext;
		
		return m_pLastNode;
	}
	void deleteNode(AStarNode *p){
		g_ASN_MPool.free(p);
	}
	virtual void resetNodes();
	virtual void setStart(int);
	virtual void setDestination(int);
	virtual void logResult(void);
	virtual void getResult(CWay &);
	virtual void setMaxRevolutions(long);

	CPQueueHash m_PQOpen;		// Priority with hashtable for the open list
	CPQueueHash m_PQClosed;		// ... and for the closed list

	AStarNode *m_pDestinationNode;	// when reached destination, this pointer points to the related node
	AStarNode *m_pLastNode;		// last node added
	AStarNode *m_pFirstNode;	// first node added

	int m_iStart;				// waypoint index of start
	int m_iDestination;			// waypoint index of destination
	long m_lRevolution;			// number of revolutions of the main loop
	long m_lMaxRevolutions;		// max number of revolutions of the main loop

	long m_lSliceSize;			// size of slice to process
	long m_lCSlice2;			// current slice is till revolution
	bool m_bFinished;				// is there still some slice to calculate ?!

	int m_iBreakReason;			// reason of breaking the main loop
	enum break_reason{
		BR_NONE = 0,
		BR_NOOPEN = 0,
		BR_MAXREV = 1,
		BR_MAXSLICE
	};
};

#endif // !defined(__ASTARBASE_H)

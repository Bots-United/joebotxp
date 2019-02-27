// AStarBase.h: interface for the AStarBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__ASTARBASE_H)
#define __ASTARBASE_H

#include "PQueueHash.h"
#include "AStarNodeContainer.h"
#include "Way.h"
#include "MemoryPool.h"

//#define _LOGRESULTS

extern CMemoryPool<AStarNode,2048> g_ASN_MPool;
class AStarBaseGoal;

class AStarBase{
public:
	AStarBase();
	virtual ~AStarBase();

	virtual bool runAStar(void) = 0;

	virtual AStarBaseGoal *getGoal(void) = 0;

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
	virtual void setStart(int)			=0;
	virtual void setDestination(int)	=0;
	virtual int getStart(void)			=0;
	virtual int getDestination(void)	=0;
	virtual void resetNodes();
	virtual void logResult(void);
	virtual void getResult(CWay &);
	virtual void getPartialResult(CWay &);		// not yet implementated
	virtual void setMaxRevolutions(long);

	void setSliceSize(long lSliceSize){
		m_lSliceSize = lSliceSize;
	}
	bool finished(void){
		return m_bFinished;
	}

	void drawTree(void);
	void drawTreeR(int,AStarNode *);

	CPQueueHash m_PQOpen;		// Priority with hashtable for the open list
	CPQueueHash m_PQClosed;		// ... and for the closed list

	AStarNode *m_pDestinationNode;	// when reached destination, this pointer points to the related node
	AStarNode *m_pLastNode;		// last node added
	AStarNode *m_pFirstNode;	// first node added

	long m_lRevolution;			// number of revolutions of the main loop
	long m_lMaxRevolutions;		// max number of revolutions of the main loop
protected:
	long m_lSliceSize;			// size of slice to process
	long m_lCSlice2;			// current slice is till revolution
	int m_iCSlice;				// count slices
	bool m_bFinished;			// is there still some slice to calculate ?!

	int m_iBreakReason;			// reason of breaking the main loop
	enum break_reason{
		BR_NONE = 0,
		BR_NOOPEN = 0,
		BR_MAXREV = 1,
		BR_MAXSLICE
	};
};

#endif // !defined(__ASTARBASE_H)



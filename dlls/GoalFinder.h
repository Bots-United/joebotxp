// GoalFinder.h: interface for the CGoalFinder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__GOALFINDER_H)
#define __GOALFINDER_H

#include "Goal.h"

#include <list>
using namespace std;

class CGoalFinder : public list<CGoal>
{
public:
	CGoalFinder();
	virtual ~CGoalFinder();
	void reset(void);				// general reset
	void resetWinner(void);			// reset the pointer to the winning behaviours
	void getWinner(void);			// determine the winners
	void executeWinner(void);		// execute them
	void finishedMove(void);		// a move has been finished ... reset the LMove pointers

	inline int slots(int);							// how many slots does a behaviour want ?
	inline bool combinable(long,long);				// are 2 goals combinable ?
	inline bool combinable(CGoal*,CGoal*);			// are 2 goals combinable ?
	inline bool combinable(long,long,long);			// are 3 goals combinable ?
	inline bool combinable(CGoal*,CGoal*,CGoal*);	// are 3 goals combinable ?
	
	CBehaviour	*m_pMoveBv,
			*m_pActionBv,
			*m_pLookBv;
	CGoal	*m_pMoveWinner,
			*m_pActionWinner,
			*m_pLookWinner;

	int m_iLMovePriority;				// priority of last moveWP task
	CBehaviour *m_pLMoveBv;			// last moveWP behaviour
};

#endif // !defined(__GOALFINDER_H)

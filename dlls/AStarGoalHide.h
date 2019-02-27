// AStarGoalHide.h: interface for the AStarGoalHide class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__ASTARGOALHIDE_H)
#define __ASTARGOALHIDE_H

#include "AStarGoalMod.h"

class AStarGoalHide : public AStarBaseGoal  
{
public:
	AStarGoalHide();
	virtual ~AStarGoalHide();

	int getHeuristic(int);
	bool isDestination(const AStarNode *);

	void hideFrom(int,const Vector&,const Vector&);
	void setMinimumCost(int);

	bool isValid(void);
protected:
	Vector m_VHideAim;			// point ->infinity (map related infinity :D), the heuristic is based on. basically defined the direction of the search
	int m_iHidefromWP;			// waypoint the destination of this path should hide from
	int m_iMinCost;				// minimum cost to return a destination
};

#endif // !defined(__ASTARGOALHIDE_H)

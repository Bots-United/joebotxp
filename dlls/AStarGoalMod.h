// AStarGoalMod.h: interface for the AStarGoalMod class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__ASTARGOALMOD_H)
#define __ASTARGOALMOD_H

#include "AStarBaseGoal.h"
#include "Map.h"

class AStarGoalMod : public AStarBaseGoal  
{
public:
	AStarGoalMod();
	virtual ~AStarGoalMod();

	void addVisibleCost(int,float);
	void addCost(CBitField &bitfield,float fCost);

	int getCost(int,const CPath_iterator &);

	float m_pfAddCosts[_MAX_WAYPOINTS];		// factors for costs
	float m_fOffsetCosts;					// maybe we need an offset for all cost factors ?
};

#endif // !defined(__ASTARGOALMOD_H)

// AStarGoalCover.h: interface for the AStarGoalCover class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__ASTARGOALCOVER_H)
#define __ASTARGOALCOVER_H

#include "AStarBaseGoal.h"
#include "Map.h"

#include "Line.h"
#include "Cube.h"

extern CObject **Objects;
extern long lObjects;
extern float rD;

class AStarGoalCover:public virtual AStarBaseGoal
{
public:
	AStarGoalCover();
	virtual ~AStarGoalCover();

	void reset(void);

	virtual void addThreadWP(int);

	int getCost(int,int);
	int getDistance(int,int);

	float m_fNCoverValue;					// additional cost when a waypoint is not covered

	float m_pfCoverCosts[_MAX_WAYPOINTS];	// factors for costs
	float m_fOffsetCosts;					// maybe we need an offset for all cost factors ?
};

#endif // !defined(__ASTARGOALCOVER_H)

// AStarGoalPFled.h: interface for the AStarGoalPFled class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__ASTARGOALPFLED_H)
#define __ASTARGOALPFLED_H

#include "AStarGoalMod.h"

class AStarGoalPFled : public AStarGoalMod  
{
public:
	AStarGoalPFled();
	virtual ~AStarGoalPFled();

	void fleeFrom(Vector &,Vector &,CBitField &,float fMinDistance = 300);

	bool isDestination(const AStarNode *);
	int getHeuristic(int);
	bool isValid(void);
	bool isOpen(int,const CPath_iterator &);
protected:
	Vector m_VFleeAim;
	CBitField m_pbPVisited;
	float m_fMinCost;
};

#endif // !defined(__ASTARGOALPFLED_H)

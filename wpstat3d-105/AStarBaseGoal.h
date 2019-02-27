// AStarBaseGoal.h: interface for the AStarBaseGoal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__ASTARBASEGOAL_H)
#define __ASTARBASEGOAL_H

class AStarBaseGoal  
{
public:
	AStarBaseGoal();
	virtual ~AStarBaseGoal();

	int getCost(int,int);
	int getDistance(int,int);
	bool isOpen(int,int);
	void reset(void){};
};

#endif // !defined(__ASTARBASEGOAL_H)

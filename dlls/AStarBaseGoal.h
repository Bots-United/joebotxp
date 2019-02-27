// AStarBaseGoal.h: interface for the AStarBaseGoal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__ASTARBASEGOAL_H)
#define __ASTARBASEGOAL_H

class AStarNode;
class CPath_iterator;

class AStarBaseGoal  
{
public:
	AStarBaseGoal();
	virtual ~AStarBaseGoal();
	
	void setStart(int iWPStart){
		m_iStart = iWPStart;
	}
	void setDestination(int iWPDestination){
		m_iDestination = iWPDestination;
	}
	int getStart(void){
		return m_iStart;
	}
	int getDestination(void){
		return m_iDestination;
	}
	bool isValid(void);		// returns true is this goal is ready to be used

	int getCost(int,const CPath_iterator &);
	int getHeuristic(int);
	bool isOpen(int,const CPath_iterator &);			// if the connection from i1 to i2 is open, let's return true
	bool isDestination(const AStarNode *);	// are we there ?!

	int m_iStart;				// waypoint index of start
	int m_iDestination;			// waypoint index of destination
};

#endif // !defined(__ASTARBASEGOAL_H)


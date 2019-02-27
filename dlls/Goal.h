// Goal.h: interface for the CGoal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__GOAL_H)
#define __GOAL_H

class CBehaviour;
class CPerceipt;

class CGoalOwner{
public:
	virtual bool attachGoal(void) = 0;
};

class CGoal  
{
public:
	CGoal();
	CGoal(int iImportance,int iType,CBehaviour *pBehaviour,CPerceipt *pPerceipt):m_iImportance(iImportance),m_iType(iType),m_pBehaviour(pBehaviour),m_pPerceipt(pPerceipt){}
	virtual ~CGoal();

	int m_iImportance;
	int m_iType;
	enum CGoalTypes{
		GT_MOVE = (1<<0),		// move
		GT_ACTION = (1<<1),		// jump, duck, whatever
		GT_LOOK = (1<<2),		// look somewhere

		GT_ALL = GT_MOVE|GT_ACTION|GT_LOOK
	};
	CPerceipt *m_pPerceipt;
	CBehaviour *m_pBehaviour;	// creating behaviour
	CGoalOwner *m_pGoalOwner;	// owner of this goal ...
};

#endif // !defined(__GOAL_H)

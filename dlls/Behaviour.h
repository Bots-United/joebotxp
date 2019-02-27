// Behaviour.h: interface for the CBehaviour class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__BEHAVIOUR_H)
#define __BEHAVIOUR_H

#include <list>
using namespace std;

class CPerceipt;
class CBaseBot;
class CGoal;
class AStarBase;

class CBehaviour  
{
public:
	CBehaviour(CBaseBot *);
	virtual ~CBehaviour();

	virtual void reset(void);
	char *getName(void){return m_szName;}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void initialize(CGoal *);
	virtual void execute(CGoal *);
	virtual void deinitialize(CGoal *);

	void percept(int i){m_iPercept = i;}

	enum PerceptionTypes{
		P_NONE = 0,
		P_STMem = (1<<0),
		P_LTMem = (1<<1),
		P_Command = (1<<2)
	};
	int m_iPercept;			// what type of perceptions to percept :)
//protected:
	char m_szName[80];			// name of this behaviour ... not really needed, but nice for debugging and stuff
	CBaseBot *m_pBot;			// the bot 'owning' this behaviour

	int m_iExecuteFSMS;						// State of execute FSM
	int m_iEvaluateFSMS;					// State of evaluate FSM

	float m_fLastExecution;					// time of last execution
};

#endif // !defined(__BEHAVIOUR_H)

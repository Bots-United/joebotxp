// Perception.h: interface for the CPerception class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__PERCEPTION_H)
#define __PERCEPTION_H

#include <list>
#include "Perceipt.h"
using namespace std;

class CPerception  
{
public:
	CPerception();
	virtual ~CPerception();

	virtual void run(void) = 0;

	virtual void resetPerceptions(void);		// remove perceptions
	virtual void reset(void);					// reset all

	virtual void merge(void);						// merge new and old perceptions
	virtual void merge2LTMem(void);
	virtual void process(void);						// look if some old perceptions should be thrown out
	virtual void newLPerception(CPerceipt*);
	virtual CPerceipt *addPerceipt(long lType = 0,long lSpec = 0);
	virtual CPerceipt *addCommand(long lSpec = 0);

	void removePerception(CEntity);

	list <CPerceipt*> m_LPerceptions;
	list <CPerceipt*> m_LNewPerceptions;
	list <CPerceipt*> m_LLTMem;
	list <CPerceipt*> m_LCommands;

	//float m_fTime2Del;				// time till a non updated perception item gets deleted
	float m_fTime2DelLT;				// time till a non updated perception item gets deleted in LTPerc
	float m_fLastRun;					// last time the perception system was run
	float m_fReactionTime;
protected:
	virtual void updatePerception(CPerceipt *,CPerceipt*);
};

#endif // !defined(__PERCEPTION_H)


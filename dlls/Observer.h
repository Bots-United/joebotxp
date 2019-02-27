// Observer.h: interface for the CObserver class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__OBSERVER_H)
#define __OBSERVER_H

#include "ObserverItem.h"

class CObserver  
{
public:
	CObserver();
	virtual ~CObserver();

	void reset(void);			// reset all this shit :D

	void observe(void);			// observer all stuff

	float m_fFCheck;			// last time of full check
	bool m_bFCheck;				// is now a full check to perform ?!

	CObserverItem m_pOData[32];		// an observer item for each player ...
	bool m_bCheckBasicWPs;			// did we already chekc the basic waypoints ?
};

extern CObserver g_Observer;

#endif // !defined(__OBSERVER_H)

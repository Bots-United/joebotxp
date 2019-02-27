// Callback_onPathCreation.h: interface for the CCallback_onPathCreation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__CALLBACK__H)
#define __CALLBACK__H

class AStarBase;
class CPlayer;

class CCallback_onPathCreation
{
public:
	virtual void onPathCreation(AStarBase *p)=0;			// callback function when an AStar Path Task has been finished.
															// dont forget to delete p !!
};

class CCallback_onReachedDestination
{
public:
	virtual void onReachedDestination(CPlayer *)=0;			// callback function when Move path is at destination.
};

#endif // !defined(__CALLBACK__H)

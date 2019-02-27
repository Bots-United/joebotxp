// AStarNodeContainer.h: interface for the AStarNodeContainer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__ASTARNODECONTAINER_H)
#define __ASTARNODECONTAINER_H

#include "AStarNode.h"

class AStarNodeContainer  
{
public:
	AStarNodeContainer();
	virtual ~AStarNodeContainer();
	operator < (const AStarNodeContainer &);
	operator > (const AStarNodeContainer &);
	const AStarNodeContainer &operator = (const AStarNodeContainer &);

	AStarNode *m_p;
};

#endif // !defined(__ASTARNODECONTAINER_H)

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
	
	bool operator < (const AStarNodeContainer &param)const {
		return m_p->m_f < param.m_p->m_f;
	}
	bool operator > (const AStarNodeContainer &param)const {
		return m_p->m_f > param.m_p->m_f;
	}
	/*const AStarNodeContainer &operator = (const AStarNodeContainer &param){
		m_p = param.m_p;
		
		return ((const AStarNodeContainer &)(*this));
	}*/
	
	AStarNode *m_p;
};

#endif // !defined(__ASTARNODECONTAINER_H)

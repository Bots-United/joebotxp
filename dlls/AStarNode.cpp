// AStarNode.cpp: implementation of the AStarNode class.
//
//////////////////////////////////////////////////////////////////////

#include "AStarNode.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AStarNodeChildren::AStarNodeChildren(){
	m_pNext = 0;
	m_iChildren = 0;
}

void AStarNodeChildren::addChild(AStarNode *p){			// adds a child to this astar node 
	if(m_pNext)
		m_pNext->addChild(p);
	else{
		if(m_iChildren < _MAX_ASTAR_CHILDREN){
			m_ppChildren[m_iChildren] = p;
			m_iChildren++;
		}
		else{
			m_pNext = new AStarNodeChildren;
			m_pNext->addChild(p);
		}
	}
}

AStarNode::AStarNode(){
	m_pParent = 0;
	m_pNext = 0;
	m_f = m_g = m_h = 0;
	m_iWaypoint = -1;
}

AStarNode::AStarNode(int iWP):m_iWaypoint(iWP){
	m_pNext = 0;
	m_pParent = 0;
	m_f = m_g = m_h = 0;
}

AStarNode::~AStarNode(){
}

// AStarNode.h: interface for the AStarNode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__ASTARNODE_H)
#define __ASTARNODE_H

#define _MAX_ASTAR_CHILDREN 8

class AStarNode;

class AStarNodeChildren{
public:
	AStarNodeChildren();
	virtual ~AStarNodeChildren(){
		if(m_pNext)
			delete m_pNext;
	}
	void reset(void){
		m_iChildren = 0;
		if(m_pNext)
			delete m_pNext;
		m_pNext = 0;
	}

	void addChild(AStarNode *p);

	int m_iChildren;
	AStarNode *m_ppChildren[_MAX_ASTAR_CHILDREN];
	AStarNodeChildren *m_pNext;
};

class AStarNode  
{
public:
	AStarNode();
	AStarNode(int);
	virtual ~AStarNode();

	AStarNode *m_pParent;
	AStarNode *m_pNext;
	int m_f,m_g,m_h;
	int m_iWaypoint;

	AStarNodeChildren m_Children;
};

#endif // !defined(__ASTARNODE_H)

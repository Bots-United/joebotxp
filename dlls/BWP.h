// BWP.h: interface for the CBWP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BWP_H__B00BCC41_8DB0_46A2_A56A_EF51C4315198__INCLUDED_)
#define AFX_BWP_H__B00BCC41_8DB0_46A2_A56A_EF51C4315198__INCLUDED_

#include "Map.h"

class CBWPNode{
public:
	CBWPNode();
	virtual ~CBWPNode();

	bool m_bLeaf;					// it's true if it's the last node here :)
	int m_iPartComp;				// component of partition
	float m_fBorder;				// border
	CBWPNode *m_pParent;			// parent
	CBWPNode *m_ppChildren[2];		// children
	CWaypoint *m_pWaypoint;			// pointer to waypoint
};

class CBWP  
{
public:
	CBWP();
	virtual ~CBWP();
	void reset(void);
	void removeChildren(CBWPNode *);
	void addWaypoint(CWaypoint *);
	CBWPNode *traceNode(Vector &);

	CBWPNode *m_pHead;
};

#endif // !defined(AFX_BWP_H__B00BCC41_8DB0_46A2_A56A_EF51C4315198__INCLUDED_)

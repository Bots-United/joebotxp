// BWP.cpp: implementation of the CBWP class.
//
//////////////////////////////////////////////////////////////////////

#include "BWP.h"

#include <iostream>
using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBWPNode::CBWPNode()
{
	m_bLeaf = true;
	m_pParent = 0;
	m_ppChildren[0] = m_ppChildren[1] = 0;
	m_fBorder = 0;
	m_iPartComp = 0;
}

CBWPNode::~CBWPNode(){
}


CBWP::CBWP(){
	m_pHead = 0;
}

CBWP::~CBWP(){
	reset();
}

void CBWP::addWaypoint(CWaypoint *pWP){
	if(m_pHead){
		CBWPNode *pNode = traceNode(pWP->m_VOrigin),
			*pIterNode = 0,
			*pLastDivNode = 0;
		int i,
			iComp = 0;			// component to divide this partition by
		Vector VDiff = pWP->m_VOrigin - pNode->m_pWaypoint->m_VOrigin;
		if(VDiff.Length() == 0)			// don't add identical waypoints
			return;

		for(i=0; i < 3; i++){
			if(fabs(VDiff[i]) > fabs(VDiff[iComp])){
				iComp = i;
		}}
		
		//cout << iComp << "  ";
		
		pNode->m_fBorder = pNode->m_pWaypoint->m_VOrigin[iComp] + VDiff[iComp] / 2.f;

		pNode->m_iPartComp = iComp;
		pNode->m_ppChildren[0] = new CBWPNode;
		pNode->m_ppChildren[1] = new CBWPNode;
		if(pWP->m_VOrigin[iComp] < pNode->m_fBorder){
			pNode->m_ppChildren[0]->m_pWaypoint = pWP;
			pNode->m_ppChildren[1]->m_pWaypoint = pNode->m_pWaypoint;
		}
		else{
			pNode->m_ppChildren[1]->m_pWaypoint = pWP;
			pNode->m_ppChildren[0]->m_pWaypoint = pNode->m_pWaypoint;
		}
		pNode->m_pWaypoint = 0;
		pNode->m_bLeaf = false;
		pNode->m_ppChildren[0]->m_pParent = pNode;
		pNode->m_ppChildren[1]->m_pParent = pNode;
	}
	else{
		m_pHead = new CBWPNode;

		m_pHead->m_bLeaf = true;
		m_pHead->m_pWaypoint = pWP;
	}
}

CBWPNode *CBWP::traceNode(Vector &VVec){
	CBWPNode *pNode = m_pHead;
	int i = 0;

	while(pNode && !pNode->m_bLeaf){
		i++;
		pNode = pNode->m_ppChildren[VVec[pNode->m_iPartComp] >= pNode->m_fBorder];
	}
	//cout << i << " ";

	return pNode;
}

void CBWP::removeChildren(CBWPNode *pRemove){
	if(pRemove->m_ppChildren[0]){
		removeChildren(pRemove->m_ppChildren[0]);
	}
	if(pRemove->m_ppChildren[1]){
		removeChildren(pRemove->m_ppChildren[1]);
	}
	delete pRemove->m_ppChildren[0];
	pRemove->m_ppChildren[0] = 0;
	delete pRemove->m_ppChildren[1];
	pRemove->m_ppChildren[1] = 0;
}

void CBWP::reset(void){
	removeChildren(m_pHead);
	delete m_pHead;
	m_pHead = 0;
}
// BWP.cpp: implementation of the CBWP class.
//
//////////////////////////////////////////////////////////////////////
#include <math.h>
#include "BWP.h"

#include <iostream>
using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#include "Line.h"
#include "Cube.h"

extern CObject **Objects;
extern long lObjects;
extern float rD;

extern bool g_bshowbwp;

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
		CBWPNode *pNode = 0;
		int i,
			iComp = -1;			// component to divide this partition by
		Vector VDiff;

		pNode = traceNode(pWP->m_VOrigin);
		VDiff = pWP->m_VOrigin - pNode->m_pWaypoint->m_VOrigin;
		if(VDiff.length() == 0)			// don't add identical waypoints
			return;
		int iNot=-1;
		if(pNode->m_pParent){
			iNot = pNode->m_pParent->m_iPartComp;
		}
		iNot = -1;

		for(i=0; i < 2; i++)
			if((iComp == -1&& i != iNot)
				|| (fabs(VDiff[i]) > fabs(VDiff[iComp]) && i != iNot))
				iComp = i;

		cout << iComp << "  ";
		pNode->m_fBorder = pNode->m_pWaypoint->m_VOrigin[iComp] + VDiff[iComp] / 2.f;
		pNode->m_iPartComp = iComp;
		pNode->m_ppChildren[0] = new CBWPNode;
		pNode->m_ppChildren[1] = new CBWPNode;
		pNode->m_ppChildren[0]->m_pParent = pNode;
		pNode->m_ppChildren[1]->m_pParent = pNode;
		if(pWP->m_VOrigin[iComp] < pNode->m_fBorder){
			pNode->m_ppChildren[0]->m_pWaypoint = pWP;
			pNode->m_ppChildren[1]->m_pWaypoint = pNode->m_pWaypoint;
		}
		else{
			pNode->m_ppChildren[0]->m_pWaypoint = pNode->m_pWaypoint;
			pNode->m_ppChildren[1]->m_pWaypoint = pWP;
		}
		pNode->m_pWaypoint = 0;
		pNode->m_bLeaf = false;

		/*{
			Objects[lObjects] = new CLine;
			if(iComp == 0){
				((CLine *)Objects[lObjects])->x1 = pNode->m_fBorder/1000;
				((CLine *)Objects[lObjects])->y1 = -4;
				((CLine *)Objects[lObjects])->z1 = 0;
			
				((CLine *)Objects[lObjects])->x2 = pNode->m_fBorder/1000;
				((CLine *)Objects[lObjects])->y2 = 4;
				((CLine *)Objects[lObjects])->z2 = 0;
			}
			else if(iComp == 1){
				((CLine *)Objects[lObjects])->y1 = pNode->m_fBorder/1000;
				((CLine *)Objects[lObjects])->x1 = -4;
				((CLine *)Objects[lObjects])->z1 = 0;
			
				((CLine *)Objects[lObjects])->y2 = pNode->m_fBorder/1000;
				((CLine *)Objects[lObjects])->x2 = 4;
				((CLine *)Objects[lObjects])->z2 = 0;
			}
			else{
				((CLine *)Objects[lObjects])->z1 = pNode->m_fBorder/1000;
				((CLine *)Objects[lObjects])->x1 = -4;
				((CLine *)Objects[lObjects])->y1 = 0;
			
				((CLine *)Objects[lObjects])->z2 = pNode->m_fBorder/1000;
				((CLine *)Objects[lObjects])->x2 = 4;
				((CLine *)Objects[lObjects])->y2 = 0;
			}
			
			((CLine *)Objects[lObjects])->r = .0f;
			((CLine *)Objects[lObjects])->g = .9f;
			((CLine *)Objects[lObjects])->b = .0f;
			
			lObjects++;
		}*/
	}
	else{
		m_pHead = new CBWPNode;

		m_pHead->m_bLeaf = true;
		m_pHead->m_pWaypoint = pWP;
	}
}

CBWPNode *CBWP::traceNode(Vector &VVec){
	CBWPNode *pNode = m_pHead;
	
	while(/*pNode && */!pNode->m_bLeaf){
		if(g_bshowbwp){
			float fl,fr;
			Objects[lObjects] = new CLine;
			if(pNode->m_iPartComp == 0){
				getBorders(pNode,1,fl,fr);
				((CLine *)Objects[lObjects])->x1 = pNode->m_fBorder/1000.f;
				((CLine *)Objects[lObjects])->y1 = fl/1000.f;
				((CLine *)Objects[lObjects])->z1 = 0;
				
				((CLine *)Objects[lObjects])->x2 = pNode->m_fBorder/1000.f;
				((CLine *)Objects[lObjects])->y2 = fr/1000.f;
				((CLine *)Objects[lObjects])->z2 = 0;
			}
			else if(pNode->m_iPartComp == 1){
				getBorders(pNode,0,fl,fr);
				((CLine *)Objects[lObjects])->y1 = pNode->m_fBorder/1000.f;
				((CLine *)Objects[lObjects])->x1 = fl/1000.f;
				((CLine *)Objects[lObjects])->z1 = 0;
				
				((CLine *)Objects[lObjects])->y2 = pNode->m_fBorder/1000.f;
				((CLine *)Objects[lObjects])->x2 = fr/1000.f;
				((CLine *)Objects[lObjects])->z2 = 0;
			}
			else{
				getBorders(pNode,0,fl,fr);
				((CLine *)Objects[lObjects])->z1 = pNode->m_fBorder/1000.f;
				((CLine *)Objects[lObjects])->x1 = fl/1000.f;
				((CLine *)Objects[lObjects])->y1 = 0;
				
				((CLine *)Objects[lObjects])->z2 = pNode->m_fBorder/1000.f;
				((CLine *)Objects[lObjects])->x2 = fr/1000.f;
				((CLine *)Objects[lObjects])->y2 = 0;
			}
			
			((CLine *)Objects[lObjects])->r = .0f;
			((CLine *)Objects[lObjects])->g = .0f;
			((CLine *)Objects[lObjects])->b = 1.f;
			
			lObjects++;
		}
		
		pNode = pNode->m_ppChildren[VVec[pNode->m_iPartComp] >= pNode->m_fBorder];
	}

	if(g_bshowbwp){
		Vector V1,V2;
		getBorders(pNode,0,V1.x,V2.x);
		getBorders(pNode,1,V1.y,V2.y);
		//getBorders(pNode,3,V1.z,V2.z);
		V1.z = V2.z = 0;
		
		Objects[lObjects] = new CLine;
		((CLine *)Objects[lObjects])->x1 = V1.x/1000;
		((CLine *)Objects[lObjects])->y1 = V1.y/1000;
		((CLine *)Objects[lObjects])->z1 = V1.z/1000;
		
		((CLine *)Objects[lObjects])->x2 = V2.x/1000;
		((CLine *)Objects[lObjects])->y2 = V1.y/1000;
		((CLine *)Objects[lObjects])->z2 = V1.z/1000;
		
		((CLine *)Objects[lObjects])->r = .0f;
		((CLine *)Objects[lObjects])->g = .9f;
		((CLine *)Objects[lObjects])->b = .0f;
		
		lObjects++;

		Objects[lObjects] = new CLine;
		((CLine *)Objects[lObjects])->x1 = V2.x/1000;
		((CLine *)Objects[lObjects])->y1 = V1.y/1000;
		((CLine *)Objects[lObjects])->z1 = V1.z/1000;
		
		((CLine *)Objects[lObjects])->x2 = V2.x/1000;
		((CLine *)Objects[lObjects])->y2 = V2.y/1000;
		((CLine *)Objects[lObjects])->z2 = V1.z/1000;
		
		((CLine *)Objects[lObjects])->r = .0f;
		((CLine *)Objects[lObjects])->g = .9f;
		((CLine *)Objects[lObjects])->b = .0f;
		
		lObjects++;

		Objects[lObjects] = new CLine;
		((CLine *)Objects[lObjects])->x1 = V2.x/1000;
		((CLine *)Objects[lObjects])->y1 = V2.y/1000;
		((CLine *)Objects[lObjects])->z1 = V1.z/1000;
		
		((CLine *)Objects[lObjects])->x2 = V1.x/1000;
		((CLine *)Objects[lObjects])->y2 = V2.y/1000;
		((CLine *)Objects[lObjects])->z2 = V1.z/1000;
		
		((CLine *)Objects[lObjects])->r = .0f;
		((CLine *)Objects[lObjects])->g = .9f;
		((CLine *)Objects[lObjects])->b = .0f;
		
		lObjects++;

		Objects[lObjects] = new CLine;
		((CLine *)Objects[lObjects])->x1 = V1.x/1000;
		((CLine *)Objects[lObjects])->y1 = V2.y/1000;
		((CLine *)Objects[lObjects])->z1 = V1.z/1000;
		
		((CLine *)Objects[lObjects])->x2 = V1.x/1000;
		((CLine *)Objects[lObjects])->y2 = V1.y/1000;
		((CLine *)Objects[lObjects])->z2 = V1.z/1000;
		
		((CLine *)Objects[lObjects])->r = .0f;
		((CLine *)Objects[lObjects])->g = .9f;
		((CLine *)Objects[lObjects])->b = .0f;
		
		lObjects++;

		Objects[lObjects] = new CLine;
		((CLine *)Objects[lObjects])->x1 = V1.x/1000;
		((CLine *)Objects[lObjects])->y1 = V1.y/1000;
		((CLine *)Objects[lObjects])->z1 = V1.z/1000;
		
		((CLine *)Objects[lObjects])->x2 = V2.x/1000;
		((CLine *)Objects[lObjects])->y2 = V2.y/1000;
		((CLine *)Objects[lObjects])->z2 = V1.z/1000;
		
		((CLine *)Objects[lObjects])->r = .0f;
		((CLine *)Objects[lObjects])->g = .9f;
		((CLine *)Objects[lObjects])->b = .0f;
		
		lObjects++;

		Objects[lObjects] = new CLine;
		((CLine *)Objects[lObjects])->x1 = V2.x/1000;
		((CLine *)Objects[lObjects])->y1 = V1.y/1000;
		((CLine *)Objects[lObjects])->z1 = V1.z/1000;
		
		((CLine *)Objects[lObjects])->x2 = V1.x/1000;
		((CLine *)Objects[lObjects])->y2 = V2.y/1000;
		((CLine *)Objects[lObjects])->z2 = V1.z/1000;
		
		((CLine *)Objects[lObjects])->r = .0f;
		((CLine *)Objects[lObjects])->g = .9f;
		((CLine *)Objects[lObjects])->b = .0f;
		
		lObjects++;
	}

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

void CBWP::getBorders(CBWPNode *pNode,int iComp,float &fLeft,float &fRight){
	bool bLeft = false;
	bool bRight= false;
	fRight = 4096;
	fLeft = -4096;
	
	CBWPNode *pIterNode = pNode,*pLastNode;

	while(!(bLeft&&bRight)){
		pLastNode = pIterNode;
		pIterNode = pIterNode->m_pParent;
		
		if(!pIterNode)
			break;
		
		if(pIterNode->m_iPartComp == iComp){
			if(pIterNode->m_ppChildren[1] == pLastNode){
				if(!bLeft){
					bLeft = true;
					fLeft = pIterNode->m_fBorder;
				}
			}
			else{
				if(!bRight){
					bRight = true;
					fRight = pIterNode->m_fBorder;
				}
			}
		}
	}
}
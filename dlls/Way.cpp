// Way.cpp: implementation of the CWay class.
//
//////////////////////////////////////////////////////////////////////

#include "Way.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWay::CWay()
{
	m_iCount = 0;
	m_iActCount = 0;
}

CWay::~CWay(){
}

int CWay::find(int iIndex){
	for(int i = 0; i < m_iCount; i ++){
		if ( m_piIndices[i] == iIndex )
			return i;
	}
	return -1;
}

int CWay::getLast(void){
	if(m_iActCount >= 1)
		return m_piIndices[m_iActCount-1];
	else
		return -1;
}

int CWay::getCurrent(void){
	return getOffset(0);
}

int CWay::advance(void){
	m_iActCount ++;
	return getOffset(0);
}

int CWay::getOffset(int iOffset){
	if(m_iActCount + iOffset >= m_iCount)
		return -1;
	else{
		return m_piIndices[m_iActCount + iOffset];
	}
}

void CWay::setMask(CWaypointGraph *pGraph,CBitField *p,bool bBroad,bool bFull){
	// bBroad : should neighbouring nodes also be set
	// bFull : should the full path be masked, or only from m_iActCount
	CPath_iterator path_iter;
	int iC,i;

	if(bFull)
		i = 0;
	else
		i = m_iActCount;
	p->zero();
	for(; i < m_iCount; i ++){
		assert(m_piIndices[i] > -1 && m_piIndices[i] < pGraph->m_iNumWaypoints);

		if ( m_piIndices[i] !=	-1){
			p->setBit(m_piIndices[i],true);
			if(bBroad){
				path_iter = (*pGraph)[m_piIndices[i]];

				iC = path_iter.getTo();
				while(iC != -1){
					if(p->bits() > iC)
						p->setBit(iC,true);
					++path_iter;
				}
			}
		}
	}
}

float CWay::calcLength(CWaypointGraph *pGraph,int iOffset){
	// count from offset to end
	if(iOffset >= m_iCount || !pGraph)
		return 0.f;

	Vector VLastWP,VNew;
	float fLength = 0.f;
	int i;

	VLastWP = (*pGraph)[m_piIndices[iOffset]].m_VOrigin;	// set the origin of the first waypoint
	for(i = iOffset+1; i < m_iCount; i ++){
		VNew = (*pGraph)[m_piIndices[i]].m_VOrigin;
		
		fLength += (VNew - VLastWP).length();

		VLastWP = VNew;
	}

	return fLength;
}

int CWay::getDestination(void){
	if(!m_iCount)
		return -1;
	else
		return m_piIndices[m_iCount-1];
}

bool CWay::isPosInWay(CWaypointGraph *pGraph,const Vector &VOrigin,int iTestWP,const float fDot){
	// from current waypoint, check if that location is within some cones around the way. the fDot specifies the cones.
	int i,ifrom, ito;
	bool bWithinWay = false;
	Vector VConnWP, VConn1,VConn2;

	if(iTestWP > left()-1)
		iTestWP = left()-1;

	for(i=0; i < iTestWP; i++){
		ifrom = getOffset(i);
		ito = getOffset(i+1);

		// check if those are valid
		if((*pGraph).isValid(ifrom) && (*pGraph).isValid(ito)){
			VConnWP = ((*pGraph)[ito].m_VOrigin - (*pGraph)[ifrom].m_VOrigin).normalize();
			VConn1 = (VOrigin - (*pGraph)[ifrom].m_VOrigin).normalize();
			// check for both if they are within the cone
			if(DotProduct(VConnWP,VConn1) < fDot){
				// if so, return true
				bWithinWay = true;
				break;
			}
			VConn2 = ((*pGraph)[ito].m_VOrigin - VOrigin).normalize();
			if(DotProduct(VConnWP,VConn2) < fDot){
				// if so, return true
				bWithinWay = true;
				break;
			}
		}
		else
			break;
	}

	return bWithinWay;
}
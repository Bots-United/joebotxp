// AStarGoalMod.cpp: implementation of the AStarGoalMod class.
//
//////////////////////////////////////////////////////////////////////

#include "AStarGoalMod.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AStarGoalMod::AStarGoalMod(){
	m_fOffsetCosts = .0f;

	memset(m_pfAddCosts,0,sizeof(float)*_MAX_WAYPOINTS);
}

AStarGoalMod::~AStarGoalMod(){
}

void AStarGoalMod::addVisibleCost(int iWP,float fVisibleCost){
	int i;
	int iVisible = 0;

	for(i = g_Map.m_Waypoints.m_iNumWaypoints -1; i>=0; i --){
		if(g_Map.m_Waypoints[i].m_lFlags & CWaypoint::DELETED){
			continue;
		}
		if(g_Map.m_Waypoints.getVisible(iWP,i)){
			// this waypoint is visible to the thread
			iVisible ++;
			m_pfAddCosts[i] += fVisibleCost;
		}
		else{
			// this waypoint isnt visible to the thread
		}
	}
	m_fOffsetCosts += (-float(iVisible)*fVisibleCost)/float(g_Map.m_Waypoints.m_iNumWaypoints);
	// let's assume that m_iNumWaypoints is the real number of waypoints
}

void AStarGoalMod::addCost(CBitField &bitfield,float fCost){
	int i,iCount;
	CBitField::CBFIterator bit_iterator;

	bit_iterator = bitfield;
	iCount = 0;

	for(i=0; i < bitfield.bits() && i < g_Map.m_Waypoints.m_iNumWaypoints; i++,++bit_iterator){		// loop thru all bits resp waypoints
		if(bit_iterator.getBit()){
			iCount ++;
			m_pfAddCosts[i] += fCost;
		}
		else{
		}
	}
	m_fOffsetCosts += (-float(iCount)*fCost)/float(g_Map.m_Waypoints.m_iNumWaypoints);
}

int AStarGoalMod::getCost(int i1,const CPath_iterator &path_iter){
	float fCost,fDistance;
	int i2 = path_iter.getTo();

	/*if(i1 < 0 || i2 < 0 || i1 >= g_Map.m_Waypoints.m_iNumWaypoints || i2 >= g_Map.m_Waypoints.m_iNumWaypoints)
		*((long*)0)=0;*/

	fDistance = (g_Map.m_Waypoints[i1].m_VOrigin - g_Map.m_Waypoints[i2].m_VOrigin).length();

	fCost = fDistance * (1.f + (float)path_iter.getStat()->m_iFailed);
	fCost = fCost * ((m_pfAddCosts[i2]+m_pfAddCosts[i2])/2.f + 1.f + m_fOffsetCosts);

	return (int)fCost;
}
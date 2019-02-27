// AStarBaseGoal.cpp: implementation of the AStarBaseGoal class.
//
//////////////////////////////////////////////////////////////////////

#include "AStarBaseGoal.h"
#include "AStarNode.h"

#include "Map.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AStarBaseGoal::AStarBaseGoal(){
	m_iDestination = -1;
	m_iStart = -1;
}

AStarBaseGoal::~AStarBaseGoal(){
}

bool AStarBaseGoal::isValid(void){		// returns true is this goal is ready to be used
	if(m_iDestination == -1)
		return false;
	if(m_iStart == -1)
		return false;
	return true;
}

int AStarBaseGoal::getHeuristic(int i1){
	float fHeuristic,fDistance;

	fDistance = (g_Map.m_Waypoints[i1].m_VOrigin
		-g_Map.m_Waypoints[getDestination()].m_VOrigin).CBLength();

	fHeuristic = fDistance;

	return (int)fHeuristic;
}

int AStarBaseGoal::getCost(int i1,const CPath_iterator &path_iter){
	float fCost,fDistance;
	int i2 = path_iter.getTo();

	fDistance = (g_Map.m_Waypoints[i1].m_VOrigin - g_Map.m_Waypoints[i2].m_VOrigin).length();

	fCost = fDistance * (1.f + (float)path_iter.getStat()->m_iFailed);

	return (int)fCost;
}

bool AStarBaseGoal::isOpen(int i1,const CPath_iterator &path_iter){
	// if the connection from i1 to i2 is open, let's return true
	return true;
}

bool AStarBaseGoal::isDestination(const AStarNode *pASN){
	return (pASN->m_iWaypoint == getDestination());
}
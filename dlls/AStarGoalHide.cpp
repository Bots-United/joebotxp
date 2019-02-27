// AStarGoalHide.cpp: implementation of the AStarGoalHide class.
//
//////////////////////////////////////////////////////////////////////

#include "AStarGoalHide.h"
#include "AStarNode.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AStarGoalHide::AStarGoalHide(){
	m_iHidefromWP = -1;
	m_VHideAim = Vector(0,0,0);
	m_iMinCost = 0;
}

AStarGoalHide::~AStarGoalHide(){
}

void AStarGoalHide::hideFrom(int iWP,const Vector&VHideFrom,const Vector&VMe){
	m_iHidefromWP = iWP;
	m_VHideAim = VMe + (VMe - VHideFrom).normalize() * 5000.f;
}

int AStarGoalHide::getHeuristic(int i1){
	float fHeuristic,fDistance;

	fDistance = (g_Map.m_Waypoints[i1].m_VOrigin-m_VHideAim).CBLength();

	fHeuristic = fDistance;

	return (int)fHeuristic;
}

void AStarGoalHide::setMinimumCost(int iMinCost){
	m_iMinCost = iMinCost;
}

bool AStarGoalHide::isDestination(const AStarNode *pASN){
    // return true for a waypoint which isnt visible from m_iHidefromWP and which is far enough
	if(pASN->m_g >= m_iMinCost)
		return (!g_Map.m_Waypoints.getVisible(m_iHidefromWP,pASN->m_iWaypoint));
	return false;
}

bool AStarGoalHide::isValid(void){		// returns true is this instance is ready to be used
	if(getStart() == -1)
		return false;
	if(m_iHidefromWP == -1)
		return false;
	return true;
}

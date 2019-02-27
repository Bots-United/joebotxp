// AStarGoalPFled.cpp: implementation of the AStarGoalPFled class.
//
//////////////////////////////////////////////////////////////////////

#include "AStarGoalPFled.h"
#include "AStarNode.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AStarGoalPFled::AStarGoalPFled(){
	m_fMinCost = 300;
}

AStarGoalPFled::~AStarGoalPFled(){
}

void AStarGoalPFled::fleeFrom(Vector &VMe,Vector &VEnemy,CBitField &pPVisited,float fMinCost){
	// this class can be used to determine a possible location of an enemy (VEnemy) fleeing from me (VMe) whileas it
	// only visits the waypoints marked true in pPVisited. To return waypoint after having reached minimum cost
	// (fMinCost)

	m_VFleeAim = VEnemy + (VEnemy-VMe).normalize()*5000.f;
	m_pbPVisited = pPVisited;
	m_fMinCost = fMinCost;
}

int AStarGoalPFled::getHeuristic(int i){
	float fHeuristic, fDistance;

	fDistance = (g_Map.m_Waypoints[i].m_VOrigin-m_VFleeAim).CBLength();

	fHeuristic = fDistance;

	return (int)fHeuristic;
}

bool AStarGoalPFled::isValid(void){
	return !!m_pbPVisited.bits();		// so if the bitfield has a size, let's assume everything has been initialized here
}

bool AStarGoalPFled::isOpen(int i1, const CPath_iterator &path_iter){
	// if the connection from i1 to i2 is open, let's return true
	// so if i2 is valid because it's marked true in the PVisited field
	int i2 = path_iter.getTo();

	return m_pbPVisited[i2];
}

bool AStarGoalPFled::isDestination(const AStarNode *pNode){
	return pNode->m_g > m_fMinCost;
}
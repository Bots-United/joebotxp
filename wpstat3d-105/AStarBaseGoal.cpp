// AStarBaseGoal.cpp: implementation of the AStarBaseGoal class.
//
//////////////////////////////////////////////////////////////////////
#include <math.h>
#include "AStarBaseGoal.h"

#include "Map.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AStarBaseGoal::AStarBaseGoal()
{

}

AStarBaseGoal::~AStarBaseGoal()
{

}

int AStarBaseGoal::getDistance(int i1,int i2){
	return int((g_Map.m_Waypoints.m_Waypoints[i1].m_VOrigin
		-g_Map.m_Waypoints.m_Waypoints[i2].m_VOrigin).CBLength());
}

int AStarBaseGoal::getCost(int i1,int i2){
	return int((g_Map.m_Waypoints.m_Waypoints[i1].m_VOrigin
		-g_Map.m_Waypoints.m_Waypoints[i2].m_VOrigin).length());
}

bool AStarBaseGoal::isOpen(int i1,int i2){
	return true;
}

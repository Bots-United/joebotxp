// AStarGoalCover.cpp: implementation of the AStarGoalCover class.
//
//////////////////////////////////////////////////////////////////////
#include <math.h>
#include <memory.h>
#include "AStarGoalCover.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AStarGoalCover::AStarGoalCover(){
	m_fNCoverValue = .3f;
	m_fOffsetCosts = .0f;

	reset();
}

AStarGoalCover::~AStarGoalCover(){
	m_fOffsetCosts = 0;
}

void AStarGoalCover::reset(void)
{
for(int i = g_Map.m_Waypoints.m_iNumWaypoints -1; i>=0; i --){
		m_pfCoverCosts[i] = 0;
	}
}

void AStarGoalCover::addThreadWP(int iWP){
	int i;
	int iVisible = 0;
	
	Objects[lObjects] = new CCube;
	((CCube *)Objects[lObjects])->x = g_Map.m_Waypoints.m_Waypoints[iWP].m_VOrigin.x/1000;
	((CCube *)Objects[lObjects])->y = g_Map.m_Waypoints.m_Waypoints[iWP].m_VOrigin.y/1000;
	((CCube *)Objects[lObjects])->z = g_Map.m_Waypoints.m_Waypoints[iWP].m_VOrigin.z/1000;
	
	((CCube *)Objects[lObjects])->rx = rD+.01f;
	((CCube *)Objects[lObjects])->ry = rD+.01f;
	((CCube *)Objects[lObjects])->rz = rD+.01f;
	
	((CCube *)Objects[lObjects])->r = 1;
	((CCube *)Objects[lObjects])->g = 1;
	((CCube *)Objects[lObjects])->b = 1;
	
	lObjects++;

	for(i = g_Map.m_Waypoints.m_iNumWaypoints -1; i>=0; i --){
		if(g_Map.m_Waypoints.m_Waypoints[i].m_lFlags & CWaypoint::DELETED){
			continue;
		}
		if(g_Map.m_Waypoints.getVisible(iWP,i)){
			// this waypoint is visible to the thread
			iVisible ++;
			m_pfCoverCosts[i] += m_fNCoverValue;
		}
		else{
			// this waypoint isnt visible to the thread
		}
	}
	m_fOffsetCosts = (-float(iVisible)*m_fNCoverValue)/float(g_Map.m_Waypoints.m_iNumWaypoints*2);
}

int AStarGoalCover::getCost(int i1,int i2){
	return int((g_Map.m_Waypoints.m_Waypoints[i1].m_VOrigin
		-g_Map.m_Waypoints.m_Waypoints[i2].m_VOrigin).length()
		* (m_pfCoverCosts[i2] + 1.0 + m_fOffsetCosts) );
}

int AStarGoalCover::getDistance(int i1,int i2){
	return int((g_Map.m_Waypoints.m_Waypoints[i1].m_VOrigin
		-g_Map.m_Waypoints.m_Waypoints[i2].m_VOrigin).length()
		* (1.0 + m_fOffsetCosts) );
}

// ObserverItem.cpp: implementation of the CObserverItem class.
//
//////////////////////////////////////////////////////////////////////

#include "ObserverItem.h"
#include "Map.h"
#include "Game.h"

float g_fD2NWP = 200;			// distance to next waypoint - normal
float g_fD2NWP_Ladder = 75;		// distance to next waypoint - on ladder

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CObserverItem::CObserverItem(){
	reset();
}

CObserverItem::~CObserverItem(){
}

void CObserverItem::reset(void){
	m_iLastAddedWP = -1;
	m_iLastNearWP = -1;
	m_iLastLastNearWP = -1;
	m_iNearestWP = -1;
	m_fNearestDistance = 0;
	m_bOnLadder = false;
	m_iRecItems = 0;

	m_bFCheck = false;
	m_pPlayer = 0;
	m_pEntity = 0;
}

void CObserverItem::observe(CPlayer *pPlayer){
	edict_t *pEdict;
	if(!pPlayer)
		return;

	if(pEdict=pPlayer->getEntity()){
		if(!pPlayer->isAlive()){
			reset();
			return;
		}
		m_pEntity = pEdict;
		m_pPlayer = pPlayer;

		preProcess();

		if(!checkDistances()){
			if(!checkLadders()){
				if(!checkCorners()){
				}
			}
		}

		postProcess();
	}
	else{
		if(m_pEntity){
			reset();
			m_pEntity = 0;
		}
	}
}

bool CObserverItem::preProcess(void){
	PROFILE("preProcess");
	m_pPlayer->m_iNearestWP =
		m_iNearestWP =
		g_Map.m_Waypoints.getNearest(m_pPlayer->getOrigin());

#ifdef DEBUGMESSAGES
	/*if(m_iNearestWP != -1)
		if(g_pGame->m_pListenserverEdict.getEntity())
			UTIL_DrawBeam(g_pGame->m_pListenserverEdict.getEntity(),g_pGame->m_pListenserverEdict.getEntity()->v.origin,g_Map.m_Waypoints[m_iNearestWP].m_VOrigin,5,0,255,255,255,255,0,1);*/
#endif

	if(m_iNearestWP == -1) m_fNearestDistance = 1000000000;
	else m_fNearestDistance = (g_Map.m_Waypoints[m_iNearestWP].m_VOrigin-m_pPlayer->getOrigin()).squareLength();

	if(m_iLastNearWP == -1) m_fLastNearDistance = 1000000000;
	else m_fLastNearDistance = (g_Map.m_Waypoints[m_iLastNearWP].m_VOrigin-m_pPlayer->getOrigin()).squareLength();

	// check height
	Vector v_down = m_pPlayer->getOrigin() - Vector(0,0,1000);
	TraceResult tr;
	g_pGame->traceLine(m_pPlayer->getOrigin(),v_down,0,&tr);
	m_fHeight = tr.flFraction * 1000.f;

	// record positions
	record();

	// log stat data ...

	// traffic - only record if a bot is moving ... for recording camping we maybe need some diff var
	if(m_pPlayer->getVelocity().squareLength() > 100*100){
		if(m_iNearestWP != -1)
			g_Map.m_Waypoints[m_iNearestWP].m_lTraffic ++;
	}

	return true;
}

void CObserverItem::record(void){
	if(m_bFCheck){
		if(m_iRecItems){
			// non first entry
			m_pRec[m_iRecItems].m_VOrigin = m_pPlayer->getOrigin();
#ifdef DEBUGMESSAGES
			//UTIL_DrawBeam(g_pGame->m_pListenserverEdict.getEntity(),m_pPlayer->getOrigin(),m_pPlayer->getOrigin()+Vector(0,0,10),10,0,255,255,255,255,0);
#endif
			m_pRec[m_iRecItems].m_fHeight = m_fHeight;
			if((m_pRec[m_iRecItems].m_VOrigin-m_pRec[m_iRecItems-1].m_VOrigin).length() > 20.f){
				if(m_iRecItems+1 < OI_LocRecLength)
					m_iRecItems ++;
			}
		}
		else{
			// first entry
			m_pRec[m_iRecItems].m_VOrigin = m_pPlayer->getOrigin();
#ifdef DEBUGMESSAGES
			//UTIL_DrawBeam(g_pGame->m_pListenserverEdict.getEntity(),m_pPlayer->getOrigin(),m_pPlayer->getOrigin()+Vector(0,0,10),10,0,255,255,255,255,0);
#endif
			m_pRec[m_iRecItems].m_fHeight = m_fHeight;
			m_iRecItems ++;
		}
	}
}

int CObserverItem::placeWaypoint(Vector VAdd, long lFlags){
	int iNewWP;			// index of added waypoint

	if(!(lFlags & CWaypoint::LADDER | CWaypoint::MIDAIR)){			// if this waypoint is no ladder waypoint ... trace down to ground
		UTIL_TraceToGround(VAdd);
	}

	iNewWP = g_Map.m_Waypoints.add(VAdd);

	m_iLastAddedWP = iNewWP;
	g_Map.m_Waypoints[iNewWP].m_lFlags |= lFlags;

	if(m_iLastNearWP != -1){
		// check if last nearest waypoint and new waypoint should be connected
		if(g_Map.getReachable(g_Map.m_Waypoints[m_iLastNearWP].m_VOrigin,
			g_Map.m_Waypoints[iNewWP].m_VOrigin)){
			g_Map.m_Waypoints.connect(m_iLastNearWP,iNewWP);
#ifdef DEBUGMESSAGES
			if(g_pGame->m_pListenserverEdict.getEntity())
				g_pGame->drawBeam(DEBUG_GAME,g_pGame->m_pListenserverEdict.getEntity(),g_Map.m_Waypoints[m_iLastNearWP].m_VOrigin,g_Map.m_Waypoints[iNewWP].m_VOrigin,10,5,Vector(255,255,255),255,0,10);
#endif
		}
		else{
			checkRec4Connection(m_iLastNearWP,iNewWP);
		}
		if(g_Map.getReachable(g_Map.m_Waypoints[iNewWP].m_VOrigin,
			g_Map.m_Waypoints[m_iLastNearWP].m_VOrigin)){
			g_Map.m_Waypoints.connect(iNewWP,m_iLastNearWP);
		}
	}
	m_iRecItems = 0;			// reset recording
	return iNewWP;
}

bool CObserverItem::checkRec4Connection(int iWP1, int iWP2){
	// this normally gets called if a path cannot be created due to no reachability
	// so go and check the recorded locations if a wp has to be created in between.

	if(m_iRecItems < 3)
		return false;

	if(m_pRec[0].m_fHeight < 50){
		if(!g_Map.m_Waypoints[iWP1].isConnectedTo(iWP2)){
			g_Map.m_Waypoints.connect(iWP1,iWP2);
		}
	}
	else if(m_pRec[m_iRecItems-1].m_fHeight < 50){
		if(!g_Map.m_Waypoints[iWP1].isConnectedTo(iWP2)){
				g_Map.m_Waypoints.connect(iWP1,iWP2);
		}
	}
	/*else if(fabs(m_pRec[m_iRecItems-1].m_fHeight-m_pRec[0].m_fHeight) < 200){
		int i;
		bool bConnect = true;
		float fLastHeight = m_pRec[0].m_fHeight;

		for(i = 0; i < m_iRecItems; i++){
			if(fabs(m_pRec[i].m_fHeight - fLastHeight) > 200){
				bConnect = false;
				break;
			}
		}
		if(bConnect){
			if(!g_Map.m_Waypoints[iWP1].isConnectedTo(iWP2))
				g_Map.m_Waypoints.connect(iWP1,iWP2);
		}
	}*/

	// first check the middle
	int iMiddle = m_iRecItems/2,
		iMiddleWP = -1;

	bool bReachable1 = g_Map.getReachable(m_pRec[0].m_VOrigin,m_pRec[iMiddle].m_VOrigin)
		,bReachable2 = g_Map.getReachable(m_pRec[iMiddle].m_VOrigin,m_pRec[m_iRecItems-1].m_VOrigin);

	if(bReachable1
		&& bReachable2){
		if( (iMiddleWP = g_Map.m_Waypoints.getNearest(m_pRec[iMiddle].m_VOrigin,false,false,0,50)) == -1){
			iMiddleWP = g_Map.m_Waypoints.add(m_pRec[iMiddle].m_VOrigin);
		}
	}
	else{
		// no waypoint placeable in the middle ...

		// just connect them
		if(!g_Map.m_Waypoints[iWP1].isConnectedTo(iWP2)){
			g_Map.m_Waypoints[iWP1].addConnectionTo(iWP2);
		}
	}

	if(iWP1 != -1 && iMiddleWP != -1){
		if(!g_Map.m_Waypoints[iWP1].isConnectedTo(iMiddleWP))
			g_Map.m_Waypoints.connect(iWP1,iMiddleWP);
	}
	if(iWP2 != -1 && iMiddleWP != -1){
		if(!g_Map.m_Waypoints[iWP2].isConnectedTo(iMiddleWP))
			g_Map.m_Waypoints.connect(iWP2,iMiddleWP);
	}

	m_iLastAddedWP = iMiddleWP;

	return false;
}

void CObserverItem::addNearPath(void){
	if(m_iLastNearWP!=-1
		&& m_iLastNearWP != m_iNearestWP){
		if(!g_Map.m_Waypoints[m_iLastNearWP].isConnectedTo(m_iNearestWP)){
			if(g_Map.getReachable(g_Map.m_Waypoints[m_iLastNearWP].m_VOrigin,
				g_Map.m_Waypoints[m_iNearestWP].m_VOrigin)){
				g_Map.m_Waypoints.connect(m_iLastNearWP,m_iNearestWP);
			}
			else{
				checkRec4Connection(m_iLastNearWP,m_iNearestWP);
			}
		}
		if(!g_Map.m_Waypoints[m_iNearestWP].isConnectedTo(m_iLastNearWP)){
			if(g_Map.getReachable(g_Map.m_Waypoints[m_iNearestWP].m_VOrigin,
				g_Map.m_Waypoints[m_iLastNearWP].m_VOrigin)){
				g_Map.m_Waypoints.connect(m_iNearestWP,m_iLastNearWP);
			}
		}
	}
}

bool CObserverItem::checkDistances(void){
	PROFILE("checkDistances");
	// check if the bot is really near to a waypoint
	if(m_fNearestDistance < 50*50){
		addNearPath();
		m_iRecItems = 0;			// reset recording
		m_iLastLastNearWP = m_iLastNearWP;
		m_iLastNearWP = m_iNearestWP;
		return true;
	}
	else{
		if(m_pPlayer->getFallSpeed() < PLAYER_MAX_SAFE_FALL_SPEED){
			if(m_bOnLadder){
				if(m_fNearestDistance > g_fD2NWP_Ladder*g_fD2NWP_Ladder){
					m_iLastAddedWP = placeWaypoint(m_pPlayer->getOrigin(),CWaypoint::LADDER);
				}
			}
			else{
				if(m_fLastNearDistance > g_fD2NWP*g_fD2NWP
					&& m_fHeight < 50){
						m_iLastAddedWP = placeWaypoint(m_pPlayer->getOrigin());
						return true;
					}
				else if(m_fNearestDistance > g_fD2NWP*g_fD2NWP
					&& m_fHeight < 50){
						m_iLastAddedWP = placeWaypoint(m_pPlayer->getOrigin());
						return true;
					}
			}
		}
		else{
			// bot is falling ... do not add any paths or waypoints

			m_iLastNearWP = -1;
			m_iLastAddedWP = -1;

			m_iRecItems = 0;		// stop recording
		}
	}
	return false;
}

bool CObserverItem::checkLadders(void){
	////PROFILE("checkLadders");
	bool bIsOnLadder = m_pPlayer->isOnLadder();

	if(bIsOnLadder){
		if(m_bOnLadder){
		}
		else{
			// just got on ladder
			if(m_fNearestDistance > 2500){
				m_iLastAddedWP = placeWaypoint(m_pPlayer->getOrigin(),CWaypoint::LADDER);
				return true;
			}
		}
	}
	else{
		if(m_bOnLadder){
			// just got off ladder
			if(m_fNearestDistance > 900){
				m_iLastAddedWP = placeWaypoint(m_pPlayer->getOrigin());
				return true;
			}
		}
	}
	m_bOnLadder = bIsOnLadder;
	return false;
}

bool CObserverItem::checkCorners(void){
	PROFILE("checkCorners");
	if(m_iLastNearWP != -1){
		if(!g_pGame->fVisible(g_Map.m_Waypoints[m_iLastNearWP].m_VOrigin,m_pPlayer->getEntity())){
			if(m_iRecItems)
				m_iLastAddedWP = placeWaypoint(m_pRec[m_iRecItems-1].m_VOrigin);
			else
				m_iLastAddedWP = placeWaypoint(m_VLastOrigin);
			return true;
		}
	}
	return false;
}

bool CObserverItem::postProcess(void){
	////PROFILE("postProcess");
	m_VLastOrigin = m_pPlayer->getOrigin();
	return true;
}
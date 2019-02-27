// Observer.cpp: implementation of the CObserver class.
//
//////////////////////////////////////////////////////////////////////

#include "Observer.h"

#include "Game.h"
#include "Map.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CObserver::CObserver(){
	m_bCheckBasicWPs = false;
	m_fFCheck = 0;
	m_bFCheck = false;
}

CObserver::~CObserver(){
}

void CObserver::observe(void){
	//return;
	PROFILE("CObserver::observe");

	if(m_fFCheck < g_pGame->getTime()){
		m_bFCheck = true;
		m_fFCheck = g_pGame->getTime() + .1f;
	}
	else
		m_bFCheck = false;

	if(!m_bCheckBasicWPs){
		if(!g_Map.m_Waypoints.m_iNumWaypoints){
			g_Map.createBasicWaypoints();
			m_bCheckBasicWPs = true;
			return;
		}
	}

	int i;
	CPlayer *p;
	for(i = g_pGame->getMaxCurrentPlayer(); i >= 0; i--){
		if(p = g_pGame->getPlayer(i)){
			m_pOData[i].m_bFCheck = m_bFCheck;
			m_pOData[i].observe(p);
		}
	}
}

void CObserver::reset(void){
	m_bCheckBasicWPs = false;

	for(int i = 31; i >= 0; i--){
		m_pOData[i].reset();
	}
}
#include <math.h>
#include "Waypoint.h"

#include "globalfunc.h"
#include "util.h"

#include "Map.h"
#include "Game.h"
#include "Perceipt.h"
#include "InterestingEntity.h"
#include <list>
using namespace std;

long g_lBeam;

CWaypoint::CWaypoint(){
	m_lFlags = DELETED;
	m_fDisplayNext = 0;

	m_iVisibleWaypoints = 0;
	m_fAvDistVWP = 0;
}

CWaypoint::~CWaypoint(){
}

void CWaypoint::resetPaths(void){
	CPath::reset();
}

void CWaypoint::addVisibleWP(CWaypoint *pVWP){
	float fVWPsm1 = float(m_iVisibleWaypoints);
	++m_iVisibleWaypoints;
	float fVWPs = float(m_iVisibleWaypoints);

	m_fAvDistVWP = (m_fAvDistVWP * fVWPsm1 + (pVWP->m_VOrigin-m_VOrigin).length()) / fVWPs;		// calculate the average distance
}

bool CWaypoint::removeLastConnection(void){
	CPath *p,*pL;
	int i;
	bool bDeleted = false;

	pL = 0;
	p = this;

	while(p){
		if(!p->m_pNext){
			for(i=_MAX_PATHS-1;i >=0;i--){
				if(p->m_piPathTo[i] != -1){
					p->m_piPathTo[i] = -1;
					if(pL && !i){				// this path is empty and not the first one : let's delete it
						delete p;
						bDeleted = true;

						pL ->m_pNext = 0;
						p = pL;
					}
					return true;
				}
			}
			/*if(!bDeleted && pL){
				delete p;
				bDeleted = true;
				
				pL ->m_pNext = 0;
				p = pL;
				return true;
			}*/
		}
		pL = p;
		p = p->m_pNext;
	}

	return false;
}

bool CWaypoint::removeConnectionTo(int iRemoveConnection){
	CPath *p,*pL;
	int i,iLastConnection;

	pL = 0;
	p = this;

	while(p){
		for(i=0; i<_MAX_PATHS;i++){
			if(iRemoveConnection == p->m_piPathTo[i]){
				iLastConnection = getLastConnection();
				if(iLastConnection != iRemoveConnection){
					p->m_piPathTo[i] = iLastConnection;
				}
				else{
				}
				removeLastConnection();
				return true;
			}
		}
		pL = p;
		p = p->m_pNext;
	}

	return false;
}

bool CWaypoint::drawTo(edict_t *pEntity){
#ifdef __HLENGINE
	Vector VStart,VEnd;
	int ir = 255, ig = 255, ib = 255;

	if(gpGlobals->time < m_fDisplayNext)
		return true;

	if(m_lFlags & CROUCH){
		VStart = m_VOrigin - Vector(0,0,17);
		VEnd = VStart + Vector(0,0,34);
		ib = 0;
	}
	else{
		VStart = m_VOrigin - Vector(0,0,34);
		VEnd = VStart + Vector(0,0,68);
	}

	if(m_lFlags & AMMO){
		UTIL_DrawBeam(pEntity,VEnd+Vector(0,10,0),VEnd-Vector(0,10,0),10,0,255,0,0,255,1);
	}

	if(m_lFlags & WEAPON){
		UTIL_DrawBeam(pEntity,VEnd+Vector(10,0,0),VEnd-Vector(10,0,0),10,0,0,255,0,255,1);
	}
	if(m_lFlags & HEALTH){
		UTIL_DrawBeam(pEntity,VEnd+Vector(10,10,0),VEnd-Vector(10,10,0),10,0,0,0,255,255,1);
	}

	if(m_lFlags & LADDER){
		UTIL_DrawBeam(pEntity,VEnd+Vector(-10,-10,0),VEnd+Vector(10,10,0),10,0,0,0,255,255,1);
		UTIL_DrawBeam(pEntity,VEnd+Vector(-10,10,0),VEnd+Vector(10,-10,0),10,0,0,0,255,255,1);
	}

	if(UTIL_DrawBeam(pEntity,VStart,VEnd,10,0,ir,ig,ib,255,1)){
		m_fDisplayNext = gpGlobals->time + 1.1f;
		return false;
	}
	else{
		m_fDisplayNext = gpGlobals->time + 1.f;
		return true;
	}
#else
	return true;
#endif
}

void CWaypoint::save(FILE *fhd){
	fwrite(&m_lFlags,sizeof(long),1,fhd);
	fwrite(&m_VOrigin,sizeof(Vector),1,fhd);

	fwrite(&m_iVisibleWaypoints,sizeof(int),1,fhd);
	fwrite(&m_fAvDistVWP,sizeof(float),1,fhd);

	CPath::save(fhd);
	CWPStatItem::save(fhd);
}

void CWaypoint::load(FILE *fhd){
	m_fDisplayNext = 0;

	fread(&m_lFlags,sizeof(long),1,fhd);
	fread(&m_VOrigin,sizeof(Vector),1,fhd);

	fread(&m_iVisibleWaypoints,sizeof(int),1,fhd);
	fread(&m_fAvDistVWP,sizeof(float),1,fhd);

	CPath::load(fhd);
	CWPStatItem::load(fhd);
}

void CWaypoint::init(){
	m_lFlags = 0;
	m_fDisplayNext = 0;
	list<CIEntity*>::iterator iter;
	//CPerceipt *pPerceipt;

#ifdef __HLENGINE
	edict_t *p_Ent=0;
	TraceResult tr;

	while(p_Ent = UTIL_FindEntityInSphere(p_Ent,m_VOrigin,75)){	// fine, now even this is based on the gameclass data about interesting entities
		for(iter = g_pGame->m_LIEntities.begin(); iter != g_pGame->m_LIEntities.end(); iter ++){
			if((*iter)->compare(p_Ent)){
				if((*iter)->m_iContaining == CIEntity::HEALTH){
					m_lFlags |= HEALTH;
				}
				if((*iter)->m_iType == CIEntity::WEAPON){
					m_lFlags |= WEAPON;
				}
				if((*iter)->m_iType == CIEntity::AMMO){
					m_lFlags |= AMMO;
				}
			}
		}
	}

	UTIL_TraceLine(m_VOrigin,m_VOrigin+Vector(0,0,34),ignore_monsters,ignore_glass,0,&tr);
	if(tr.flFraction != 1.f){
		m_lFlags |= CROUCH;
		UTIL_TraceLine(m_VOrigin,m_VOrigin+Vector(0,0,-50),ignore_monsters,ignore_glass,0,&tr);
		m_VOrigin = tr.vecEndPos+Vector(0,0,18);
	}
#endif
}
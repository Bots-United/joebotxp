#include "Waypoint.h"

#include "globalfunc.h"
#include "util.h"

#include "Map.h"
#include "Game.h"
#include "Perceipt.h"
#include "InterestingEntity.h"
#include <list>
#include <iostream>
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
		g_pGame->drawBeam(0,pEntity,VEnd+Vector(0,10,0),VEnd-Vector(0,10,0),10,0,Vector(255,0,0),255,1);	// red
	}
	if(m_lFlags & WEAPON){
		g_pGame->drawBeam(0,pEntity,VEnd+Vector(10,0,0),VEnd-Vector(10,0,0),10,0,Vector(0,255,0),255,1);	// green
	}
	if(m_lFlags & HEALTH){
		g_pGame->drawBeam(0,pEntity,VEnd+Vector(10,10,0),VEnd-Vector(10,10,0),10,0,Vector(0,0,255),255,1);	// blue
	}
	if(m_lFlags & LADDER){																	// blue cross
		g_pGame->drawBeam(0,pEntity,VEnd+Vector(-10,-10,0),VEnd+Vector(10,10,0),10,0,Vector(0,0,255),255,1);
		g_pGame->drawBeam(0,pEntity,VEnd+Vector(-10,10,0),VEnd+Vector(10,-10,0),10,0,Vector(0,0,255),255,1);
	}
	if(m_lFlags & FLAG){																	// red cross
		g_pGame->drawBeam(0,pEntity,VEnd+Vector(-10,-10,0),VEnd+Vector(10,10,0),10,0,Vector(255,0,0),255,1);
		g_pGame->drawBeam(0,pEntity,VEnd+Vector(-10,10,0),VEnd+Vector(10,-10,0),10,0,Vector(255,0,0),255,1);
	}
	if(m_lFlags & FLAG_GOAL){																// green cross
		g_pGame->drawBeam(0,pEntity,VEnd+Vector(-10,-10,0),VEnd+Vector(10,10,0),10,0,Vector(0,255,0),255,1);
		g_pGame->drawBeam(0,pEntity,VEnd+Vector(-10,10,0),VEnd+Vector(10,-10,0),10,0,Vector(0,255,0),255,1);
	}

	if(g_pGame->drawBeam(0,pEntity,VStart,VEnd,10,0,Vector(ir,ig,ib),255,1)){
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

void CWaypoint::saveXML(FILE *fhd,int iNumber){
	fprintf(fhd,"<waypoint><index>%i</index>\n",iNumber);
	fprintf(fhd,"<flags>%li</flags>\n",m_lFlags);
	fprintf(fhd,"<origin>%.0f,%.0f,%.0f</origin>\n",m_VOrigin.x,m_VOrigin.y,m_VOrigin.z);

	fprintf(fhd,"<viswp>%li,%.2f</viswp>\n",m_iVisibleWaypoints,m_fAvDistVWP);
	
	CPath::saveXML(fhd);
	CWPStatItem::saveXML(fhd);

	fprintf(fhd,"</waypoint>\n");
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
	CEntity ent=0;
	TraceResult tr;

	while(ent = UTIL_FindEntityInSphere(ent,m_VOrigin,75)){	// fine, now even this is based on the gameclass data about interesting entities
		//cout << "-";
		for(iter = g_pGame->m_LIEntities.begin(); iter != g_pGame->m_LIEntities.end(); iter ++){
			if((*iter)->compare(ent)){
				//cout << (*iter)->m_szClassname << endl;
				if((*iter)->m_iContaining == CIEntity::HEALTH){
					m_lFlags |= HEALTH;
				}
				if((*iter)->m_iType == CIEntity::WEAPON){
					m_lFlags |= WEAPON;
				}
				if((*iter)->m_iType == CIEntity::AMMO){
					m_lFlags |= AMMO;
				}
				if((*iter)->m_iType == CIEntity::ZONE){
					//cout << "z";
					m_lFlags |= (*iter)->m_lWPFlag;
				}
			}
		}
	}

	//cout << "+" << endl;

	g_pGame->traceLine(m_VOrigin,m_VOrigin+Vector(0,0,34),ignore_monsters,ignore_glass,0,&tr);
	if(tr.flFraction != 1.f){
		m_lFlags |= CROUCH;
		g_pGame->traceLine(m_VOrigin,m_VOrigin+Vector(0,0,-50),ignore_monsters,ignore_glass,0,&tr);
		m_VOrigin = tr.vecEndPos+Vector(0,0,18);
	}
	else{
		g_pGame->traceLine(m_VOrigin,m_VOrigin+Vector(0,0,-34),ignore_monsters,ignore_glass,0,&tr);		// if it's not crouch wp, let's place it in a normal distance to the ground
		if(tr.flFraction != 1.f)
			m_VOrigin = tr.vecEndPos+Vector(0,0,34);
	}
#endif
}

void CWaypoint::copyStat(const CWaypoint &s){
	*((CWPStatItem *)(this)) = *((CWPStatItem*)(&s));
}
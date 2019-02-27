// CMap.cpp: implementation of the CMap class.
//
//////////////////////////////////////////////////////////////////////

#include "Map.h"
#include "Game.h"

#include "InterestingEntity.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMap g_Map;

CMap::CMap(){
}

CMap::~CMap(){
}

long CMap::createBasicWaypoints(void){
#ifdef __HLENGINE
	list<CIEntity *>::const_iterator iter;
	long lCount = 0;
	// Iterate through Interesting Entity list and process each element.
	for (iter=g_pGame->m_LIEntities.begin(); iter != g_pGame->m_LIEntities.end(); iter++){
		if((*iter)->m_bStatic){	// only static stuff
			CEntity ent=0;
			
			while(ent = UTIL_FindEntityByClassname(ent,(*iter)->m_szClassname)){	// find all entities of this type on the map
				if(!ent.getOwner()){					// don't add owned stuff - it's not ours ;)
					lCount += (*iter)->addWP(ent);	// let the interesting entity add a waypoint
				}
			}
		}
	}
	return lCount;
#else
	return 0;
#endif
}
extern edict_t *listenserver_edict;
bool CMap::getReachable(const Vector & V1, const Vector &V2){
#ifdef __HLENGINE
	TraceResult tr;
	Vector VDiff = V2-V1;
	float fLength = VDiff.length(),
		fCurrCheck = 0,fLastDepth = 50,fDepth,fDiff;
	Vector VDirection = VDiff*(1.f/fLength),
		VCheck,
		VCheckDown;

	/*if(fLength > 400)
		return false;*/

	// check for special case of both waypoints being underwater...
	if ((POINT_CONTENTS( V1 ) == CONTENTS_WATER) &&
		(POINT_CONTENTS( V2 ) == CONTENTS_WATER)){
		return true;
	}

	g_pGame->traceLine(V1,V2,0,&tr);

	if(tr.flFraction != 1.f)
		return false;

	while(fCurrCheck < fLength){
		VCheck = V1 + VDirection * fCurrCheck;
		VCheckDown = VCheck-Vector(0,0,1000);

		fCurrCheck += 10.f;

		g_pGame->traceLine(VCheck,VCheckDown,0,&tr);
		fDepth = tr.flFraction * 1000.f;

		//if(listenserver_edict) UTIL_DrawBeam(listenserver_edict,VCheck,tr.vecEndPos,5,0,255,255,255,255,0);

		fDiff = fLastDepth - fDepth;
		if(fDiff > 30.f){			// only 30 up
			return false;
		}
		if(fDiff < -100.f){		// and only 100 down
			return false;
		}
		fLastDepth = fDepth;
	}
	if(fLastDepth > 120.f)
		return false;

	return true;
#else
	return true;
#endif
}

bool CMap::load(const char *szPCMapname){
	char szName[255]="joebot/",szFileName[255];
	bool bReturn = false;

	if(szPCMapname){
		strcat(szName,szPCMapname);
	}
#ifdef __JOEBOTXPDLL
	else
		strcat(szName,g_pGame->getMapname());
#endif

	strcpy(szFileName,szName);
	strcat(szFileName,".wjx");
	bReturn |=! m_Waypoints.load(szFileName);
	/*if(!bReturn){
		strcpy(szFileName,szName);
		strcat(szFileName,".wsx");
		bReturn |=! m_WPStat.load(szFileName);
	}
	else
		m_WPStat.reset();*/

	return !bReturn;
}

bool CMap::save(const char *szPCMapname){
	char szName[255]="joebot/",szFileName[255];
	bool bReturn = false;

	if(szPCMapname){
		strcat(szName,szPCMapname);
	}
#ifdef __JOEBOTXPDLL
	else
		strcat(szName,g_pGame->getMapname());
#endif

	strcpy(szFileName,szName);
	strcat(szFileName,".wjx");
	bReturn |=! m_Waypoints.save(szFileName);
	/*strcpy(szFileName,szName);
	strcat(szFileName,".wsx");
	bReturn |=! m_WPStat.save(szFileName);*/

	return !bReturn;
}

void CMap::reset(void){
	m_Waypoints.reset();
	//m_WPStat.reset();
}
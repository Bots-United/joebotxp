// Entity.cpp: implementation of the CEntity class.
//
//////////////////////////////////////////////////////////////////////

#include "Entity.h"

#include "Game.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEntity::CEntity(){
	m_pEntity = 0;
}

CEntity::~CEntity(){
}

int CEntity::isInFOV(const Vector &VOrigin){
	Vector	 vecLOS;
	float    flDot;
	
	//UTIL_MakeVectors ( getAngles() );
	
	vecLOS = ( VOrigin - getOrigin());
	vecLOS = vecLOS.normalize();
	
	flDot = DotProduct (vecLOS , getAngles().angles2vec() );
	
	if ( flDot > 0.50 ){  // 60 degree field of view ( arccos .5 )
		return (1.f-flDot)* (255*2) + 1;
	}
	else{
		return 0;
	}
}

bool CEntity::isInLOS(const Vector &VOrigin){
	TraceResult tr;
	
	g_pGame->traceLine(getOrigin()+getVOffset(),VOrigin,ignore_monsters,ignore_glass,getEntity(),&tr);
	return (tr.flFraction == 1.f);
}

int CEntity::isInFOVof(CEntity *pPlayer){
	return pPlayer->isInFOV(getOrigin());
}

bool CEntity::isInLOSof(CEntity *pPlayer){
	return pPlayer->isInLOS(getOrigin() + getVOffset());
}

bool CEntity::views(const Vector &VOrigin){
	if(isInFOV(VOrigin)){
		if(isInLOS(VOrigin)){
			return true;
		}
	}
	return false;
}
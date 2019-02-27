// Action.cpp: implementation of the CActionHL class.
//
//////////////////////////////////////////////////////////////////////

#include "ActionHL.h"

#include "util.h"
#include "Game.h"

#include <iostream>

float g_fFrameTime;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CActionHL::CActionHL(){
	m_fMaxSpeed = CVAR_GET_FLOAT("sv_maxspeed");
}

CActionHL::~CActionHL(){	
}

void CActionHL::move(void){
	PROFILE("CActionHL::move");
	if(!m_pEntity){
		cout << "action call without entity" << endl;
		CRASH;
		return;
	}

	look();
	m_pEntity.getEntity()->v.button = m_lPressButton;

	if(m_fSlower > g_pGame->getTime()){
		m_fSpeed = m_fMaxSpeed * m_fSlowDownF;
	}
	else{
		m_fSpeed = m_fMaxSpeed;
	}
	
	if(m_bJump){
		m_bJump = false;
		m_pEntity.getEntity()->v.button |= IN_JUMP;
	}
	
	if(m_fDuck > g_pGame->getTime()){
		m_pEntity.getEntity()->v.button |= IN_DUCK;
	}

	if(m_fUseTill > g_pGame->getTime()){
		m_pEntity.getEntity()->v.button |= IN_USE;
	}
	
	if(m_fAttackPause < g_pGame->getTime()){
		if(m_fAttackTill > g_pGame->getTime()){
			m_pEntity.getEntity()->v.button |= IN_ATTACK;
		}
	}
	if(m_fAttack2Till > g_pGame->getTime()){
		m_pEntity.getEntity()->v.button |= IN_ATTACK2;
	}
	
	// call engine
	g_engfuncs.pfnRunPlayerMove( m_pEntity.getEntity(), m_VRunDirAngles, m_fSpeed,
		m_fStrafe, 0, m_pEntity.getEntity()->v.button, 0, msec());
}

void CActionHL::lookAlong(const Vector &VDirection,const Vector &VSpeed){
	//Vector v_angle = UTIL_VecToAngles(VDirection.normalize() );
	Vector v_angle = VDirection.vec2angles();
	
	// Paulo-La-Frite - START bot aiming bug fix
	if (v_angle.x > 180)
		v_angle.x -=360;
	
	if(v_angle.x > 85)
		v_angle.x = 85;
	if(v_angle.x < -85)
		v_angle.x = -85;
	if (v_angle.y > 180)
		v_angle.y -= 360;
	if (v_angle.y < -180)
		v_angle.y += 360;
	
	// set the body angles to point the gun correctly
	Vector angles;
	angles.x = v_angle.x / 3.0;
	angles.y = v_angle.y;
	angles.z = 0;
	
	// adjust the view angle pitch to aim correctly (MUST be after body v.angles stuff)
	v_angle.x = -v_angle.x;
	// Paulo-La-Frite - END
	
	// just copy the angles directly to entity data, that's the simplest way now
	m_VIdealAngles = angles;
	m_VIdealVAngle = v_angle;

	/*if(g_pGame->m_pListenserverEdict.getEntity())
	g_pGame->drawBeam(g_pGame->m_pListenserverEdict.getEntity(),m_pEntity.getEntity()->v.origin,VLookTo,4,0,Vector(255,255,255),255,0);*/
}

void CActionHL::look(void){
	CAction::look();

	m_pEntity.getEntity()->v.angles = m_VOutAngles;
	m_pEntity.getEntity()->v.v_angle = m_VOutVAngle;
}

float CActionHL::msec(void){
	// start tobias' code
	if ((m_fmsecStart+m_fmsecCount/1000) < (g_pGame->getTime()-0.5)){
		// after pause              
		m_fmsecStart = g_pGame->getTime() - 0.05;             
		m_fmsecCount = 0;             
		//debugMsg( "MSEC: SUPPOSED PAUSE\n" );       
	}
	if (m_fmsecStart > g_pGame->getTime()){
		// after map changes...             
		m_fmsecStart = g_pGame->getTime() - 0.05;             
		m_fmsecCount = 0;             
		//debugMsg( "MSEC: SUPPOSED MAPCHANGE\n" );       
	}
	float opt = (g_pGame->getTime()-m_fmsecStart) * 1000;            // optimal msec value since start of 1 sec period       
	m_fCurrentMSec = opt - m_fmsecCount;                              // value ve have to add to reach optimum       
	g_fFrameTime = m_fCurrentMSec/1000.f;                        // duration of last frame in sec       
	m_fmsecCount = opt;       
	if (m_fmsecCount > 1000){                                          
		// do we have to start a new 1 sec period?             
		m_fmsecStart += m_fmsecCount/1000;
		m_fmsecCount = 0;       
	}       // check from THE FATAL:       
	if (m_fCurrentMSec < 5) 
		m_fCurrentMSec = 5;     
	else{
		if (m_fCurrentMSec > 255){
			m_fCurrentMSec = 255;
		}
	}

	return int(m_fCurrentMSec); 
	//end tobias' code
}
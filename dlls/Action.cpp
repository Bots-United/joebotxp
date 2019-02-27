// Action.cpp: implementation of the CAction class.
//
//////////////////////////////////////////////////////////////////////

#include "Action.h"

#include "util.h"
#include "Game.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAction::CAction(){
	m_fStrafe = 0;
	m_fLastJump = 0;
	m_fJumpPause = 1.f;
	m_fDuck = 0;
	m_fSlower = 0;

	m_fAttackPause = 0;
	m_fAttack2Till = 0;
	m_fAttackTill = 0;
	m_fUseTill = 0;

	m_bJump = false;
	
	m_fmsecStart = 0;
	m_fmsecCount = 0;
	m_fCurrentMSec = 5;

	m_fSlowDownF = 1;

	m_VRunDirAngles = Vector(0,1,0);
	m_VAngLookSpeed = m_VOutAngles = m_VOutVAngle =
		m_VCurrentAngles = m_VCurrentVAngle = 
		m_VIdealAngles = m_VIdealVAngle = 
		m_VVAngleSpeed = m_VAnglesSpeed = Vector(0,0,0);
}

CAction::~CAction(){	
}

void CAction::reset(void){
	m_fLastJump =
		m_fJumpPause =
		m_fDuck =
		m_fSlower =
		m_fAttackPause =
		m_fAttackTill =
		m_fUseTill =
		m_fAttack2Till = 0.f;
}

bool CAction::jump(void){
	if(m_fLastJump + m_fJumpPause < g_pGame->getTime()){
		m_fLastJump = g_pGame->getTime();
		m_bJump = true;
		return true;
	}
	return false;
}

bool CAction::reload(void){
	pressButton(IN_RELOAD);
	return true;
}

bool CAction::duck(void){
	return duck(g_pGame->getTime() + .1f);
}

bool CAction::duck(float fDuckTill){
	m_fDuck = fDuckTill;
	return true;
}

bool CAction::duckO(float fDuck){
	return duck(g_pGame->getTime() + fDuck);
}

bool CAction::pauseAttack(float m_fPause){
	m_fAttackPause = m_fPause;
	return true;
}

bool CAction::pauseAttackO(float m_fPauseD){
	return pauseAttack(g_pGame->getTime() + m_fPauseD);
}

bool CAction::slowDown(float fSDownTill,float fFactor){
	m_fSlower = fSDownTill;
	m_fSlowDownF = fFactor;
	return true;
}

bool CAction::slowDownO(float fSDown,float fFactor){
	return slowDown(g_pGame->getTime() + fSDown,fFactor);
}

bool CAction::attack(void){
	return attack(g_pGame->getTime() + .1f);
}

bool CAction::attackO(float fTime){
	return attack(g_pGame->getTime() + fTime);
}

bool CAction::attack(float fAttackTill){
	m_fAttackTill = fAttackTill;
	return true;
}

bool CAction::use(void){
	return use(g_pGame->getTime() + .01f);
}

bool CAction::useO(float fTime){
	return use(g_pGame->getTime() + fTime);
}

bool CAction::use(float fUseTill){
	m_fUseTill = fUseTill;
	return true;
}

bool CAction::attack2(void){
	return attack2(g_pGame->getTime() + .001f);
}

bool CAction::attack2O(float fTime){
	return attack2(g_pGame->getTime() + fTime);
}

bool CAction::attack2(float fAttack2Till){
	m_fAttack2Till = fAttack2Till;
	return true;
}

void CAction::runTo(const Vector &VRunTo){
	m_VRunDir = (VRunTo - m_pEntity.getOrigin()).normalize();
	//m_VRunDirAngles = UTIL_VecToAngles(m_VRunDir);
	m_VRunDirAngles = m_VRunDir.vec2angles();

	m_VRunDirAngles.x = -m_VRunDirAngles.x;
}

bool CAction::ducked(void){
	return m_fDuck > g_pGame->getTime();
}

void CAction::lookTo(const Vector &VLookTo,const Vector &VSpeed){
	Vector VAngularSpeed;
	if(VSpeed.CBLength() < .00001){
		VAngularSpeed = Vector(0,0,0);
	}
	else{
		Vector VLookAngles, VLookSpeedAngles;

		VLookAngles = (VLookTo-m_pEntity.getOrigin()).vec2angles();
		VLookSpeedAngles = (VLookTo-m_pEntity.getOrigin() + VSpeed).vec2angles();

		VAngularSpeed = VLookSpeedAngles - VLookAngles;
		VAngularSpeed.clamp(clamp180180);

		g_pGame->debugMsg(DEBUG_SYSTEM,"%f %f %f\n",VAngularSpeed.x,VAngularSpeed.y,VAngularSpeed.z);
		g_pGame->drawBeam(DEBUG_SYSTEM,0,m_pEntity.getOrigin() + (VLookTo - m_pEntity.getOrigin()).length() * (VLookAngles).angles2vec(),m_pEntity.getOrigin() + (VLookTo - m_pEntity.getOrigin()).length() * (VAngularSpeed+VLookAngles).angles2vec());
	}

	lookAlong(VLookTo-CEntity(m_pEntity).getGunPosition(),VAngularSpeed);
}

void CAction::look(void){
	int i=2;
	switch(i){
	case 1:
		{
			float a=.95,		// momemtum
				idealSpeed,
				c=.02,			// overall_speed
				aNow,			// recalculated momentum based on framerate
				fADiff;
			float fmsecCount50 = m_fmsecCount / 50.0f;
			float fAngleChange;

			fADiff = abs(m_VIdealAngles.y - m_VCurrentAngles.y);
			if(fADiff>180.0f){if(m_VIdealAngles.y > 0){m_VIdealAngles.y -= 360.0f;}
			else{m_VIdealAngles.y += 360.0f;}}
			// compute the ideal angle speed (i.e, the amount of turning we WOULD want to do)
			idealSpeed = (m_VIdealAngles.y - m_VCurrentAngles.y);
			idealSpeed = idealSpeed*c;
			// compute the new momentum given the previous one
			aNow = pow(a,fmsecCount50);
			// compute the angle speed (i.e, the amount of turning we will actually achieve this frame)
			m_VAnglesSpeed.y = m_VAnglesSpeed.y*aNow + idealSpeed*(1.f-aNow);
			// calculate how big the actual angel change is
			fAngleChange = m_VAnglesSpeed.y * (fmsecCount50);
			m_VCurrentAngles.y += fAngleChange;
			// ((1.f - c) * fAngleChange) to compensate 'shooting behind enemy'
			m_VOutAngles.y = m_VCurrentAngles.y + (1.f - c) * fAngleChange;


			idealSpeed = (m_VIdealAngles.x - m_VCurrentAngles.x);
			idealSpeed = idealSpeed*c;
			aNow = pow(a, fmsecCount50);
			m_VAnglesSpeed.x = m_VAnglesSpeed.x*aNow + idealSpeed*(1.f-aNow);
			fAngleChange = m_VAnglesSpeed.x * (fmsecCount50);
			m_VCurrentAngles.x += fAngleChange;
			m_VOutAngles.x = m_VCurrentAngles.x + (1.f - c) * fAngleChange;


			fADiff = abs(m_VIdealVAngle.y - m_VCurrentVAngle.y);
			if(fADiff>180.0f){if(m_VIdealVAngle.y > 0){m_VIdealVAngle.y -= 360.0f;}
				else{m_VIdealVAngle.y += 360.0f;}}
			idealSpeed = (m_VIdealVAngle.y - m_VCurrentVAngle.y);
			idealSpeed = idealSpeed*c;
			aNow = pow(a, fmsecCount50);
			m_VVAngleSpeed.y = m_VVAngleSpeed.y*aNow + idealSpeed*(1.f-aNow);
			fAngleChange = m_VVAngleSpeed.y * (fmsecCount50);
			m_VCurrentVAngle.y += fAngleChange;
			m_VOutVAngle.y = m_VCurrentVAngle.y + (1.f - c) * fAngleChange;


			idealSpeed = (m_VIdealVAngle.x - m_VCurrentVAngle.x);
			idealSpeed = idealSpeed*c;
			aNow = pow(a, fmsecCount50);
			m_VVAngleSpeed.x = m_VVAngleSpeed.x*aNow + idealSpeed*(1.f-aNow);
			fAngleChange = m_VVAngleSpeed.x * (fmsecCount50);
			m_VCurrentVAngle.x += fAngleChange;
			m_VOutVAngle.x = m_VCurrentVAngle.x +  + (1.f - c) * fAngleChange;
		   }
		break;
	case 2:{
			float m = 1,
				k = -1,
				d = k*k / 4.f / m;

			if(abs(m_VIdealAngles.y - m_VCurrentAngles.y) > 180){
				if(m_VIdealAngles.y > 0){
					m_VIdealAngles.y -= 360.0f;
				}
				else{
					m_VIdealAngles.y += 360.0f;
				}
			}

			m_VAnglesSpeed.y += (k/m * (m_VAnglesSpeed.y - m_VAngLookSpeed.y) + d/m * (m_VIdealAngles.y - m_VCurrentAngles.y))*m_fmsecCount / 1000.f;
			m_VCurrentAngles.y += m_fmsecCount / 1000.f * m_VAnglesSpeed.y;

			m_VAnglesSpeed.x += (k/m * (m_VAnglesSpeed.x - m_VAngLookSpeed.x) + d/m * (m_VIdealAngles.x - m_VCurrentAngles.x)) * m_fmsecCount / 1000.f;
			m_VCurrentAngles.x += m_fmsecCount / 1000.f * m_VAnglesSpeed.x;

			//---------------------------

			if(abs(m_VIdealVAngle.y - m_VCurrentVAngle.y) > 180){
				if(m_VIdealVAngle.y > 0){
					m_VIdealVAngle.y -= 360.0f;
				}
				else{
					m_VIdealVAngle.y += 360.0f;
				}
			}

			m_VVAngleSpeed.y += (k/m * (m_VVAngleSpeed.y - m_VAngLookSpeed.y) + d/m * (m_VIdealVAngle.y - m_VCurrentVAngle.y))*m_fmsecCount / 1000.f;
			m_VCurrentVAngle.y += m_fmsecCount / 1000.f * m_VVAngleSpeed.y;

			m_VVAngleSpeed.x += (k/m * (m_VVAngleSpeed.x - m_VAngLookSpeed.x) + d/m * (m_VIdealVAngle.x - m_VCurrentVAngle.x))*m_fmsecCount / 1000.f;
			m_VCurrentVAngle.x += m_fmsecCount / 1000.f * m_VVAngleSpeed.x;

			m_VOutVAngle = m_VCurrentVAngle;
			m_VOutAngles = m_VCurrentAngles;
		   }
		   break;
	case 0:
	default:
		m_VOutAngles = m_VIdealAngles;
		m_VCurrentAngles = m_VIdealAngles;
		m_VOutVAngle = m_VIdealVAngle;
		m_VCurrentVAngle = m_VIdealVAngle;
		break;
	}
}
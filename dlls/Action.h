// Action.h: interface for the CAction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__ACTION_H)
#define __ACTION_H

#include "Entity.h"

class CAction  {
public:
	CAction();
	virtual	~CAction();

	void reset(void);

	bool jump(void);				// jump, check if it's useful again, and if	it is, jump	again
	bool reload(void);				// reload
	bool duck(void);				// duck	for	a .1s
	bool duck(float);				// duck	till a certain POT
	bool ducked(void);
	bool duckO(float);
	bool pauseAttack(float);		// pause attack	till a certain POT
	bool pauseAttackO(float);		// pause for param seconds
	bool slowDown(float	fSDown,float fFactor);
	bool slowDownO(float fSDown,float fFactor);
	float getSlowDown(void){return m_fSlowDownF;}			// THIS IS ONLY ACTIVE WHEN fSLower is > time
	float getSlowDownTime(void){return m_fSlower;}
	bool attack(void);				// attack for .1s
	bool attack(float);				// attack till
	bool attackO(float);

	bool attack2(void);				//	like attack, only for attack2
	bool attack2(float);
	bool attack2O(float);

	bool use(void);				// use for .1s
	bool use(float);			// use till
	bool useO(float);

	void pressButton(long lB){m_lPressButton |=	lB;}
	void releaseButton(long	lB=0xffffffff){m_lPressButton &=~ lB;}// default is	releasing all buttons

	void runTo(const Vector	&);
	const Vector &getRunDir(void){return m_VRunDir;}	// normalized run direction

	void setMaxSpeed(float fSpeed)		{m_fMaxSpeed = fSpeed;};
	float getMaxSpeed(void)				{return	m_fMaxSpeed;};
	float getSpeed(void)				{return	m_fSpeed;};
	void setStrafe(float fStrafe)		{m_fStrafe = fStrafe;}
	float getStrafe(void)				{return	m_fStrafe;}

	void setEntity(CEntity pEntity)	   {m_pEntity =	pEntity;};
	CEntity	getEntity(void)			   {return m_pEntity;};

	virtual	void move(void)=0;
	virtual	void look(void);
	virtual	void lookTo(const Vector &,const Vector &VSpeed = Vector(0,0,0));
	virtual	void lookAlong(const Vector &,const Vector &VSpeed = Vector(0,0,0))=0;

protected:
	float m_fMaxSpeed;			// max speed
	float m_fSpeed;				// current speed
	float m_fStrafe;
	float m_fLastJump;			// time	of last	jump
	float m_fJumpPause;			// pause between jumps	// more	or less	hardcoded
	float m_fDuck;				// duck	till
	float m_fSlower;
	float m_fSlowDownF;			// slow	down factor	till time >	m_fSlower

	float m_fAttackPause;		// time	till atack should be paused
	float m_fAttackTill;
	float m_fAttack2Till;
	float m_fUseTill;

	bool m_bJump;

	Vector m_VRunDir;			// this	is normalizes run direction
	Vector m_VRunDirAngles;
	long m_lPressButton;		// press this buttons till it's	changed

	// msec HL engine stuff
	float m_fmsecStart;
	float m_fmsecCount;
	float m_fCurrentMSec;
	// end msec HL engine stuff

	Vector m_VIdealAngles,
		m_VIdealVAngle;
	Vector m_VAngLookSpeed;
	Vector m_VCurrentAngles,
		m_VCurrentVAngle;
	Vector m_VOutAngles,
		m_VOutVAngle;

	/*float m_fViewSpeedYaw;
	float m_fViewSpeedPitch;
	float m_fAngleSpeedPitch;
	float m_fAngleSpeedYaw;*/

	Vector m_VVAngleSpeed;
	Vector m_VAnglesSpeed;

	CEntity	m_pEntity;
};

#endif // !defined(__ACTION_H)

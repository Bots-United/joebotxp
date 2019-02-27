// Entity.h: interface for the CEntity class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__ENTITY_H)
#define __ENTITY_H

#include "extdll.h"
#include "util.h"

class CBaseBot;
void FakeClientCommand(edict_t *pBot, char *arg1, char *arg2, char *arg3);

class CEntity  {
public:
	CEntity();
	CEntity(edict_t *p){m_pEntity = p;}
	operator edict_t*(){return m_pEntity;}
	virtual ~CEntity();

	void fakeClientCommand(char *arg1, char *arg2=0, char *arg3=0){
		::FakeClientCommand(getEntity(),arg1,arg2,arg3);
	}

	void setEntity(edict_t *pEntity){m_pEntity = pEntity;}
	edict_t *getEntity(void)const{return m_pEntity;}
	edict_t *getContainingEntity(void)const{return m_pEntity->v.pContainingEntity;}

	virtual CBaseBot* isBot(void){return 0;}

	long isBot(void)const       {return m_pEntity->v.flags&FL_FAKECLIENT;}
	bool isAlive(void)const     {if(m_pEntity){return ((m_pEntity->v.deadflag == DEAD_NO)
		&&(m_pEntity->v.health > 0) && (m_pEntity->v.movetype != MOVETYPE_NOCLIP));}else return false;}

	float getFallSpeed(void)const{return m_pEntity->v.flFallVelocity;}

	int getDefaultFOV(void)const{return 90;}
	Vector& getVelocity(void)   {return ((Vector &)(m_pEntity->v.velocity));}
	Vector& getOrigin(void)     {return ((Vector &)(m_pEntity->v.origin));}
	Vector& getVAngle(void)     {return ((Vector &)(m_pEntity->v.v_angle));}
	Vector& getAngles(void)     {return ((Vector &)(m_pEntity->v.angles));}
	Vector& getVOffset(void)    {return ((Vector &)(m_pEntity->v.view_ofs));}
	Vector& getPunchAngle(void) {return ((Vector &)(m_pEntity->v.punchangle));}
	Vector getGunPosition(void) {return (getOrigin() + getVOffset());}
	Vector &getAbsMax(void)		{return m_pEntity->v.absmax;}
	Vector &getAbsMin(void)		{return m_pEntity->v.absmin;}
	Vector &getSize(void)		{return m_pEntity->v.size;}
	Vector getBModelOrigin(void){return getAbsMin() + (getSize() * 0.5f);}
	long isAttacking(void)const {return m_pEntity->v.button & IN_ATTACK;}
	long isAttacking2(void)const{return m_pEntity->v.button & IN_ATTACK2;}
	long isDucking(void)const   {return m_pEntity->v.button & IN_DUCK;}
	long isJumping(void)const   {return m_pEntity->v.button & IN_JUMP;}

	int isInFOV(const Vector &);
	bool isInLOS(const Vector &);
	int isInFOVof(CEntity *);
	bool isInLOSof(CEntity *);
	bool views(const Vector &);

	bool isOnLadder(void)const  {return m_pEntity->v.movetype == MOVETYPE_FLY;}
	int getWaterlevel(void)const{return m_pEntity->v.waterlevel;}
	int getHealth(void)const    {return (int)m_pEntity->v.health;}
	const char *getWeaponModel(void)    {return STRING(m_pEntity->v.weaponmodel);}
	const char *getClassName(void)  {return STRING(m_pEntity->v.classname);}
	const char *getName(void)   {return STRING(m_pEntity->v.netname);}
	const char *getModel(void)  {return STRING(m_pEntity->v.model);}
	virtual int getTeam(void)   {return m_pEntity->v.team;}
	CEntity getOwner(void)      {return CEntity(m_pEntity->v.owner);}

	bool operator ==(CEntity p){return getEntity() == p.getEntity();}
	const CEntity & operator =(edict_t *p){m_pEntity = p;return ((const CEntity &)(*this));}
private:
	edict_t *m_pEntity;
};

#endif // !defined(__ENTITY_H)

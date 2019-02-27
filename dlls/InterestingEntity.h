// CIEntity.h: interface for the CIEntity class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__INTERESTINGENTITY_H)
#define __INTERESTINGENTITY_H

#include "extdll.h"

class CPerceipt;
class CEntity;

class CIEntity  
{
public:
	CIEntity();
	CIEntity(const char *,bool);
	virtual ~CIEntity();

	bool compare(CEntity&);
	void assign(CEntity&,CPerceipt *);

	virtual bool isValid(CEntity&);		// looks if this entity is valid - for weapons e.g. look if there's an owner ... if there is one, someone carries it, it's uninteresting

	virtual Vector getWPOrigin(CEntity&);	// get the location ... may not be pEdict->v.origin for some enteties
	virtual const Vector &getOrigin(CEntity&);	// get location - for distance and visibility checks
	virtual long addWP(CEntity&);			// add a waypoint at this entity

	char m_szClassname[30];		// classname
	int m_iType;				// type of entity
	int m_iWeaponId;			// weapon ID ( for weapon enteties )
	int m_iAmmoId;				// ammo id ( for ammo enteties )
	int m_iContaining;			// health, armor, whatever
	bool m_bStatic;				// if true, you are allowed to place a waypoint there
	long m_lWPFlag;				// waypoint flag to set -> for ZONE Type IEntities
	//float m_fDistance;		// distance to next wp if placing a waypoint
	int m_iId;					// all interesting enteties are numbered

	// define types of interesting enteties ...
	enum types{
		NONE = 0,
		WEAPON,
		WEAPONWBX,
		AMMO,
		CHARGER,
		ITEM,
		LADDER,
		ZONE
	};
	enum containing{
		HEALTH = 1,
		ARMOR
	};
};

class CInterestingWeapon:public CIEntity{
public:
	CInterestingWeapon();
	CInterestingWeapon(const char *,int,bool);
	virtual ~CInterestingWeapon();
	virtual bool isValid(CEntity &);
};

class CInterestingWeaponboxWeapon:public CIEntity{
public:
	CInterestingWeaponboxWeapon();
	CInterestingWeaponboxWeapon(const char *,int,bool);
	virtual ~CInterestingWeaponboxWeapon();
	virtual bool isValid(CEntity &);
	virtual const Vector &getOrigin(CEntity &);
};

class CInterestingAmmo:public CIEntity{
public:
	CInterestingAmmo();
	CInterestingAmmo(const char *,int,bool);
	virtual ~CInterestingAmmo();
};

class CInterestingItem:public CIEntity{
public:
	CInterestingItem();
	CInterestingItem(const char *,int,bool);
	virtual ~CInterestingItem();
};

class CInterestingCharger:public CIEntity{
public:
	CInterestingCharger();
	CInterestingCharger(const char *,int,bool);
	virtual ~CInterestingCharger(){}
	virtual Vector getWPOrigin(CEntity &);
};

class CInterestingLadder:public CIEntity{
public:
	CInterestingLadder();
	CInterestingLadder(const char *);
	virtual ~CInterestingLadder(){}
	//virtual Vector getWPOrigin(CEntity &);
	virtual long addWP(CEntity &);
};

class CInterestingZone:public CIEntity{
public:
	CInterestingZone();
	CInterestingZone(const char*,const long);
	~CInterestingZone();
	virtual bool isValid(CEntity&);// looks if this entity is valid - here it's always true, zones are normally not visible anyway
};

#endif // !defined(__INTERESTINGENTITY_H)

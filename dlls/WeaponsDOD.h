// WeaponsDOD.h: interface for the CWeaponsDOD class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__WEAPONSDOD_H)
#define __WEAPONSDOD_H

#include "Weapons.h"

class CWeaponsDOD : public CWeapons  
{
public:
	CWeaponsDOD();
	virtual ~CWeaponsDOD();

	virtual long isKnife(long);
	virtual long isPrimary(long);
	virtual long isSecondary(long);
	virtual long isGrenade(long);
	virtual long isSniper(long);
	virtual long isShotgun(long);
	virtual long isSubM(long);
	virtual long isRifle(long);
	virtual long isMachineGun(long);
};

#endif // !defined(__WEAPONSDOD_H)

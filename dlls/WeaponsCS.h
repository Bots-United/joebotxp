// WeaponsCS.h: interface for the CWeaponsCS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__WEAPONSCS_H)
#define __WEAPONSCS_H

#include "Weapons.h"

class CWeaponsCS : public CWeapons  
{
public:
	CWeaponsCS();
	virtual ~CWeaponsCS();

	virtual long isKnife(long);
	virtual long isPrimary(long);
	virtual long isSecondary(long);
	virtual long isGrenade(long);
	virtual long isExplGrenade(long);
	virtual long isSniper(long);
	virtual long isShotgun(long);
	virtual long isSubM(long);
	virtual long isRifle(long);
	virtual long isMachineGun(long);
};

#endif // !defined(__WEAPONSCS_H)

// PlayerCS.h: interface for the CPlayerCS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__PLAYERCS_H)
#define __PLAYERCS_H

#include "Player.h"

// we need this class for an implementation of the getTeam code.
// counter-strike doesnt use the normal v.team, but instead you have to get
// the information via the player models.

class CPlayerCS : virtual public CPlayer  
{
public:
	//CPlayerCS();
	//virtual ~CPlayerCS();

	virtual int getTeam(void);
};

#endif // !defined(__PLAYERCS_H)

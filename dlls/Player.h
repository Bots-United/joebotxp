// CPlayer.h: interface for the CPlayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__PLAYER_H)
#define __PLAYER_H

#include "Entity.h"
#include "Action.h"
class CBaseBot;

class CPlayer : virtual public CEntity{
public:
	CPlayer();
	CPlayer(edict_t *pEntity){setEntity(pEntity);}
	virtual ~CPlayer();

	int m_iNearestWP;
};

#endif // !defined(__PLAYER_H)

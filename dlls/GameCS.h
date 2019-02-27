// GameCS.h: interface for the CGameCS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__GAMECS_H)
#define __GAMECS_H

#include "GameHL.h"

class CGameCS : public CGameHL  
{
public:
	CGameCS();
	virtual ~CGameCS();

	virtual CBaseBot *allocBot(void);
	virtual CPlayer *allocPlayer(void);
	virtual CBaseTeam *allocTeamData(void);

	virtual bool getTeamplay(void);
	virtual long weaponModel2ID(const char *);

	virtual void onRoundStart(void);

	int m_iIEntityIdHostage;
	int m_iIEntityIdGrenade;
};

#endif // !defined(__GAMECS_H)
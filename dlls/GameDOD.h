// GameDOD.h: interface for the CGameDOD class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__GAMEDOD_H)
#define __GAMEDOD_H


#include "GameHL.h"

class CGameDOD : public CGameHL
{
public:
	CGameDOD();
	virtual ~CGameDOD();

	virtual CBaseBot *allocBot(void);

	virtual bool createBot(const char *,const char *,const char *,const char *);
	virtual bool getTeamplay(void);
};

#endif // !defined(__GAMEDOD_H)

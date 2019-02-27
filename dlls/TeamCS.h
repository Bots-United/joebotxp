#ifndef __TEAMDATACS_H
#define __TEAMDATACS_H

#include "BaseTeam.h"

class CTeamCS :
	public CBaseTeam
{
public:
	CTeamCS(void);
	virtual ~CTeamCS(void);

	virtual void processMessage(CBaseBot *pBot,int iPlayerIndex, int iType, int iOtherPlayerIndex=-1, int iNWP=-1);
};

#endif __TEAMDATACS_H
// GameHLDM.h: interface for the GameHLDM class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__GAMEHLDM_H)
#define __GAMEHLDM_H

#include "GameHL.h"

class CGameHLDM:public CGameHL{
public:
	CGameHLDM();
	virtual ~CGameHLDM();

	virtual CBaseBot *allocBot(void);

	int m_iIEntityIdWBox;
};

#endif // !defined(__GAMEHLDM_H)

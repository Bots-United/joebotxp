// BotDOD.h: interface for the CBotDOD class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__BOTDOD_H)
#define __BOTDOD_H

#include "BaseBot.h"

class CBotDOD : public CBaseBot
{
public:
	CBotDOD();
	virtual ~CBotDOD();

	virtual void menu(void);

	bool m_bBleeding;
	bool m_bSlowed;
	int m_iStamina;
};

#endif // !defined(__BOTDOD_H)

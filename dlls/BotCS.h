// BotCS.h: interface for the CBotCS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__CBOTCS_H)
#define __CBOTCS_H

#include "BaseBot.h"
#include "Radio.h"
#include "Entity.h"
#include "PlayerCS.h"

class CBotCS : public CBaseBot, virtual public CPlayerCS
{
public:
	CBotCS();
	virtual ~CBotCS();

	virtual void menu(void);
	virtual void init(void);

	float m_fRoundTime;			// round time
	int m_iMoney;				// money the bot has
	bool m_bInBuyZone;			// is in buy zone
	bool m_bHasDefuser;			// has a defuser
	bool m_bInBombZone;			// is in bomb zone
	bool m_bGlockBurst;			// is the glock using burst fire ?
	CRadio m_Radio;				// last heard and not already processed radio command
	list <CEntity> m_LHostages;
};

#endif // !defined(__CBOTCS_H)

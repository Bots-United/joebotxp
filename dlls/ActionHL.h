#if !defined(__ACTIONHL_H)
#define __ACTIONHL_H

#include "Action.h"
#include "extdll.h"

class CActionHL : public CAction {
public:
	CActionHL();
	virtual ~CActionHL();

	void move(void);
	void look(void);

	void lookAlong(const Vector &,const Vector &VSpeed = Vector(0,0,0));

	void setMaxSpeed(float fSpeed)      {m_fMaxSpeed = fSpeed;};
	float getMaxSpeed(void)             {return m_fMaxSpeed;};
	float getSpeed(void)                {return m_fSpeed;};
	void setStrafe(float fStrafe)       {m_fStrafe = fStrafe;}
	float getStrafe(void)               {return m_fStrafe;}

protected:
	float msec(void);
};

#endif // !defined(__ACTIONHL_H)

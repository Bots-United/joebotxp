#ifndef __BOTRADIO_H
#define __BOTRADIO_H

#include "extdll.h"

// radio commands
enum general_messages{
	RC_COVER_ME=0,
	RC_YOU_TAKE_THE_POINT,
	RC_HOLD_THIS_POSITION,
	RC_REGROUP_TEAM,
	RC_FOLLOW_ME,
	RC_TAKING_FIRE,
	RC_GO_GO_GO,
	RC_FALL_BACK,
	RC_STICK_TOGETHER,
	RC_GET_IN_POSITION,
	RC_STORM_THE_FRONT,
	RC_REPORT_IN_TEAM,
	RC_AFFIRMATIVE,
	RC_ENEMY_SPOTTED,
	RC_NEED_BACKUP,
	RC_SECTOR_CLEAR,
	RC_IN_POSITION,
	RC_REPORTING_IN,
	RC_GET_OUT_OF_THERE,
	RC_NEGATIVE,
	RC_ENEMY_DOWN,
	RC_TEAMATTACK,
	RC_HOSTAGE_DOWN,
	RC_FIRE_IN_THE_HOLE,
	RC_SNIPER_SPOTTED,
	RC_DIED,
	RC_RESPAWNED,
	RC_ENGAGING_ENEMY,
	RC_PICKUPWEAPON,
	RC_MAX
};

class CPlayer;

class CRadio{
public:
	CRadio();
	~CRadio();

	float f_Time;
	//edict_t *pECalling;
	CPlayer *pPCalling;
	long lMessage;
protected:
private:
};

#endif  __BOTRADIO_H


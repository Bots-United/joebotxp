// Perceipt.h: interface for the CPerceipt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__PERCEIPT_H)
#define __PERCEIPT_H

#include "extdll.h"
#include "util.h"

#include "Game.h"

class CPerceipt  
{
public:
	CPerceipt();
	virtual ~CPerceipt();
	void reset(void);
	void kill(void);

	long	m_lType;				// type of perception
	long	m_lTypeSpec;			// additional type information
	float	m_fLUpdate;				// last time of an update
	float	m_fLifetime;			// Lifetime for this perceipt
	float	m_fDistance;			// distance
	const   CIEntity*m_pIEntity;	// if it's related to an interesting entity, this points to it
	bool	m_bLTMem;				// is it an LTMem perceipt instance ?
	CEntity m_pEntity;				// related entity
	CPlayer *m_pPlayer;				// if it's a player
	float	m_fValue;				// value. this can be e.g. the loudness of a sound ...
	Vector  m_VOrigin;				// somewhen we just need an origin ?
	int		m_iAddInfo;				// additional informaiton, maybe something like how big the healthpack e.g. is
	int		m_iAddInfo1;			// additional information, the second
	void	*m_pPointer;			// misc pointer
	void	*m_pPointer1;			// misc pointer 2

	float	m_fCreate;				// time this perception has been created, i.e. the time this was first perceipt

	enum PERCEIPT_TYPES{		// that's what we got here, it's
		PT_NONE=0,					// nothing
		PT_ENEMY,					// an enemy
							// m_iAddInfo is player index in game class
		PT_TMATE,					// a teammate
							// m_iAddInfo is player index in game class
		PT_DAMAGE,					// damage taken
		PT_ENTITY,					// some entity
		PT_SOUND,					// or some sound
		PT_KILL,					// a kill has happened
		PT_ACTION,					// some behaviours might add a perception to be able to perform sequences of actions. You can regard this as a memory item which reminds the bots of continue his 'work'
		PT_COMMAND
	};

	enum PERCEIPT_TYPES_EX{		// additional information for the types
		PTX_SOUND_LOC		= (1<<0),		// PT_SOUND EXT	// location is known
		PTX_SOUND_NLOC		= (1<<1),						// location is unknown
		PTX_SOUND_WALK		= (1<<2),						// somebody is walking
		PTX_SOUND_SHOOT		= (1<<3),						// somebody is shooting
		PTX_SOUND_ENEMY		= (1<<4),						// sound created by an enemy player
		PTX_SOUND_TMATE		= (1<<5),						// sound created by a tmate

		PTX_SHOOT			= (1<<6),		// PT_ENEMY		// enemy shoots
		PTX_SHOOTATME		= (1<<7),						// somebody is shooting at me :|
		PTX_LOOKATME		= (1<<8),						// enemy looks at me

		PTX_DAMAGE_STRANGE	= (1<<9),		// damage is done to the bot, but it's not attacked.

										//---------- only to be used with PT_ENTITY ----------
		PTX_ENTITY_AMMO		= (1<<0),
		PTX_ENTITY_WEAPON	= (1<<1),
		PTX_ENTITY_WEAPONWBX= (1<<2),
		PTX_ENTITY_ARMOR	= (1<<3),
		PTX_ENTITY_HEALTH	= (1<<4),
		PTX_ENTITY_ARMORC	= (1<<5),		// ammocharger
		PTX_ENTITY_HEALTHC	= (1<<6),		// healthcharger
		PTX_ENTITY_LADDER	= (1<<7),

										//---------- only to be used with PT_ACTION ----------
		PTX_ACTION_ENEMY	= (1<<0),
		PTX_ACTION_PICKUP	= (1<<1),
		PTX_ACTION_CAMP		= (1<<2),
		PTX_ACTION_SUPPFIRE	= (1<<3),		// iAddInfo may contain lowest allowed ammo count if >0

										//---------- only to be used with PT_COMMAND ----------
		PTX_COMMAND_SQUAD	= (1<<0),		// general flag that this command is some squad command
		PTX_COMMAND_TEAM	= (1<<1),		// general flag that this command is some squad command
		PTX_COMMAND_GOTO	= (1<<2),		// goto location (m_iAddInfo)
		PTX_COMMAND_GOTODEF	= (1<<3),		// goto location ( m_pPointer contains a pointer to a AStarMachine, pPointer1 may contain a callback function onReachedDestination)
		PTX_COMMAND_GOSTAY	= (1<<4),		// goto location ( done via goto stuff ) and stay there for m_fValue seconds or until gogogo command
		PTX_COMMAND_GOGOGO	= (1<<5),		// aggresive attack to m_iAddInfo ( if needed further informaiton )
		PTX_COMMAND_COVER	= (1<<6),		// special attention to m_iAddInfo location for m_fValue seconds	- nyi
		PTX_COMMAND_SDOWN	= (1<<7),		// slow down ... for m_iAddInfo seconds to m_fValue /1 of the normal speed
		PTX_COMMAND_STCAMP	= (1<<8),		// stop camping
		PTX_COMMAND_DUCK	= (1<<9),

		PTX_COMMAND_IN_WORK = (1<<28),
		PTX_COMMAND_INTERRUPTED = (1<<29),	// set flag if the behaviour has been deinitialized without being finished



		PTX_ATTACKED		= (1<<30),
		PTX_INVALID			= (1<<31),
		PTX_ASTARINSIDE		= PTX_COMMAND_GOTODEF,	// PT_COMMAND containing AStarMachines
	};
};

#endif // !defined(__PERCEIPT_H)

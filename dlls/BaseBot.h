// CBaseBot.h: interface for the CBaseBot class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__BASEBOT_H)
#define __BASEBOT_H

#include "Goal.h"
#include "Player.h"
#include "Action.h"
#include "Way.h"
#include "GoalFinder.h"
#include "Weapons.h"
#include "globaldefs.h"

class CPerceptionBot;
class CBehaviour;
class CEmotion;
class CWeapons;
class CEntity;
class CBVNP_HLDM_MovePath;
class CBV_HLDM_SquadCommand;
class CEM_HLDM_Team;
class CEM_HLDM_Enemy;
class CEM_HLDM_LastEnemy;
class CBVNP_HLDM_Camp;

#include "AStarMachine.h"
#include "AStarGoalMod.h"
#include "AStarTaskManager.h"

#define WEAPONB(a) (1<<a)

class CBaseBot : virtual public CPlayer{
public:
	CBaseBot();
	virtual ~CBaseBot();

	virtual CBaseBot* isBot(void){return this;}		// this is a bot

	virtual void init(void);					// init on respawn
	virtual bool think(void);					// think
	virtual void menu(void);					// make menuselect calls to handle the menues
	virtual void callBehaviours(void);			// call bots behaviours and emotions
	virtual void resetBehaviours(void);			// call the reset function of behaviours
	virtual bool checkDeath(void);				// check if the bot is dead
	virtual long hasWeapon(long lWeapon){return m_pWeapons->m_iWeapons&(1<<lWeapon);}
	virtual long hasWeaponB(long lWeaponBit){return m_pWeapons->m_iWeapons&lWeaponBit;}
	virtual void change2Weapon(long lWeapon);	// change to param weapon
	virtual int isZooming(void)
		{return m_iFOV < 90?1:0;}
	//int getTeam(void){ return m_iTeam; };

	virtual AStarBase *getAStarMachine(void);	// this returns an appropriate AStarMachine

	virtual void kill(edict_t *,long);			// is called when a bot has a kill
	virtual void killed(edict_t *,long);		// is called when the bot is killed

	virtual void addCostVisEnemies(AStarMachine<AStarGoalMod> *,float);

	virtual void assignSquad(int iSquad);
	virtual void assignTeam(int iTeam);

	virtual void onDeath(void);
	virtual void onRespawn(void);
	
	CAction *m_pAction;							// pointer to action class
	CPerceptionBot *m_pPerception;					// pointer to current perception class
	CWeapons *m_pWeapons;
	list<CEmotion*>		m_LpEmotion;			// 'emotions' or counting special entities and related variables
	list<CBehaviour*>	m_LpBehaviour;			// behaviours
	float m_fCalledBV;							// time the behaviours have been called
	CGoalFinder m_LGoals;						// goals 'produced' by behaviours

	CBVNP_HLDM_MovePath *m_pMovePath;			// movement 'behaviour'
	CBVNP_HLDM_Camp *m_pCamp;					// camp behaviour
	CBV_HLDM_SquadCommand *m_pSquadCommand;		// squad command behaviour
	CEM_HLDM_Team *m_pEMTeam;					// team 'emotion'
	CEM_HLDM_Enemy *m_pEMEnemy;					// enemy 'emotion'
	CEM_HLDM_LastEnemy *m_pEMLEnemy;

	float m_fNextThink;							// think every m_fThinkPer second
	float m_fThinkPer;							// period of 'thinking'
	bool m_bNeed2Init;
	
	int m_respawn_state;
	//CPlayer *m_pPlayer;
	bool m_need_to_initialize;
	bool m_bAlive;				// was this bot alive before calling think this time ?
	char m_name[BOT_NAME_LEN+1];
	char m_skin[BOT_SKIN_LEN+1];
	int m_not_started;
	int m_start_action;
	float m_create_time;		// when was this bot created
	int m_iFOV;					// current bot's field of view
	bool m_bFlashlight;			// is the flashlight switched on ?
	float m_fBlinded;
	int m_iSquad;				// the squad the bot belongs to
	
	int m_iClass;				// class the bot should join
	int m_iSkill;				// skill of bot
	int m_iJoinTeam;			// team the bot should join		// be careful, here is a difference between those variables
	int m_iTeam;				// team the bot is in			// 
	int m_iHealth;				// current bot's health
	int m_iArmor;				// armor of bot
	
	float m_prev_speed;
	float m_prev_time;
	Vector m_v_prev_origin;

	float m_fAggressivity;		// aggressivity

	//void fakeClientCommand(char *arg1, char *arg2=0, char *arg3=0);
};

#endif // !defined(__BASEBOT_H)

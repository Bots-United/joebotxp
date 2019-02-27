// CGame.h: interface for the CGame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__GAMEHL_H)
#define __GAMEHL_H

#include "Game.h"

void player( entvars_t *pev );

class CBaseBot;
class CBaseTeam;

class CGameHL:public CGame{
public:
	CGameHL();
	virtual ~CGameHL();

	virtual void debugMsg(int,const char *,...);
	virtual void userMsg(CEntity,const char *,...);

	virtual bool drawBeam(int,CEntity pEntity, const Vector &start, const Vector &end, int width,int noise, const Vector &color, int brightness, int speed,int iLife);

	//virtual void reset(void);				// general reset
	virtual void startFrame(void);			// called by the engine on each start of a frame
	virtual void checkMapChange(void);		// checks if a map change has happened
	virtual void updateClientData(void);

	virtual CPlayer *allocPlayer(void);		// allocate a player ...
	//virtual CPlayer *preAddPlayer(void);
	//virtual bool addPlayer(edict_t *);		// connecting a new player
	//virtual bool removePlayer(edict_t *);	// disconnecting a player
	virtual bool createBot(const char *,const char *,const char *,const char *);
	//int isPlayer(CEntity);				// is this edict_t * a player ?
	//int isPlayer(CPlayer *);				// is this CPlayer * a player ?
	virtual CBaseBot *allocBot(void);		// allocate a bot instance

	//CBaseBot **getFreeBotSlot(void);		// returns a free pointer for a bot in the bot array
	//void freeBotSlot(int);					// frees a bot slot - checks if this is valid and changes counter variables
	
	//virtual void onBotCreation(void);		// should be called after a bot has been created
	//virtual void onMapStart(void);			// should be called on map start
	//virtual void onMapEnd(void);			// should be called on map end
	//virtual void onRoundStart(void);		// should be called on round start

	virtual long weaponModel2ID(const char *){return -1;}
	//virtual long weaponClass2ID(const char *);


	virtual CBaseTeam *allocTeamData(void);
	//CBaseTeam *getTeamData(int);

	//vector <CBaseTeam*> m_LTeamData;

	//int getTeamMembers(int);	// returns number of members in a team

	virtual float getTime(void){return gpGlobals->time;}	// returning the current
	virtual const char * getMapname(void){return STRING(gpGlobals->mapname);};	// returning the name of the map
	//virtual bool getTeamplay(void);			// is this a teamplay mod ?
	//virtual CPlayer *getPlayer(int);		// get a CPlayer pointer to a player
	//virtual int getMaxCurrentPlayer(void);	// get maximum count of players

	virtual void traceLine(const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, IGNORE_GLASS ignoreGlass, CEntity pentIgnore, TraceResult *ptr);
	virtual void traceLine(const Vector &vecStart, const Vector &vecEnd, CEntity pentIgnore, TraceResult *ptr);
	virtual void traceHull(const Vector &vecStart, const Vector &vecEnd, HULLS hullNumber, edict_t *pentIgnore, TraceResult *ptr);
	virtual void traceHull(const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, IGNORE_GLASS ignoreGlass,HULLS hullNumber, edict_t *pentIgnore, TraceResult *ptr);

	virtual bool fVisibleEx(Vector v_start, Vector v_end, bool bignore_doors=false, bool bignore_breakables=false, bool bignore_monsters=false, bool bignore_glass=false,CEntity pEdictIgnore=0);
};

extern CGame *g_pGame;

#endif // !defined(__GAMEHL_H)

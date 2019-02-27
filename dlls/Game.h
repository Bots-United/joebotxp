// CGame.h: interface for the CGame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__GAME_H)
#define __GAME_H

#include "extdll.h"
#include "util.h"
#include "bot_modid.h"

#include "globaldefs.h"

#include "Profiler.h"

class CIEntity;
#include "Player.h"

#include <list>		// stl list class
using namespace std;

#include "BaseBot.h"
class CBaseTeam;

typedef struct{
	int respawn_state;
	float kick_time;
	char name[BOT_NAME_LEN+1];
	char skin[BOT_SKIN_LEN+1];
	int bot_skill;
	int bot_team;
	int bot_class;
}SInfo_t;

enum _debugident{
	DEBUG_SYSTEM = 1<<0,
	DEBUG_GAME = 1<<1,
	DEBUG_TEAM = 1<<2,
	DEBUG_SQUAD = 1<<3,
	DEBUG_BOT = 1<<4,
	DEBUG_NAV = 1<<5,
	DEBUG_ACTION = 1<<6
};

extern char g_szDebugIdent[][80];

typedef enum { ignore_monsters=1, dont_ignore_monsters=0, missile=2 } IGNORE_MONSTERS;
typedef enum { ignore_glass=1, dont_ignore_glass=0 } IGNORE_GLASS;
typedef enum { point_hull=0, human_hull=1, large_hull=2, head_hull=3 } HULLS;

class CGame  
{
public:
	CGame();
	virtual ~CGame();

	virtual void debugMsg(int,const char *, ...) = 0;
	virtual void userMsg(CEntity,const char *, ...) = 0;

	virtual bool drawBeam(int,CEntity pEntity, const Vector &start, const Vector &end, int width=10,int noise=0, const Vector &color=Vector(255,255,255), int brightness=255, int speed=0,int iLife=10) = 0;

	virtual void reset(void);				// general reset
	virtual void startFrame(void)=0;			// called by the engine on each start of a frame
	virtual void checkMapChange(void)=0;		// checks if a map change has happened
	virtual void updateClientData(void)=0;
	virtual void processBotCfgFile(void);	// processes the bot's cfg file

	virtual CPlayer *allocPlayer(void)=0;		// allocate a player ...
	virtual CPlayer *preAddPlayer(void);
	virtual bool addPlayer(edict_t *);		// connecting a new player
	virtual bool removePlayer(edict_t *);	// disconnecting a player
	virtual bool createBot(const char *,const char *,const char *,const char *)=0;
	int isPlayer(CEntity);				// is this edict_t * a player ?
	int isPlayer(CPlayer *);				// is this CPlayer * a player ?
	virtual CBaseBot *allocBot(void);		// allocate a bot instance

	CPlayer **getFreeSlot(void);		// returns a free pointer for a bot in the bot array
	//void freeBotSlot(int);					// frees a bot slot - checks if this is valid and changes counter variables
	inline CBaseBot *getBot(int iIndex){	// returns a pointer to a bot
		//if(iIndex >= 0 && iIndex <32)	
		if(m_ppPlayer[iIndex]){
			return m_ppPlayer[iIndex]->isBot();
				//return dynamic_cast<CBaseBot*>(m_ppPlayer[iIndex]);
			//return (CBaseBot*)(m_ppPlayer[iIndex]);
		}
		return 0;
	}
	//inline int getMaxCurrentBot(void){return m_iMaxCurrentBot;}		// get maximum count of bots
	int isBot(const edict_t *);				// is this edict_t a bot ?	// if yes, return the index
	int isBot(const CPlayer *);				// is this player a bot ?
	int isBot(const CBaseBot *);			// is this bot pointer a valid bot ?
	
	virtual void onBotCreation(void);		// should be called after a bot has been created
	virtual void onMapStart(void);			// should be called on map start
	virtual void onMapEnd(void);			// should be called on map end
	virtual void onRoundStart(void);		// should be called on round start

	virtual long weaponModel2ID(const char *)=0;
	virtual long weaponClass2ID(const char *);

	virtual CBaseTeam *allocTeamData(void);
	CBaseTeam *getTeamData(int);

	vector <CBaseTeam*> m_LTeamData;

	int getTeamMembers(int);	// returns number of members in a team

	virtual float getTime(void){return 0;};	// returning the current time
	virtual float getRoundStart(void){return m_fRoundStart;}
	virtual const char * getMapname(void)=0;	// returning the name of the map
	virtual bool getTeamplay(void);			// is this a teamplay mod ?
	virtual CPlayer *getPlayer(int);		// get a CPlayer pointer to a player
	virtual int getMaxCurrentPlayer(void);	// get maximum count of players

	bool m_bTeamplayChecked;				// did we check if this game is a teamplay game ?
	bool m_bTeamplay;						// if we already checked it, get it here
	int m_iMod_Id;							// id of the mod we are currently playing
	float m_fRoundStart;					// time of round start

	int m_iMaxCurrentPlayer;
	CPlayer *m_ppPlayer[32];				// array of players
	SInfo_t SBInfo[32];						// respawn infos
	/*int m_iMaxCurrentBot;					// maximum number of bots
	CBaseBot *m_pBots[32];					// max of 32 bots in a game*/
	list<CIEntity *> m_LIEntities;			// list of interesting entities
	long m_lThought;

	long m_lBeamDebug;				// which beams of which type should be shown ? 0-type is always shown ( waypoint graph etc )
	long m_lMsgDebug;				// which debug msg should be shown ? 0-type is always shown

	float m_fCheckServerCmd;
	float m_fPreviousTime;
	float m_fClientUpdateTime;
	float m_fBotCfgPauseTime;
	float m_fBotCheckTime;
	float m_fRespawnTime;
	FILE *m_pBotCfgfp;
	int m_iNumBots;
	int m_iPrevNumBots;
	bool m_bNeed2OpenCfg;
	bool m_bSpawnTimeReset;

	CEntity m_pListenserverEdict;

	// 'util' functions //

	virtual bool fVisible( const Vector &vecOrigin, CEntity pEdict );
	virtual bool fVisibleEx(Vector v_start, Vector v_end, bool bignore_doors=false, bool bignore_breakables=false, bool bignore_monsters=false, bool bignore_glass=false,CEntity pEdictIgnore=0);
	bool fInViewCone(const Vector &vOrigin, CEntity pEdict);

	// for trace line and trace hull
	virtual void traceLine(const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, IGNORE_GLASS ignoreGlass, CEntity pentIgnore, TraceResult *ptr) = 0;
	virtual void traceLine(const Vector &vecStart, const Vector &vecEnd, CEntity pentIgnore, TraceResult *ptr) = 0;

	virtual void traceHull(const Vector &vecStart, const Vector &vecEnd, HULLS hullNumber, edict_t *pentIgnore, TraceResult *ptr)=0;
	virtual void traceHull(const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, IGNORE_GLASS ignoreGlass,HULLS hullNumber, edict_t *pentIgnore, TraceResult *ptr)=0;
};

extern CGame *g_pGame;

#endif // !defined(__GAME_H)

// CBaseBot.cpp: implementation of the CBaseBot class.
//
//////////////////////////////////////////////////////////////////////

#include "BaseBot.h"

#include "AStarMachine.h"
#include "AStarGoalMod.h"

#include "Player.h"
#include "Action.h"
#include "Emotion.h"
#include "PerceptionBot.h"
#include "BaseTeam.h"

#include "util.h"
#include <algorithm>
#include <iostream>
using namespace std;
#ifdef _DEBUG
extern bool g_debuginfo;
#endif

#include "BV_HLDM.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void FakeClientCommand(edict_t *pBot, char *arg1, char *arg2, char *arg3);

CBaseBot::CBaseBot(){
	m_iFOV = 90;
	m_bFlashlight = false;
	m_fBlinded = 0;
	m_bAlive = false;

	m_pMovePath = 0;
	m_pEMTeam = 0;

	m_iTeam = -1;

	m_iSquad = -1;		// no squad yet

	m_fAggressivity = 0;
	m_fNextThink = 0;
	m_fThinkPer = .1f;
}

CBaseBot::~CBaseBot()
{
	delete m_pAction;
	m_pAction = 0;
	
	delete m_pPerception;
	m_pPerception = 0;

	for_each(m_LpBehaviour.begin(),m_LpBehaviour.end(),Destroy<CBehaviour*>());
	for_each(m_LpEmotion.begin(),m_LpEmotion.end(),Destroy<CEmotion*>());
}

void CBaseBot::init(void){
	m_pAction->setEntity(getEntity());
	m_pAction->reset();
	m_pPerception->setBot(this);
	m_pPerception->reset();

	m_LGoals.reset();		// reset goalfinder
	m_LGoals.finishedMove();

	resetBehaviours();

	m_fNextThink = 0;

	//g_AStarTasks.deleteTasksFrom(this);
}

void CBaseBot::menu(void){
}

void CBaseBot::assignSquad(int iSquad){
	m_iSquad = iSquad;
}

void CBaseBot::assignTeam(int iTeam){
	m_iTeam = iTeam;

	m_iSquad = -1;
	g_pGame->getTeamData(iTeam)->addMe2Squad(g_pGame->isPlayer(this));	// add me to some squad
}

void CBaseBot::change2Weapon(long lWeapon){
	assert(lWeapon >= 0 && lWeapon < 32);
	fakeClientCommand(weapon_defs[lWeapon].szClassname,0,0);
}

bool CBaseBot ::think(void){
	PROFILE("CBaseBot::think");
	// return true if the bot 'thought' this time.

	getEntity()->v.flags|=FL_FAKECLIENT;

	if(m_fNextThink > g_pGame->getTime()){
		m_pAction->move();
		return false;
	}
	{
	PROFILE("realthink");

	menu();

	if(checkDeath()){		// if it's dead ... call necessary functions and get back
		m_pAction->move();
		m_fNextThink = g_pGame->getTime() + .1f;
		return false;
	}
#ifdef DEBUGMESSAGES
	/*if(g_pGame->m_pListenserverEdict.getEntity()
		&&m_pMovePath->m_iCurrWP != -1)
		g_pGame->drawBeam(g_pGame->m_pListenserverEdict.getEntity(),g_Map.m_Waypoints.m_Waypoints[m_pMovePath->m_iCurrWP].m_VOrigin,getOrigin(),10,10,255,255,255,255,20,1);*/
#endif


#ifdef _DEBUG
	if(g_pGame->m_pListenserverEdict.getEntity()
		&& g_pGame->m_iMod_Id == VALVE_DLL
		&& !g_debuginfo){
		char szBotText[4][512];
		char szBV[4][512];

		sprintf(szBV[1],"%s",m_LGoals.m_pActionBv?m_LGoals.m_pActionBv->getName():"-");
		sprintf(szBV[2],"%s",m_LGoals.m_pLookBv?m_LGoals.m_pLookBv->getName():"-");
		sprintf(szBV[3],"%s",m_LGoals.m_pLMoveBv?m_LGoals.m_pLMoveBv->getName():"-");

		UTIL_ShortenBV(szBV[1]);UTIL_ShortenBV(szBV[2]);UTIL_ShortenBV(szBV[3]);
		
		sprintf(szBotText[0],"%s-%i",getName(),getHealth());
		sprintf(szBotText[1],"A %s-%2i",szBV[1],
						m_LGoals.m_pActionWinner?m_LGoals.m_pActionWinner->m_iImportance:-1);
		sprintf(szBotText[2],"L %s-%2i",szBV[2],
						m_LGoals.m_pLookWinner?m_LGoals.m_pLookWinner->m_iImportance:-1);
		sprintf(szBotText[3],"M %s-%2i",szBV[3],
						m_LGoals.m_iLMovePriority);

		BotDebug(VARS(g_pGame->m_pListenserverEdict.getEntity()),getOrigin(),szBotText[0],szBotText[1],szBotText[2],szBotText[3],ENTINDEX(getEntity()),1);
	}
#endif

	//m_iTeam = getTeam();

	/*char sztest[80];
	sprintf(sztest,"%i",m_iTeam);
	fakeClientCommand("say",sztest);*/

	m_pAction->releaseButton();
	m_pAction->setStrafe(0);

	m_pPerception->run();
	callBehaviours();

	/*if(!m_pMovePath->m_CurrWay.size()){
		if(g_pGame->m_pListenserverEdict.getEntity())
			g_pGame->drawBeam(g_pGame->m_pListenserverEdict.getEntity(),getOrigin()+Vector(0,0,30),getOrigin()+Vector(0,0,40),10,10,Vector(255,255,255),255,255);
	}*/

	m_pAction->move();
	m_fNextThink = g_pGame->getTime() + m_fThinkPer;

	/*if(g_AStarTasks.m_LTasks.size() < 100){
		AStarBase *p = new AStarMachine<AStarBaseGoal>;
		p->setStart(RANDOM_LONG(1,g_Map.m_Waypoints.m_iNumWaypoints));
		p->setDestination(RANDOM_LONG(1,g_Map.m_Waypoints.m_iNumWaypoints));
		g_AStarTasks.addTask(p,RANDOM_LONG(1,10));
	}*/
	}
	return true;
}

void CBaseBot::callBehaviours(void){
	PROFILE("CBaseBot::callBehaviours");

	list<CBehaviour *>::iterator iter;
	list<CEmotion *>::iterator iter_emo;
	list<CPerceipt *>::iterator iter_percs;

	// delete old goals
	m_LGoals.clear();

	// call emotions
	{
		PROFILE("emotions");
		for(iter_emo = m_LpEmotion.begin(); iter_emo != m_LpEmotion.end(); iter_emo ++){
			(*iter_emo)->preprocess();			// preprocess
			for(iter_percs = m_pPerception->m_LPerceptions.begin();
				iter_percs != m_pPerception->m_LPerceptions.end(); iter_percs ++){
					(*iter_emo)->evaluate(&m_LGoals,*iter_percs);		// process -> that's going thru the percs
				}
				(*iter_emo)->postprocess();			// postprocess
		}
	}

	// call behaviours
	{
		PROFILE("behaviours");
		for(iter = m_LpBehaviour.begin(); iter != m_LpBehaviour.end(); iter ++){
			if((*iter)->m_iPercept == CBehaviour::P_NONE){
				PROFILE("CBehaviour::P_NONE");
				(*iter)->evaluate(&m_LGoals,0);			// or just call the evaluate function if not perceptive
			}

			if((*iter)->m_iPercept & CBehaviour::P_STMem){							// process short term perceptions
				PROFILE("CBehaviour::P_STMem");
				for(iter_percs = m_pPerception->m_LPerceptions.begin();
					iter_percs != m_pPerception->m_LPerceptions.end(); iter_percs ++){
						(*iter)->evaluate(&m_LGoals,*iter_percs);
					}
			}
			if((*iter)->m_iPercept & CBehaviour::P_LTMem){							// and long term perceptions
				PROFILE("CBehaviour::P_LTMem");
				for(iter_percs = m_pPerception->m_LLTMem.begin();
					iter_percs != m_pPerception->m_LLTMem.end(); iter_percs ++){
						(*iter)->evaluate(&m_LGoals,*iter_percs);
					}
			}
			if((*iter)->m_iPercept & CBehaviour::P_Command){							// and command perceptions
				PROFILE("CBehaviour::P_Command");
				for(iter_percs = m_pPerception->m_LCommands.begin();
					iter_percs != m_pPerception->m_LCommands.end(); iter_percs ++){
						(*iter)->evaluate(&m_LGoals,*iter_percs);
					}
					// and call it once to let them see if there has been still some old interrupted task to be fulfilled
					(*iter)->evaluate(&m_LGoals,0);
			}
		}
	}

	{
		PROFILE("goalfinder");
		m_LGoals.getWinner();
		m_LGoals.executeWinner();
	}

	m_fCalledBV = g_pGame->getTime();
}

void CBaseBot::resetBehaviours(void){
	list<CBehaviour *>::iterator iter;
	list<CEmotion *>::iterator iter_emo;

	// call emotions' reset
	for(iter_emo = m_LpEmotion.begin(); iter_emo != m_LpEmotion.end(); iter_emo ++)
		(*iter_emo)->reset();

	// call behaviours' reset
	for(iter = m_LpBehaviour.begin(); iter != m_LpBehaviour.end(); iter ++)
		(*iter)->reset();
}

bool CBaseBot::checkDeath(void){
	if(!isAlive()){				// it's dead
		if(!isAttacking()
			&& RANDOM_LONG(0,100) < 50){
			m_pAction->attack();
		}
		m_bNeed2Init = true;
		if(m_bAlive){
			// last time it was still alive
			onDeath();
		}
		m_bAlive = false;
		return true;
	}
	else{									// it's alive
		if(!m_bAlive){
			// last time it was still dead
			onRespawn();
		}
		m_bAlive = true;
		return false;
	}
}

void CBaseBot::kill(edict_t *p,long lWeapon){
	if(lWeapon >= 0 && lWeapon < 32)
		m_pWeapons->m_pWeapons[lWeapon].m_iPreference ++;
}

void CBaseBot::killed(edict_t *p,long lWeapon){
	if(lWeapon >= 0 && lWeapon < 32)
		m_pWeapons->m_pWeapons[m_pWeapons->getCWeaponID()].m_iPreference --;
}

AStarBase *CBaseBot::getAStarMachine(void){
	AStarBase *p = new AStarMachine<AStarGoalMod>;

	return p;
}

void CBaseBot::addCostVisEnemies(AStarMachine<AStarGoalMod> *p,float fCost){
	list<CPerceipt *>::iterator iter_percs;
	for(iter_percs = m_pPerception->m_LPerceptions.begin();
	iter_percs != m_pPerception->m_LPerceptions.end(); iter_percs ++){
		if((*iter_percs)->m_lType == CPerceipt::PT_ENEMY){
			p->m_Goal.addVisibleCost((*iter_percs)->m_pPlayer->m_iNearestWP,fCost);
		}
	}
}

void CBaseBot::onDeath(void){
	// clear perceptions
	m_pPerception->resetPerceptions();

	CBaseTeam *pTD;
	pTD = g_pGame->getTeamData(m_iTeam);
	if(pTD)
		pTD->tellDied(this);
}

void CBaseBot::onRespawn(void){
	init();

	CBaseTeam *pTD;
	pTD = g_pGame->getTeamData(m_iTeam);
	if(pTD)
		pTD->tellRespawned(this);
}


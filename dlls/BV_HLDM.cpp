#include "BV_HLDM.h"

#include "BaseBot.h"
#include "Map.h"
#include "Player.h"
#include "AStarMachine.h"
#include "AStarBaseGoal.h"
#include "AStarGoalHide.h"
#include "AStarGoalPFled.h"
#include "BaseTeam.h"

#include "PerceptionBot.h"
#include "Action.h"

#include "InterestingEntity.h"

#include "GameHLDM.h"

#include <iostream>
using namespace std;

//#define DEBUGMESSAGES1

// handle squad commands

void CBV_HLDM_SquadCommand::evaluate(list<CGoal> *pGoals,CPerceipt *p){
	if(m_iExecuteFSMS)
		return;
	if(p){
		if(p->m_lType != CPerceipt::PT_COMMAND)
			return;	// no command

		if(p->m_lTypeSpec & CPerceipt::PTX_COMMAND_IN_WORK)
			return;

		if(!(p->m_lTypeSpec & (CPerceipt::PTX_COMMAND_SQUAD|CPerceipt::PTX_COMMAND_TEAM)))
			return;	// no squad or team command
#ifdef DEBUGMESSAGES
		g_pGame->drawBeam(DEBUG_BOT|DEBUG_BOT,g_pGame->m_pListenserverEdict.getEntity(),m_pBot->getOrigin(),m_pBot->getOrigin()+Vector(0,0,50),5,0,Vector(0,255,0),  255,0,10);
#endif
		}
	else{
		return;

		/*if(m_LastCommand.m_lType != CPerceipt::PT_COMMAND)
			return;	// no command

		if(!(m_LastCommand.m_lTypeSpec & CPerceipt::PTX_COMMAND_INTERRUPTED))	// return if this already has been done
			return;
#ifdef DEBUGMESSAGES
		g_pGame->drawBeam(g_pGame->m_pListenserverEdict.getEntity(),m_pBot->getOrigin(),m_pBot->getOrigin()+Vector(0,0,50),5,0,Vector(255,0,0),  255,0,100);
#endif
		CRASH;*/
	}

	// check if the current move is a move by us ...
	if(m_pBot->m_LGoals.m_pLMoveBv == this){
		// if we shall go somewhere
		if(p->m_lTypeSpec & (CPerceipt::PTX_COMMAND_GOTO | CPerceipt::PTX_COMMAND_GOTODEF )){
			// it's me that's still responsible for this task ? KILL ME !
			m_pBot->m_LGoals.finishedMove();
		}
	}

	if(p->m_lTypeSpec & (CPerceipt::PTX_COMMAND_GOTO | CPerceipt::PTX_COMMAND_GOTODEF )){
		pGoals->push_back(
			CGoal(
			30,
			CGoal::GT_MOVE,
			this,
			p));
	}
	else{
		// no direct move command expected
		pGoals->push_back(
			CGoal(
			30,
			CGoal::GT_ACTION,
			this,
			p));
	}
}

void CBV_HLDM_SquadCommand::execute(CGoal *pGoal){
	if(!pGoal)
		return;
	if(m_iExecuteFSMS)
		return;

	if(!pGoal->m_pPerceipt)
		return;

	pGoal->m_pPerceipt->m_lTypeSpec |= CPerceipt::PTX_COMMAND_IN_WORK;

	// new job
	if(pGoal->m_pPerceipt->m_lTypeSpec & CPerceipt::PTX_COMMAND_GOTO){
		//m_pBot->fakeClientCommand("say","goto command");
		m_pBot->m_pAction->slowDownO(0,1);	// no more slowed down

		AStarBase *pAStar = m_pBot->getAStarMachine();		// create a direct path

		pAStar->setDestination(pGoal->m_pPerceipt->m_iAddInfo);
		pAStar->setStart(m_pBot->m_iNearestWP);

		g_AStarTasks.addTask(pAStar,5,m_pBot,this);
		m_iExecuteFSMS ++;

		m_LastCommand = *pGoal->m_pPerceipt;

		// kill it
		pGoal->m_pPerceipt->kill();

		m_iExecuteFSMS ++;
	}
	if(pGoal->m_pPerceipt->m_lTypeSpec & CPerceipt::PTX_COMMAND_COVER){
	}
	if(pGoal->m_pPerceipt->m_lTypeSpec & CPerceipt::PTX_COMMAND_STCAMP){
		m_pBot->m_pCamp->stopCamping();
	}
	if(pGoal->m_pPerceipt->m_lTypeSpec & CPerceipt::PTX_COMMAND_GOTODEF){
		if(pGoal->m_pPerceipt->m_pPointer){
			//g_pGame->debugMsg("GOTODEF\n");
			AStarBase *pAStar = (AStarBase *)pGoal->m_pPerceipt->m_pPointer;

			m_pBot->m_pMovePath->getResult(pAStar,this);

			delete pAStar;
			//g_pGame->debugMsg("%p\n",pAStar);
			pGoal->m_pPerceipt->m_pPointer = 0;
		}
	}
	if(pGoal->m_pPerceipt->m_lTypeSpec & CPerceipt::PTX_COMMAND_SDOWN){
		m_pBot->m_pAction->slowDownO(pGoal->m_pPerceipt->m_iAddInfo,pGoal->m_pPerceipt->m_fValue);
	}
	if(pGoal->m_pPerceipt->m_lTypeSpec & CPerceipt::PTX_COMMAND_DUCK){
		m_pBot->m_pAction->duckO(pGoal->m_pPerceipt->m_iAddInfo);
	}
	if(pGoal->m_pPerceipt->m_lTypeSpec & CPerceipt::PTX_COMMAND_GOGOGO){
		// make them agressive
	}
}

void CBV_HLDM_SquadCommand::initialize(CGoal *pGoal){
	/*m_iExecuteFSMS++;
	// are we recontinuing ?
	if(m_LastCommand.m_lTypeSpec & CPerceipt::PTX_COMMAND_INTERRUPTED){
		m_LastCommand.m_lTypeSpec &=~ CPerceipt::PTX_COMMAND_INTERRUPTED;

		pGoal->m_pPerceipt = &m_LastCommand;		// if we wanna continue, we gotta check that old data
		// ( let's hope there is no other function using this variable since we didnt change the copy of that goal
		// but the original )
#ifdef DEBUGMESSAGES
		g_pGame->drawBeam(g_pGame->m_pListenserverEdict.getEntity(),m_pBot->getOrigin(),m_pBot->getOrigin()+Vector(50,50,50),5,0,255,255,0,  255,0,100);
#endif
	}
	else{
#ifdef DEBUGMESSAGES
		g_pGame->drawBeam(g_pGame->m_pListenserverEdict.getEntity(),m_pBot->getOrigin(),m_pBot->getOrigin()+Vector(50,50,50),5,0,255,255,255,  255,0,100);
#endif
	}
	// new job
	if(pGoal->m_pPerceipt->m_lTypeSpec & CPerceipt::PTX_COMMAND_GOTO){
		m_pBot->m_pCamp->stopCamping();

		AStarBase *pAStar = m_pBot->getAStarMachine();		// create a direct path

		pAStar->setDestination(pGoal->m_pPerceipt->m_iAddInfo);
		pAStar->setStart(m_pBot->m_iNearestWP);

		g_AStarTasks.addTask(pAStar,5,m_pBot,this);
		m_iExecuteFSMS ++;

		m_LastCommand = *pGoal->m_pPerceipt;

		// kill it
		pGoal->m_pPerceipt->m_fLifetime = 0;
	}
	if(pGoal->m_pPerceipt->m_lTypeSpec & CPerceipt::PTX_COMMAND_COVER){
	}
	if(pGoal->m_pPerceipt->m_lTypeSpec & CPerceipt::PTX_COMMAND_GOGOGO){
		// make them agressive
	}*/
}

void CBV_HLDM_SquadCommand::deinitialize(CGoal *pGoal){
	/*m_LastCommand.m_lTypeSpec |= CPerceipt::PTX_COMMAND_INTERRUPTED;*/

	CBehaviour::deinitialize(pGoal);

	m_iExecuteFSMS = 0;
}

void CBV_HLDM_SquadCommand::onReachedDestination(CPlayer *pPlayer){
	m_LastCommand.m_lTypeSpec &=~ CPerceipt::PTX_COMMAND_INTERRUPTED; // task done :-)

	// check if we should stay in here

	if(m_LastCommand.m_lTypeSpec & CPerceipt::PTX_COMMAND_GOSTAY){
		m_pBot->m_pCamp->campO(m_LastCommand.m_fValue);
	}
	if(m_LastCommand.m_lTypeSpec & CPerceipt::PTX_COMMAND_GOTO){
		// remove this command from the list

		// TODO

		// make last command invalid
		m_LastCommand.m_lType = 
			m_LastCommand.m_lTypeSpec = 0;

		// do we have a custom onreacheddestination callback ?
		if(m_LastCommand.m_pPointer1){
			((CCallback_onReachedDestination *)m_LastCommand.m_pPointer1)->onReachedDestination(m_pBot);
			m_LastCommand.m_pPointer1 = 0;
		}
	}
	if(m_LastCommand.m_lTypeSpec & CPerceipt::PTX_COMMAND_GOTODEF){
		// make last command invalid
		m_LastCommand.m_lType = 
			m_LastCommand.m_lTypeSpec = 0;

		// do we have a custom onreacheddestination callback ?
		if(m_LastCommand.m_pPointer1){
			((CCallback_onReachedDestination *)m_LastCommand.m_pPointer1)->onReachedDestination(m_pBot);
			m_LastCommand.m_pPointer1 = 0;
		}
	}
}

void CBV_HLDM_SquadCommand::onPathCreation(AStarBase *p){
	m_iExecuteFSMS = 0;

	m_pBot->m_pMovePath->getResult(p,this);
	
	delete p;
}

void CBV_HLDM_SquadCommand::reset(void){
	m_LastCommand.m_lType = 0;

	CBehaviour::reset();
}

// camp

void CBVNP_HLDM_Camp::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if(m_fCamp > g_pGame->getTime()){
		pGoals->push_front(CGoal(26,/*CGoal::GT_ACTION|*/CGoal::GT_MOVE,this,0));
	}	
	return;
}

void CBVNP_HLDM_Camp::execute(CGoal *pGoal){
	m_fLastExecution = g_pGame->getTime();
	
	m_pBot->m_pAction->slowDownO(.25f,0);			// stop here for .25 second

	if(m_fCamp < g_pGame->getTime()){
		m_pBot->m_LGoals.finishedMove();	// so let's cancel this shit if we dont need it any more :D
	}
}

void CBVNP_HLDM_Camp::camp(float fTime){
	m_fCamp = fTime;
}

void CBVNP_HLDM_Camp::campO(float fTime){
	camp(g_pGame->getTime() + fTime);
}

void CBVNP_HLDM_Camp::deinitialize(CGoal *pGoal){
	m_fCamp = 0;
}

bool CBVNP_HLDM_Camp::isCamping(void){
	return m_fCamp > g_pGame->getTime();
}

void CBVNP_HLDM_Camp::stopCamping(void){
	m_fCamp = g_pGame->getTime();
}

// duck under obstacles

void CBVNP_HLDM_Duck::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if(m_pBot->m_pPerception->m_pbObstacle[CPerceptionBot::POD_Eye]
		/*&&!m_pBot->m_pPerception->m_pbObstacle[CPerceptionBot::POD_Origin]*/){
		pGoals->push_front(CGoal(50,CGoal::GT_ACTION,this,0));
	}
	return;
}

void CBVNP_HLDM_Duck::execute(CGoal *pGoal){
	m_fLastExecution = g_pGame->getTime();
	
	m_pBot->m_pAction->duckO(.3f);
}

// stop zoomed mode if we havnt seen an enemy for a while

void CBVNP_HLDM_ZoomBack::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if(m_pBot->m_iFOV != m_pBot->getDefaultFOV()
		&& m_pBot->m_pEMLEnemy->m_fTime < g_pGame->getTime() - 3.f){
		pGoals->push_front(CGoal(20,CGoal::GT_ACTION,this,0));
	}
	return;
}

void CBVNP_HLDM_ZoomBack::execute(CGoal *pGoal){
	m_fLastExecution = g_pGame->getTime();
	
	m_pBot->m_pAction->attack2();
}

// jump over obstacles

void CBVNP_HLDM_Jump::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if(m_pBot->m_pPerception->m_pTObstacle[CPerceptionBot::POD_MJump].flFraction<1.f
		&& m_pBot->m_pPerception->m_pTObstacle[CPerceptionBot::POD_MJump].vecPlaneNormal.z < .8f		//that's DotProduct(vecPlaneNormal,Vector(0,0,1))
		&& !m_pBot->m_pPerception->m_pbObstacle[CPerceptionBot::POD_Eye]){

			// test if we have a path
			if(m_pBot->m_pMovePath->hasWay()){
				if(!m_pBot->m_pMovePath->m_CurrWay.isPosInWay(&g_Map.m_Waypoints,m_pBot->m_pPerception->m_pTObstacle[CPerceptionBot::POD_MJump].vecEndPos,2,.6f)){
					return;
				}
			}
			pGoals->push_front(CGoal(50,CGoal::GT_ACTION,this,0));
			return;
	}
	return;
}

void CBVNP_HLDM_Jump::execute(CGoal *pGoal){
	m_fLastExecution = g_pGame->getTime();
	
	m_pBot->m_pAction->jump();
	m_pBot->m_pAction->duckO(.5f);
}

// just look a bit around :)

/*void CBVNP_HLDM_LookAround::reset(void){
	CBehaviour::reset();
}*/

void CBVNP_HLDM_LookAround::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if(fabs(g_pGame->getTime() - m_fLastExecution) < .7f)		// dont change the view too often
		return;

	/* check view */
	int i;
	int iWaypointAt=-1;
	float fTime = g_pGame->getTime();
	float    flDot;
	Vector VOrigin = m_pBot->getOrigin();
	Vector vecLOS;
	Vector VLookDir;

	CBaseTeam *pTeamData = g_pGame->getTeamData(m_pBot->getTeam());
	if(!pTeamData){
		g_pGame->debugMsg(DEBUG_BOT,"say","shit, no team data",0);
		return;
	}

	// I manually 'inlined' the isInFOV function, cause this way we only need to call UTIL_MAKEVECTORS once
	//UTIL_MakeVectors ( m_pBot->getAngles() );	// convert angles to vector
	VLookDir = m_pBot->getAngles().angles2vec();

	iWaypointAt = m_pBot->m_iNearestWP;

	if(iWaypointAt != -1){
		for(i = 0; i < g_Map.m_Waypoints.m_iNumWaypoints; i++){
			if(g_Map.m_Waypoints.getVisible(i,iWaypointAt)){		// only mark visible WPs
				//if(m_pBot->isInFOV(g_Map.m_Waypoints[i].m_VOrigin))
								
				vecLOS = g_Map.m_Waypoints[i].m_VOrigin - VOrigin;
				vecLOS = vecLOS.normalize();
				
				flDot = DotProduct ( vecLOS , VLookDir );
				
				if ( flDot > 0.50 ){  // 60 degree field of view 
					pTeamData->m_pfSeen[i] = fTime;										// mark wps in viewcone
				}
				else{
					// not in viewcone
				}
			}
		}
	}

	pGoals->push_front(CGoal(5,CGoal::GT_LOOK,this,0));

	return;
}

void CBVNP_HLDM_LookAround::execute(CGoal *pGoal){
	m_fLastExecution = g_pGame->getTime();

	int iCheck = m_pBot->m_pMovePath->m_iCurrWP;		// from which waypoint to check ...
	if(iCheck == -1){
		iCheck = m_pBot->m_iNearestWP;
		if(iCheck == -1)
			return;
	}

	int iWaypoint = -1,i;
	float fMax = 6,fTemp;
	float fTime = g_pGame->getTime();
	float fDiffTime;
	Vector VLookDir;
	
	CBaseTeam *pTeamData = g_pGame->getTeamData(m_pBot->getTeam());

	//UTIL_MakeVectors ( m_pBot->getAngles() );	// convert angles to vector
	VLookDir = m_pBot->getAngles().angles2vec();

	for(i = 0; i < g_Map.m_Waypoints.m_iNumWaypoints; i++){
		if(g_Map.m_Waypoints.getVisible(i,iCheck)){
			// search maximum time a waypoint hasnt been seen. Additionally the damage done to players at this
			// waypoint is taken into account. for extremely dangerous waypoints, the difference to the last
			// time is multiplied by 2
			fDiffTime = fTime - pTeamData->m_pfSeen[i];
			if(fDiffTime > 15)
				fDiffTime = 15;

			fTemp = fDiffTime		// time difference
				// let's make dangerous places more probable
				* (1.f+float(g_Map.m_Waypoints[i].m_lTotalDamage)/float(g_Map.m_Waypoints.m_lMaxTotalDamage+1))
				// and let's make it more probable that we take a waypoint which is in front of us :D 
				* (1.f+DotProduct(m_pBot->m_pAction->getRunDir(),(g_Map.m_Waypoints[i].m_VOrigin - m_pBot->getOrigin()).normalize())/5.f)
				* (1.f + DotProduct(VLookDir,(g_Map.m_Waypoints[i].m_VOrigin - m_pBot->getOrigin()).normalize())/3.f)
				;
			if(fTemp > fMax){
				if((g_Map.m_Waypoints[i].m_VOrigin - m_pBot->getOrigin()).squareLength() > 125.f*125.f){
					fMax = fTemp;
					iWaypoint = i;
				}
			}
		}
	}

	if(iWaypoint != -1){
		m_pBot->m_pAction->lookTo(g_Map.m_Waypoints[iWaypoint].m_VOrigin);
#ifdef DEBUGMESSAGES
		g_pGame->drawBeam(DEBUG_BOT|DEBUG_ACTION,g_pGame->m_pListenserverEdict.getEntity(),m_pBot->getOrigin(),g_Map.m_Waypoints[iWaypoint].m_VOrigin,4,0,Vector(255,0,255),255,0);
#endif
	}
}

// avoid getting stuck

void CBVNP_HLDM_AvoidStuck::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if((!m_pBot->m_pAction->ducked() && (m_pBot->getVelocity().squareLength2D() < 70.f						// too slow
		&& m_pBot->m_pAction->getSpeed() > 100.f) )	// or an obstacle in front
		&& !m_pBot->isOnLadder()
		&& m_fLastExecution < g_pGame->getTime() - .7f){
		m_lCStuck ++;
		
		if(m_lCStuck > 1){			// only if the bot is more than once stuck
			int iPriority = 20 + m_lCStuck * 5;
			
			if(iPriority>100)
				iPriority = 100;
			if(fabs(g_pGame->getTime()-m_fLastExecution)<1)
				return;
			//if(g_pGame->m_pListenserverEdict.getEntity())g_pGame->drawBeam(g_pGame->m_pListenserverEdict.getEntity(),m_pBot->getOrigin(),m_pBot->getOrigin()+Vector(0,0,100),10,10,255,255,255,255,255,10);
			
			pGoals->push_front(CGoal(iPriority,CGoal::GT_ACTION,this,0));
			
			return;
		}
	}
	else{
		if(m_lCStuck)
			m_lCStuck --;
	}
	return;
}

void CBVNP_HLDM_AvoidStuck::execute(CGoal *pGoal){
	m_fLastExecution = g_pGame->getTime();

	m_pBot->m_pAction->jump();
	m_pBot->m_pAction->duck(g_pGame->getTime() + .5f);
	
	if(m_lCStuck)
		m_lCStuck --;
}

// avoid getting stuck			// wp :D
// avoid getting stuck while reaching just one waypoint

void CBVNP_HLDM_AvoidStuckWP::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if( (!m_pBot->m_pMovePath->m_CurrWay.size() && m_pBot->m_pMovePath->m_iState == 0) )
		return;

	if(g_pGame->getTime () - g_pGame->m_fRoundStart < 10)
		return;
	
	if(m_pBot->m_pMovePath->m_fLastVisitedWP+5 > g_pGame->getTime())
		return;
	
	int iPriority = (g_pGame->getTime() - m_pBot->m_pMovePath->m_fLastVisitedWP)*4 + 50;
	
	pGoals->push_front(CGoal(iPriority,CGoal::GT_MOVE,this,0));
	return;
}

void CBVNP_HLDM_AvoidStuckWP::execute(CGoal *pGoal){
	// let's store that we got stuck here
	// todo only if there is no possibility that we just got stuck because of another player
	g_pGame->debugMsg(DEBUG_BOT,"avoidstuck::execute\n");
	if(m_pBot->m_pMovePath->m_CurrWay.getCurrent() != -1
		&&m_pBot->m_pMovePath->m_CurrWay.getLast() != -1){
			g_pGame->debugMsg(DEBUG_BOT,"avoidstuck::increase failed %i %i\n",m_pBot->m_pMovePath->m_CurrWay.getLast(),m_pBot->m_pMovePath->m_CurrWay.getCurrent());
			g_Map.m_Waypoints[m_pBot->m_pMovePath->m_CurrWay.getLast()].
				increasedFailedEntry(m_pBot->m_pMovePath->m_CurrWay.getCurrent());
		}

	m_fLastExecution = g_pGame->getTime();
	
	m_pBot->m_pMovePath->reset();
}

// ladder handling

void CBVNP_HLDM_Ladder::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if(m_pBot->isOnLadder())
		pGoals->push_front(CGoal(85,CGoal::GT_ACTION|CGoal::GT_LOOK,this,0));
	return;
}

void CBVNP_HLDM_Ladder::execute(CGoal *pGoal){
	m_fLastExecution = g_pGame->getTime();
	
	m_pBot->m_pAction->pressButton(IN_FORWARD);
	if(m_pBot->m_pMovePath->m_CurrWay.getCurrentIndex() != -1){
		if(m_pBot->m_pMovePath->m_fLastDist < 50){
			m_pBot->m_pMovePath->m_CurrWay.advance();
			m_pBot->m_pMovePath->m_fLastDist = 1000;
		}
		m_pBot->m_pAction->lookTo(g_Map.m_Waypoints[m_pBot->m_pMovePath->m_CurrWay.getCurrent()].m_VOrigin);
		m_pBot->m_pAction->runTo(g_Map.m_Waypoints[m_pBot->m_pMovePath->m_CurrWay.getCurrent()].m_VOrigin);
#ifdef DEBUGMESSAGES
		g_pGame->drawBeam(DEBUG_BOT|DEBUG_ACTION,g_pGame->m_pListenserverEdict.getEntity(),m_pBot->getOrigin(),g_Map.m_Waypoints[m_pBot->m_pMovePath->m_CurrWay.getCurrent()].m_VOrigin,10,0,Vector(0,255,0),255,0);
#endif
	}
}

void CBVNP_HLDM_Ladder::deinitialize(void){
	m_pBot->m_pAction->releaseButton(IN_FORWARD);
}
//////////////////////////////////////////////////////
// RunAround
//////////////////////////////////////////////////////

void CBVNP_HLDM_RunAround::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if(!m_pBot->m_pMovePath->hasWay())
		pGoals->push_front(CGoal(10,CGoal::GT_MOVE,this,0));
	
	return;
}

void CBVNP_HLDM_RunAround::initialize(CGoal *pGoal){
	m_fLastExecution = g_pGame->getTime();

	AStarBase *pAStar = m_pBot->getAStarMachine();
	int i1 = RANDOM_LONG(0,g_Map.m_Waypoints.m_iNumWaypoints-1);

	pAStar->setDestination(i1);
	pAStar->setStart(m_pBot->m_iNearestWP);
	m_pBot->m_pMovePath->m_fLastVisitedWP = g_pGame->getTime();
	
	g_AStarTasks.addTask(pAStar,5,m_pBot,this);		// add task
	
	m_iExecuteFSMS ++;		// increment state
}

void CBVNP_HLDM_RunAround::onPathCreation(AStarBase *p){
	m_iExecuteFSMS ++;

	m_pBot->m_pMovePath->getResult(p);
	
	delete p;
}

// GoCamping

void CBVNP_HLDM_GoCamping::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if(!m_pBot->m_pWeapons->isSniper(WEAPONB(m_pBot->m_pWeapons->getCWeaponID())))
		return;

	if(!m_pBot->m_pMovePath->m_CurrWay.size())
		pGoals->push_front(CGoal(12,CGoal::GT_MOVE,this,0));
}

void CBVNP_HLDM_GoCamping::initialize(CGoal *pGoal){
	m_fLastExecution = g_pGame->getTime();

	AStarBase *pAStar = m_pBot->getAStarMachine();
	int i1 = -1;		// somehow get a nice sniping point,which is still a bit random

	pAStar->setDestination(i1);
	pAStar->setStart(m_pBot->m_iNearestWP);
	m_pBot->m_pMovePath->m_fLastVisitedWP = g_pGame->getTime();
	
	g_AStarTasks.addTask(pAStar,5,m_pBot,this);		// add task
	
	m_iExecuteFSMS ++;		// increment state
}

void CBVNP_HLDM_GoCamping::onPathCreation(AStarBase *p){
	m_iExecuteFSMS ++;

	m_pBot->m_pMovePath->getResult(p,this);
	
	delete p;
}

void CBVNP_HLDM_GoCamping::onReachedDestination(CPlayer *pPlayer){
	m_pBot->m_pCamp->campO(15);
}

// Run2Sound

void CBV_HLDM_Run2Sound::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if(!m_pBot->m_pMovePath->m_CurrWay.size()){
		if(p->m_lType == CPerceipt::PT_SOUND
			&&p->m_lTypeSpec & CPerceipt::PTX_SOUND_ENEMY
			&&p->m_lTypeSpec & CPerceipt::PTX_SOUND_LOC
			&&p->m_fDistance < 1500 ){
			pGoals->push_front(CGoal(20-(p->m_fDistance<1000?p->m_fDistance/100:10),CGoal::GT_MOVE,this,p));
		}
	}
	
	return;
}

void CBV_HLDM_Run2Sound::initialize(CGoal *pGoal){
	m_fLastExecution = g_pGame->getTime();
	
	AStarBase *pAStar = m_pBot->getAStarMachine();
	int i1 = g_Map.m_Waypoints.getNearest(pGoal->m_pPerceipt->m_VOrigin);
	
	pAStar->setDestination(i1);
	pAStar->setStart(m_pBot->m_iNearestWP);
	m_pBot->m_pMovePath->m_fLastVisitedWP = g_pGame->getTime();
	
	g_AStarTasks.addTask(pAStar,5,m_pBot,this);		// add task
	
	m_iExecuteFSMS ++;		// increment state
}

void CBV_HLDM_Run2Sound::onPathCreation(AStarBase *p){
	m_iExecuteFSMS ++;

	m_pBot->m_pMovePath->getResult(p);
	
	delete p;
}

// RunToHealthWP

void CBVNP_HLDM_RunToHealthWP::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if(m_pBot->getHealth() > 90)
		return;
	
	pGoals->push_front(CGoal(10+(100-m_pBot->getHealth())/10,CGoal::GT_MOVE,this,0));
	
	return;
}

void CBVNP_HLDM_RunToHealthWP::initialize(CGoal *pGoal){
	m_fLastExecution = g_pGame->getTime();
	
	AStarBase *pAStar = m_pBot->getAStarMachine();
	int i1 = g_Map.m_Waypoints.getNearesto(m_pBot->getOrigin(),false,false,100,100000,CWaypoint::HEALTH);
	
	if(i1 != -1){
		pAStar->setDestination(i1);
		pAStar->setStart(m_pBot->m_iNearestWP);
		//m_pBot->m_pMovePath->m_fLastVisitedWP = g_pGame->getTime();		// avoid that another behaviour is searching a path too, because 
		
		g_AStarTasks.addTask(pAStar,5,m_pBot,this);		// add task
		
		m_iExecuteFSMS ++;		// increment state
	}
	
}

void CBVNP_HLDM_RunToHealthWP::onPathCreation(AStarBase *p){
	m_iExecuteFSMS ++;

	m_pBot->m_pMovePath->getResult(p);
	
	delete p;
}

// RunToWeapon

void CBV_HLDM_RunToWeapon::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if(!(p->m_lType == CPerceipt::PT_ENTITY))
		return;
	
	if(!(p->m_lTypeSpec & CPerceipt::PTX_ENTITY_WEAPON))
		return;
	
	if(m_pBot->hasWeapon(p->m_pIEntity->m_iWeaponId))
		return;
	
	pGoals->push_front(CGoal(20-p->m_fDistance/100.f,CGoal::GT_MOVE,this,p));
	
	return;
}

void CBV_HLDM_RunToWeapon::initialize(CGoal *pGoal){
	AStarBase *pAStar = m_pBot->getAStarMachine();
	int i1 = g_Map.m_Waypoints.getNearest(pGoal->m_pPerceipt->m_VOrigin,true);	// get a waypoint near to the weapon
	
	pAStar->setDestination(i1);
	pAStar->setStart(m_pBot->m_iNearestWP);
	//m_pBot->m_fLastVisitedWP = g_pGame->getTime();
	
	g_AStarTasks.addTask(pAStar,5,m_pBot,this);		// add task

	m_iExecuteFSMS ++;		// increment state
}

void CBV_HLDM_RunToWeapon::onPathCreation(AStarBase *p){
	m_iExecuteFSMS ++;
	
	m_pBot->m_pMovePath->getResult(p);
	
	delete p;
}

// RunToHealth

void CBV_HLDM_RunToHealth::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if(!(p->m_lType = CPerceipt::PT_ENTITY))
		return;
	
	if(!(p->m_lTypeSpec & CPerceipt::PTX_ENTITY_HEALTH))
		return;
	
	pGoals->push_front(CGoal(100-m_pBot->getHealth(),CGoal::GT_MOVE,this,p));
	
	return;
}

void CBV_HLDM_RunToHealth::initialize(CGoal *pGoal){
	AStarBase *pAStar = m_pBot->getAStarMachine();
	int i1 = g_Map.m_Waypoints.getNearest(pGoal->m_pPerceipt->m_VOrigin,true);	// get a waypoint near to the weapon
	
	pAStar->setDestination(i1);
	pAStar->setStart(m_pBot->m_iNearestWP);
	//m_pBot->m_fLastVisitedWP = g_pGame->getTime();
	
	g_AStarTasks.addTask(pAStar,5,m_pBot,this);		// add task
#ifdef DEBUGMESSAGES
	g_pGame->drawBeam(DEBUG_BOT|DEBUG_NAV,g_pGame->m_pListenserverEdict.getEntity(),pGoal->m_pPerceipt->m_VOrigin,m_pBot->getOrigin(),10,10,Vector(255,0,255),255,20);
#endif
	m_iExecuteFSMS ++;		// increment state
}

void CBV_HLDM_RunToHealth::onPathCreation(AStarBase *p){
	m_iExecuteFSMS ++;
	
	m_pBot->m_pMovePath->getResult(p);
	
	delete p;
}

// RunToWeaponbox

void CBV_HLDM_RunToWeaponbox::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if(!(p->m_lType == CPerceipt::PT_ENTITY))
		return;

	if(!p->m_pIEntity)
		return;
	
	if(p->m_pIEntity->m_iId != ((CGameHLDM*)(g_pGame))->m_iIEntityIdWBox)
		return;
	
	pGoals->push_front(CGoal(20,CGoal::GT_MOVE,this,p));
	
	return;
}

void CBV_HLDM_RunToWeaponbox::initialize(CGoal *pGoal){
	VWBox = pGoal->m_pPerceipt->m_VOrigin;

	AStarBase *pAStar = m_pBot->getAStarMachine();
	int i1 = g_Map.m_Waypoints.getNearest(VWBox,true);	// get a waypoint near to the weapon
	
	pAStar->setDestination(i1);
	pAStar->setStart(m_pBot->m_iNearestWP);
	
	g_AStarTasks.addTask(pAStar,5,m_pBot,this);		// add task

	m_iExecuteFSMS ++;		// increment state
}

void CBV_HLDM_RunToWeaponbox::onPathCreation(AStarBase *p){
	m_iExecuteFSMS ++;
	
	m_pBot->m_pMovePath->m_VGoal = VWBox;
	m_pBot->m_pMovePath->m_bMove2Origin = true;
	m_pBot->m_pMovePath->getResult(p);
	
	delete p;
}

// adjust ladder handling

void CBV_HLDM_AdjustLadder::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if(p->m_pIEntity && m_pBot->m_pMovePath->m_iCurrWP>-1){
		if(g_Map.m_Waypoints[m_pBot->m_pMovePath->m_iCurrWP].m_lFlags & CWaypoint::LADDER
			&& p->m_pIEntity->m_iType == CIEntity::LADDER
			&& (p->m_VOrigin - m_pBot->getOrigin()).length2D() < 125){
			pGoals->push_front(CGoal(60,CGoal::GT_ACTION|CGoal::GT_LOOK,this,p));
#ifdef DEBUGMESSAGES
			g_pGame->drawBeam(DEBUG_BOT|DEBUG_ACTION,g_pGame->m_pListenserverEdict.getEntity(),m_pBot->getOrigin(),p->m_VOrigin,4,0,Vector(0,255,255),255,0,300);
#endif
		}
	}
	return;
}

void CBV_HLDM_AdjustLadder::execute(CGoal *pGoal){
	m_fLastExecution = g_pGame->getTime();
	
	Vector ladderLeft = pGoal->m_pPerceipt->m_pEntity.getEntity()->v.absmin;
	Vector ladderRight = pGoal->m_pPerceipt->m_pEntity.getEntity()->v.absmax;
	
	ladderLeft.z = ladderRight.z;
	Vector ladderSide = ladderLeft - ladderRight;
	Vector normal = CrossProduct( ladderSide, Vector(0,0,1) );
	normal = normal.normalize();
	Vector LadderMiddle = (ladderLeft+ladderRight)/2.0f;
	Vector VLBDiff = m_pBot->getOrigin() - LadderMiddle;
	Vector VLBDiffN = VLBDiff.normalize();
	if(VLBDiff.length() > 30){
		VLBDiff.z = ladderSide.z;
		float fDotLB = DotProduct(VLBDiffN,normal);
		
		if(fDotLB<0){
			normal =- normal;
			fDotLB = DotProduct(VLBDiffN,normal);
		}
		normal.z=VLBDiff.z=0;
		Vector VCross = CrossProduct(VLBDiff,normal);
		
		if(fabs(VCross.z)>0){
			if(VCross.z>0){
				m_pBot->m_pAction->setStrafe(160);
			}
			else{
				m_pBot->m_pAction->setStrafe(-160);
			}
		}
	}
	m_pBot->m_pAction->lookTo(pGoal->m_pPerceipt->m_VOrigin);
#ifdef DEBUGMESSAGES
	if(g_pGame->m_pListenserverEdict.getEntity()){
		g_pGame->drawBeam(DEBUG_BOT|DEBUG_ACTION,g_pGame->m_pListenserverEdict.getEntity(),m_pBot->getOrigin(),pGoal->m_pPerceipt->m_VOrigin,10,0,Vector(0,0,255),255,0);
		g_pGame->drawBeam(DEBUG_BOT|DEBUG_ACTION,g_pGame->m_pListenserverEdict.getEntity(),m_pBot->getOrigin(),g_pGame->m_pListenserverEdict.getOrigin(),10,0,Vector(0,0,255),255,0);
	}
#endif
}

// what to do when there's a kill ?

void CBV_HLDM_CheckKill::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if(p->m_lType == CPerceipt::PT_KILL){
		pGoals->push_front(CGoal(20,CGoal::GT_LOOK,this,p));

		// tell the team if it was the killer
		if(p->m_pPointer == m_pBot->getEntity())
			g_pGame->getTeamData(m_pBot->m_iTeam)->tellEnemyDown(m_pBot,-1,-1);

		// remove all stuff together with this DEAD MAN :)
		// we can do this here, although the execute function relies on this data. removePerception just
		// marks the respective perceipt, it doesnt delete them. they are deleted within the next
		// perception merge process
		m_pBot->m_pPerception->removePerception(p->m_pEntity);
	}
	return;
}

void CBV_HLDM_CheckKill::execute(CGoal *pGoal){
	m_pBot->m_pAction->lookTo(pGoal->m_pPerceipt->m_VOrigin);
#ifdef DEBUGMESSAGES1
	g_pGame->drawBeam(DEBUG_BOT|DEBUG_ACTION,g_pGame->m_pListenserverEdict.getEntity(),m_pBot->getOrigin(),pGoal->m_pPerceipt->m_VOrigin,4,10,Vector(255,255,255),255,0);
#endif
	pGoal->m_pPerceipt = 0;			// just for safety ... if some stupid would try to get this perception again ... haha
}

// watch enemy

void CBV_HLDM_WatchEnemy::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if(p->m_lType == CPerceipt::PT_ENEMY){
		pGoals->push_front(CGoal(
			(20 - (p->m_fDistance<1000?p->m_fDistance/100:10))*((p->m_fLifetime - g_pGame->getTime() + p->m_fLUpdate)/p->m_fLifetime),
			CGoal::GT_LOOK,this,p));
	}
	return;
}

void CBV_HLDM_WatchEnemy::execute(CGoal *pGoal){
	m_fLastExecution = g_pGame->getTime();
	
	if(pGoal->m_pPerceipt){
		m_pBot->m_pAction->lookTo(pGoal->m_pPerceipt->m_VOrigin);
#ifdef DEBUGMESSAGES1
		g_pGame->drawBeam(DEBUG_BOT|DEBUG_ACTION,g_pGame->m_pListenserverEdict.getEntity(),m_pBot->getOrigin(),pGoal->m_pPerceipt->m_VOrigin,4,0,Vector(255,0,0),255,0);
#endif
	}
}

// shoot enemy

void CBV_HLDM_ShootEnemy::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	//m_pBot->m_pAction->slowDownO(10,0);
	if(p->m_lType == CPerceipt::PT_ENEMY){
		if((g_pGame->getTime() - p->m_fLUpdate)>.5f)	// more than 1/2 a second old ...
			return;													// -> do nothing

		float fPriority;

		fPriority = 40;
		fPriority += p->m_lTypeSpec & CPerceipt::PTX_LOOKATME ? 10 : 0;		// lower priority when the enemy doesnt look here
		//fPriority += -(p->m_fDistance<1000?p->m_fDistance/100:10);
		if(m_pBot->m_pWeapons->getCWeapon()){
			fPriority += m_pBot->m_pWeapons->getCWeapon()->m_Usability.getValue(p->m_fDistance);
		}
		fPriority *= (p->m_fLifetime - g_pGame->getTime() + p->m_fLUpdate)/p->m_fLifetime;

		pGoals->push_front(CGoal(
			(int)fPriority,
			CGoal::GT_ACTION|CGoal::GT_LOOK,this,p));

		m_fLastEnemyPerc = g_pGame->getTime();
	}
	return;
}

void CBV_HLDM_ShootEnemy::execute(CGoal *pGoal){
	m_fLastExecution = g_pGame->getTime();
	
	if(pGoal->m_pPerceipt){
		float fDuckNow,
			fZoomNow;
		// look there
		m_pBot->m_pAction->lookTo(
			pGoal->m_pPerceipt->m_VOrigin
			+ 0.8f * pGoal->m_pPerceipt->m_pEntity.getVOffset()
			, pGoal->m_pPerceipt->m_pEntity.getVelocity() - m_pBot->getVelocity()
			/*+Vector(RANDOM_LONG(-30,30),RANDOM_LONG(-30,30),RANDOM_LONG(-40,40))*/);
		
		// check for best weapon
		if(fabs(m_fLastWeaponCheck-g_pGame->getTime()) > 1){		// do fabs, so we don't need to take care of mapchanges
			int iBest = m_pBot->m_pWeapons->getBestWeapon(pGoal->m_pPerceipt->m_fDistance);
			
			if(iBest != m_pBot->m_pWeapons->getCWeaponID()){
				m_pBot->change2Weapon(iBest);
			}
			
			m_fLastWeaponCheck = g_pGame->getTime();
		}
		
		// ----- we are attacking an enemy -----

		// tell the team
		CBaseTeam *pTD;
		pTD = g_pGame->getTeamData(m_pBot->m_iTeam);
		if(pTD){
			pTD->tellEngagingEnemy(m_pBot, g_pGame->isPlayer(pGoal->m_pPerceipt->m_pPlayer),pGoal->m_pPerceipt->m_pPlayer->m_iNearestWP);
		}

		// so create an short term memory item
		CPerceipt *pPerc;
		pPerc = m_pBot->m_pPerception->addPerceipt();
		pPerc->m_lType = CPerceipt::PT_ACTION;
		pPerc->m_lTypeSpec |= CPerceipt::PTX_ACTION_ENEMY;
		pPerc->m_pPlayer = pGoal->m_pPerceipt->m_pPlayer;
		pPerc->m_pEntity = pGoal->m_pPerceipt->m_pEntity;
		pPerc->m_VOrigin = pGoal->m_pPerceipt->m_VOrigin;
		pPerc->m_iAddInfo = pGoal->m_pPerceipt->m_lTypeSpec;
		pPerc->m_fDistance = pGoal->m_pPerceipt->m_fDistance;

		// check view
		Vector VA = m_pBot->getAngles();
		
		VA.x = -VA.x * 3;
		VA.z = 0;
		
		// check if we already aimed close enough to the enemy
		//UTIL_MakeVectors(VA);
		Vector VLook = VA.angles2vec();
		Vector VDiff = (pGoal->m_pPerceipt->m_VOrigin - m_pBot->getOrigin()).normalize();

		float fTest = m_pBot->isZooming()?
			(m_pBot->isDucking()?7.f:10.f):40.f;
		Vector VTestDir = CrossProduct(VDiff,Vector(0,0,1));
		//g_pGame->drawBeam(DEBUG_SYSTEM,g_pGame->m_pListenserverEdict,pGoal->m_pPerceipt->m_VOrigin + Vector(0,0,50) - VTestDir*fTest,pGoal->m_pPerceipt->m_VOrigin + Vector(0,0,50) + VTestDir*fTest);
		g_pGame->drawBeam(DEBUG_SYSTEM,g_pGame->m_pListenserverEdict,pGoal->m_pPerceipt->m_VOrigin,pGoal->m_pPerceipt->m_VOrigin + ((VLook-VDiff)*pGoal->m_pPerceipt->m_fDistance),10,10,Vector(0,255,0));
		
		if(//DotProduct(VLook,VDiff) <.97
			((VLook-VDiff)*pGoal->m_pPerceipt->m_fDistance).length() > (m_pBot->isZooming()?7.f:40.f)		// if we zoom, let's aim better at our enemy
			&& pGoal->m_pPerceipt->m_fDistance>100.f)
			return;
		
		// check recoil
		double fAngle = ((fabs(m_pBot->getPunchAngle().y) 
			+ fabs(m_pBot->getPunchAngle().x)) / 2.0)/180.0*3.1415926535;
		if(tan(fAngle) * pGoal->m_pPerceipt->m_fDistance > 17.5f){
			// cool down for some time :)
			m_pBot->m_pAction->pauseAttackO(m_pBot->m_pWeapons->getCWeapon()->m_fPauseTime);
#ifdef DEBUGMESSAGES
			g_pGame->drawBeam(DEBUG_BOT|DEBUG_ACTION,g_pGame->m_pListenserverEdict.getEntity(),m_pBot->getOrigin(),m_pBot->getOrigin()+Vector(0,0,50),10,10,Vector(255,255,255),255,0);
#endif
			return;
		}
		// check if we should duck
		fDuckNow = m_pBot->m_pWeapons->getCWeapon()->m_Duck.getValue(pGoal->m_pPerceipt->m_fDistance);
		if(fDuckNow > 1
			&& m_pBot->m_pWeapons->m_iCurrentWClip > 2){
			m_pBot->m_pAction->duckO(.75);
			// shall we only shoot when ducked ?
			if(fDuckNow > 2
				&& !m_pBot->isDucking()){
				return;
			}
		}
		// check if we should zoom :)
		fZoomNow = m_pBot->m_pWeapons->getCWeapon()->m_Zoom.getValue(pGoal->m_pPerceipt->m_fDistance);
		if(!m_pBot->isZooming()
			&& fZoomNow > 1
			&&!m_pBot->isAttacking2()){
			m_pBot->m_pAction->attack2();			// zoom in

			if(fZoomNow > 2){
				m_pBot->m_pAction->pauseAttackO(.3f);
			}
			else{
				m_pBot->m_pAction->pauseAttackO(.15f);
			}
		}
		if(fZoomNow > 2
			&& !m_pBot->isZooming())
			return;		// return if we should only shoot when we are in zoomed mode

		// check TK

		Vector v_shoot;
		Vector v_gun;

		v_gun = m_pBot->getGunPosition();

		v_shoot = m_pBot->getVAngle() + m_pBot->getPunchAngle();
		v_shoot.z = 0;  // reset roll to 0 (straight up and down)

		//UTIL_MakeVectors( v_shoot );

		v_shoot = v_shoot.angles2vec().normalize() * 1000.f;	// 1000 should be sufficient

		TraceResult tr;

		g_pGame->traceLine(v_gun,v_gun+v_shoot,dont_ignore_monsters,ignore_glass,m_pBot->getContainingEntity(),&tr);

		if(// tr.flFraction < .9
			tr.pHit
			&& g_pGame->isPlayer(tr.pHit) != -1
			&& (tr.pHit != pGoal->m_pPerceipt->m_pEntity)){// line isnt fract by enemy
				// let's better stop shootin'
				m_pBot->m_pAction->pauseAttackO(.15f);
			}
		
		// check if we should press attack
		if(!m_pBot->isAttacking()){			// just do this so we dont have to care about pistols and stuff
			m_pBot->m_pAction->attack();
		}
	}
}

// shoot obstacle

void CBVNP_HLDM_ShootObstacle::evaluate(list<CGoal> *pGoals,CPerceipt *p){
	if(m_pBot->m_pPerception->m_pbObstacle[CPerceptionBot::POD_Eye]){
		TraceResult *ptr;

		ptr = &m_pBot->m_pPerception->m_pTObstacle[CPerceptionBot::POD_Eye];
		if(ptr->pHit){
			if(ptr->pHit->v.takedamage == DAMAGE_YES){	// don't shoot undestroyable breakables
				if(!strcmp(CEntity(ptr->pHit).getClassName(),"func_breakable")){
					// it's a breakable ent

					pGoals->push_front(CGoal(
						(int)30,
						CGoal::GT_ACTION|CGoal::GT_LOOK,this,p));
				}
			}
		}
	}
}

void CBVNP_HLDM_ShootObstacle::execute(CGoal *pGoal){
	Vector VAim = m_pBot->m_pPerception->m_pTObstacle[CPerceptionBot::POD_Eye].vecEndPos;

	m_pBot->m_pAction->lookTo(VAim);
#ifdef DEBUGMESSAGES1
	g_pGame->drawBeam(DEBUG_BOT|DEBUG_ACTION,g_pGame->m_pListenserverEdict.getEntity(),m_pBot->getOrigin(),VAim,4,0,Vector(255,255,255),255,0);
#endif
	// check if the bot already looks there
	/*Vector VA = m_pBot->getAngles();

	VA.x = -VA.x * 3;
	VA.z = 0;

	UTIL_MakeVectors(VA);
	Vector VLook = gpGlobals->v_forward.normalize();
	Vector VDiff = (VAim - m_pBot->getOrigin() + m_pBot->getVOffset()).normalize();

	if(DotProduct(VLook,VDiff) >.7f){
		m_pBot->m_pAction->attack();
	}*/
	m_pBot->m_pAction->attack();
}

// watch Damage

void CBV_HLDM_WatchDamage::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if(p->m_lType == CPerceipt::PT_DAMAGE
		&&!(p->m_lTypeSpec & CPerceipt::PTX_DAMAGE_STRANGE)){
		pGoals->push_front(CGoal(
			(15)*((p->m_fLifetime - g_pGame->getTime() + p->m_fLUpdate)/p->m_fLifetime),
			CGoal::GT_LOOK,this,p));
	}
	return;
}

void CBV_HLDM_WatchDamage::execute(CGoal *pGoal){
	m_fLastExecution = g_pGame->getTime();
	
	if(pGoal->m_pPerceipt){
		m_pBot->m_pAction->lookTo(pGoal->m_pPerceipt->m_VOrigin);
#ifdef DEBUGMESSAGES1
		g_pGame->drawBeam(DEBUG_BOT|DEBUG_ACTION,g_pGame->m_pListenserverEdict.getEntity(),m_pBot->getOrigin(),pGoal->m_pPerceipt->m_VOrigin,4,0,Vector(0,255,0),255,0);
#endif
	}
}

// hear

void CBV_HLDM_Hear::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if(p->m_lType == CPerceipt::PT_SOUND
		&& p->m_lTypeSpec & CPerceipt::PTX_SOUND_ENEMY
		&& p->m_lTypeSpec & CPerceipt::PTX_SOUND_LOC){

			if(p->m_fDistance > 1400.f * p->m_fValue / 256.f && !(p->m_lTypeSpec & CPerceipt::PTX_SHOOTATME)) {
				return;
			}

			int iPriority = 20 + p->m_lTypeSpec & CPerceipt::PTX_SHOOTATME ? 15:0;
			iPriority += (p->m_fDistance<1000?-p->m_fDistance/100:-10)	// distance
				* (p->m_fLifetime - g_pGame->getTime() + p->m_fLUpdate)/p->m_fLifetime;	// old stuff isnt important :D

			pGoals->push_front(CGoal(
				iPriority,
				CGoal::GT_LOOK,
				this,p));
		}
		return;
}

void CBV_HLDM_Hear::execute(CGoal *pGoal){
	m_fLastExecution = g_pGame->getTime();
	
	if(pGoal->m_pPerceipt){
		m_pBot->m_pAction->lookTo(pGoal->m_pPerceipt->m_VOrigin);
#ifdef DEBUGMESSAGES1
		if(pGoal->m_pPerceipt->m_lTypeSpec & CPerceipt::PTX_SHOOTATME)
			g_pGame->drawBeam(DEBUG_BOT|DEBUG_ACTION,g_pGame->m_pListenserverEdict.getEntity(),m_pBot->getOrigin(),pGoal->m_pPerceipt->m_VOrigin,4,0,Vector(0,0,255),255,0);
#endif
	}
}

// is there the possibility to throw a grenade somewhere ?

void CBV_HLDM_ExplGrenade::evaluate(list<CGoal> *pGoals,CPerceipt *p){
	if(!m_pBot->m_pWeapons->hasExplGrenade())
		return;

	/*if(fabs(g_pGame->getTime() - m_fLastCheck) < 1.f)
		return;*/

	if(p->m_lType != CPerceipt::PT_ENEMY)			// look out for enemies
		return;

	if(p->m_fDistance < 400 || p->m_fDistance > 1300)		// which are in range
		return;

	if(fabs(p->m_fLUpdate -g_pGame->getTime()) < .5)		// which just went out of sight
		return;

	m_fLastCheck = g_pGame->getTime();

	// check if we can get the permission from the squad to throw a grenade
	CBaseTeam *pTeamData = g_pGame->getTeamData(m_pBot->getTeam());
	CBaseSquad *pSquad;
	if(pTeamData &&
		(pSquad = pTeamData->getSquad(m_pBot->m_iSquad)) &&
		!pSquad->requestPermission(m_pBot,TP_THROWGRENADE,4.f))
		return;

	pGoals->push_front(CGoal(
		100,
		CGoal::GT_LOOK | CGoal::GT_ACTION,
		this,p));
}

void CBV_HLDM_ExplGrenade::execute(CGoal *pGoal){
	if(!m_pBot->m_pWeapons->hasExplGrenade())
		return;

	/*if(g_pGame->m_pListenserverEdict.getEntity()){
		g_pGame->drawBeam(g_pGame->m_pListenserverEdict.getEntity(),m_pBot->getOrigin(),m_pBot->getOrigin() + Vector(0,0,50),5,5,Vector(255,255,255),255,1,100);
	}*/

	Vector VAim = pGoal->m_pPerceipt->m_VOrigin;

	Vector VDir = VecCheckThrow(&m_pBot->getEntity()->v,m_pBot->getOrigin(),VAim,630,0.67);
	if(VDir.length() < .1f)
		return;

	if(m_pBot->m_pWeapons->getCWeaponID() != m_pBot->m_pWeapons->hasExplGrenade()){
		m_pBot->change2Weapon(m_pBot->m_pWeapons->hasExplGrenade());
		return;
	}

	m_pBot->m_pAction->lookAlong(VDir);
	if(RANDOM_LONG(0,100) < 50)
		m_pBot->m_pAction->attackO(.01);
}

// Suppressing fire ?

void CBV_HLDM_SuppFire::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	// do we have an enemy here which just went out of our view ?
	if(p->m_lType != CPerceipt::PT_ENEMY)
		return;

	if(p->m_lTypeSpec & CPerceipt::PTX_INVALID)
		return;

	if(m_pBot->m_fCalledBV - p->m_fLUpdate > .5f)	// it hasnt been updated for little time
		return;

	if(p->m_fLUpdate > g_pGame->getTime() - .5f)
		return;

	if(m_fLastCheck > g_pGame->getTime() - .5f)
		return;

	// enough ammo ? -> at least 33%
	if(m_pBot->m_pWeapons->getClipPerc() < .3f)
		return;

	// are we aggressive or should we better be calm ?
	if(m_pBot->m_fAggressivity < 0 && 0)
		return;

	if((p->m_VOrigin-m_pBot->getOrigin()).length() < 300)
		return;

	// how many wps left on this path ?
	if(m_pBot->m_pMovePath->m_CurrWay.size() - m_pBot->m_pMovePath->m_CurrWay.getCurrentIndex() < 3)
		// just a few left ... we dont really know where we are going to go
		return;

	if(!(m_pBot->m_pWeapons->isSubM(WEAPONB(m_pBot->m_pWeapons->getCWeaponID()))||		// only use appr. weapons
		m_pBot->m_pWeapons->isRifle(WEAPONB(m_pBot->m_pWeapons->getCWeaponID()))||
		m_pBot->m_pWeapons->isMachineGun(WEAPONB(m_pBot->m_pWeapons->getCWeaponID()))))
		return;


	m_fLastCheck = g_pGame->getTime();			// so that the following stuff isnt done too many times a second

	// distance to enemy and distance to enemy 2 waypoints later on that path
	float fDist1 = ( m_pBot->getOrigin() - p->m_VOrigin ).length();
	float fDist2 = ( p->m_VOrigin - g_Map.m_Waypoints[m_pBot->m_pMovePath->m_CurrWay.getOffset(2)].m_VOrigin).length();
	//float fDistWP = ( m_pBot->getOrigin() - g_Map.m_Waypoints[m_pBot->m_pMovePath->m_CurrWay.m_piIndices[m_pBot->m_pMovePath->m_CurrWay.m_iActCount+2]].m_VOrigin ).length();

	if(fDist2 < fDist1 - 100.f){	// we are approaching this enemy
		CBitField BFNearEnemy(g_Map.m_Waypoints.m_iNumWaypoints),
			BFNotNearBot(g_Map.m_Waypoints.m_iNumWaypoints),
			BFVis(g_Map.m_Waypoints.m_iNumWaypoints);

		CBitField::bf_iterator iter;

		// look if we can see some of the nearby waypoints to the enemy
		g_Map.m_Waypoints.setDistanceMask(&BFNearEnemy,1,350,p->m_VOrigin);
		// dont take the waypoints which are less than 250 units away
		g_Map.m_Waypoints.setDistanceMask(&BFNotNearBot,1,250,m_pBot->getOrigin());
		BFNotNearBot.negate();
		// take only visible waypoints from here
		g_Map.m_Waypoints.setVisibleMask(&BFVis,m_pBot->m_iNearestWP);

		BFNearEnemy &= BFVis;			// everything that's near to the enemy and visible to us
		//g_Map.m_Waypoints.broadenMask(&BFNearEnemy);
		BFNearEnemy &= BFNotNearBot;	// everything that's far enough from me

		long lShootAt = BFNearEnemy.getSet();
		if(lShootAt != -1){
			// create suppfire perceipt

			CPerceipt *pNewPerceipt = m_pBot->m_pPerception->addPerceipt(CPerceipt::PT_ACTION,CPerceipt::PTX_ACTION_SUPPFIRE);
			
			pNewPerceipt->m_VOrigin = g_Map.m_Waypoints[lShootAt].m_VOrigin;
			// just let'em shoot about 5 rounds or 15 with a machinegun
			pNewPerceipt->m_iAddInfo = m_pBot->m_pWeapons->m_iCurrentWClip
				- m_pBot->m_pWeapons->isMachineGun(WEAPONB(m_pBot->m_pWeapons->getCWeaponID())) ? 18:8;
			if(pNewPerceipt->m_iAddInfo<0)pNewPerceipt->m_iAddInfo=0;
#ifdef DEBUGMESSAGES
			g_pGame->drawBeam(DEBUG_BOT|DEBUG_ACTION,g_pGame->m_pListenserverEdict.getEntity(),m_pBot->getOrigin(),m_pBot->getOrigin() + Vector(0,0,50),5,5,Vector(255,0,0),255,1,100);
#endif
		}
	}
}

void CBV_HLDM_SuppFire::execute(CGoal *pGoal){
	m_fLastExecution = g_pGame->getTime();
}

// Suppressing fire -> reacting to PTX_ACTION_SUPPFIRE items

void CBV_HLDM_SuppFireAc::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if(p->m_lType != CPerceipt::PT_ACTION)
		return;
	if(!(p->m_lTypeSpec & CPerceipt::PTX_ACTION_SUPPFIRE))
		return;
	if(p->m_iAddInfo > 0){		// is a limit set to which we are allowed to fire bullets ?
		if(m_pBot->m_pWeapons->m_iCurrentWClip <= p->m_iAddInfo)	// well, we arent allowed to shoot
			return;
	}

	pGoals->push_front(CGoal(
		100,
		CGoal::GT_LOOK|CGoal::GT_ACTION,
		this,p));
}

void CBV_HLDM_SuppFireAc::execute(CGoal *pGoal){
	m_fLastExecution = g_pGame->getTime();
	
	m_pBot->m_pAction->lookTo(pGoal->m_pPerceipt->m_VOrigin);
#ifdef DEBUGMESSAGES1
	g_pGame->drawBeam(DEBUG_BOT|DEBUG_ACTION,g_pGame->m_pListenserverEdict.getEntity(),m_pBot->getOrigin(),pGoal->m_pPerceipt->m_VOrigin,4,0,Vector(255,255,0),255,0);
#endif
	m_pBot->m_pAction->attack();
}

// movePath

void CBVNP_HLDM_MovePath::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	float fLastDist;
	
	switch(m_iState){
	case 0:{			// waypoint based moving
		if(!m_CurrWay.size()){
			// no way to move along
			m_iCurrWP = -1;

			// let's camp for a second
			//m_pBot->m_pCamp->campO(1);
			m_pBot->m_pAction->slowDownO(.4f,0);
			return;
		}
		
		if(m_iCurrWP >-1){	// it's a valid waypoint
			fLastDist = (g_Map.m_Waypoints[m_iCurrWP].m_VOrigin - m_pBot->getOrigin()).length();
		}
		else{	// no waypoint to run to set yet ?
			if(!m_CurrWay.size()){
				reset();
			}
			else{
				m_fLastDist = 10000;
				initPath();
				
				m_bSetTarget = true;
				execute(0);
			}
			return;
		}
		
		if(m_fLastDist < fLastDist){	// we are moving away from current waypoint we aim to
			if(fLastDist > 75.f && m_pBot->m_iNearestWP != m_CurrWay.getOffset(1)){		// we don't skip a waypoint, we gotta head back !
				m_bSetTarget = true;
				execute(0);
				//m_pAction->runTo(g_Map.m_Waypoints[m_iCurrWP].m_VOrigin);
			}
			else{
				// we wanna go to the next waypoint on this path, because we are near to our next waypoint, or even near to the following waypoint
				m_iLastWP = m_iCurrWP;
				if((m_iCurrWP = m_CurrWay.advance()) != -1){
					// so we have a subsequent waypoint to go to
					m_fLastVisitedWP = g_pGame->getTime();
					
					m_bSetTarget = true;
					execute(0);
					//m_pAction->lookTo(g_Map.m_Waypoints.m_Waypoints[iRun2].m_VOrigin);
#ifdef DEBUGMESSAGES				
					//if(g_pGame->m_pListenserverEdict.getEntity())g_pGame->drawBeam(g_pGame->m_pListenserverEdict.getEntity(),g_Map.m_Waypoints.m_Waypoints[m_iCurrWP].m_VOrigin,m_pPlayer->getOrigin(),10,10,255,255,255,255,20);
					//if(g_pGame->m_pListenserverEdict.getEntity())g_pGame->drawBeam(g_pGame->m_pListenserverEdict.getEntity(),g_pGame->m_pListenserverEdict.getEntity()->v.origin,m_pPlayer->getOrigin(),10,10,255,255,255,255,20);
#endif
					
					fLastDist = 10000;
				}
				else{
					// we are at the end of the path
					if(m_bMove2Origin){
						m_iState = 1;		// go to direct moving state
						m_bSetTarget = true;
						m_fLastVisitedWP = g_pGame->getTime();
					}
					else{
						if(m_pDReached){			// call callback function
							m_pDReached->onReachedDestination(m_pBot);
						}
						reset();
					}
				}
			}
		}
		else{
			// just check the distance to the nearest waypoint ... and crouch if it is a crouch waypoint, but
			// only if it's the next on our path
			/*if(fLastDist < 60.f){
				if(g_Map.m_Waypoints[m_pBot->m_iNearestWP].m_lFlags & CWaypoint::CROUCH){
					if(m_pBot->m_iNearestWP == m_CurrWay.getCurrent()
						||m_pBot->m_iNearestWP == m_CurrWay.getOffset(1)){
							m_pBot->m_pAction->duckO(.5f);
						}
				}
			}*/
			if(g_Map.m_Waypoints[m_pBot->m_iNearestWP].m_lFlags & CWaypoint::CROUCH
				|| (m_CurrWay.getOffset(1)!=-1
				&& g_Map.m_Waypoints[m_CurrWay.getOffset(1)].m_lFlags & CWaypoint::CROUCH)){
					m_pBot->m_pAction->duckO(.5f);
				}
		}
		/*else{
			if(m_pBot->getVelocity().length() > 100){
				Vector VAim;
				switch(m_iState){
				case 0:{
					VAim = g_Map.m_Waypoints[m_iCurrWP].m_VOrigin;
					   }break;
				case 1:{
					VAim = m_VGoal;
					   }break;
				}
				Vector Cross = CrossProduct(m_pBot->getVelocity(),VAim-m_pBot->getOrigin());
				float fError = Cross.length() / m_pBot->getVelocity().length();
				char szTemp[20];
				sprintf(szTemp,"%f",fError);
				m_pBot->fakeClientCommand("say",szTemp);
			}
		}*/
		if(fabs(m_fLastExecution - g_pGame->getTime()) > .5f){
			m_bSetTarget = true;
			execute(0);
		}
		m_fLastDist = fLastDist;
		   }
		break;
	case 1:{
		fLastDist = (m_VGoal - m_pBot->getOrigin()).length();
		if(m_fLastDist < fLastDist){
			if(fLastDist > 75){
				m_bSetTarget = true;
			}
			else{
				// reached location
				if(m_pDReached){			// call callback function
					m_pDReached->onReachedDestination(m_pBot);
				}
				reset();
			}
		}
		execute(0);
		m_fLastDist = fLastDist;
		   }
		break;
	}
	
	return;
}

void CBVNP_HLDM_MovePath::execute(CGoal *pGoal){
	//if(g_pGame->m_pListenserverEdict.getEntity())g_pGame->drawBeam(g_pGame->m_pListenserverEdict.getEntity(),Vector(0,0,0),m_pBot->getOrigin(),10,10,0,0,255,255,20);
	if(m_bSetTarget ||
		m_iState ||		// set move direction each think frame when moving without waypoints.
		m_fLastExecution != m_pBot->m_fCalledBV){		// last time this move behaviour wasnt called, so set the move target again
		switch(m_iState){
		case 0:{
			if(m_iCurrWP != -1){
				m_pBot->m_pAction->runTo(g_Map.m_Waypoints[m_iCurrWP].m_VOrigin);
				m_bSetTarget = false;
#ifdef DEBUGMESSAGES
				//if(g_pGame->m_pListenserverEdict.getEntity())g_pGame->drawBeam(g_pGame->m_pListenserverEdict.getEntity(),g_Map.m_Waypoints.m_Waypoints[m_iCurrWP].m_VOrigin,m_pBot->getOrigin(),10,10,255,255,255,255,20);
#endif
			}
			else{
				reset();
			}
			   }break;
		case 1:{
			m_pBot->m_pAction->runTo(m_VGoal);
			   }break;
		}
	}
	m_fLastExecution = g_pGame->getTime();
}

void CBVNP_HLDM_MovePath::getResult(AStarBase *p,CCallback_onReachedDestination *pDReached){
	if(p){
		p->getResult(m_CurrWay);
		m_iTargetWP = p->getDestination();
	}
	else{
		m_iTargetWP = -1;
		m_CurrWay.reset();
	}

	m_iState = 0;
	
	m_iCurrWP = -1;

	if(m_CurrWay.getDestination() == -1){
		m_pBot->m_LGoals.finishedMove();
		return;
	}
	
	m_pDReached = pDReached;
	
	// check if we should run there without taking the waypoints
	if((m_VGoal-m_pBot->getOrigin()).length() < 250.f
		&&m_bMove2Origin){
		m_iState = 1;
	}
	else{
		if(!m_CurrWay.size()
			&& !m_bMove2Origin){
			m_pBot->m_LGoals.finishedMove();
		}
	}
}

void CBVNP_HLDM_MovePath::initPath(void){
	// simplest method would be
	// m_iCurrWP = m_CurrWay[0];
	// but we wanna check which would be the best first waypoint

	if(m_CurrWay.size() <= 2){
		m_iCurrWP = m_CurrWay.getStart();
	}
	else{
		Vector V1,
			V2,
			VNow = m_pBot->getOrigin() + m_pBot->getVelocity() * .3f;

		float fLength0,fLength1; 

		V1 = g_Map.m_Waypoints[m_CurrWay.m_piIndices[0]].m_VOrigin - VNow;
		fLength0 = V1.length();
		V2 = g_Map.m_Waypoints[m_CurrWay.m_piIndices[1]].m_VOrigin - g_Map.m_Waypoints[m_CurrWay.m_piIndices[0]].m_VOrigin;
		fLength1 = (g_Map.m_Waypoints[m_CurrWay.m_piIndices[1]].m_VOrigin - VNow).length();

		if(fLength1 < fLength0				// check if second waypoint is already nearer
			||DotProduct(V1,V2) < .0f){		// check if we would have to go back
			m_iCurrWP = m_CurrWay.m_piIndices[1];
		}
		else{
			m_iCurrWP = m_CurrWay.m_piIndices[0];
		}
	}

	// check direct move to
	if(m_bMove2Origin
		&& m_CurrWay.size() >= 2){
		Vector VN,
			VNN;

		while(m_CurrWay.size() >= 2){
			VN = (g_Map.m_Waypoints[m_CurrWay.getDestination()].m_VOrigin - m_VGoal).normalize();
			VNN = (g_Map.m_Waypoints[m_CurrWay.getDestination()].m_VOrigin - g_Map.m_Waypoints[m_CurrWay[m_CurrWay.size() - 2]].m_VOrigin).normalize();
			if(DotProduct(VNN,VN) < 0.f){
				m_CurrWay.m_iCount --;
			}
			break;
		}
	}
}

void CBVNP_HLDM_MovePath::reset(void){
	m_CurrWay.reset();
	m_iState = 0;		// we wanna run go waypoints initially
	m_bMove2Origin = false;
	m_iCurrWP = -1;
	m_iLastWP = -1;
	m_fLastDist = 10000;
	m_fLastVisitedWP = g_pGame->getTime();
	m_bSetTarget = false;
	m_pDReached = 0;
	
	m_pBot->m_LGoals.finishedMove();
}

// explore surrounding ... if we find some place where we could go and where are just a few waypoints or even none,
// go there

void CBVNP_HLDM_Explore::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if(fabs(m_fLastCheck - g_pGame->getTime()) < 1.f)
		return;

	if(m_pBot->m_LGoals.m_pLMoveBv)
		return;

	pGoals->push_back(CGoal(40,CGoal::GT_MOVE,this,0));			// default was 12
	m_fLastExecution = 0;

	return;
}

void CBVNP_HLDM_Explore::execute(CGoal *pGoal){
	/*if(fabs(m_fLastExecution - g_pGame->getTime()) < 3.f)
		return;*/
	if(m_fLastExecution > 0)
		return;
	if(m_iExecuteFSMS)
		return;

	m_fLastExecution = g_pGame->getTime();

	Vector VViewAngles,
		VOViewAngles,
		VViewDir,
		VStart,
		VEnd,VEnd2;
	int iNearWP;
	int i;
	float fDistance,fCheckDist = 2000;
	TraceResult tr,trD;

	// trace start to the ground
	g_pGame->traceLine(m_pBot->getOrigin() + m_pBot->getVOffset(),m_pBot->getOrigin() + m_pBot->getVOffset()-Vector(0,0,1000),ignore_monsters,ignore_glass,0,&tr);
	VStart = tr.vecEndPos + Vector(0,0,60);

	VOViewAngles = VViewAngles = m_pBot->getVAngle();
	VOViewAngles.x = 0;
	
	for(i=0; i < m_iChecks; i++){				// do 10 checks to different directions
		VViewAngles = VOViewAngles;
		fCheckDist = 500.f + RANDOM_LONG(0,4)*500.f;		// check at different distances ... maybe there are waypoints at some distant wall but not in between ?

		VViewAngles.y += RANDOM_LONG(-180,180);		// just look somewhere

		//UTIL_MakeVectors(VViewAngles);
		VViewDir = VViewAngles.angles2vec();
		
		VEnd2 = VStart + VViewDir * 200.f;			// first trace just 200 units to see if there are possibly stairs
		
		g_pGame->traceLine(VStart,VEnd2,ignore_monsters,ignore_glass,0,&tr);

		VEnd2 = tr.vecEndPos - VViewDir * (10.f*tr.flFraction);

#ifdef DEBUGMESSAGES
		//if(g_pGame->m_pListenserverEdict.getEntity())g_pGame->drawBeam(g_pGame->m_pListenserverEdict.getEntity(),VStart,tr.vecEndPos,5,0,255,0,255,0,0,20);
#endif

		// trace end position to the ground
		g_pGame->traceLine(VEnd2,VEnd2-Vector(0,0,1000),ignore_monsters,ignore_glass,0,&tr);
		VEnd2 = tr.vecEndPos + Vector(0,0,60);

		if(VEnd2.z - VStart.z > 50){
			// significant slope up

			//fCheckDist = 200.f / fabs(VEnd2.z - VStart.z) * 200.f;
			fCheckDist = 500.f;
		}

		VViewDir = (VEnd2-VStart).normalize();

		VEnd = VStart + VViewDir * fCheckDist;			// first trace just 200 units to see if there are possibly stairs
		
		//g_pGame->traceLine(VStart,VEnd,ignore_monsters,ignore_glass,0,&tr);
		g_pGame->traceHull(VStart,VEnd,head_hull,0,&tr);

		VEnd = tr.vecEndPos;
		fDistance = tr.flFraction * fCheckDist;

		// trace end position to the ground
		g_pGame->traceLine(VEnd,VEnd-Vector(0,0,1000),ignore_monsters,ignore_glass,0,&tr);
		VEnd = tr.vecEndPos + Vector(0,0,60);

		if(fDistance < 200){
#ifdef DEBUGMESSAGES
			g_pGame->drawBeam(DEBUG_BOT|DEBUG_ACTION,g_pGame->m_pListenserverEdict.getEntity(),VStart,VEnd,5,0,Vector(255,0,0),255,0,20);
#endif
			// this is just to fucking close ... no need to go there
			continue;
		}
		
		iNearWP = g_Map.m_Waypoints.getNearest(VEnd,false,false,0,225);	// check for waypoints near the end position
		
		if(iNearWP != -1){
#ifdef DEBUGMESSAGES
			g_pGame->drawBeam(DEBUG_BOT|DEBUG_ACTION,g_pGame->m_pListenserverEdict.getEntity(),VStart,VEnd,5,0,Vector(0,255,0),255,0,20);
#endif
			continue;
		}
		
		if(!g_Map.getReachable(m_pBot->getOrigin(),VEnd)){
#ifdef DEBUGMESSAGES
			g_pGame->drawBeam(DEBUG_BOT|DEBUG_ACTION,g_pGame->m_pListenserverEdict.getEntity(),VStart,VEnd,5,10,Vector(0,0,255),255,0,20);
#endif
			// no reachable ... get on checking other more interesting places
			continue;
		}

#ifdef DEBUGMESSAGES
		g_pGame->drawBeam(DEBUG_BOT|DEBUG_ACTION,g_pGame->m_pListenserverEdict.getEntity(),VStart,VEnd,5,0,Vector(255,255,255),255,0,60);
#endif

		// let's override the priorities here and set a higher priority to get there ... it's important 
		// to have a good wp grid, but it's also important that this behaviour doesnt suppress everything else
		// when it's just searching
		pGoal->m_iImportance += 20;

		// set the stuff for movepath
		m_pBot->m_pMovePath->m_VGoal = VEnd;
		
		AStarBase *pAStar = m_pBot->getAStarMachine();
		int i1 = g_Map.m_Waypoints.getNearesto(VEnd);
		
		pAStar->setDestination(i1);
		pAStar->setStart(m_pBot->m_iNearestWP);
		m_pBot->m_pMovePath->m_fLastVisitedWP = g_pGame->getTime();
		
		g_AStarTasks.addTask(pAStar,5,m_pBot,this);		// add task
		m_iExecuteFSMS = 1;		// dont search more

		break;
	}
	m_fLastCheck = g_pGame->getTime();
	if(!m_iExecuteFSMS){
		m_pBot->m_pMovePath->reset();
	}
}

void CBVNP_HLDM_Explore::onPathCreation(AStarBase *p){
	m_iExecuteFSMS = 0;

	m_pBot->m_pMovePath->m_bMove2Origin = true;
	m_pBot->m_pMovePath->getResult(p);
	m_pBot->m_pMovePath->m_iState = 1;
	m_pBot->m_pMovePath->m_fLastVisitedWP = g_pGame->getTime() + 7.f;

	delete p;
}

// last enemy emotion

void CEM_HLDM_LastEnemy::preprocess(void){
	m_fTime = 0;
}

void CEM_HLDM_LastEnemy::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if(p->m_lType != CPerceipt::PT_ACTION)
		return;
	if(!(p->m_lTypeSpec & CPerceipt::PTX_ACTION_ENEMY))
		return;

	if(m_fTime < p->m_fLUpdate){				// update the last enemy seen time to the highest value
		m_fTime = p->m_fLUpdate;
		m_VOrigin = p->m_VOrigin;
	}
}

void CEM_HLDM_LastEnemy::postprocess(void){
}

// team emotion

void CEM_HLDM_Team::preprocess(void){
	m_iVisCount = 0;
	m_iCount = 0;

	m_VAvPos = Vector(0,0,0);
	m_VAvVel = Vector(0,0,0);
	m_iCount = 0;
}

void CEM_HLDM_Team::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if(p->m_lType == CPerceipt::PT_TMATE
		|| (p->m_lType & CPerceipt::PT_SOUND && p->m_lTypeSpec & CPerceipt::PTX_SOUND_TMATE)){		
			++m_iCount;
			m_VAvPos = m_VAvPos + p->m_VOrigin;
			if(p->m_pPlayer){
				m_VAvVel = m_VAvVel + p->m_pPlayer->getVelocity();

				if(g_Map.m_Waypoints.getVisible(m_pBot->m_iNearestWP,p->m_pPlayer->m_iNearestWP)){
					++m_iVisCount;
					m_VVisAvPos = m_VVisAvPos + p->m_VOrigin;
					m_VVisAvVel = m_VVisAvVel + p->m_pPlayer->getVelocity();
				}
			}
	}
	
	return;
}

void CEM_HLDM_Team::postprocess(void){
	m_VAvPos = m_VAvPos * (1.f/float(m_iCount));
	m_VAvVel = m_VAvVel * (1.f/float(m_iCount));
	
	m_VVisAvPos = m_VVisAvPos * (1.f/float(m_iVisCount));
	m_VVisAvVel = m_VVisAvVel * (1.f/float(m_iVisCount));
}

// enemy emotion

void CEM_HLDM_Enemy::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if(p->m_lType == CPerceipt::PT_ENEMY
		|| (p->m_lType & CPerceipt::PT_SOUND && p->m_lTypeSpec & CPerceipt::PTX_SOUND_ENEMY)){
			++m_iCount;
			m_VAvPos = m_VAvPos + p->m_VOrigin;
			if(p->m_pPlayer){
				m_VAvVel = m_VAvVel + p->m_pPlayer->getVelocity();

				if(g_Map.m_Waypoints.getVisible(m_pBot->m_iNearestWP,p->m_pPlayer->m_iNearestWP)){
					++m_iVisCount;
					m_VVisAvPos = m_VVisAvPos + p->m_VOrigin;
					m_VVisAvVel = m_VVisAvVel + p->m_pPlayer->getVelocity();
				}
			}
	}

	return;
}

// attack enemy directly

void CBV_HLDM_AttackDirect::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if(p->m_lType != CPerceipt::PT_ACTION)
		return;

	if(!(p->m_lTypeSpec & CPerceipt::PTX_ACTION_ENEMY))
		return;

	if(m_pBot->m_pEMTeam->m_iVisCount < 2)
		return;

	if(g_pGame->getTime() - p->m_fLUpdate > .05f)
		return;

	pGoals->push_front(CGoal(20
		+ m_pBot->m_fAggressivity * 4.f
		,CGoal::GT_MOVE,this,p));
	
	return;
}

void CBV_HLDM_AttackDirect::initialize(CGoal *pGoal){
	m_fLastExecution = g_pGame->getTime();

	AStarBase *pAStar = m_pBot->getAStarMachine();
	int i1 = pGoal->m_pPerceipt->m_pPlayer->m_iNearestWP;

	pAStar->setDestination(i1);
	pAStar->setStart(m_pBot->m_iNearestWP);
	m_pBot->m_pMovePath->m_fLastVisitedWP = g_pGame->getTime();
	
	g_AStarTasks.addTask(pAStar,5,m_pBot,this);		// add task
	
	m_iExecuteFSMS ++;		// increment state
}

void CBV_HLDM_AttackDirect::onPathCreation(AStarBase *p){
	m_iExecuteFSMS ++;

	m_pBot->m_pMovePath->getResult(p);

	delete p;
}

void CBV_HLDM_AttackDirect::execute(CGoal *pGoal){
	m_fLastExecution = g_pGame->getTime();
}

// attack enemy covered

void CBV_HLDM_AttackIndirect::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if(p->m_lType != CPerceipt::PT_ACTION)
		return;

	if(!(p->m_lTypeSpec & CPerceipt::PTX_ACTION_ENEMY))
		return;

	if(p->m_fDistance < 600.f)
		return;

	if(g_pGame->getTime() - p->m_fLUpdate > .05f)
		return;

	pGoals->push_front(CGoal(20
		- m_pBot->m_fAggressivity * 4.f
		+ (m_pBot->m_pEMTeam->m_iVisCount<2?1:0) * 5
		+ (100-m_pBot->getHealth())/20.f
		,CGoal::GT_MOVE,this,p));
	
	return;
}

void CBV_HLDM_AttackIndirect::initialize(CGoal *pGoal){
	m_fLastExecution = g_pGame->getTime();

	AStarBase *pAStar = m_pBot->getAStarMachine();
	m_pBot->addCostVisEnemies((AStarMachine<AStarGoalMod> *)pAStar,.7f);
	int i1 = pGoal->m_pPerceipt->m_pPlayer->m_iNearestWP;

	pAStar->setDestination(i1);
	pAStar->setStart(m_pBot->m_iNearestWP);
	m_pBot->m_pMovePath->m_fLastVisitedWP = g_pGame->getTime();
	
	g_AStarTasks.addTask(pAStar,5,m_pBot,this);		// add task
	
	m_iExecuteFSMS ++;		// increment state
}

void CBV_HLDM_AttackIndirect::onPathCreation(AStarBase *p){
	m_iExecuteFSMS ++;

	m_pBot->m_pMovePath->getResult(p);

	delete p;
}

// pullback from enemy

void CBV_HLDM_Pullback::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if(p->m_lType != CPerceipt::PT_ACTION)
		return;

	if(!(p->m_lTypeSpec & CPerceipt::PTX_ACTION_ENEMY))
		return;

	if(g_pGame->getTime() - m_fLastExecution < .5f)
		return;

	if(g_pGame->getTime() - p->m_fLUpdate > .05f)
		return;

	int iPriority = 25;
	Vector VDiff = p->m_VOrigin - m_pBot->getOrigin();
	Vector VDiffN = VDiff.normalize();

	if(m_pBot->m_pEMTeam->m_iVisCount >= 2){		// a bit cohesion ?!
		float fAwayVel = DotProduct(VDiffN,m_pBot->m_pEMTeam->m_VVisAvVel);	// velocity away from enemy of the others
		if(fAwayVel > 120.f){
			iPriority += 10;			// the others go away ... we should too
		}
		if(fAwayVel < -120.f){
			iPriority -= 10;
		}
	}

	if(m_pBot->m_pWeapons->getCWeaponID() != -1){			// check if our weapon would be better on higher distance
		float fBestDistance = m_pBot->m_pWeapons->getBestDistance(m_pBot->m_pWeapons->getCWeaponID());
		if(fBestDistance > p->m_fDistance){
			iPriority += 7;
		}
		else{
			iPriority -= 7;
		}
	}

	if(float(m_pBot->m_pWeapons->m_iCurrentWClip) / float(m_pBot->m_pWeapons->getCWeapon()->m_iClipSize) < .25f)
		iPriority += 10;

	if(m_pBot->m_pEMTeam->m_iVisCount >= 1){		// if most others are way back, go back too
		if(VDiff.length() < (p->m_VOrigin - m_pBot->m_pEMTeam->m_VVisAvPos).length() - 120.f){
			iPriority += 6;
		}
	}

	pGoals->push_front(CGoal(iPriority
		,CGoal::GT_MOVE,this,p));

	return;
}

void CBV_HLDM_Pullback::initialize(CGoal *pGoal){
	m_fLastExecution = g_pGame->getTime();

	int iENow = pGoal->m_pPerceipt->m_pPlayer->m_iNearestWP;
	/*int iDestination = -1;
	int iNow = m_pBot->m_iNearestWP;
	float fDistance,fMin = 100000;
	Vector VDiffP = m_pBot->getOrigin() - pGoal->m_pPerceipt->m_VOrigin;
	float fPDistance = VDiffP.Length();
	Vector VDiffPN = VDiffP * (1.f/fPDistance);
	Vector VDiff;

	// find a fucking sufficient waypoint
	{
		int i;
		for(i = 0; i< g_Map.m_Waypoints.m_iNumWaypoints;i++){
			if(g_Map.m_Waypoints[i].m_lFlags & CWaypoint::DELETED)
				continue;
			if(g_Map.m_Waypoints.getVisible(iENow,i))
				continue;
			VDiff = g_Map.m_Waypoints[i].m_VOrigin - m_pBot->getOrigin();
			fDistance = VDiff.Length();
			if(fDistance < 300.f)
				continue;
			if(DotProduct(VDiff,VDiffPN) < .5*fDistance)
				continue;

			if(fDistance < fMin){
				fMin = fDistance;
				iDestination = i;
			}
		}
	}*/

	AStarMachine<AStarGoalHide> *pAStar = new AStarMachine<AStarGoalHide>;

	pAStar->setStart(m_pBot->m_iNearestWP);
	pAStar->m_Goal.hideFrom(iENow,pGoal->m_pPerceipt->m_VOrigin,m_pBot->getOrigin());
	pAStar->m_Goal.setMinimumCost(400);

	m_pBot->m_pMovePath->m_fLastVisitedWP = g_pGame->getTime();
	
	g_AStarTasks.addTask(pAStar,5,m_pBot,this);		// add task
	
	m_iExecuteFSMS ++;		// increment state
}

void CBV_HLDM_Pullback::onPathCreation(AStarBase *p){
	m_iExecuteFSMS ++;

	m_pBot->m_pMovePath->getResult(p);
#ifdef DEBUGMESSAGES
	if(!m_pBot->m_pMovePath->m_CurrWay.m_iCount){
		g_pGame->drawBeam(DEBUG_BOT|DEBUG_NAV,g_pGame->m_pListenserverEdict.getEntity(),m_pBot->getOrigin(),m_pBot->getOrigin()+Vector(0,0,100),10,0,Vector(255,0,0),255,20);
	}
#endif
	m_pBot->m_pMovePath->m_CurrWay.m_iCount--;	// dont go to the last waypoint ... that's invisible :)

#ifdef DEBUGMESSAGES
	/*int i;
	for(i = 1; i < m_pBot->m_pMovePath->m_CurrWay.m_iCount; i++){
		if(g_pGame->m_pListenserverEdict.getEntity()){
			g_pGame->drawBeam(g_pGame->m_pListenserverEdict.getEntity(),
				g_Map.m_Waypoints[m_pBot->m_pMovePath->m_CurrWay.m_piIndices[i]].m_VOrigin,
				g_Map.m_Waypoints[m_pBot->m_pMovePath->m_CurrWay.m_piIndices[i-1]].m_VOrigin,10,0,255,255,0,255,20);
		}
	}*/
#endif

	delete p;
}

// Hide and reload then

void CBV_HLDM_HideReload::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if(p->m_lType != CPerceipt::PT_ACTION)
		return;

	if(!(p->m_lTypeSpec & CPerceipt::PTX_ACTION_ENEMY))
		return;

	if(g_pGame->getTime() - m_fLastExecution < .5f)
		return;

	if(g_pGame->getTime() - p->m_fLUpdate > .05f)
		return;

	int iPriority;
	
	if(float(m_pBot->m_pWeapons->m_iCurrentWClip) / float(m_pBot->m_pWeapons->getCWeapon()->m_iClipSize) < .15f
		|| m_pBot->m_pWeapons->m_iCurrentWClip <= 2){
		iPriority = 50;

		pGoals->push_front(CGoal(iPriority
			,CGoal::GT_MOVE,this,p));
	}

	return;
}

void CBV_HLDM_HideReload::initialize(CGoal *pGoal){
	m_fLastExecution = g_pGame->getTime();

	int iENow = pGoal->m_pPerceipt->m_pPlayer->m_iNearestWP;

	AStarMachine<AStarGoalHide> *pAStar = new AStarMachine<AStarGoalHide>;

	pAStar->setStart(m_pBot->m_iNearestWP);
	pAStar->m_Goal.hideFrom(iENow,pGoal->m_pPerceipt->m_VOrigin,m_pBot->getOrigin());
	pAStar->m_Goal.setMinimumCost(200);

	m_pBot->m_pMovePath->m_fLastVisitedWP = g_pGame->getTime();
	
	g_AStarTasks.addTask(pAStar,5,m_pBot,this);		// add task
	
	m_iExecuteFSMS ++;		// increment state
}

void CBV_HLDM_HideReload::onPathCreation(AStarBase *p){
	m_iExecuteFSMS ++;

	m_pBot->m_pMovePath->getResult(p,this);

	delete p;
}

void CBV_HLDM_HideReload::onReachedDestination(CPlayer *pPlayer){
	if(float(m_pBot->m_pWeapons->m_iCurrentWClip) / float(m_pBot->m_pWeapons->getCWeapon()->m_iClipSize) < .15f		// still right ?!
		|| m_pBot->m_pWeapons->m_iCurrentWClip <= 2){
		m_pBot->m_pAction->reload();
		if(!m_pBot->m_pEMEnemy->m_iVisCount)
			m_pBot->m_pAction->slowDownO(2.f,0.f);
#ifdef DEBUGMESSAGES
		g_pGame->drawBeam(DEBUG_BOT|DEBUG_NAV,g_pGame->m_pListenserverEdict.getEntity(),m_pBot->getOrigin(),m_pBot->getOrigin()+Vector(0,0,100),10,0,Vector(255,0,0),255,20);
#endif
	}
}


// follow enemy we have inside our LTM

void CBVLT_HLDM_AttackFollow::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if(g_pGame->getTime() - p->m_fLUpdate > 10.f)		// dont take stuff that's been in your LTM for more than 10 seconds
		return;
	if(p->m_lTypeSpec & CPerceipt::PTX_ATTACKED)
		return;
	if(m_pBot->m_pWeapons->isSniper(WEAPONB(m_pBot->m_pWeapons->getCWeaponID())))	// dont attack follow with a sniper weapon
		return;

	pGoals->push_front(CGoal(15
		+ m_pBot->m_fAggressivity * 4.f
		+ 10-(g_pGame->getTime() - p->m_fLUpdate)
		,CGoal::GT_MOVE,this,p));
	
	return;
}

void CBVLT_HLDM_AttackFollow::initialize(CGoal *pGoal){
	m_fLastExecution = g_pGame->getTime();

	pGoal->m_pPerceipt->m_lTypeSpec |= CPerceipt::PTX_ATTACKED;		// mark this entry as already being attacked

	AStarMachine<AStarGoalPFled> *pAStar = new AStarMachine<AStarGoalPFled>;
	CBitField pbVisible(g_Map.m_Waypoints.m_iNumWaypoints);
	g_Map.m_Waypoints.setVisibleMask(&pbVisible,m_pBot->m_iNearestWP);
	pbVisible.negate();

	pAStar->m_Goal.fleeFrom(m_pBot->getOrigin(),pGoal->m_pPerceipt->m_VOrigin,pbVisible,(g_pGame->getTime() - pGoal->m_pPerceipt->m_fLUpdate)*150.f);
	pAStar->setStart(g_Map.m_Waypoints.getNearest(pGoal->m_pPerceipt->m_VOrigin,true));
	m_pBot->m_pMovePath->m_fLastVisitedWP = g_pGame->getTime();
	
	g_AStarTasks.addTask(pAStar,7,m_pBot,this);		// add task
	
	m_iExecuteFSMS ++;		// increment state
}

void CBVLT_HLDM_AttackFollow::onPathCreation(AStarBase *p){
	switch(m_iExecuteFSMS){
	case 1:
		{
			// this is the first onPathCreation call ... it was to determine the possible location of the enemy
			int iHide = -1;
			if(p->m_pDestinationNode)
				iHide = p->m_pDestinationNode->m_iWaypoint;		// get the destination waypoint index
			if(iHide == -1){
#ifdef DEBUGMESSAGES
				g_pGame->drawBeam(DEBUG_BOT|DEBUG_ACTION,g_pGame->m_pListenserverEdict.getEntity(),m_pBot->getOrigin(),g_pGame->m_pListenserverEdict.getOrigin(),10,0,Vector(0,255,255),255,255,50);//wod
#endif
				m_pBot->m_LGoals.finishedMove();	// let's quit this behaviour, because we cannot procuce a final path
				break;
			}

			delete p;
#ifdef DEBUGMESSAGES
			g_pGame->drawBeam(DEBUG_BOT|DEBUG_ACTION,g_pGame->m_pListenserverEdict.getEntity(),m_pBot->getOrigin(),g_Map.m_Waypoints[iHide].m_VOrigin,10,0,Vector(0,255,0),255,0,50);// wod
#endif
			AStarBase *pAStar = m_pBot->getAStarMachine();			// and let's go there ( directly atm )
			
			pAStar->setDestination(iHide);
			pAStar->setStart(m_pBot->m_iNearestWP);
			m_pBot->m_pMovePath->m_fLastVisitedWP = g_pGame->getTime();
			
			g_AStarTasks.addTask(pAStar,5,m_pBot,this);		// add task
			m_iExecuteFSMS ++;
			break;
		}
	case 2:
		{
			// this time we should got a path there ...
			m_pBot->m_pMovePath->getResult(p);
			
			delete p;

			m_iExecuteFSMS ++;
			break;
		}
	default:
		// this shouldnt happen
		break;
	};
}

void CBVLT_HLDM_AttackFollow::execute(CGoal *pGoal){
	m_fLastExecution = g_pGame->getTime();
}

// Go somewhere to camp and snipe to get some enemy from LTM

void CBVLT_HLDM_AttackSnipe::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if(g_pGame->getTime() - p->m_fLUpdate > 10.f)		// dont take stuff that's been in your LTM for more than 10 seconds
		return;
	if(p->m_lTypeSpec & CPerceipt::PTX_ATTACKED)
		return;
	if(!m_pBot->m_pWeapons->isSniper(WEAPONB(m_pBot->m_pWeapons->getCWeaponID())))	// don't snipe without a sniper weapon
		return;

	pGoals->push_front(CGoal(15
		+ m_pBot->m_fAggressivity * 4.f
		+ 10-(g_pGame->getTime() - p->m_fLUpdate)
		,CGoal::GT_MOVE,this,p));
	
	return;
}

void CBVLT_HLDM_AttackSnipe::initialize(CGoal *pGoal){
	m_fLastExecution = g_pGame->getTime();

	pGoal->m_pPerceipt->m_lTypeSpec |= CPerceipt::PTX_ATTACKED;		// mark this entry as already being attacked

	int iSnipe = -1;
	CBitField pbVisible(g_Map.m_Waypoints.m_iNumWaypoints),
		pbEDistance(g_Map.m_Waypoints.m_iNumWaypoints),
		pbODistance(g_Map.m_Waypoints.m_iNumWaypoints);

	// so what do we want ?
	// we want a waypoint which is at least 500 units away from the enemy and
	// <1000 units away from me. Additionally it shall be visible to the thread
	g_Map.m_Waypoints.setVisibleMask(&pbVisible,g_Map.m_Waypoints.getNearest(pGoal->m_pPerceipt->m_VOrigin));
	g_Map.m_Waypoints.setDistanceMask(&pbEDistance,1,500,pGoal->m_pPerceipt->m_VOrigin);// less than 500 units away
	pbEDistance.negate();		// it's faster to just mark the waypoints inside the 500 circle and then negate all that stuff :)
	g_Map.m_Waypoints.setDistanceMask(&pbODistance,1,1000,m_pBot->getOrigin());// less than 1000 units away from me
	
	pbVisible &= pbEDistance;
	pbVisible &= pbODistance;
	
	iSnipe = g_Map.m_Waypoints.getMaximum(&pbVisible,CWaypointGraph::WG_SEARCH_IVWP,-1);		// search waypoints which isnt seen by many waypoints, but by the enemies :)
	
	if(iSnipe != -1){
		g_pGame->drawBeam(DEBUG_BOT|DEBUG_NAV,g_pGame->m_pListenserverEdict.getEntity(),m_pBot->getOrigin(),g_Map.m_Waypoints[iSnipe].m_VOrigin,10,0,Vector(0,255,255),255,0,50); // wod
		
		AStarBase *pAStar = m_pBot->getAStarMachine();			// and let's go there ( directly atm )
		
		pAStar->setDestination(iSnipe);
		pAStar->setStart(m_pBot->m_iNearestWP);
		
		m_pBot->m_pMovePath->m_fLastVisitedWP = g_pGame->getTime();
		
		g_AStarTasks.addTask(pAStar,7,m_pBot,this);		// add task
		
		m_iExecuteFSMS ++;		// increment state
	}
	else
		m_pBot->m_LGoals.finishedMove();
}

void CBVLT_HLDM_AttackSnipe::onPathCreation(AStarBase *p){
	// hey, we got a path there :)
	m_pBot->m_pMovePath->getResult(p,this);
	
	delete p;
	
	m_iExecuteFSMS ++;
}

void CBVLT_HLDM_AttackSnipe::execute(CGoal *pGoal){
	m_fLastExecution = g_pGame->getTime();
}

void CBVLT_HLDM_AttackSnipe::onReachedDestination(CPlayer *pPlayer){
	m_pBot->m_pCamp->campO(15.f);		// camp there for some seconds
}

void CBVNP_HLDM_AvoidTK::evaluate(list<CGoal> *pGoals,CPerceipt *p){
	// we wanna execute this code every time if we are in teamm mode	
	if(g_pGame->getTeamplay())
		execute(0);
}

void CBVNP_HLDM_AvoidTK::execute(CGoal *pGoal){
	// tk avoid
	Vector v_shoot;
	Vector v_gun;

	v_gun = m_pBot->getGunPosition();

	v_shoot = m_pBot->getAngles() + m_pBot->getPunchAngle();
	v_shoot.z = 0;  // reset roll to 0 (straight up and down)
	
	//UTIL_MakeVectors( v_shoot );
	
	v_shoot = v_shoot.angles2vec().normalize() * 2000.0;	// 3000 should be sufficient
	
	TraceResult tr;
	
	g_pGame->traceLine(v_gun,v_gun+v_shoot,dont_ignore_monsters,ignore_glass,m_pBot->getEntity(),&tr);
	int iPlayer = g_pGame->isPlayer(tr.pHit);
	if(iPlayer != -1){
		CPlayer *pPlayer = g_pGame->getPlayer(iPlayer);

		if(// tr.flFraction < .9
			pPlayer
			&& pPlayer->getTeam() == m_pBot->getTeam()){// line is probably fract by enemy
				// let's strafe
				// stop shootin
				m_pBot->m_pAction->pauseAttack(.12f);
			}
		else{
			// so we can shoot ...
		}
	}
}
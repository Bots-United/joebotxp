#include "sbv_hldm.h"

#include "Player.h"
#include "BaseBot.h"
#include "Radio.h"
#include "BV_HLDM.h"
#include "PerceptionBot.h"

void CSBVNP_HLDM_RunAround::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	pGoals->push_back(CGoal(10,
		CGoal::GT_MOVE,
		this,
		p  ));

	return;
}

void CSBVNP_HLDM_RunAround::execute(CGoal *){
}

void CSBVNP_HLDM_RunAround::initialize(CGoal *){
	unsigned int ui;
	CBaseBot *pBot;
	bool m_bHasNoWay = false;
	int iWPOther=-1;

	//CPerceipt *pPerceipt;
	int iWP = RANDOM_LONG(0,g_Map.m_Waypoints.m_iNumWaypoints-1);

	/*if(iWPOther != -1)
	iWP = iWPOther;*/

	for(ui=0; ui < m_pSquad->m_LMembers.size(); ui++){
		pBot=g_pGame->getBot(m_pSquad->m_LMembers[ui].m_iPlayerIndex);
		if(!pBot)
			continue;

		m_pSquad->m_LMembers[ui].m_iGoalWP = iWP;
	}
}

void CSBVNP_HLDM_Cohesion::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	unsigned int ui;
	CBaseBot *pBot;
	int iGoal = -1,iNGoal;

	// loop thru all the members which are bots
	for(ui=0; ui < m_pSquad->m_LMembers.size(); ui++){
		pBot=g_pGame->getBot(m_pSquad->m_LMembers[ui].m_iPlayerIndex);
		if(!pBot)
			continue;
		if(!pBot->isAlive())
			continue;

		if(!pBot->m_pMovePath->hasWay())
			return;

		iNGoal = pBot->m_pMovePath->m_CurrWay.getDestination();
		if(iNGoal == -1)
			return;

		if(iNGoal != iGoal && iGoal != -1)	// i.e. not all the members are running to the same point
			return;

		iGoal = iNGoal;	// so store the goal here
	}

	pGoals->push_back(CGoal(10,
		CGoal::GT_ACTION,
		this,
		p  ));
}

void CSBVNP_HLDM_Cohesion::execute(CGoal *){
	unsigned int ui;
	CBaseBot *pBot;
	CPerceipt *pPerceipt;
	float fAvPath = 0;
	int iBots=0;
	float fLength[32],
		fWeaponSpecDist,
		fWeaponMax;

	// loop thru all the members which are bots
	for(ui=0; ui < m_pSquad->m_LMembers.size(); ui++){
		pBot=g_pGame->getBot(m_pSquad->m_LMembers[ui].m_iPlayerIndex);
		if(!pBot)
			continue;
		if(!pBot->isAlive())
			continue;

		if(pBot->m_pWeapons->getCWeapon()){
			fWeaponMax = pBot->m_pWeapons->getCWeapon()->m_Usability.getMaximum();
			if(fWeaponMax > 1000)
				fWeaponMax = 1000;

			fWeaponSpecDist = fWeaponMax * .5f;
		}
		else
			fWeaponSpecDist = 250.f;	// apprx 500 units / 2

		fLength[ui] = 
			// rest length of way
			pBot->m_pMovePath->m_CurrWay.calcLength(&g_Map.m_Waypoints,pBot->m_pMovePath->m_CurrWay.getCurrentIndex())
			// weapon specific addition, since we don't wanna somebody with an awp run in the first line :)
			+ fWeaponSpecDist;

		fAvPath += fLength[ui];

		iBots ++;
		/*if(fLength[ui] == .0f)
		CRASH;*/
	}

	fAvPath/=(float)iBots;

	for(ui=0; ui < m_pSquad->m_LMembers.size(); ui++){
		pBot=g_pGame->getBot(m_pSquad->m_LMembers[ui].m_iPlayerIndex);
		if(!pBot)
			continue;
		if(!pBot->isAlive())
			continue;

		if(fLength[ui] < fAvPath - 250.f){
			// tell that bot to wait a bit
			// stop for 2 seconds
			if(pBot->m_pAction->getSlowDownTime() < g_pGame->getTime()){
				long lWannaDuck = (RANDOM_LONG(0,(pBot->m_pWeapons->hasPrimary()&&!pBot->m_pWeapons->hasShotgun())?100:70)<20?CPerceipt::PTX_COMMAND_DUCK:0);

				pPerceipt = pBot->m_pPerception->addCommand(CPerceipt::PTX_COMMAND_SQUAD | CPerceipt::PTX_COMMAND_SDOWN | lWannaDuck);
				pPerceipt->m_iAddInfo = lWannaDuck?4:2;		// duration
				pPerceipt->m_fValue = ((lWannaDuck && RANDOM_LONG(0,100)<50)?0:1)*.5f;		// sometimes walk, sometimes stop

				/*char szText[200];
				sprintf(szText,"cohesion fAvP %.2f fLength %.2f",fAvPath,fLength[ui]);
				pBot->fakeClientCommand("say",szText);*/
			}
		}
	}
	m_fLastExecution = g_pGame->getTime();
}

void CSBVNP_HLDM_Avoidance::evaluate(list<CGoal> *pGoals,CPerceipt *p){
	pGoals->push_back(CGoal(5,
		CGoal::GT_ACTION,
		this,
		p  ));
}

void CSBVNP_HLDM_Avoidance::execute(CGoal *){
	unsigned int ui1,ui2;
	CPlayer *pPlayer1,*pPlayer2;
	CBaseBot *pBot1,*pBot2,*pBot2Stop;
	CPerceipt *pPerceipt;

	// loop thru all the members which are bots
	for(ui1=0; ui1 < m_pSquad->m_LMembers.size(); ui1++){
		pPlayer1 = g_pGame->getPlayer(m_pSquad->m_LMembers[ui1].m_iPlayerIndex);
		if(!pPlayer1)
			continue;
		if(!pPlayer1->isAlive())
			continue;
		pBot1 = pPlayer1->isBot();
		if(!pBot1)
			continue;

		for(ui2=ui1+1; ui2 < m_pSquad->m_LMembers.size(); ui2++){	// search in a trianglelike area, so no additional checks if this has already been checked are required
			pPlayer2 = g_pGame->getPlayer(m_pSquad->m_LMembers[ui2].m_iPlayerIndex);
			if(!pPlayer2)
				continue;
			if(!pPlayer2->isAlive())
				continue;
			pBot2 = pPlayer2->isBot();
			if(!pBot2)
				continue;

			// test for avoidance of pBot1 and pBot2
			if(pBot1->m_pMovePath->m_iCurrWP != -1 &&
				pBot1->m_pMovePath->m_iCurrWP == pBot2->m_pMovePath->m_iCurrWP){
					// stop the bot which is farer away from that waypoitn
					if(pBot1->m_pMovePath->m_fLastDist > pBot2->m_pMovePath->m_fLastDist){
						pBot2Stop = pBot1;
					}
					else{
						pBot2Stop = pBot2;
					}
					pPerceipt = pBot2Stop->m_pPerception->addCommand(CPerceipt::PTX_COMMAND_SQUAD | CPerceipt::PTX_COMMAND_SDOWN);
					pPerceipt->m_iAddInfo = 1;		// for one second, 
					pPerceipt->m_fValue = .1f;		// slow down to this value

					//pBot2Stop->fakeClientCommand("say","avoid");
				}
		}
	}
	m_fLastExecution = g_pGame->getTime();
}

void CSBVNP_CS_GotoBombSite::evaluate(list<CGoal> *pGoals,CPerceipt *p){
	if(m_pSquad->hasWeapon(1<<CS_WEAPON_C4)){
		// if we have the c4
		int iImportance = 0;

		// after about 90 seconds the bots should start to go to the bomb places - then the priority is
		// above the threshold for changing the move behaviour. the base behaviour is run around with a
		// priority of 10, so we need to have here at least 15
		iImportance += (g_pGame->getTime() - g_pGame->getRoundStart())/5.f;

		pGoals->push_back(CGoal(iImportance,
			CGoal::GT_MOVE,
			this,
			p  ));
	}
}

void CSBVNP_CS_GotoBombSite::initialize(CGoal *pGoal){
	m_iBombsiteWP = g_Map.m_Waypoints.getNearestB(m_pSquad->getMemberAlive()->getOrigin(),0,false,false,100,10000000,CWaypoint::FLAG);
	CPlayer *pPlayer;
	// if squad is together
	if(m_pSquad->m_fSigmaDist < 300){
		pPlayer = m_pSquad->getMemberAlive();
		if(pPlayer){
			if(g_Map.m_Waypoints[pPlayer->m_iNearestWP].m_lFlags & CWaypoint::FLAG){
				m_pSquad->m_LGoals.finishedMove();
				return;
			}
		}
	}

	g_pGame->debugMsg(DEBUG_SQUAD,"CSBVNP_CS_GotoBombSite::execute\n");
	unsigned int ui;
	CBaseBot *pBot;

	for(ui=0; ui < m_pSquad->m_LMembers.size(); ui++){
		pBot=g_pGame->getBot(m_pSquad->m_LMembers[ui].m_iPlayerIndex);
		if(!pBot)
			continue;

		if(pBot->m_pMovePath->m_CurrWay.getDestination() != m_iBombsiteWP){
			/*CPerceipt *pPerceipt = pBot->m_pPerception->addCommand(CPerceipt::PTX_COMMAND_SQUAD 
				| CPerceipt::PTX_COMMAND_GOTO
				| CPerceipt::PTX_COMMAND_STCAMP);
			pPerceipt->m_iAddInfo = m_iBombsiteWP;*/

			m_pSquad->m_LMembers[ui].m_iGoalWP = m_iBombsiteWP;
		}
	}
	m_fLastExecution = g_pGame->getTime();
}

void CSBVNP_CS_GotoBombSite::execute(CGoal *pGoal){
}

void CSBV_HLDM_AttackDirect::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if(p->m_lType != RC_ENGAGING_ENEMY)
		return;

	int iPlayerISent = g_pGame->isPlayer(p->m_pPlayer);
	if(m_pSquad->isMember(iPlayerISent) == -1)
		return;

	pGoals->push_back(CGoal(30,
		CGoal::GT_MOVE,
		this,
		p  ));
}

void CSBV_HLDM_AttackDirect::initialize(CGoal *pGoal){
	// this bot is a member of this squad
	CPlayer *pEnemy = g_pGame->getPlayer(pGoal->m_pPerceipt->m_iAddInfo);
	int iNearWP = -1;

	if(pEnemy)
		iNearWP = pEnemy->m_iNearestWP;

	if(iNearWP != -1){
		// let all bots go to this position
		vector<CBaseSquadMember>::iterator iter;

		for(iter = m_pSquad->m_LMembers.begin(); iter != m_pSquad->m_LMembers.end(); iter ++){
			iter->m_iGoalWP = iNearWP;
		}
	}
}

void CSBV_HLDM_AttackDirect::execute(CGoal *){	
}
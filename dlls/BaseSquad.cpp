#include "BaseSquad.h"
#include "Game.h"
#include "BaseTeam.h"
#include "BaseBot.h"
#include "Player.h"
#include "PerceptionBot.h"
#include "GoalFinder.h"

#include "sbv_hldm.h"
#include "bv_hldm.h"

//#include <iostream>
//using namespace std;

void FakeClientCommand(edict_t *pBot, char *arg1, char *arg2, char *arg3);

CBaseSquad::CBaseSquad(void){
	m_iSquadID = -1;

	m_fPenaltyforSameWay = 1.f;
	m_bASMinWork = false;

	m_fNextThink = 0;
	m_iMembers = 0;
	m_pTeam = 0;

	m_fThinkPer = .5f;

	m_pPerception = new CPerceptionSquad;

	m_LpBehaviour.push_back(new CSBVNP_HLDM_RunAround(this));
	m_LpBehaviour.push_back(new CSBVNP_HLDM_Cohesion(this));
	m_LpBehaviour.push_back(new CSBVNP_HLDM_Avoidance(this));

	m_LpBehaviour.push_back(new CSBV_HLDM_AttackDirect(this));

	m_LpBehaviour.push_back(new CSBVNP_CS_GotoBombSite(this));
}

CBaseSquad::~CBaseSquad(void){
	delete m_pPerception;
	m_pPerception = 0;

	// free us from any AStarCallback
	g_AStarTasks.deleteTasksFrom(this);

	// free behaviuors
	for_each(m_LpBehaviour.begin(),m_LpBehaviour.end(),Destroy<CSBehaviour*>());
}

void CBaseSquad::onRoundStart(void){
	//FILE *fhd=fopen("log-sq.txt","a");fprintf(fhd,"round start\n");fclose(fhd);
	// reset members at round start
	m_fNextThink = 0;
	CBaseBot *pBot;
	vector<CBaseSquadMember>::iterator iter;
	// reset squad assignement

	for(iter = m_LMembers.begin(); iter != m_LMembers.end(); iter ++){
		if(pBot = g_pGame->getPlayer(iter->m_iPlayerIndex)->isBot())
			pBot->assignSquad(-1);		// tell the bot that he isnt in any squad anymore
	}
	m_LMembers.clear();		// clear list of members
}

void CBaseSquad::think(void){
	if(m_fNextThink > g_pGame->getTime()){	// only think at certain times
		return;
	}
	m_fNextThink = g_pGame->getTime() + m_fThinkPer;	// in 1 second we wanna think the next time
	PROFILE("squadthink");

	// if some bot has no path, call finished move
	CBaseBot *pBot;
	vector<CBaseSquadMember>::iterator iter_members;
	for(iter_members = m_LMembers.begin(); iter_members != m_LMembers.end(); iter_members ++){
		if(iter_members->m_iOldGoalWP == -1
			|| iter_members->m_iOldGoalWP == iter_members->m_iGoalWP){
				// so we know that no path is in work, the bot hasnt got any path yet,
				// or it is at the end of a valid path

				pBot = g_pGame->getBot(iter_members->m_iPlayerIndex);
				if(pBot){
					if(!pBot->m_pMovePath->hasWay()){
						m_LGoals.finishedMove();
						break;
					}
				}
			}
	}

	/*if(!m_LMembers.size()){	// no members yet
	getMembers();
	}*/

	// do we need more members 
	getMembers();

	// percept stuff ... since we get most stuff by messages, clean the lists
	m_pPerception->run();

	// is anybody still alive ?
	int iMembersAlive = countMembersAlive();

	if(!iMembersAlive)		// if we havnt got any members alive, return
		return;

	calcMidPos();		// calculate middle position and sigma

	if(iMembersAlive == 1					// is somebody alone in this squad ?
		&& m_pTeam->countMembersAlive()>1){	// and are there still other players in this team ?
		unsigned int ui = 0;
		bool bFoundAlive = false;
		while(! g_pGame->getPlayer(m_LMembers[ui].m_iPlayerIndex)->isAlive()){
			ui++;
			bFoundAlive = true;
		}
		if(!bFoundAlive)
			return;

		m_pTeam->addMe2BiggestSquad(ui);	// then let'im join another squad

		g_pGame->debugMsg(DEBUG_SQUAD,"%s assigned to a Squad of team #%i - smerge\n",
			g_pGame->getPlayer(ui)->getName(),m_pTeam->m_iTeamID);
	}

	list<CSBehaviour *>::iterator iter;
	list<CPerceipt *>::iterator iter_percs;

	// delete old goals
	m_LGoals.clear();

	// call behaviours
	for(iter = m_LpBehaviour.begin(); iter != m_LpBehaviour.end(); iter ++){
		if((*iter)->m_iPercept == CBehaviour::P_NONE){
			(*iter)->evaluate(&m_LGoals);			// or just call the evaluate function if not perceptive
		}
		if((*iter)->m_iPercept & CBehaviour::P_STMem){							// process short term perceptions
			for(iter_percs = m_pPerception->m_LPerceptions.begin();
				iter_percs != m_pPerception->m_LPerceptions.end(); iter_percs ++){
					(*iter)->evaluate(&m_LGoals,*iter_percs);
				}
		}
		if((*iter)->m_iPercept & CBehaviour::P_LTMem){							// and long term perceptions
			for(iter_percs = m_pPerception->m_LLTMem.begin();
				iter_percs != m_pPerception->m_LLTMem.end(); iter_percs ++){
					(*iter)->evaluate(&m_LGoals,*iter_percs);
				}
		}
		if((*iter)->m_iPercept & CBehaviour::P_Command){							// and command perceptions
			for(iter_percs = m_pPerception->m_LCommands.begin();
				iter_percs != m_pPerception->m_LCommands.end(); iter_percs ++){
					(*iter)->evaluate(&m_LGoals,*iter_percs);
				}
				// and call it once to let them see if there has been still some old interrupted task to be fulfilled
				(*iter)->evaluate(&m_LGoals,0);
		}
	}

	m_LGoals.getWinner();			// determine the best choice of behaviours
	m_LGoals.executeWinner();		// run those behaviours

	thinkPaths();					// check if we gotta start some pathfinding

	// show the squads
#ifdef DEBUGMESSAGES
	if(g_pGame->m_pListenserverEdict.getEntity()){
		int i = -1;
		unsigned int ui;
		for(ui=0; ui < m_LMembers.size(); ui++){
			if(g_pGame->getPlayer(m_LMembers[ui].m_iPlayerIndex)->isAlive()){
				if(i!=-1){
					g_pGame->drawBeam(DEBUG_SQUAD,g_pGame->m_pListenserverEdict.getEntity(),g_pGame->getPlayer(i)->getOrigin(),g_pGame->getPlayer(m_LMembers[ui].m_iPlayerIndex)->getOrigin(),5,0,Vector(255,255,255),255,0,10);
				}
				i = m_LMembers[ui].m_iPlayerIndex;
			}
		}
	}
#endif
}

void CBaseSquad::getMembers(void){
	// get some members
	int iCount = (int)m_LMembers.size();	// how many members do we already have ?
	list<CPlayer*>::iterator iter;
	iter = m_pTeam->m_LpPlayersNotAssigned.begin();
	while( iter != m_pTeam->m_LpPlayersNotAssigned.end() ){
		if(iCount >= m_iMembers)
			break;

		addMember(g_pGame->isPlayer(*iter));

		// tell the bot about what squad it is in
		int iBotIndex = g_pGame->isBot(*iter);
		if(iBotIndex != -1){
			g_pGame->getBot(iBotIndex)->assignSquad(m_iSquadID);
		}

		iCount ++;

		/*char szText[80];
		sprintf(szText,"%s assigned to Squad #%i(%i,%u,%u) of team #%i",(*iter)->getName(),m_iSquadID,m_iMembers,m_LMembers.size(),m_pTeam->m_LpPlayersNotAssigned.size(),m_pTeam->m_iTeamID);
		FakeClientCommand((*iter)->getEntity(),"say",szText,0);
		FILE *fhd=fopen("log-sq.txt","a");fprintf(fhd,"%s\n",szText);fclose(fhd);*/

		iter = m_pTeam->m_LpPlayersNotAssigned.erase(iter); // erase it from that list
	}
}

bool CBaseSquad::addMember(int iPlayer){
	m_LMembers.push_back(CBaseSquadMember(iPlayer));

	//cout << "joining squad as " <<iPlayer << endl;

	return true;
}

bool CBaseSquad::removeMember(int iPlayer){
	// remove player with index iPlayer
	vector<CBaseSquadMember>::iterator iter;
	CBaseBot *pBot;

	for(iter = m_LMembers.begin(); iter != m_LMembers.end(); iter ++){
		if(iter->m_iPlayerIndex == iPlayer){
			if(pBot = g_pGame->getPlayer(iter->m_iPlayerIndex)->isBot())
				pBot->assignSquad(-1);		// tell the bot that he isnt in any squad anymore
			iter = m_LMembers.erase(iter);		// remove this player from squad list
			return true;
		}
	}
	return false;
}

int CBaseSquad::isMember(int iPlayer){
	// check if a player with this index is a member of this squad
	unsigned int ui;

	for(ui=0; ui < m_LMembers.size(); ui++){
		if(m_LMembers[ui].m_iPlayerIndex == iPlayer)
			return (int)ui;
	}
	return -1;
}

void CBaseSquad::tellIncomingMessage(CBaseBot *pBot,int iPlayerIndex, int iType, int iOtherPlayerIndex, int iNWP){
	CPerceipt *pPerc = m_pPerception->addPerceipt();

	// Example : RC_ENGAGING_ENEMY - m_iAddInfo Index of enemy, m_lTypeSpec near waypoint to enemy
	pPerc->m_pPlayer = pBot;
	pPerc->m_lType = iType;
	pPerc->m_lTypeSpec = iNWP;
	pPerc->m_iAddInfo = iPlayerIndex;
	pPerc->m_iAddInfo1 = iOtherPlayerIndex;
}

int CBaseSquad::countMembersAlive(void){
	// how many members of this squad are still alive ?
	int iCount = 0;
	CPlayer *pPlayer;
	vector<CBaseSquadMember>::iterator iter;

	for(iter = m_LMembers.begin(); iter != m_LMembers.end(); iter ++){
		pPlayer = g_pGame->getPlayer(iter->m_iPlayerIndex);
		if(pPlayer){
			if(pPlayer->isAlive())
				iCount ++;
		}
	}

	return iCount;
}

int CBaseSquad::hasWeapon(long lBit){
	// check if anybody int he squad has such weapon ... parameter is bitfield, no actual index
	CPlayer *pPlayer;
	int iCount = 0;
	vector<CBaseSquadMember>::iterator iter;

	for(iter = m_LMembers.begin(); iter != m_LMembers.end(); iter ++){
		pPlayer = g_pGame->getPlayer(iter->m_iPlayerIndex);
		if(!pPlayer)
			continue;
		if(!pPlayer->isBot())
			continue;
		if(!pPlayer->isAlive())
			continue;

		if(pPlayer->isBot()->hasWeaponB(lBit)){
			iCount ++;
		}
	}
	return iCount;
}

CPlayer *CBaseSquad::getMemberAlive(void){
	// returns a member which is still alive
	CPlayer *pPlayer;
	vector<CBaseSquadMember>::iterator iter;

	for(iter = m_LMembers.begin(); iter != m_LMembers.end(); iter ++){
		pPlayer = g_pGame->getPlayer(iter->m_iPlayerIndex);
		if(!pPlayer)
			continue;
		if(!pPlayer->isAlive())
			continue;
		return pPlayer;
	}
	return 0;
}

void CBaseSquad::calcMidPos(void){
	unsigned int iSize=0,ui;
	Vector m_pVPos[10];
	CPlayer *pPlayer;
	vector<CBaseSquadMember>::iterator iter;

	m_VMiddle = Vector(0,0,0);

	for(iter = m_LMembers.begin(); iter != m_LMembers.end(); iter ++){
		pPlayer = g_pGame->getPlayer(iter->m_iPlayerIndex);
		if(!pPlayer)
			continue;
		if(!pPlayer->isAlive())
			continue;

		m_VMiddle += pPlayer->getOrigin();
		m_pVPos[iSize] = pPlayer->getOrigin();

		iSize ++;
		if(iSize == 10)		// I guess we'd never squads of that size
			break;
	}
	if(iSize!=0){
		m_VMiddle /= float(iSize);
		m_fSigmaDist = 0;

		for(ui=0; ui<iSize;ui++){
			m_fSigmaDist += (m_pVPos[ui] - m_VMiddle).squareLength();
		}

		m_fSigmaDist /= float(iSize);
		m_fSigmaDist = sqrt(m_fSigmaDist);
	}
}

void CBaseSquad::thinkPaths(void){
	onPathCreation(0);
}

void CBaseSquad::onPathCreation(AStarBase *p){
	bool m_bSent;
	vector<CBaseSquadMember>::iterator iter,iter2;
	CBaseBot *pBot;
	CPerceipt *pPerc;
	int iOredPaths = 0;

	if(p)
		m_bASMinWork = false;		// so no AStarMachine in work any more
	else
		if(m_bASMinWork)			// if there is one in the task manager, do nothing, just wait
			return;

	// if we got an AStarMachine, we should give it to the respective bot via
	// the command perceipt system.
	if(p){
		m_bSent = false;
		for(iter = m_LMembers.begin(); iter != m_LMembers.end(); iter ++){
			if(iter->m_pAStarMachine == p){
				pBot = g_pGame->getBot(iter->m_iPlayerIndex);
				if(pBot){
					// store path locally
					iter->m_pAStarMachine->getResult(iter->m_WSquadWay);

					pPerc = pBot->m_pPerception->addCommand(CPerceipt::PTX_COMMAND_SQUAD|
						CPerceipt::PTX_COMMAND_GOTODEF|iter->m_lCommandFlag);
					pPerc->m_pPointer = p;
					pPerc->m_pPointer1 = iter->m_pOnReachedDestination;

					m_bSent = true;		// set flag so we know that the message has been sent
					//delete iter->m_pAStarMachine;
					iter->m_pAStarMachine = 0;		// no machine in work now any more
					iter->m_pOnReachedDestination = 0;	// no callback function pointer needed anymore

					// set old wp to current goalWP ... we are going there, NOW !
					iter->m_iOldGoalWP = iter->m_iGoalWP;
					iter->m_fCommandSent = g_pGame->getTime();
					break;
				}
			}
		}
		if(!m_bSent){
			// AStarMachine not send further to bot. in this case, delete the AStarMachine
			delete p;
			p=0;
		}
	}

	// in all cases, so no new AStarMachine back, or new call without preceding AStarMachine
	// tasks finished, we gotta check if we have new paths to find

	// loop thru all members
	for(iter = m_LMembers.begin(); iter != m_LMembers.end(); iter ++){
		if(iter->m_iGoalWP == -1)	// no goal here ? skip the shit !
			continue;

		pBot = g_pGame->getBot(iter->m_iPlayerIndex);
		if(!pBot)
			continue;
		if(!pBot->isAlive())
			continue;
		if(pBot){

			if((iter->m_iOldGoalWP != iter->m_iGoalWP
				|| !pBot->m_pMovePath->hasWay())
				&& fabs(iter->m_fCommandSent - g_pGame->getTime()) > .001f){	// the command hasnt been sent this time
					CBitField BFWays(g_Map.m_Waypoints.m_iNumWaypoints),
						BFTemp(g_Map.m_Waypoints.m_iNumWaypoints);	// bitfield storing other paths
					BFWays.zero();

					// goal has changed or the bot has no way
					if(!iter->m_pAStarMachine){
						iter->m_pAStarMachine = pBot->getAStarMachine();
					}

					iter->m_pAStarMachine->setStart(g_Map.m_Waypoints.getNearest(pBot->getOrigin()));
					iter->m_pAStarMachine->setDestination(iter->m_iGoalWP);

					// loop thru all members and mark the respective bits in the bitfield
					iOredPaths = 0;
					for(iter2 = m_LMembers.begin(); iter2 != m_LMembers.end(); iter2 ++){
						if(iter == iter2 ||				// same bot ?		skip the shit !
							iter2->m_iOldGoalWP != iter2->m_iGoalWP ||	// wanna have this bot elsewhere ? dont take old paths
							iter2->m_iGoalWP == -1){	// no goal here ?	skip the shit !
								continue;
							}

							if(iter2->m_WSquadWay.size()){	// does this bot even have a path ?
								iter2->m_WSquadWay.setMask(&g_Map.m_Waypoints,&BFTemp);
								BFWays |= BFTemp;	// or all paths

								iOredPaths ++;
							}
					}

					if(pBot){
						/*char szTemp[80];
						sprintf(szTemp,"SID: %i OP%i BS %li",m_iSquadID,iOredPaths,BFWays.countSet());
						pBot->fakeClientCommand("say",szTemp);*/
						//g_pGame->debugMsg(szTemp);
					}

					AStarGoalMod *pGoal = (AStarGoalMod*) (iter->m_pAStarMachine->getGoal());

					// mark waypoints where other squadmates are already about to run along
					pGoal->addCost(BFWays,m_fPenaltyforSameWay);

					// add this AStarMachine to the AStarTaskManager
					g_AStarTasks.addTask(iter->m_pAStarMachine,5,0,this);
					m_bASMinWork = true;		// set this flag so we know a path is being searched

					return;	// and return ... just one AStarMachine at a time
				}
		}
	}
}
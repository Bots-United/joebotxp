// BaseTeamData.cpp: implementation of the CBaseTeam class.
//
//////////////////////////////////////////////////////////////////////

#include "BaseTeam.h"

#include "Game.h"
#include "BaseBot.h"
#include "Player.h"

#include <algorithm>
#include <iostream>
using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void FakeClientCommand(edict_t *pBot, char *arg1, char *arg2, char *arg3);

CBaseTeam::CBaseTeam(){
	reset();
	m_iTeamID = -1;

	m_fThinkPer = 1.f;			// think periods of normal 'think'
	m_fBThinkPer = 10.f;		// think periods of behavioral 'think'
}

CBaseTeam::~CBaseTeam(){
	for_each(m_LpSquads.begin(),m_LpSquads.end(),Destroy<CBaseSquad *>());
}

void CBaseTeam::reset(void){
	int i;

	for(i=0; i < _MAX_WAYPOINTS; i++){
		m_pfSeen[i] = g_pGame->getTime();
	}
	memset(m_ppfLastMessageSent,0,sizeof(float)*32*RC_MAX);
	memset(m_pfRadioUsed,0,sizeof(float)*32);

	for(i=0; i < RC_MAX; i++){
		m_pfTime4Message[i] = 8;		// default minimum time between two radio messages of the same type is 8secs
	}

	m_fNextThink = 0;
}

CBaseSquad* CBaseTeam::getSquad(int iSquad){
	if(iSquad < 0 || iSquad >= (int)m_LpSquads.size())
		return 0;
	else
		return m_LpSquads[iSquad];
}

void CBaseTeam::onRoundStart(void){
	m_fNextThink = 0;

	memset(m_ppfLastMessageSent,0,sizeof(float)*32*RC_MAX);
	memset(m_pfRadioUsed,0,sizeof(float)*32);

	unsigned int ui;

	// reset "seen waypoint"-table
	for(ui=0; ui < _MAX_WAYPOINTS; ui++){
		m_pfSeen[ui] = g_pGame->getTime();
	}

	// delete squads
	for_each(m_LpSquads.begin(),m_LpSquads.end(),Destroy<CBaseSquad *>());
	m_LpSquads.clear();

	// recreate not assigned list
	m_LpPlayersNotAssigned.clear();

	int i;
	for(i=0; i < g_pGame->getMaxCurrentPlayer(); i++){
		if(g_pGame->getPlayer(i)){
			if(g_pGame->getPlayer(i)->getTeam() == m_iTeamID)
				m_LpPlayersNotAssigned.push_back(g_pGame->getPlayer(i));
		}
	}
	/*char szText[80];
	sprintf(szText,"unassigned : %u(%i)",m_LpPlayersNotAssigned.size(),m_iTeamID);
	FakeClientCommand(listenserver_edict,"say",szText,0);
	FILE *fhd=fopen("log-sq.txt","a");fprintf(fhd,"%s\n",szText);fclose(fhd);*/
}

CBaseSquad *CBaseTeam::allocSquad(void){
	return new CBaseSquad;
}

void CBaseTeam::createSquads(void){
	/*************************************/
	// create squad instances, at least one
	/*************************************/

	int iPlayersInThisTeam = (int)m_LpPlayersNotAssigned.size();
	int iSquadSize =	iPlayersInThisTeam <= 4 ? 2 : 
						iPlayersInThisTeam < 8 ? 3 : 4;
	int iSquads = iPlayersInThisTeam / iSquadSize;
	int i,iAssigned = iPlayersInThisTeam;
	CBaseSquad *pNewSquad;

	if(!iSquads)			// we at least need one squad
		iSquads++;

	for(i = 0; i < iSquads; i++){
		m_LpSquads.push_back(pNewSquad = allocSquad());

		pNewSquad->m_iSquadID = i;
		pNewSquad->m_pTeam = this;

		// set the member count of each squad
		if(i == iSquads-1){							// last squad to create here
			pNewSquad->m_iMembers = iAssigned;		// take the rest there
		}
		else{
			pNewSquad->m_iMembers = iSquadSize;		// all squads of a fixed size
			iAssigned -= iSquadSize;
		}
	}
}

void CBaseTeam::think(void){
	unsigned int ui;

	if(m_fNextThink > g_pGame->getTime()){
		return;
	}

	m_fNextThink = g_pGame->getTime() + m_fThinkPer;

	if(!m_LpSquads.size()){
		// if we havnt any squads yet
		createSquads();
	}
	else{
		// call think for all squads
		for(ui = 0; ui < m_LpSquads.size(); ui++){
			m_LpSquads[ui]->think();
		}
	}

	if(m_fNextBThink > g_pGame->getTime()){
		return;
	}

	m_fNextBThink = g_pGame->getTime() + m_fBThinkPer;
}

int CBaseTeam::countMembersAlive(void){
	// how many members of this team are still alive ?
	unsigned int ui;
	int iCount = 0;

	for(ui = 0; ui < m_LpSquads.size(); ui++){
		iCount += m_LpSquads[ui]->countMembersAlive();
	}

	return iCount;
}

bool CBaseTeam::removeMember(int iPlayer){
	unsigned int ui;
	for(ui = 0; ui < m_LpSquads.size(); ui++){
		if(m_LpSquads[ui]->removeMember(iPlayer))
			return true;
	}
	return false;
}

bool CBaseTeam::addMe2Squad(int iPlayer){
	//cout << "add me to squad " << iPlayer << endl;
	// to make it easy, just take the first squad we have

	removeMember(iPlayer);		// because client messages are often send more than once :)
	if(!m_LpSquads.size()){
		// if we havnt any squads yet
		createSquads();
	}

	return m_LpSquads[0]->addMember(iPlayer);
}

bool CBaseTeam::addMe2BiggestSquad(int iPlayer){
	// search for biggest squad

	unsigned int ui,uiBiggest=-1,iBMembers=0,uitemp;

	for(ui=0; ui < m_LpSquads.size(); ui++){
		uitemp = m_LpSquads[ui]->countMembersAlive();
		if(uitemp > iBMembers){
			iBMembers = uitemp;
			uiBiggest = ui;
		}
	}
	removeMember(iPlayer);		// because client messages are often send more than once :)

	if(!m_LpSquads.size()){
		// if we havnt any squads yet
		createSquads();
		uiBiggest = 0;
	}

	return m_LpSquads[uiBiggest]->addMember(iPlayer);
}

// possible team radio messages

void CBaseTeam::processMessage(CBaseBot *pBot,int iPlayerIndex, int iType, int iOtherPlayerIndex, int iNWP){
	m_ppfLastMessageSent[iPlayerIndex][iType] = g_pGame->getTime();

	// tell squads about this
	unsigned int ui;
	for(ui = 0; ui < m_LpSquads.size(); ui++){
		m_LpSquads[ui]->tellIncomingMessage(pBot,iPlayerIndex,iType,iOtherPlayerIndex,iNWP);
	}
}

void CBaseTeam::tellEnemySpotted(CBaseBot *pBot, int iEnemyPlayerIndex, int iEnemyNWP){
	int iMessageIndex = RC_ENEMY_SPOTTED;

	processMessage(pBot,g_pGame->isBot(pBot),iMessageIndex,iEnemyPlayerIndex,iEnemyNWP);
}

void CBaseTeam::tellEnemyDown(CBaseBot *pBot, int iEnemyPlayerIndex, int iEnemyNWP){
	int iMessageIndex = RC_ENEMY_DOWN;

	processMessage(pBot,g_pGame->isBot(pBot),iMessageIndex,iEnemyPlayerIndex,iEnemyNWP);
}

void CBaseTeam::tellSniperSpotted(CBaseBot *pBot, int iEnemyPlayerIndex, int iEnemyNWP){
	int iMessageIndex = RC_SNIPER_SPOTTED;

	processMessage(pBot,g_pGame->isBot(pBot),iMessageIndex,iEnemyPlayerIndex,iEnemyNWP);
}

void CBaseTeam::tellSectorClear(CBaseBot *pBot){
	int iMessageIndex = RC_SECTOR_CLEAR;

	processMessage(pBot,g_pGame->isBot(pBot),iMessageIndex);
}

void CBaseTeam::tellGoGoGo(CBaseBot *pBot){
	int iMessageIndex = RC_GO_GO_GO;

	processMessage(pBot,g_pGame->isBot(pBot),iMessageIndex);
}

void CBaseTeam::tellCoverMe(CBaseBot *pBot, int iDestination){
	int iMessageIndex = RC_COVER_ME;

	processMessage(pBot,g_pGame->isBot(pBot),iMessageIndex,-1,iDestination);
}

void CBaseTeam::tellInPosition(CBaseBot *pBot, int iPositionWP){
	int iMessageIndex = RC_IN_POSITION;

	processMessage(pBot,g_pGame->isBot(pBot),iMessageIndex,-1,iPositionWP);
}

void CBaseTeam::tellNeedBackup(CBaseBot *pBot, int iPositionWP){
	int iMessageIndex = RC_NEED_BACKUP;

	processMessage(pBot,g_pGame->isBot(pBot),iMessageIndex,-1,iPositionWP);
}

void CBaseTeam::tellFallBack(CBaseBot *pBot){
	int iMessageIndex = RC_FALL_BACK;

	processMessage(pBot,g_pGame->isBot(pBot),iMessageIndex);
}

void CBaseTeam::tellStickTogether(CBaseBot *pBot){
	int iMessageIndex = RC_STICK_TOGETHER;

	processMessage(pBot,g_pGame->isBot(pBot),iMessageIndex);
}

void CBaseTeam::tellGetInPosition(CBaseBot *pBot){
	int iMessageIndex = RC_GET_IN_POSITION;

	processMessage(pBot,g_pGame->isBot(pBot),iMessageIndex);
}

void CBaseTeam::tellAffirmative(CBaseBot *pBot){
	int iMessageIndex = RC_AFFIRMATIVE;

	processMessage(pBot,g_pGame->isBot(pBot),iMessageIndex);
}

void CBaseTeam::tellNegative(CBaseBot *pBot){
	int iMessageIndex = RC_NEGATIVE;

	processMessage(pBot,g_pGame->isBot(pBot),iMessageIndex);
}

void CBaseTeam::tellDied(CBaseBot *pBot){
	int iMessageIndex = RC_DIED;

	processMessage(pBot,g_pGame->isBot(pBot),iMessageIndex);

	// tell squad we died
	if(pBot->m_iSquad != -1){
		assert(getSquad(pBot->m_iSquad));
		getSquad(pBot->m_iSquad)->removeMember(g_pGame->isPlayer(pBot));
	}
}

void CBaseTeam::tellRespawned(CBaseBot *pBot){
	int iMessageIndex = RC_RESPAWNED;

	processMessage(pBot,g_pGame->isBot(pBot),iMessageIndex);
}

void CBaseTeam::tellPickupWeapon(CBaseBot *pBot){
	int iMessageIndex = RC_PICKUPWEAPON;

	processMessage(pBot,g_pGame->isBot(pBot),iMessageIndex,-1,g_Map.m_Waypoints.getNearest(pBot->getOrigin()));
}

void CBaseTeam::tellEngagingEnemy(CBaseBot *pBot,int iEnemyPlayerIndex,int iEnemyNWP){
	int iMessageIndex = RC_ENGAGING_ENEMY;

	processMessage(pBot, iEnemyPlayerIndex,iMessageIndex);
}

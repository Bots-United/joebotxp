#include "PerceptionBot.h"

#include "BaseBot.h"
#include "BaseTeam.h"
#include "InterestingEntity.h"

CPerceptionBot::CPerceptionBot(void){
	m_fNextPerceptEntities = 0;
}

CPerceptionBot::~CPerceptionBot(void)
{
}

void CPerceptionBot::run(void){
	process();
	perceptObstacles();
	perceptPlayers();
	perceptEntities();
	merge();
	//merge2LTMem();

	m_fLastRun = g_pGame->getTime();
}

void CPerceptionBot::reset(void){
	m_fNextPerceptEntities = 0;

	CPerception::reset();
}

void CPerceptionBot::perceptPlayers(void){
	PROFILE("CPerceptionBot::perceptPlayers");
	if(!m_pBot)
		return;

	int i,iInFoV;
	CPerceipt *pNewPerceipt;
	CPlayer *pCPlayer;

	for(i=g_pGame->getMaxCurrentPlayer();i>=0;i--){
		pCPlayer = g_pGame->getPlayer(i);

		if(m_pBot == pCPlayer)	// dont check yourself !
			continue;

		if(pCPlayer && pCPlayer->getEntity()){

			if(!pCPlayer->isAlive())
				continue;

			/*if(m_pBot->getEntity() == pCPlayer->getEntity())	// dont check yourself !
				continue;*/

			if(!pCPlayer->isInFOVof(m_pBot)||
				!pCPlayer->isInLOSof(m_pBot)){
				perceptSoundOf(pCPlayer);
				continue;
			}

			//pNewPerceipt = addPerceipt();
			pNewPerceipt = &(m_pPlayerPercs[i]);
			if(pNewPerceipt->m_fLUpdate < m_fLastRun - .1f){
				// this means that this perception is a new one ...
				pNewPerceipt->m_fCreate = 0;
			}
			else{
				// we already percepted this player last time
				pNewPerceipt->m_fCreate = pNewPerceipt->m_fLUpdate;
			}

			pNewPerceipt -> m_fLUpdate = g_pGame->getTime();

			pNewPerceipt->m_pPlayer = pCPlayer;
			pNewPerceipt->m_pEntity = pCPlayer->getEntity();
			pNewPerceipt->m_VOrigin = pCPlayer->getOrigin();
			pNewPerceipt->m_fDistance = (pNewPerceipt->m_VOrigin - m_pBot->getOrigin()).length();
			pNewPerceipt->m_iAddInfo = i;		// store index here

			iInFoV = m_pBot->isInFOVof(pCPlayer);
			if(iInFoV){
				pNewPerceipt->m_lTypeSpec |= CPerceipt::PTX_LOOKATME;
			}
			/*if(m_pBot->isAttacking()){
				int iWID = g_pGame->weaponModel2ID(pCPlayer->getWeaponModel());
				if(iWID!=-1)
					pNewPerceipt->m_fValue = float(m_pBot->m_pWeapons->getLoudness(iWID))/255.f;
				else
					pNewPerceipt->m_fValue = 1.f;

				pNewPerceipt->m_lTypeSpec |= CPerceipt::PTX_SHOOT;
				if(iInFoV > 200){
					pNewPerceipt->m_lTypeSpec |= CPerceipt::PTX_SOUND_SHOOTATME;
				}
			}*/

			if(g_pGame->getTeamplay()){
				// teamplay enabled
				if(m_pBot->getTeam() == pCPlayer->getTeam()){
					// same team
					pNewPerceipt->m_lType = CPerceipt::PT_TMATE;
				}
				else{
					// different teams
					pNewPerceipt->m_lType = CPerceipt::PT_ENEMY;

					CBaseTeam *pTData = g_pGame->getTeamData(m_pBot->m_iTeam);
					if(pTData){
						pTData->tellEnemySpotted(m_pBot,pNewPerceipt->m_iAddInfo,pNewPerceipt->m_pPlayer->m_iNearestWP);
					}
				}
			}
			else{
				// teamplay disabled
				pNewPerceipt->m_lType = CPerceipt::PT_ENEMY;
			}
		}
	}
	mergePlayerPercs();
}

void CPerceptionBot::mergePlayerPercs(void){
	int i;
	CPerceipt *pNewPerceipt;

	for(i=g_pGame->getMaxCurrentPlayer();i>=0;i--){
		if(m_pPlayerPercs[i].m_fLUpdate > m_fLastRun){
			// so we have a current perception of a player here
			if((m_fReactionTime == .0f
				|| (g_pGame->getTime() - m_pPlayerPercs[i].m_fCreate > m_fReactionTime
					&& m_pPlayerPercs[i].m_fCreate != 0.f)
					)
					&& m_pBot->m_fBlinded < g_pGame->getTime()){

					pNewPerceipt = addPerceipt();

					// if the create entry is zero, then we can set it to the LUpdate entry
					if(m_pPlayerPercs[i].m_fCreate = 0.f){
						m_pPlayerPercs[i].m_fCreate = m_pPlayerPercs[i].m_fLUpdate;
					}
					*pNewPerceipt = m_pPlayerPercs[i];
				}
		}
	}
}

void CPerceptionBot::perceptSoundOf(CPlayer *pPlayer){
	PROFILE("CPerceptionBot::perceptSoundOf");
	bool bSameTeam = false;

	if(g_pGame->getTeamplay()){
		// teamplay enabled
		if(m_pBot->getTeam() == pPlayer->getTeam()){
			// same team
			bSameTeam = true;
		}
		else{
			// different teams
		}
	}
	else{
		// teamplay disabled
	}

	perceptWalkingOf(pPlayer,bSameTeam);
	perceptShootingOf(pPlayer,bSameTeam);
}

void CPerceptionBot::perceptWalkingOf(CPlayer *pPlayer,bool bSameTeam){
	PROFILE("CPerceptionBot::perceptWalkingOf");
	if(pPlayer->getVelocity().length() > 200.f || bSameTeam){
		CPerceipt *pNewPerceipt = addPerceipt();

		pNewPerceipt->m_lType = CPerceipt::PT_SOUND;
		pNewPerceipt->m_lTypeSpec |= (CPerceipt::PTX_SOUND_LOC|CPerceipt::PTX_SOUND_WALK);
		if(bSameTeam){
			pNewPerceipt->m_lTypeSpec |= CPerceipt::PTX_SOUND_TMATE;
		}
		else{
			pNewPerceipt->m_lTypeSpec |= CPerceipt::PTX_SOUND_ENEMY;
		}
		pNewPerceipt->m_fValue = 140;
		pNewPerceipt->m_pPlayer = pPlayer;
		pNewPerceipt->m_pEntity = pPlayer->getEntity();
		pNewPerceipt->m_VOrigin = pPlayer->getOrigin();
		pNewPerceipt->m_fDistance = (pNewPerceipt->m_VOrigin - m_pBot->getOrigin()).length();
	}
}

void CPerceptionBot::perceptShootingOf(CPlayer *pPlayer,bool bSameTeam){
	// this is now done by hooking currentWeapon in bot_client.cpp
	/*if(pPlayer->isAttacking()
		|| pPlayer->isAttacking2() ){
		CPerceipt *pNewPerceipt = addPerceipt();

		pNewPerceipt->m_lType = CPerceipt::PT_SOUND;
		pNewPerceipt->m_lTypeSpec |= (CPerceipt::PTX_SOUND_LOC|CPerceipt::PTX_SOUND_SHOOT);
		if(bSameTeam){
			pNewPerceipt->m_lTypeSpec |= CPerceipt::PTX_SOUND_TMATE;
		}
		else{
			pNewPerceipt->m_lTypeSpec |= CPerceipt::PTX_SOUND_ENEMY;
		}
		pNewPerceipt->m_pBot = pPlayer;
		pNewPerceipt->m_pEntity = pPlayer->getEntity();
		pNewPerceipt->m_VOrigin = pPlayer->getOrigin();
		pNewPerceipt->m_fDistance = (pNewPerceipt->m_VOrigin - m_pBot->getOrigin()).Length();
	}*/
}

void CPerceptionBot::perceptEntities(void){
	PROFILE("CPerceptionBot::perceptEntities");
	if(!m_pBot)
		return;

	if(m_fNextPerceptEntities > g_pGame->getTime() ){
		return;
	}
	m_fNextPerceptEntities = g_pGame->getTime() + .18f;

	CEntity Entity = 0;
	list<CIEntity*>::iterator iter_entities,
		begin_entities = g_pGame->m_LIEntities.begin(),
		end_entities = g_pGame->m_LIEntities.end();

	CPerceipt *pAllocPerc = /*new CPerceipt*/0;			// an instance of a perceipt
	Vector VOrigin = m_pBot->getOrigin();

	while(Entity = UTIL_FindEntityInSphere(Entity,VOrigin,_ENT_PERC_RADIUS)){	// loop thru entities around here
		iter_entities = begin_entities;

		while(iter_entities != end_entities){
			// check this entity
			
			if( (*iter_entities)->compare(Entity) ){
				if(m_pBot->views((*iter_entities)->getOrigin(Entity))){
					pAllocPerc = new CPerceipt;			// create a new perceipt
					(*iter_entities)->assign(Entity,pAllocPerc);

					pAllocPerc->m_fDistance = (VOrigin-pAllocPerc->m_VOrigin).length();	// todo : more elegant

					m_LNewPerceptions.push_back(pAllocPerc);
					break;		// there arent double classname IEntities !? am I right ?
				}
			}
			iter_entities ++;
		}
	}
}

//#include <iostream>

void CPerceptionBot::perceptObstacles(void){
	PROFILE("CPerceptionBot::perceptObstacles");
	//UTIL_MakeVectors(m_pBot->getAngles());
	Vector VDir = m_pBot->getVelocity().normalize();
	Vector VStart, VEnd;

	if(m_pBot->isDucking()){
		// check if there's the need to jump + check origin
		VStart = m_pBot->getOrigin();
		VEnd = VStart + VDir * 50;

		g_pGame->traceLine(VStart,VEnd,0,&m_pTObstacle[POD_MJump]);
		//if(listenserver_edict)UTIL_DrawBeam(listenserver_edict,VStart,VEnd,3,0,(m_pTObstacle[POD_MJump].flFraction==1.0)*255,255,255,255,0,10);

		m_pbObstacle[POD_MJump]
			= m_pbObstacle[POD_Origin]
			= (m_pTObstacle[POD_MJump].flFraction != 1.f);
	}
	else{
		// check if there's the need to jump
		VStart = m_pBot->getOrigin();
		VStart.z -= 20;
		VEnd = VStart + VDir * 50;

		g_pGame->traceLine(VStart,VEnd,0,&m_pTObstacle[POD_MJump]);
		//if(listenserver_edict)UTIL_DrawBeam(listenserver_edict,VStart,VEnd,3,0,(m_pTObstacle[POD_MJump].flFraction==1.0)*255,255,255,255,0,10);

		m_pbObstacle[POD_MJump] = m_pTObstacle[POD_MJump].flFraction != 1.f;

		// check origin trace
		VStart = m_pBot->getOrigin();
		VEnd = VStart + VDir * 50;

		g_pGame->traceLine(VStart,VEnd,0,&m_pTObstacle[POD_Origin]);
		//if(listenserver_edict)UTIL_DrawBeam(listenserver_edict,VStart,VEnd,3,0,255,255,255,255,0,1);

		m_pbObstacle[POD_Origin] = m_pTObstacle[POD_Origin].flFraction != 1.f;
	}

	// check eyelevel trace
	VStart = m_pBot->getOrigin() + m_pBot->getVOffset();
	VEnd = VStart + VDir * 50;
	
	g_pGame->traceLine(VStart,VEnd,0,&m_pTObstacle[POD_Eye]);
	//if(listenserver_edict)UTIL_DrawBeam(listenserver_edict,VStart,VEnd,3,0,255,255,255,255,0,1);
	
	m_pbObstacle[POD_Eye] = m_pTObstacle[POD_Eye].flFraction != 1.f;

	/*g_pGame->traceLine(m_pBot->getOrigin(),m_pBot->getOrigin()-Vector(0,0,1000),ignore_monsters,0,&tr);
	cout << tr.flFraction*1000 << endl;*/
}

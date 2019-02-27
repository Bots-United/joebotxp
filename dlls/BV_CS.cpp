#include "BV_CS.h"

#include "BV_HLDM.h"

#include "Game.h"
#include "GameCS.h"
#include "Player.h"
#include "AStarMachine.h"
#include "AStarBaseGoal.h"
#include "Map.h"
#include "BotCS.h"
//#include "WeaponDefs.h"
#include "Weapons.h"
#include "Perceipt.h"
#include "Perception.h"
#include "InterestingEntity.h"
#include "BaseTeam.h"

void BotBuy_PAmmo(CBaseBot *pBot);
void BotBuy_SAmmo(CBaseBot *pBot);

// check if there are any cs specific squad or team commands
void CBV_CS_SquadCommand::execute(CGoal *pGoal){
	if(!pGoal)
		return;
	if(m_iExecuteFSMS)
		return;

	if(!pGoal->m_pPerceipt)
		return;

	// let call the std stuff
	CBV_HLDM_SquadCommand::execute(pGoal);
}

// buy weapons

void CBVNP_CS_Buy::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if(!((CBotCS*)(m_pBot))->m_bInBuyZone)
		return;

	if(m_fLastExecution+1 > g_pGame->getTime() )
		return;

	if(((CBotCS*)(m_pBot))->m_iMoney < 600)
		return;
	
	// just call execute here, this behaviour isnt interfering with any other goals
	execute(0);
	
	return;
}

void CBVNP_CS_Buy::execute(CGoal *pGoal){
	m_fLastExecution = g_pGame->getTime();
	
	int i;
	int ibot_money = ((CBotCS*)(m_pBot))->m_iMoney-200;
	int iWeaponDecision=-1;
	float fChoice;
	float fBuyLProb[32];
	
	if(ibot_money > 3500)
		ibot_money -= 1000;
	
	if(!m_pBot->m_pWeapons->hasPrimary()){
	
	//////////////////////////////////
	float fTemp[32];
	for(i=0; i < 32; i++){
		fTemp[i] = m_pBot->m_pWeapons->getBProp(i);
	}
	float fSum=0,fLast=0;
	long lschl;
	for(lschl=0;lschl<32;lschl++){
		if(m_pBot->m_pWeapons->getCost(lschl) > ibot_money){	// no prob for too expenive weapons
			fTemp[lschl]=0;
		}
		if(m_pBot->m_pWeapons->getTeam(lschl) != -1){
			if(m_pBot->m_pWeapons->getTeam(lschl) != m_pBot->m_iTeam){
				fTemp[lschl]=0;
			}
		}
	}
	for(lschl=0;lschl<32;lschl++)
		fSum+=fTemp[lschl];
	
	for(lschl=0;lschl<32;lschl++){
		fBuyLProb[lschl] = fLast + fTemp[lschl];
		fLast = fBuyLProb[lschl];
		if(fTemp[lschl] == .0)
			fBuyLProb[lschl] = 0;
	}
	//////////////////////////////////
	
	fChoice = RANDOM_FLOAT(0.0,fSum);
	for(i=0;i<32;i++){
		if(fChoice < fBuyLProb[i]){
			iWeaponDecision = i;
			break;
		}
	}
	if(iWeaponDecision!=-1){
#ifdef __LOG
		FILE *fhdf;fhdf=fopen("buy.txt","a");fprintf(fhdf,"%s",weapon_defs[iWeaponDecision].szClassname);fclose(fhdf);
#endif
		
		if(m_pBot->m_pWeapons->getCost(iWeaponDecision) > ibot_money){
#ifdef __LOG
			FILE *fhd;fhd=fopen("buy.txt","a");fprintf(fhd," is too expensive - %i\n",ibot_money);fclose(fhd);
#endif
			iWeaponDecision = -1;
		}
		else if(m_pBot->m_pWeapons->getTeam(iWeaponDecision) != -1
			&&m_pBot->m_pWeapons->getTeam(iWeaponDecision) != m_pBot->m_iTeam){
			iWeaponDecision = -1;
#ifdef __LOG
			FILE *fhd;fhd=fopen("buy.txt","a");fprintf(fhd," is wrong team \n");fclose(fhd);
#endif
		}
		else if(iWeaponDecision !=-1 ){
#ifdef __LOG
			FILE *fhd;fhd=fopen("buy.txt","a");fprintf(fhd," is ok\n");fclose(fhd);
#endif
		}
		
		//FakeClientCommand(pEdict,"menuselect","0",0);	//quit fuckin menues
		if(m_pBot->m_pWeapons->m_pWeapons[iWeaponDecision].m_pFBuy)
			m_pBot->m_pWeapons->m_pWeapons[iWeaponDecision].m_pFBuy(m_pBot);
	}
	}
	
	BotBuy_PAmmo(m_pBot);

	m_pBot->m_pWeapons->m_pWeapons[CS_WEAPON_HEGRENADE].m_pFBuy(m_pBot);
	//m_pBot->m_pWeapons->m_pWeapons[CS_WEAPON_FLASHBANG].m_pFBuy(m_pBot);
	
	/*if(ibot_money > 900 &&
		m_pBot->m_pWeapons->hasPrimary()){
		if(RANDOM_LONG(0,100) > 70){
			m_pBot->m_pWeapons->m_pWeapons[CS_WEAPON_HEGRENADE].m_pFBuy(m_pBot);
		}
		if(RANDOM_LONG(0,100) > 70){
			m_pBot->m_pWeapons->m_pWeapons[CS_WEAPON_FLASHBANG].m_pFBuy(m_pBot);
		}
		if(RANDOM_LONG(0,100) > 70){
			m_pBot->m_pWeapons->m_pWeapons[CS_WEAPON_SMOKEGRENADE].m_pFBuy(m_pBot);
		}
	}*/
	
	if((m_pBot->m_pWeapons->hasSniper() == CS_WEAPON_AWP
		||m_pBot->m_pWeapons->hasSniper() == CS_WEAPON_SCOUT
		||m_pBot->m_pWeapons->hasSniper() == CS_WEAPON_G3SG1
		||m_pBot->m_pWeapons->hasSniper() == CS_WEAPON_SG550)
		&& ibot_money > 5000
		&& m_pBot->m_pWeapons->hasSecondary() != CS_WEAPON_DEAGLE
		&& m_pBot->m_pWeapons->hasSecondary() != CS_WEAPON_ELITE
		&& m_pBot->m_pWeapons->hasSecondary() != CS_WEAPON_FIVESEVEN
		&& m_pBot->m_pWeapons->hasSecondary() != CS_WEAPON_P228){
		if(m_pBot->m_iTeam== CT){
			if(RANDOM_LONG(0,100) < 33){
				m_pBot->m_pWeapons->m_pWeapons[CS_WEAPON_DEAGLE].m_pFBuy(m_pBot);
			}
			if(RANDOM_LONG(0,100) < 50){
				m_pBot->m_pWeapons->m_pWeapons[CS_WEAPON_FIVESEVEN].m_pFBuy(m_pBot);
			}
			else{
				m_pBot->m_pWeapons->m_pWeapons[CS_WEAPON_P228].m_pFBuy(m_pBot);
			}
			BotBuy_SAmmo(m_pBot);
		}
		else{
			if(RANDOM_LONG(0,100) < 33){
				m_pBot->m_pWeapons->m_pWeapons[CS_WEAPON_DEAGLE].m_pFBuy(m_pBot);
			}
			if(RANDOM_LONG(0,100) < 50){
				m_pBot->m_pWeapons->m_pWeapons[CS_WEAPON_ELITE].m_pFBuy(m_pBot);
			}
			else{
				m_pBot->m_pWeapons->m_pWeapons[CS_WEAPON_P228].m_pFBuy(m_pBot);
			}
			BotBuy_SAmmo(m_pBot);
		}
	}
	/*if(m_pBot->m_iTeam == CT				// def kit
		&&RANDOM_LONG(0,100) < 30){
		
		FakeClientCommand(m_pBot->getEntity(),"buyequip",0,0);
		FakeClientCommand(m_pBot->getEntity(),"menuselect","6",0);
	}*/
	
	if(m_pBot->m_iArmor<50){
		m_pBot->fakeClientCommand("buyequip",0,0);
		m_pBot->fakeClientCommand("menuselect","2",0);// kevlar + helmet
		
													 /*FakeClientCommand(pEdict,"buyequip",0,0);
		FakeClientCommand(pEdict,"menuselect","1",0);// kevlar without helmet - if not enough money*/
	}
	return;
}


// guard c4 weaponbox

void CBV_CS_GuardWBBomb::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if(m_iExecuteFSMS)
		return;
	if(p->m_lType != CPerceipt::PT_ENTITY)				// it isnt an entity
		return;
	if(!(p->m_lTypeSpec & CPerceipt::PTX_ENTITY_WEAPONWBX))	// it isnt a InterestingWeaponboxWeapon IEntity
		return;
	if(m_pBot->m_iTeam != CT)
		return;
	if(m_pBot->m_pEMTeam->m_iVisCount>0)
		return;

	int iWeapon = p->m_pIEntity->m_iWeaponId;

	if(iWeapon == CS_WEAPON_C4){
		pGoals->push_front(CGoal(30,CGoal::GT_MOVE,this,p));
	}

	return;
}

void CBV_CS_GuardWBBomb::initialize(CGoal *pGoal){
	CBitField BFBVisible(g_Map.m_Waypoints.m_iNumWaypoints);
	CBitField BFPVisible(g_Map.m_Waypoints.m_iNumWaypoints);
	CBitField BFBNear(g_Map.m_Waypoints.m_iNumWaypoints);
	int iNearBomb = g_Map.m_Waypoints.getNearest(pGoal->m_pPerceipt->m_VOrigin);
	int iDestination;

	g_Map.m_Waypoints.setVisibleMask(&BFBVisible,iNearBomb);
	g_Map.m_Waypoints.setVisibleMask(&BFPVisible,m_pBot->m_iNearestWP);
	g_Map.m_Waypoints.setDistanceMask(&BFBNear,1,450,pGoal->m_pPerceipt->m_VOrigin);

	// search only the waypoint which are visible to the bomb and to the bot
	// if you would just search the best waypoint in the view  of the bomb,
	// you would have sometimes more than one bot near a waypoint, this way
	// they may guard the bomb from more than one side
	BFBVisible &= BFPVisible;
	BFBVisible &= BFBNear;

	// search the waypoint with the least visible waypoints around
	iDestination = g_Map.m_Waypoints.getMaximum(&BFBVisible,CWaypointGraph::WG_SEARCH_AVDISTVWP,-1);
	
	if(iDestination != -1){
		if(iDestination != m_pBot->m_iNearestWP){
#ifdef DEBUGMESSAGES
			g_pGame->drawBeam(DEBUG_BOT|DEBUG_NAV|DEBUG_ACTION,g_pGame->m_pListenserverEdict.getEntity(),
				g_Map.m_Waypoints[m_pBot->m_iNearestWP].m_VOrigin,
				g_Map.m_Waypoints[iDestination].m_VOrigin,20,0,Vector(255,255,255),255,20);
#endif

			AStarBase *pAStar = m_pBot->getAStarMachine();

			/*if(m_pBot->m_iNearestWP == -1)
				CRASH;*/
			
			pAStar->setStart(m_pBot->m_iNearestWP);
			pAStar->setDestination(iDestination);
			
			g_AStarTasks.addTask(pAStar,5,m_pBot,this);		// add task
			m_iExecuteFSMS++;
		}
		else{
			onReachedDestination(m_pBot);
			m_pBot->m_LGoals.finishedMove();		// cause we are already here, tell them this behaviour is finished
		}
	}
}

void CBV_CS_GuardWBBomb::execute(CGoal *pGoal){
	m_fLastExecution = g_pGame->getTime();
	
	//m_pBot->m_pCamp->campO(2);
	//m_pBot->m_pAction->slowDownO(3,0);
	
	return;
}

void CBV_CS_GuardWBBomb::onPathCreation(AStarBase *p){
	m_iExecuteFSMS ++;
	
	m_pBot->m_pMovePath->getResult(p,this);			// ,this so we get the onreacheddestination call

	delete p;
}

void CBV_CS_GuardWBBomb::onReachedDestination(CPlayer *pPlayer){
	m_pBot->m_pCamp->campO(15);
	m_pBot->m_LGoals.finishedMove();			// directly call this because we havnt necessarily created a path, but allocated a move slot in the goalfinder

	// tell other bots - we wanna be supported here !
	CBaseTeam *pTeamData = g_pGame->getTeamData(m_pBot->getTeam());
	if(pTeamData){
		pTeamData->tellNeedBackup(m_pBot,m_pBot->m_iNearestWP);
	}
}

// pickup weapons

void CBV_CS_PickupWeapon::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if(m_iExecuteFSMS)
		return;
	if(p->m_lType != CPerceipt::PT_ENTITY)				// it isnt an entity
		return;
	if(!(p->m_lTypeSpec & CPerceipt::PTX_ENTITY_WEAPONWBX))	// it isnt a InterestingWeaponboxWeapon IEntity
		return;

	//if(g_pGame->m_pListenserverEdict.getEntity())UTIL_DrawBeam(g_pGame->m_pListenserverEdict.getEntity(),m_pBot->getOrigin(),m_pBot->getOrigin()+Vector(0,0,100),40,10,255,255,255,255,255,4);

	if(fabs(m_fLastExecution - g_pGame->getTime()) < 2.f)
		return;
	if(fabs(p->m_fLUpdate - g_pGame->getTime())>.001f)		// take only current perceptions
		return;
	
	int iPriority = 40;
	int iWeapon = p->m_pIEntity->m_iWeaponId;
	int iHasWeapon = m_pBot->m_pWeapons->hasPrimary();
	int iValue = getWeaponValue(iWeapon);
	int iHasValue = getWeaponValue(iHasWeapon);

	m_bDrop = false;
	if(iWeapon == CS_WEAPON_C4
		&& m_pBot->m_iTeam == TE){		// set priority and dont drop the bomb
		iValue = 20;
	}
	else
		if(m_pBot->m_pWeapons->hasPrimary())	// you need to drop your primary gun before picking up a new one
			m_bDrop = true;

	iPriority += iValue;

	if(iPriority>60){
		iPriority=60;
	}

	if(iValue > iHasValue){
		// check if we can get the permission to pickup a weapon - this isnt locally restricted, i.e. only one bot from
		// one team can pick up a weapon at a time !
		CBaseTeam *pTeamData = g_pGame->getTeamData(m_pBot->getTeam());
		if(pTeamData &&
			!pTeamData->requestPermission(m_pBot,TP_PICKUPWEAPON,p->m_fDistance / 150.f+5.f))
			return;

		if(m_pBot->m_LGoals.m_pMoveBv != this)
			pGoals->push_front(CGoal(iPriority,CGoal::GT_MOVE,this,p));
		//if(g_pGame->m_pListenserverEdict.getEntity())UTIL_DrawBeam(g_pGame->m_pListenserverEdict.getEntity(),m_pBot->getOrigin(),m_pBot->getOrigin()+Vector(0,0,100),40,10,255,255,255,255,255,30);
	}
	
	return;
}

void CBV_CS_PickupWeapon::execute(CGoal *pGoal){
	m_fLastExecution = g_pGame->getTime();
	if(m_bDrop){
		if(m_pBot->m_pWeapons->getCWeaponID() != m_pBot->m_pWeapons->hasPrimary()){
			m_pBot->change2Weapon(m_pBot->m_pWeapons->hasPrimary());
		}
	}
}

void CBV_CS_PickupWeapon::initialize(CGoal *pGoal){
	// look for a path
	int iWPW,iWPB;		// next waypoint to weapon, to bot
	iWPW = g_Map.m_Waypoints.getNearest(pGoal->m_pPerceipt->m_VOrigin,true,false,0,250);
	if(iWPW != m_pBot->m_pMovePath->m_iTargetWP
		&& iWPW != -1){
		iWPB = m_pBot->m_iNearestWP;
		
		AStarBase *pAStar = m_pBot->getAStarMachine();
		
		pAStar->setStart(iWPB);
		pAStar->setDestination(iWPW);
		
		g_AStarTasks.addTask(pAStar,5,m_pBot,this);		// add task
		m_iExecuteFSMS++;
		
		m_pBot->m_pMovePath->m_VGoal = pGoal->m_pPerceipt->m_VOrigin;
	}
}

void CBV_CS_PickupWeapon::onPathCreation(AStarBase *p){
	m_iExecuteFSMS ++;
	
	m_pBot->m_pMovePath->getResult(p,this);			// ,this so we get the onreacheddestination call
	m_pBot->m_pMovePath->m_bMove2Origin = true;

	//*((long*)0)=0;
	
	delete p;
}

void CBV_CS_PickupWeapon::onReachedDestination(CPlayer *pPlayer){
	m_iExecuteFSMS = 0;
	m_fLastExecution = g_pGame->getTime();
	if(m_bDrop)
		m_pBot->fakeClientCommand("drop","","");
}

int CBV_CS_PickupWeapon::getWeaponValue(int iWeapon){
	int iReturn = 0;
	if(iWeapon < 0 || iWeapon >= 32)
		return iReturn;
	long lWeaponBit = WEAPONB(iWeapon);

	if(m_pBot->m_pWeapons->isMachineGun(lWeaponBit))
		iReturn += 3;

	if(m_pBot->m_pWeapons->isShotgun(lWeaponBit))
		iReturn += 2;

	if(m_pBot->m_pWeapons->isSubM(lWeaponBit))
		iReturn += 2;

	if(m_pBot->m_pWeapons->isRifle(lWeaponBit))
		iReturn += 4;

	if(m_pBot->m_pWeapons->isSniper(lWeaponBit))
		iReturn += 3;

	iReturn += m_pBot->m_pWeapons->m_pWeapons[iWeapon].m_iPreference;

	return iReturn;
}

// watch the teamm8s if one is shooting

void CBV_CS_WatchTMate::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	/*if( (p->m_lType == CPerceipt::PT_TMATE
		&& p->m_lTypeSpec & CPerceipt::PTX_SHOOT) ||
		(p->m_lType == CPerceipt::PT_SOUND
		&& (p->m_lTypeSpec & (CPerceipt::PTX_SOUND_LOC|CPerceipt::PTX_SOUND_SHOOT|CPerceipt::PTX_SOUND_TMATE) ) == (CPerceipt::PTX_SOUND_LOC|CPerceipt::PTX_SOUND_SHOOT|CPerceipt::PTX_SOUND_TMATE) )){
		pGoals->push_front(CGoal(9.f-(p->m_fDistance<1000.f?p->m_fDistance/200.f:5.f),CGoal::GT_LOOK,this,p));
	}*/
	return;
}

void CBV_CS_WatchTMate::execute(CGoal *pGoal){
	/*int iWP,i;

	for(i=0; i < g_Map.m_Waypoints.m_iNumWaypoints; i ++){
		if(pGoal->m_pPerceipt->m_pPlayer->
	}

	m_pBot->m_pAction->lookTo();

	UTIL_DrawBeam(g_pGame->m_pListenserverEdict.getEntity(),m_pBot->getOrigin(),m_pBot->getOrigin()+Vector(0,0,100),10,0,0,255,0,255,20);
	*/
}

// 'use' a hostage

void CBV_CS_UseHostage::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if(m_pBot->m_iTeam != CT)
		return;
	if(p->m_lType != CPerceipt::PT_ENTITY)				// it isnt an entity
		return;
	if(p->m_pIEntity->m_iId != ((CGameCS*)(g_pGame))->m_iIEntityIdHostage)
		return;
	if(p->m_fDistance > 300.f)
		return;
	if(p->m_pEntity.getVelocity().length() > 10.f)
		return;

	list <CEntity>::iterator iter = ((CBotCS *)(m_pBot))->m_LHostages.begin();
	while(iter != ((CBotCS *)(m_pBot))->m_LHostages.end()){
		if((*iter) == p->m_pEntity)
			return;
		iter ++;
	}

	pGoals->push_front(CGoal(40-p->m_fDistance/30,CGoal::GT_LOOK|CGoal::GT_ACTION,this,p));
	
	return;
}

void CBV_CS_UseHostage::execute(CGoal *pGoal){
	m_fLastExecution = g_pGame->getTime();

	float fDistance = pGoal->m_pPerceipt->m_fDistance;

	/*char szT[80];
	sprintf(szT,"%i %f",pGoal->m_pPerceipt->m_pIEntity->m_iWeaponId,fDistance);
	m_pBot->fakeClientCommand("say",szT);*/

	m_pBot->m_pMovePath->m_bMove2Origin = true;
	m_pBot->m_pMovePath->m_VGoal = pGoal->m_pPerceipt->m_VOrigin;
	m_pBot->m_pMovePath->getResult(0);
	m_pBot->m_pAction->lookTo(pGoal->m_pPerceipt->m_VOrigin);

	if(fDistance < 80){
		m_pBot->m_pAction->use();

		((CBotCS *)(m_pBot))->m_LHostages.push_back(pGoal->m_pPerceipt->m_pEntity);
	}
}

// defuse the bomb

void CBV_CS_DefuseBomb::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if(m_pBot->m_iTeam != CT)
		return;
	if(p->m_lType != CPerceipt::PT_ENTITY)				// it isnt an entity
		return;
	if(p->m_pIEntity->m_iId != ((CGameCS*)(g_pGame))->m_iIEntityIdGrenade)
		return;
	if(p->m_fDistance > 60.f)
		return;
	if(strcmp(p->m_pEntity.getModel(),"models/w_c4.mdl"))
		return;

	pGoals->push_front(CGoal(
		60,
		CGoal::GT_LOOK|CGoal::GT_ACTION,
		this,
		p));
	
	return;
}

void CBV_CS_DefuseBomb::execute(CGoal *pGoal){
	m_fLastExecution = g_pGame->getTime();

	m_pBot->m_pAction->lookTo(pGoal->m_pPerceipt->m_VOrigin);

	m_pBot->m_pAction->useO(.5f);
	m_pBot->m_pAction->slowDownO(1,0);
}

// Goto the bomb to defuse it then

void CBV_CS_GotoBomb::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if(m_pBot->m_iTeam != CT)
		return;
	if(p->m_lType != CPerceipt::PT_ENTITY)				// it isnt an entity
		return;
	if(p->m_pIEntity->m_iId != ((CGameCS*)(g_pGame))->m_iIEntityIdGrenade)
		return;
	if(strcmp(p->m_pEntity.getModel(),"models/w_c4.mdl"))
		return;
	
	pGoals->push_front(CGoal(50,CGoal::GT_MOVE,this,p));
	
	return;
}

void CBV_CS_GotoBomb::initialize(CGoal *pGoal){
	// look for a path
	int iWPW,iWPB;		// next waypoint to weapon, to bot
	iWPW = g_Map.m_Waypoints.getNearest(pGoal->m_pPerceipt->m_VOrigin,true,false,0,250);
	if(iWPW != m_pBot->m_pMovePath->m_iTargetWP
		&& iWPW != -1){
		iWPB = m_pBot->m_iNearestWP;
		
		AStarBase *pAStar = m_pBot->getAStarMachine();
		
		pAStar->setStart(iWPB);
		pAStar->setDestination(iWPW);
		
		g_AStarTasks.addTask(pAStar,5,m_pBot,this);		// add task
		m_iExecuteFSMS++;
		
		m_pBot->m_pMovePath->m_VGoal = pGoal->m_pPerceipt->m_VOrigin;
	}
	else{
		m_pBot->m_LGoals.finishedMove();		// cause we are already here, tell them this behaviour is finished
	}
}

void CBV_CS_GotoBomb::onPathCreation(AStarBase *p){
	m_iExecuteFSMS ++;
	
	m_pBot->m_pMovePath->getResult(p,0);
	m_pBot->m_pMovePath->m_bMove2Origin = true;

	delete p;
}

// look at bomb

void CBV_CS_LookAtBomb::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if(m_pBot->m_iTeam != CT)
		return;
	if(p->m_lType != CPerceipt::PT_ENTITY)				// it isnt an entity
		return;
	if(p->m_pIEntity->m_iId != ((CGameCS*)(g_pGame))->m_iIEntityIdGrenade)
		return;
	if(strcmp(p->m_pEntity.getModel(),"models/w_c4.mdl"))
		return;
	
	pGoals->push_front(CGoal(30,CGoal::GT_LOOK,this,p));
	
	return;
}

void CBV_CS_LookAtBomb::execute(CGoal *pGoal){
	m_fLastExecution = g_pGame->getTime();

	m_pBot->m_pAction->lookTo(pGoal->m_pPerceipt->m_VOrigin);
}

// plant a bomb

void CBVNP_CS_PlantBomb::evaluate(list<CGoal> *pGoals,CPerceipt* p){
	if(!((CBotCS *)(m_pBot))->m_bInBombZone)
		return;

	if(!m_pBot->hasWeapon(CS_WEAPON_C4))		// if you don't have the bomb, this isnt interesting/possible at all :)
		return;

	pGoals->push_front(CGoal(60,CGoal::GT_ACTION,this,p));
	
	return;
}

void CBVNP_CS_PlantBomb::execute(CGoal *pGoal){
	m_fLastExecution = g_pGame->getTime();

	if(m_pBot->m_pWeapons->getCWeaponID() != CS_WEAPON_C4){
		m_pBot->change2Weapon(CS_WEAPON_C4);
	}
	else{
		m_pBot->m_pAction->attackO(1);		// press the attack button for just a second
		m_pBot->m_pAction->slowDownO(1,0);	// stay where you are ! ( for a second at least )
	}
}


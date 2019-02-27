// GoalFinder.cpp: implementation of the CGoalFinder class.
//
//////////////////////////////////////////////////////////////////////

#include "GoalFinder.h"
#include "Behaviour.h"
#include "BaseBot.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGoalFinder::CGoalFinder(){
	m_iLMovePriority = -1;
	m_pLMoveBv = 0;

	reset();
}

CGoalFinder::~CGoalFinder(){
}

void CGoalFinder::reset(void){
	m_pActionBv = 0;
	m_pLookBv = 0;
	m_pMoveBv = 0;

	resetWinner();
}

void CGoalFinder::resetWinner(void){
	m_pActionWinner = 0;
	m_pLookWinner = 0;
	m_pMoveWinner = 0;
}

void CGoalFinder::getWinner(void){
	PROFILE("CGoalFinder::getWinner");
	CGoal *pG[3]={0,0,0};
	
	int iMaxScore = 0,iOImportance,i;
	list<CGoal>::iterator iter1,iter2,iter3;

	resetWinner();

	for(iter1 = begin();iter1 != end(); iter1++){	// multiply the priority of a goal by the slots it needs
		iter1->m_iImportance *= slots(iter1->m_iType);
	}
	
	for(iter1 = begin();iter1 != end(); iter1++){
		if(iter1->m_iImportance > iMaxScore){
			pG[0] = &*iter1;
			pG[1] = 0;
			pG[2] = 0;
			iMaxScore = iter1->m_iImportance;
		}
		else
			continue;

		if( ((pG[0]->m_iType) & CGoal::GT_ALL) == CGoal::GT_ALL)
			continue;
		// flag field isnt full
		for(iter2 = begin();iter2 != end(); iter2++){
			if(iter2 == iter1)
				continue;
			if(!combinable(pG[0],&*iter2))
				continue;
			
			if((iOImportance = pG[0]->m_iImportance
				+iter2->m_iImportance) > iMaxScore){
				pG[1] = &*iter2;
				pG[2] = 0;
				iMaxScore = iOImportance;
			}
			else
				continue;

			if(((pG[0]->m_iType|pG[1]->m_iType)
				& CGoal::GT_ALL ) == CGoal::GT_ALL)
				continue;
			
			// flag field isnt full
			for(iter3 = begin();iter3 != end(); iter3++){
				if(iter3 == iter1
					|| iter3 == iter2)
					continue;
				if(!combinable(pG[0],pG[1],&*iter3))
					continue;
				
				if((iOImportance = pG[0]->m_iImportance
					+pG[1]->m_iImportance
					+iter3->m_iImportance) > iMaxScore){
					pG[2] = &*iter3;
					iMaxScore = iOImportance;
				}
			}
		}
	}

	for(i = 0; i < 3; i++){
		if(pG[i]&&pG[i]->m_iType & CGoal::GT_MOVE){
			m_pMoveWinner = pG[i];
			pG[i] = 0;
		}
		else if(pG[i]&&pG[i]->m_iType & CGoal::GT_ACTION){
			m_pActionWinner = pG[i];
			pG[i] = 0;
		}
		else if(pG[i]&&pG[i]->m_iType & CGoal::GT_LOOK){
			m_pLookWinner = pG[i];
			pG[i] = 0;
		}
		else 
			break;	// break if there's nothing to check any more
	}
}

void CGoalFinder :: executeWinner(void){
	PROFILE("CGoalFinder::executeWinner");

	// Action
	if(m_pActionWinner){
		if(m_pActionWinner->m_pBehaviour != m_pActionBv){
			if(m_pActionBv){						// was there a old behaviour ?
				m_pActionBv->deinitialize(m_pActionWinner);
			}
			
			m_pActionBv = m_pActionWinner->m_pBehaviour;		// set new winner
			if(m_pActionBv)						// and call it if it's available
				m_pActionBv->initialize(m_pActionWinner);
		}
	}
	else{
		if(m_pActionBv){						// was there a old behaviour ?
			m_pActionBv->deinitialize(0);
		}
		m_pActionBv = 0;
	}
	if(m_pActionBv)
		m_pActionBv->execute(m_pActionWinner);

	// Look
	if(m_pLookWinner){
		if(m_pLookWinner->m_pBehaviour != m_pLookBv){
			if(m_pLookBv){						// was there a old behaviour ?
				m_pLookBv->deinitialize(m_pLookWinner);
			}
			
			m_pLookBv = m_pLookWinner->m_pBehaviour;		// set new winner
			if(m_pLookBv)						// and call it if it's available
				m_pLookBv->initialize(m_pLookWinner);
		}
	}
	else{
		if(m_pLookBv)						// was there a old behaviour ?
			m_pLookBv->deinitialize(0);
		m_pLookBv = 0;
	}
	if(m_pLookBv)
		m_pLookBv->execute(m_pLookWinner);

	if(m_pMoveWinner){
		if(m_pMoveWinner->m_iImportance > m_iLMovePriority + 5		// if the goal's priority is significantly higher than the last goal's
			|| m_iLMovePriority == -1){								// or there is no significant last goal
			m_iLMovePriority = m_pMoveWinner->m_iImportance;			// cpy the importance

			if(m_pLMoveBv)
				m_pLMoveBv->deinitialize(m_pMoveWinner);

			m_pLMoveBv = m_pMoveWinner->m_pBehaviour;					// cpy a pointer to the related behaviour

			m_pLMoveBv->initialize(m_pMoveWinner);
		}
	}
	
	if(m_pLMoveBv)
		m_pLMoveBv->execute(m_pMoveWinner);						// execute the behaviour
}

void CGoalFinder::finishedMove(void){
	m_iLMovePriority = -1;
	if(m_pLMoveBv)
		m_pLMoveBv->deinitialize(0);
	m_pLMoveBv = 0;
}

bool CGoalFinder::combinable(long l1,long l2){
	return ( ( l1 | l2 ) == ( l1 + l2 ) );
}

bool CGoalFinder::combinable(CGoal *pG1,CGoal *pG2){
	return combinable(pG1->m_iType,pG2->m_iType);
}

bool CGoalFinder::combinable(long l1,long l2,long l3){
	return ( ( l1 | l2 | l3) == ( l1 + l2 + l3 ) );
}

bool CGoalFinder::combinable(CGoal *pG1,CGoal *pG2,CGoal *pG3){
	return combinable(pG1->m_iType,pG2->m_iType,pG3->m_iType);
}

int CGoalFinder::slots(int lP){
	int iReturn = 0;

	if(lP & CGoal::GT_MOVE)
		iReturn ++;
	if(lP & CGoal::GT_ACTION)
		iReturn ++;
	if(lP & CGoal::GT_LOOK)
		iReturn ++;

	return iReturn;
}
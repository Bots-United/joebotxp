// Perception.cpp: implementation of the CPerception class.
//
//////////////////////////////////////////////////////////////////////

#include "Perception.h"

#include "InterestingEntity.h"
#include "BaseBot.h"
#include "Player.h"
#include "BaseTeam.h"

#include <algorithm>
using namespace std;

extern edict_t *listenserver_edict;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPerception::CPerception(){
	m_fTime2DelLT = 30;		// long term deletion time

	m_fReactionTime = .0f;

	reset();
}

CPerception::~CPerception(){
	for_each(m_LPerceptions.begin(),m_LPerceptions.end(),Destroy<CPerceipt*>());
	for_each(m_LNewPerceptions.begin(),m_LNewPerceptions.end(),Destroy<CPerceipt*>());
	for_each(m_LLTMem.begin(),m_LLTMem.end(),Destroy<CPerceipt*>());
	for_each(m_LCommands.begin(),m_LCommands.end(),Destroy<CPerceipt*>());
}

void CPerception::resetPerceptions(void){
	while (!m_LNewPerceptions.empty()){
		delete m_LNewPerceptions.back();
		m_LNewPerceptions.pop_back();
	}

	while (!m_LPerceptions.empty()){
		delete m_LPerceptions.back();
		m_LPerceptions.pop_back();
	}

	while (!m_LLTMem.empty()){
		delete m_LLTMem.back();
		m_LLTMem.pop_back();
	}

	while (!m_LCommands.empty()){
		delete m_LCommands.back();
		m_LCommands.pop_back();
	}
}

void CPerception::reset(void){
	resetPerceptions();
}

void CPerception::merge(void){
	bool bUpdated;
	list<CPerceipt*>::iterator iter_old;
	list<CPerceipt*>::iterator iter_new = m_LNewPerceptions.begin();
	
	while(!m_LNewPerceptions.empty()){
		// while we still have new perceptions to handle
		// loop thru all 'old' perceptions and see if we need to update something
		iter_old = m_LPerceptions.begin();
		bUpdated = false;
		while(iter_old != m_LPerceptions.end()){
			if((*iter_old)->m_pEntity == (*iter_new)->m_pEntity
				&&(*iter_old)->m_lType == (*iter_new)->m_lType
				&&(*iter_old)->m_lTypeSpec == (*iter_new)->m_lTypeSpec){
				// we found an 'old' perception which fits the new one.
				// so let's update that one
				updatePerception(*iter_old,*iter_new);
				bUpdated = true;	// and set a flag accordingly
				// once we updated something here, we assume there isnt any other like this and break
				break;
			}
			iter_old ++;
		}
		if(!bUpdated){
			// if we havnt found an 'old' perception to update, 
			newLPerception(*iter_new);			// move the pointer to that object to the perception list
		}
		else{
			// we already copied the important information to the 'old' perceipt instance using "updatePerception",
			// so we can delete that one
			delete *iter_new;					// no move to perc list, so delete that Perceipt instance,
		}
		
		m_LNewPerceptions.erase(iter_new);		// and delete the pointer from the NewPerception list
		iter_new = m_LNewPerceptions.begin();	// go back to the start of the perception list
	}
}

void CPerception::merge2LTMem(void){
	/*list<CPerceipt*>::iterator iter_percs = m_LPerceptions.begin();
	list<CPerceipt*>::iterator iter_LTMem = m_LLTMem.begin();
	bool bUpdated;

	while(iter_percs != m_LPerceptions.end()){
		if((*iter_percs)->m_lType == CPerceipt::PT_ENEMY){						// only remember enemies for longer time
			iter_LTMem = m_LLTMem.begin();
			bUpdated = false;
			while(iter_LTMem != m_LLTMem.end()){
				if((*iter_LTMem)->m_pEntity == (*iter_percs)->m_pEntity
					&&(*iter_LTMem)->m_lType == (*iter_percs)->m_lType
					&&(*iter_LTMem)->m_lTypeSpec == (*iter_percs)->m_lTypeSpec){
					updatePerception(*iter_LTMem,*iter_percs);
					bUpdated = true;
					break;
				}
				iter_LTMem ++;
			}
			if(!bUpdated){		// not updated, i.e. a new perception into the LTM
				CPerceipt *pNewPerc;							// create an extra perceipt item for the LTMem
				m_LLTMem.push_back(pNewPerc = new CPerceipt);	// put it to the list
				
				*pNewPerc = *(*iter_percs);						// and copy the fucking data
				pNewPerc->m_bLTMem = true;
			}
		}

		iter_percs ++;
	}*/
}

void CPerception::updatePerception(CPerceipt *pOld,CPerceipt*pnew){
	pOld->m_fLUpdate = g_pGame->getTime();
	pOld->m_lTypeSpec = pnew->m_lTypeSpec;
	pOld->m_VOrigin = pnew->m_VOrigin;
	pOld->m_fDistance = pnew->m_fDistance;
}

void CPerception::newLPerception(CPerceipt*pnew){	// adds a perceipt to the longer list ( not new lst, and _not_ LTM)
	//CPerceipt *pNewPerceipt = new CPerceipt;
	
	m_LPerceptions.push_front(pnew);
}

CPerceipt *CPerception::addPerceipt(long lType, long lSpec){
	CPerceipt *pNew = new CPerceipt;
	
	pNew->m_fCreate =
		pNew->m_fLUpdate = g_pGame->getTime();
	pNew->m_lType = lType;
	pNew->m_lTypeSpec = lSpec;

	m_LNewPerceptions.push_front(pNew);
	
	return pNew;
}

CPerceipt *CPerception::addCommand(long lSpec){
	CPerceipt *pNew = new CPerceipt;
	
	pNew->m_fCreate =
		pNew->m_fLUpdate = g_pGame->getTime();
	pNew->m_lType = CPerceipt::PT_COMMAND;
	pNew->m_lTypeSpec = lSpec;

	m_LCommands.push_front(pNew);
	
	return pNew;
}

void CPerception::removePerception(CEntity Entity){
	list<CPerceipt*>::iterator iter;

	for(iter = m_LPerceptions.begin();iter != m_LPerceptions.end(); iter ++){
		if((*iter)->m_pEntity == Entity){
			/*delete *iter;
			iter = m_LPerceptions.erase(iter);*/
			(*iter)->kill();
		}
	}
	// mark LTMem
	for(iter = m_LLTMem.begin();iter != m_LLTMem.end(); iter ++){
		if((*iter)->m_pEntity == Entity){
			(*iter)->m_lTypeSpec |= CPerceipt::PTX_INVALID;
		}
	}
}

void CPerception::process(void){
	float fTime = g_pGame->getTime();
	list<CPerceipt*>::iterator iter;
	list<CPerceipt*>::iterator iter_LTMem;
	bool bUpdated;
	
	// check perception list if something is old enough to be thrown out
	for(iter = m_LPerceptions.begin();iter != m_LPerceptions.end(); iter ++){
		/*CPerceipt *pTT = *iter;
		long lsize = m_LPerceptions.size();*/
		if( (*iter)->m_lType & CPerceipt::PTX_INVALID					// this perceipt is invalid ... get rid of it
			|| fTime - (*iter)->m_fLUpdate > (*iter)->m_fLifetime){		// this perceipt is already pretty old
			if((*iter)->m_lType == CPerceipt::PT_ENEMY){						// only remember enemies for longer time
				iter_LTMem = m_LLTMem.begin();
				bUpdated = false;
				while(iter_LTMem != m_LLTMem.end()){
					if((*iter_LTMem)->m_pEntity == (*iter)->m_pEntity
						&&(*iter_LTMem)->m_lType == (*iter)->m_lType
						&&(*iter_LTMem)->m_lTypeSpec == (*iter)->m_lTypeSpec){
						updatePerception(*iter_LTMem,*iter);
						bUpdated = true;
						break;
					}
					iter_LTMem ++;
				}
				if(!bUpdated){
					CPerceipt *pNewPerc;							// create an extra perceipt item for the LTMem
					m_LLTMem.push_back(pNewPerc = new CPerceipt);	// put it to the list
					
					*pNewPerc = **iter;						// and copy the fucking data
					pNewPerc->m_bLTMem = true;
				}
			}
			else{
				// if it's nothing worth puttin to LTMem ... delete it since it's invalid or too old
				delete *iter;
			}
			iter = m_LPerceptions.erase(iter);
		}
	}

	// check LT perception list if something is old enough to be thrown out
	for(iter = m_LLTMem.begin();iter != m_LLTMem.end(); iter ++){
		if(fTime - (*iter)->m_fLUpdate > m_fTime2DelLT){
			delete *iter;
			iter = m_LLTMem.erase(iter);
		}
	}
	// check commands
	for(iter = m_LCommands.begin();iter != m_LCommands.end(); iter ++){
		if(fTime - (*iter)->m_fLUpdate > (*iter)->m_fLifetime){
			delete *iter;
			iter = m_LCommands.erase(iter);
		}
	}
}
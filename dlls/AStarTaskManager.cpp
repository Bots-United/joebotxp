// AStarTaskManager.cpp: implementation of the AStarTaskManager class.
//
//////////////////////////////////////////////////////////////////////

#include "AStarTaskManager.h"

#include "AStarBase.h"
#include "Behaviour.h"

#include "Callback.h"

#include "Profiler.h"
//#include "game.h"

AStarTaskManager g_AStarTasks;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AStarTask::AStarTask(){
	m_pAStar = 0;
	m_pCB_onPathCreation = 0;
	m_iPriority = 5;
}

AStarTask::~AStarTask(){
}


AStarTaskManager::AStarTaskManager(){
	m_iSlicespF = 20;
	m_lCTasksTotal = 0;
}

AStarTaskManager::~AStarTaskManager(){
}

void AStarTaskManager::addTask(AStarBase *pAStar,int iPriority,CBaseBot *pBot,CCallback_onPathCreation *pPC){
	m_LTasks.push_back(AStarTask(pAStar,iPriority,pBot,pPC,m_lCounter));		// add task to the list
	m_lCTasksTotal ++;

	list<AStarTask>::iterator iter;

	for(iter = m_LTasks.begin(); iter != m_LTasks.end(); iter ++){
		// reset the executed counter so all tasks start again from the same position
		iter->m_lExecuted = 0;
	}
}

void AStarTaskManager::run(void){				// this should be run every frame !
	PROFILE("g_AStarTasks.run()");
	long lSize;

	m_iSlicesNow = 0;		// executed slices

	if(lSize = (long)m_LTasks.size()){
		m_lCounter ++;

		list<AStarTask>::iterator iter,iter_run = m_LTasks.begin();
		long lMin = 2000000000,lTemp;

		// find the task which is the best one to execute now
		// only work on one task each frame, that's better for our cache and therefore
		// for our speed.
		for(iter = m_LTasks.begin(); iter != m_LTasks.end(); iter ++){
			if(iter->m_iPriority)
				lTemp = iter->m_lExecuted * 256 / iter->m_iPriority;
			else
				lTemp = 100000000;

			if(lMin > lTemp){
				lMin = lTemp;
				iter_run = iter;
			}
		}

		iter_run->m_pAStar->setSliceSize(m_iSlicespF);
		iter_run->m_pAStar->runAStar();
		iter_run->m_lExecution = m_lCounter;
		iter_run->m_lExecuted += m_iSlicespF;

		m_iSlicesNow += m_iSlicespF;
	
		// check if this task is completed and can be deleted therefore
		if(iter_run->m_pAStar->finished()){
			//g_pGame->debugMsg("%li\n",m_lCounter-iter_run->m_lAdded);
			// call back
			if(iter_run->m_pCB_onPathCreation){
				iter_run->m_pCB_onPathCreation->onPathCreation(iter_run->m_pAStar);
			}
			// erase task
			iter_run = m_LTasks.erase(iter_run);
		}
	}
}

int AStarTaskManager::deleteTasksFrom(CCallback_onPathCreation *pCB){
	list<AStarTask>::iterator iter;
	int iDeletedTasks = 0;

	for(iter = m_LTasks.begin();iter != m_LTasks.end();iter ++){
		// check if this task is completed and can be deleted therefore
		if(iter->m_pCB_onPathCreation){
			if(iter->m_pCB_onPathCreation == pCB){
				// delete astarmachines
				delete iter->m_pAStar;
				iter->m_pAStar = 0;

				// erase task
				iter = m_LTasks.erase(iter);
				iDeletedTasks ++;
			}
		}
	}
	return iDeletedTasks;
}
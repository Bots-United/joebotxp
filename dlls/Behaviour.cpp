// Behaviour.cpp: implementation of the CBehaviour class.
//
//////////////////////////////////////////////////////////////////////

#include "Behaviour.h"

#include "Goal.h"
#include "Perceipt.h"
#include "BaseBot.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBehaviour::CBehaviour(CBaseBot *pBot):m_pBot(pBot){
	*m_szName=0;

	m_iExecuteFSMS = 0;
	m_iEvaluateFSMS = 0;

	m_fLastExecution = 0;

	m_iPercept = P_STMem;		// just percept the normal short term perceptions

	reset();
}

CBehaviour::~CBehaviour(){
	// in case this is really a CCallback_onPathCreation, the function might find an entry
	g_AStarTasks.deleteTasksFrom((CCallback_onPathCreation*)this);
}

void CBehaviour::reset(void){
	m_iExecuteFSMS = 0;
	m_iExecuteFSMS = 0;

	m_fLastExecution = 0;
}

void CBehaviour::evaluate(list<CGoal> *pGoals,CPerceipt* pPerceipt){
}

void CBehaviour::initialize(CGoal *pGoal){
}

void CBehaviour::execute(CGoal *pGoal){
}

void CBehaviour::deinitialize(CGoal *pGoal){
	m_iExecuteFSMS = 0;
	m_iEvaluateFSMS = 0;
}
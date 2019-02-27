// AStarTaskManager.h: interface for the AStarTaskManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__ASTARTASKMANAGER_H)
#define __ASTARTASKMANAGER_H

#include <list>
using namespace std;

class AStarBase;
class CBaseBot;
class CBehaviour;
class CCallback_onPathCreation;

// these classes handle astar task, so that you don't have any lags when paths are created. Each process is 
// executed only for some time ( based on the iteration count of A* ) and then next frame calculation is
// continued based on priority and time of last execution

class AStarTask{
    friend class AStarTaskManager;
public:
	AStarTask();
	AStarTask(AStarBase *p,int iPriority,CBaseBot *pBot,CCallback_onPathCreation *pPC=0,long lTime=0)
		:m_pAStar(p),m_iPriority(iPriority),m_pBot(pBot),m_pCB_onPathCreation(pPC),m_lExecution(lTime),m_lExecuted(0),m_lAdded(lTime){}
	virtual ~AStarTask();
protected:
	AStarBase *m_pAStar;
	CCallback_onPathCreation *m_pCB_onPathCreation;
	CBaseBot *m_pBot;
	int m_iPriority;			// priority of task - 0 is lowest, 5 is default
	long m_lExecution;			// last execution cycle
	long m_lAdded;				// when was this task added ?

	long m_lExecuted;			// how many slices are already executed ?
};

class AStarTaskManager  {
public:
	AStarTaskManager();
	virtual ~AStarTaskManager();
	void addTask(AStarBase *,int,CBaseBot *,CCallback_onPathCreation *pPC=0);
	void run(void);
	int deleteTasksFrom(CCallback_onPathCreation*);			// delete tasks with this callback

	long getTotalTasks(void){return m_lCTasksTotal;}
	unsigned long getCurrentTotalTasks(void){return (unsigned long)m_LTasks.size();}
	int getSlicesCPFrame(void){return m_iSlicesNow;}

protected:
	int m_iSlicespF;				// Slices per frame
	long m_lCounter;				// couting up each iteration
	long m_lCTasksTotal;			// Total Tasks added since start of manager
	int m_iSlicesNow;				// how many slices have been calculated ?
	list <AStarTask> m_LTasks;		// list of tasks
};

extern AStarTaskManager g_AStarTasks;

#endif // !defined(__ASTARTASKMANAGER_H)

// AStarMachine.h: interface for the AStarMachine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__ASTARMACHINE_H)
#define __ASTARMACHINE_H

#include "AStarBase.h"

#include "Profiler.h"

template <class tAStarGoal>
class AStarMachine : public AStarBase
{
public:
	AStarMachine(){
	}
	virtual ~AStarMachine(){
	}

	virtual void setStart(int iWP){
		m_Goal.setStart(iWP);
	}
	virtual void setDestination(int iWP){
		m_Goal.setDestination(iWP);
	}
	virtual int getStart(void){
		return m_Goal.getStart();
	}
	virtual int getDestination(void){
		return m_Goal.getDestination();
	}
	virtual AStarBaseGoal *getGoal(void){
		return &m_Goal;
	}
	
	virtual void linkChild(AStarNode *pNode,CPath_iterator &path_iter){
		// we have a new node: So we gotta check if it's on the open or on the closed list 
		// If none of both, we gotta put it on the open list.
		// in all cases we have to set the pointers correctly, so we'll get a nice tree here :)

		int iChildWP = path_iter.getTo();

		int g = pNode->m_g +
			m_Goal.getCost(pNode->m_iWaypoint,path_iter);		// cost for this child
		
		int iCheck;		// if it is already in some list, the index is stored here
		
		assert(iChildWP > -1 && iChildWP < _MAX_WAYPOINTS);
		
		if(iCheck = m_PQOpen.getIndex(iChildWP)){
			assert(iCheck <= m_PQOpen.m_ln && iCheck >= 0);
			// it's already on the open list ...
			AStarNode *pCheck = m_PQOpen.getItem(iCheck)->m_p;
			
			pNode->m_Children.addChild(pCheck);
			if(g < pCheck->m_g){
				pCheck->m_pParent = pNode;
				pCheck->m_g = g;
				pCheck->m_f = g + pCheck->m_h;
				
				m_PQOpen.updated(iCheck);
			}
			return;
		}
		else if(iCheck = m_PQClosed.getIndex(iChildWP)){
			assert(iCheck <= m_PQClosed.m_ln && iCheck >= 0);
			// it's already on the closed list ...
			AStarNode *pCheck = m_PQClosed.getItem(iCheck)->m_p;
			
			//pNode->m_Children.addChild(pCheck);
			if(g < pCheck->m_g){
				pCheck->m_pParent = pNode;
				pCheck->m_g = g;
				pCheck->m_f = g + pCheck->m_h;
				
				//updateParents(pCheck);
				m_PQClosed.removeUnsorted(iCheck);
				//return;
			}
			else
				return;
		}
		
		// it's neither on the open nor on the closed list ...
		AStarNode *pNewChild;
		pNewChild = newNode(iChildWP);
		
		pNewChild->m_pParent = pNode;
		pNewChild->m_g = g;
		pNewChild->m_h = m_Goal.getHeuristic(pNewChild->m_iWaypoint);
		pNewChild->m_f = pNewChild->m_h + g;
		
		AStarNodeContainer PnC;
		PnC.m_p = pNewChild;
		
		pNode->m_Children.addChild(pNewChild);
		m_PQOpen.insert(PnC);
	}
	
	virtual bool runAStar(void){
		///PROFILE("runAStar();");
		// main function of all this stuff :)
		// - main loop
		// - handle slices
		// - getting best nodes
		// - linking nodes
		// - getting new open nodes

		int iConnectedWP,
			iCurrentWP;

		AStarNode *P,*B;
		AStarNodeContainer PC;
		CPath_iterator path_iter;

		if(!m_Goal.isValid())
			return false;
		
		if(m_bFinished){			// if the last job was finished, this is a new job
			resetNodes();

			m_iCSlice = 0;
			
			m_iBreakReason = BR_NONE;			// no break reason by default
			
			m_lRevolution = 0;
			m_lCSlice2 = m_lSliceSize;
			
			P = newNode(m_Goal.getStart());
			
			P->m_g = 0;
			P->m_h = m_Goal.getHeuristic(m_Goal.getStart());
			P->m_f = P->m_g + P->m_h;

			PC.m_p = P;			// set container pointer
			
			m_PQOpen.insert(PC);
		}
		else{			// job isnt finished yet, so just prepare a new slice and go on 
			m_iCSlice ++;
			m_lCSlice2 += m_lSliceSize;
		}
		
		while(true){
			//PROFILE("runAStar-loop");
			// check if this is enough
			if(m_lRevolution > m_lMaxRevolutions){	// is the maximum revolution count reached ?
				m_iBreakReason = BR_MAXREV;
				m_bFinished = true;
				logResult();
				return false;
			}

			if(m_lRevolution > m_lCSlice2){			// a slice is full
				m_iBreakReason = BR_MAXSLICE;

				m_bFinished = false;
				return true;
			}

			if(m_PQOpen.empty()){		// no more open nodes - no path has been found
				m_bFinished = true;
				logResult();
				return false;
			}
			PC = m_PQOpen.getTop();
			B = PC.m_p;
			if(m_Goal.isDestination(B)){		// a path has been found
				m_pDestinationNode = B;
				m_bFinished = true;
				m_iBreakReason = BR_NONE;		// it's not a break :D
				logResult();
				return true;
			}
			iCurrentWP = B->m_iWaypoint;

			path_iter = g_Map.m_Waypoints[iCurrentWP];
			while(true){
				iConnectedWP = path_iter.getTo();

				if(iConnectedWP != -1){
					if(m_Goal.isOpen(iCurrentWP,path_iter)){
						linkChild(B,path_iter);
					}
					++path_iter;
				}
				else
					break;
			}

			m_PQClosed.insertUnsorted(PC);

			m_lRevolution ++;
		}
	}
	// data
	
	tAStarGoal m_Goal;
};

#endif // !defined(__ASTARMACHINE_H)

#ifndef __CPQUEUEHASH_H
#define __CPQUEUEHASH_H

#include "PQueue.h"
#include "Map.h"
#include "AStarNodeContainer.h"

class CPQueueHash:public CPQueue<AStarNodeContainer>{
	public:
		
		CPQueueHash(){
			memset(m_piHash,0,sizeof(int)*_MAX_WAYPOINTS);
		}

		virtual ~CPQueueHash(){
		}

		void reset(void){
			m_ln = 0;
			memset(m_piHash,0,sizeof(int)*_MAX_WAYPOINTS);
		}

		virtual void swap(long l1,long l2){
			AStarNodeContainer temp;
			temp = m_pData[l1];
			m_pData[l1] = m_pData[l2];
			m_piHash[m_pData[l1].m_p->m_iWaypoint] = l1;
			m_pData[l2] = temp;
			m_piHash[m_pData[l2].m_p->m_iWaypoint] = l2;
		}

		virtual int getIndex(int iWaypoint){
			return m_piHash[iWaypoint];
			/*for(int i=1;i <= m_ln;i++){
				if(m_pData[i].m_p->m_iWaypoint == iWaypoint)
					return i;
			}
			return 0;*/
		}

		void updated(long l){
			if(l != m_ln){
				AStarNodeContainer temp = m_pData[l];		// get this element
				//m_piHash[m_pData[l].m_p->m_iWaypoint] = 0;
				
				m_pData[l] = m_pData[m_ln];	// remove it
				m_piHash[m_pData[l].m_p->m_iWaypoint] = l;
				m_ln--;
				downHeap(l);				// resort tree
				m_ln ++;
				
				m_pData[m_ln] = temp;		// reinsert it
				m_piHash[temp.m_p->m_iWaypoint] = m_ln;
				
				upHeap(m_ln);
			}
			else{
				upHeap(m_ln);
			}
		}

		void insert(const AStarNodeContainer &param){
			check1More();

			assert(param.m_p->m_iWaypoint != -1);

			m_ln ++;

			m_pData[m_ln] = param;
			m_piHash[param.m_p->m_iWaypoint] = m_ln;

			upHeap(m_ln);
		}

		void insertUnsorted(const AStarNodeContainer &param){
			check1More();

			assert(param.m_p->m_iWaypoint != -1);

			m_ln ++;

			m_pData[m_ln] = param;
			m_piHash[param.m_p->m_iWaypoint] = m_ln;
			//upHeap(m_ln);
		}

		AStarNodeContainer getTop(void){
			AStarNodeContainer ret = m_pData[1];
			
			m_piHash[m_pData[m_ln].m_p->m_iWaypoint] = 1;
			m_piHash[m_pData[1].m_p->m_iWaypoint] = 0;
			
			m_pData[1] = m_pData[m_ln];
			
			m_ln--;
			downHeap(1);

			return ret;
		}

		int m_piHash[_MAX_WAYPOINTS];
	protected:
};

#endif
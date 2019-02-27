#ifndef __CPQUEUEHASH_H
#define __CPQUEUEHASH_H

#include "Map.h"
#include "AStarNodeContainer.h"

class CPQueueHash{
	public:
		inline long left(long l)	{return l*2;}
		inline long right(long l)	{return l*2+1;}
		inline long parent(long l)	{return l/2;}

		CPQueueHash(){
			m_ln = 0;
			m_lAllocPlus = 1024;
			m_lAlloc = m_lAllocPlus;
			m_pData = new AStarNodeContainer[m_lAlloc];

			memset(m_piHash,0,sizeof(int)*_MAX_WAYPOINTS);		// no entrys yet, so keep the hashtable clear
		}

		virtual ~CPQueueHash(){
			if(m_pData){
				delete [] m_pData;
				m_pData = 0;
			}
		}

		inline void reset(void){
			m_ln = 0;
			memset(m_piHash,0,sizeof(int)*_MAX_WAYPOINTS);
		}

		inline void swap(long l1,long l2){
			AStarNodeContainer temp;
			temp = m_pData[l1];
			m_pData[l1] = m_pData[l2];
			m_piHash[m_pData[l1].m_p->m_iWaypoint] = l1;
			m_pData[l2] = temp;
			m_piHash[m_pData[l2].m_p->m_iWaypoint] = l2;
		}

		inline int getIndex(int iWaypoint){
			return m_piHash[iWaypoint];
			/*for(int i=1;i <= m_ln;i++){
				if(m_pData[i].m_p->m_iWaypoint == iWaypoint)
					return i;
			}*/
			return 0;
		}

		inline AStarNodeContainer *getItem(long lI){
			return &(m_pData[lI]);
		}

		inline long getCount(void){
			return m_ln;
		}

		inline bool empty(void){
			return (m_ln == 0);
		}

		inline void check1More(void){
			if(m_ln +1 >= m_lAlloc){
				long l_newAlloc = m_lAlloc + m_lAllocPlus;
				AStarNodeContainer *p_newData = new AStarNodeContainer[l_newAlloc];
				long lschl;

				for(lschl = 0;lschl < m_lAlloc;lschl ++){
					p_newData[lschl] = m_pData[lschl];
				}
				m_lAlloc = l_newAlloc;
				delete [] m_pData;
				m_pData = p_newData;
			}
		}

		inline void upHeap(long l){
			long lParent = parent(l);
			while(lParent){
				if(m_pData[l] < m_pData[lParent]){
					swap(l,lParent);

					l = lParent;
					lParent = parent(l);
				}
				else 
					break;
			}
		}

		inline void downHeap(long l){
			long lChild;
			while(true){
				lChild = left(l);

				if(lChild <= m_ln){
					if(lChild+1 <= m_ln){		// 2 childs
						if(m_pData[lChild] > m_pData[lChild+1]){	// tak the smaller one
							lChild ++;
						}
					}
					//else{					// one child
					//}

					if(m_pData[l] > m_pData[lChild]){
						swap(l,lChild);
					}

					l = lChild;
				}
				else
					break;
			}
		}

		inline void updated(long l){
			if(l != m_ln){
				AStarNodeContainer temp = m_pData[l];		// get this element
				m_piHash[temp.m_p->m_iWaypoint] = 0;
				
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

		inline void insert(AStarNodeContainer &param){
			check1More();

			assert(param.m_p->m_iWaypoint != -1);

			m_ln ++;

			m_pData[m_ln] = param;
			m_piHash[param.m_p->m_iWaypoint] = m_ln;

			upHeap(m_ln);
		}

		inline void insertUnsorted(AStarNodeContainer &param){
			check1More();

			assert(param.m_p->m_iWaypoint != -1);

			m_ln ++;

			m_pData[m_ln] = param;
			m_piHash[param.m_p->m_iWaypoint] = m_ln;
		}

		inline AStarNodeContainer getTop(void){
			AStarNodeContainer ret = m_pData[1];
			
			m_piHash[m_pData[m_ln].m_p->m_iWaypoint] = 1;
			m_piHash[m_pData[1].m_p->m_iWaypoint] = 0;
			
			m_pData[1] = m_pData[m_ln];
			
			m_ln--;
			downHeap(1);

			return ret;
		}

		inline void remove(long l){
			m_piHash[m_pData[m_ln].m_p->m_iWaypoint] = l;
			m_piHash[m_pData[l].m_p->m_iWaypoint] = 0;
			
			m_pData[l] = m_pData[m_ln];
			
			m_ln--;
			downHeap(l);
		}

		inline void removeUnsorted(long l){
			m_piHash[m_pData[m_ln].m_p->m_iWaypoint] = l;
			m_piHash[m_pData[l].m_p->m_iWaypoint] = 0;
			
			m_pData[l] = m_pData[m_ln];
			
			m_ln--;
			//downHeap(l);
		}

		inline AStarNodeContainer getBack(void){
			AStarNodeContainer ret = m_pData[m_ln];
			
			m_piHash[m_pData[m_ln].m_p->m_iWaypoint] = 0;
			
			m_ln--;

			return ret;
		}

		inline AStarNodeContainer getTopUnsorted(void){
			AStarNodeContainer ret = m_pData[1];
			
			m_piHash[m_pData[m_ln].m_p->m_iWaypoint] = 1;
			m_piHash[m_pData[1].m_p->m_iWaypoint] = 0;
			
			m_pData[1] = m_pData[m_ln];
			
			m_ln--;

			return ret;
		}

		int m_piHash[_MAX_WAYPOINTS];
	//protected:
		long m_ln;
		long m_lAlloc;
		long m_lAllocPlus;
		AStarNodeContainer *m_pData;
};

#endif


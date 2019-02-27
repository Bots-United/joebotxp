#ifndef __PQUEUE_H
#define __PQUEUE_H

template<class TYPE>
class CPQueue{
	public:
		long left(long l)	{return l*2;}
		long right(long l)	{return l*2+1;}
		long parent(long l)	{return l/2;}

		CPQueue(){
			m_ln = 0;
			m_lAllocPlus = 1024;
			m_lAlloc = m_lAllocPlus;
			m_pData = new TYPE[m_lAlloc];
		}

		virtual ~CPQueue(){
			if(m_pData){
				delete [] m_pData;
				m_pData = 0;
			}
		}

		void reset(void){
			m_ln = 0;
		}

		virtual void swap(long l1,long l2){
			TYPE temp;
			temp = m_pData[l1];
			m_pData[l1] = m_pData[l2];
			m_pData[l2] = temp;
		}

		long getCount(void){
			return m_ln;
		}

		bool empty(void){
			return (m_ln == 0);
		}

		TYPE *getItem(long lI){
			return &(m_pData[lI]);
		}

		void insert(const TYPE &param){
			check1More();

			m_ln ++;

			m_pData[m_ln] = param;

			upHeap(m_ln);
		}

		void updated(long l){
			if(l != m_ln){
				TYPE temp = m_pData[l];		// get this element
				
				m_pData[l] = m_pData[m_ln];	// remove it
				m_ln--;
				downHeap(l);				// resort tree
				m_ln ++;
				
				m_pData[m_ln] = temp;		// reinsert it
				
				upHeap(m_ln);
			}
			else{
				upHeap(m_ln);
			}
		}

		void insertUnsorted(const TYPE &param){
			check1More();

			m_ln ++;

			m_pData[m_ln] = param;
		}

		void upHeap(long l){
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

		void downHeap(long l){
			long lChild;
			while(1){
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

		TYPE getTop(void){
			TYPE ret = m_pData[1];

			m_pData[1] = m_pData[m_ln];
			m_ln--;
			downHeap(1);

			return ret;
		}

		void check1More(void){
			if(m_ln +1 >= m_lAlloc){
				long l_newAlloc = m_lAlloc + m_lAllocPlus;
				TYPE *p_newData = new TYPE[l_newAlloc];
				long lschl;

				for(lschl = 0;lschl < m_lAlloc;lschl ++){
					p_newData[lschl] = m_pData[lschl];
				}
				m_lAlloc = l_newAlloc;
				delete [] m_pData;
				m_pData = p_newData;
			}
		}
//	protected:
		long m_ln;
		long m_lAlloc;
		long m_lAllocPlus;
		TYPE *m_pData;
};

#endif
#ifndef __MEMORYPOOL_H
#define __MEMORYPOOL_H

#include <vector>
//#include <iostream>
using namespace std;

template <class TTYPE, int TiBlockSize>
class CMemoryPool{
public:
	CMemoryPool(){
		m_lAlloc = 0;
		m_lNextFree = 0;
		m_ppFree = 0;
		m_lCAlloc = m_lCFree = m_lCMax = 0;
	}

	~CMemoryPool(){
		while(!m_LAlloc.empty()){
			delete [] m_LAlloc.back();
			m_LAlloc.pop_back();
		}
	}

	inline void allocNew(void){
		TTYPE *pnew;
		TTYPE **pnewFArray,**ppSet;
		long l;
		long lNAlloc = m_lAlloc + TiBlockSize;		// size of newly allocated pointer array
	
		pnew = new TTYPE[TiBlockSize];				// new isnatnces
		pnewFArray = new TTYPE*[lNAlloc];			// new free array
		if(m_ppFree){
			memcpy(pnewFArray,m_ppFree,sizeof(TTYPE *)*m_lAlloc);	// copy old free array
			delete [] m_ppFree;						// delete old free array
		}
		m_ppFree = pnewFArray;						// set normal free array to new one

		m_LAlloc.push_back(pnew);					// enlist the newly allocated field for the destructor

		ppSet = &(m_ppFree[m_lAlloc]);				// add the elements to the free field
		for(l = 0; l<TiBlockSize; l++){
			ppSet[l] = &(pnew[l]);
		}
		m_lAlloc = (long)m_LAlloc.size()*TiBlockSize;		// update number of total free instances
	}

	inline TTYPE *get(void){
		//return new TTYPE;
		if(m_lNextFree == m_lAlloc){ // >=, but > should be in either case.
			allocNew();				// get new free instances
		}
		TTYPE *p;
		
		p = m_ppFree[m_lNextFree];
		m_lNextFree ++;


		if(m_lNextFree > m_lCMax){		// just for logging
			m_lCMax = m_lNextFree;
		}
		m_lCAlloc ++;	// just count it for logging
		return p;
	}
	inline void free(TTYPE *p){
		//delete p;
		return;
		//assert(isValidPointer(p) && "an invalid pointer has been given to CMemoryPool::free");
		m_lCFree ++;	// just count it for logging
		if(m_lNextFree)
			m_lNextFree --;
		m_ppFree[m_lNextFree] = p;		// enqueue it again in the free field
	}

	// check if this pointer can be valid
	inline bool isValidPointer(TTYPE *p){
		int max = sizeof(TTYPE) * TiBlockSize;		// hash tables and stuff would be way faster ... and we still not check if this has been freed twice :(
		for(int i=0;i < m_LAlloc.size(); i++){
			if( p >= m_LAlloc[i]
				&& p < m_LAlloc[i] + max){
				return true;
			}
		}
		return false;
	}

	// data members

	long m_lCAlloc,m_lCFree,m_lCMax;	// counting calls of get, free and maximum use of this pool
//protected:
	TTYPE **m_ppFree;					// an array with free instances
	long m_lNextFree;					// index of next free instance
	long m_lAlloc;						// number of total allocated fields
	vector<TTYPE *> m_LAlloc;			// list of allocated instance arrays
protected:
private:
};

#endif


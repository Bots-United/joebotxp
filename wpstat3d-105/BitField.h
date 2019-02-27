// CBitField.h: interface for the CBitField class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__BITFIELD_H)
#define __BITFIELD_H

#include <stdio.h>
#include <assert.h>
#include <memory.h>

class CBitField  
{
public:
	CBitField(long lBits){
		long lSize = lBits/sizeof(long)/8+1;
		
		m_plData = new long[lSize];
		m_lBits = lBits;
		memset(m_plData,0,sizeof(long)*lSize);
	}

	CBitField(const CBitField &param){
		long lSize = param.m_lBits / sizeof(long) / 8 + 1;
		
		m_plData = new long(lSize);
		m_lBits = param.m_lBits;

		memcpy(m_plData,param.m_plData,sizeof(long)*lSize);
	}
	virtual ~CBitField();

	void load(FILE *);
	void save(FILE *);

	void setBit(long lBit,bool bSet){
		assert(lBit <= m_lBits);

		long lIndex,lShift,lSet;

		lIndex = lBit / (sizeof(long) * 8);
		lShift = lBit - lIndex * (sizeof(long) * 8);

		lSet = (1<<lShift);

		if(bSet){
			m_plData[lIndex] |= lSet;
		}
		else{
			m_plData[lIndex] &=~ lSet;
		}
	};

	void zero(void){
		long lSize = m_lBits/sizeof(long)/8+1;
		
		memset(m_plData,0,sizeof(long)*lSize);
	}

	bool getBit(long lBit){
		long lIndex,lShift,lSet;

		lIndex = lBit / (sizeof(long) * 8);
		lShift = lBit - lIndex * (sizeof(long) * 8);

		lSet = m_plData[lIndex];
		lSet>>=lShift;

		return (lSet&1);
	};

	CBitField &operator &= (const CBitField &param){
		long l,lMax;
		if(m_lBits>param.m_lBits){
			lMax = m_lBits / sizeof(long) / 8 + 1;
		}
		else{
			lMax = param.m_lBits / sizeof(long) / 8 + 1;
		}
		for(l=0;l < lMax; l++){
			m_plData[l] &= param.m_plData[l];
		}
		return ((CBitField &)(*this));
	}

	CBitField &operator |= (const CBitField &param){
		long l,lMax;
		if(m_lBits>param.m_lBits){
			lMax = m_lBits / sizeof(long) / 8 + 1;
		}
		else{
			lMax = param.m_lBits / sizeof(long) / 8 + 1;
		}
		for(l=0;l < lMax; l++){
			m_plData[l] |= param.m_plData[l];
		}
		return ((CBitField &)(*this));
	}

	CBitField &operator = (const CBitField &param){
		long lSize = param.m_lBits / sizeof(long) / 8 + 1;
		if(param.m_lBits != m_lBits){
			delete m_plData;
			m_plData = new long(lSize);
			m_lBits = param.m_lBits;
		}
		memcpy(m_plData,param.m_plData,sizeof(long)*lSize);

		return ((CBitField &)(*this));
	}
private:
	long *m_plData;
	long m_lBits;
};

#endif // !defined(__BITFIELD_H)

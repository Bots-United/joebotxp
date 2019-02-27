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
	friend class CBitProxy;
	friend class CBFIterator;

	class CBitProxy
	{
	public:
		CBitProxy(CBitField &array, long pos):m_Array(array), m_Pos(pos){}
		
		CBitProxy &operator=(bool value){
			m_Array.setBit(m_Pos, value);
			return *this;
		}

		CBitProxy &operator=(const CBitProxy &that){
			m_Array.setBit(m_Pos, that.m_Array.getBit(that.m_Pos));
			return *this;
		}

		operator bool() const{
			return m_Array.getBit(m_Pos);
		}
	private:
		CBitField	&m_Array;
		long		m_Pos;
	};

	// a iterator class for bitfields ... the typedef shit is just because there is somewhere an iterator
	// struct in std ... so we had to do something to stop the compiler nagging around
	typedef class CBFIterator{
	public:
		CBFIterator(){
			m_pBField = 0;
		}
		CBFIterator(const CBFIterator &P){
			m_lBit = P.m_lBit;
			m_lOffset = P.m_lOffset;
			m_lPos = P.m_lPos;
			m_pBField = P.m_pBField;
		}
		// calculate offset and bit from m_lPos
		void calcPos(void){
			m_lOffset = m_lPos / (sizeof(long) * 8);
			m_lBit = m_lPos - m_lOffset * (sizeof(long) * 8);
		}
		// set pointer to new Bitfield and reset counter
		void initialize(CBitField &PBitField){
			m_lPos = 0;
			m_pBField = &PBitField;
			// calcPos();		// doing this directly might be faster
			m_lOffset = 0;
			m_lBit = 0;
		}
		// set iterator to some position
		void setPos(long lPPos){
			m_lPos = lPPos;
			calcPos();
		}
		// where are we
		long getPos(void){
			return m_lPos;
		}
		// read the bit there
		bool getBit(void){
			assert(inRange());

			long lSet;

			lSet = m_pBField->m_plData[m_lOffset];
			lSet>>=m_lBit;

			return (lSet&1);
		}
		// only predecrement ... is postdecrement really needed ?!
		const CBFIterator & operator ++(){
			m_lPos ++;
			m_lBit ++;
			if(m_lBit >= (sizeof(long) * 8)){
				m_lBit = 0;
				m_lOffset ++;
			}
			return ((const CBFIterator &)(*this));
		}
		// check if the iterator is still valid
		bool inRange(void){
			if(m_lPos < m_pBField->m_lBits)
				return true;
			return false;
		}

		CBFIterator &operator =(CBitField &bitfield){
			initialize(bitfield);

			return ((CBFIterator &)(*this));
		}
		/*~CBFIterator(){
		}*/
	private:
		const CBitField *m_pBField;

		long m_lPos;
		long m_lOffset;	// at which element of the array are we ?!
		long m_lBit;	// and which bit ?
	}bf_iterator;

	CBitField(){
		// nothing here yet
		m_plData = 0;
		m_lBits = 0;
	}

	CBitField(long lBits){	// we want a bitfield with lBits bits please
		long lSize = lBits/sizeof(long)/8+1;

		m_plData = new long[lSize];
		m_lBits = lBits;
		memset(m_plData,0,sizeof(long)*lSize);
	}

	CBitField(const CBitField &param){		// heyho, just copy that
		long lSize = param.m_lBits / sizeof(long) / 8 + 1;

		m_plData = new long[lSize];
		m_lBits = param.m_lBits;

		memcpy(m_plData,param.m_plData,sizeof(long)*lSize);
	}
	virtual ~CBitField();

	void load(FILE *);
	void save(FILE *);

	long bits(void){
		return m_lBits;
	}

	void setBit(long lBit,bool bSet){
		// set a bit ... or delete it
		assert(lBit < m_lBits);

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
	}

	void zero(void){
		// reset the whole bitfield
		long lSize = m_lBits/sizeof(long)/8+1;

		memset(m_plData,0,sizeof(long)*lSize);
	}

	bool getBit(long lBit) const{
		// read some bit
		assert(lBit < m_lBits);
		long lIndex,lShift,lSet;

		lIndex = lBit / (sizeof(long) * 8);
		lShift = lBit - lIndex * (sizeof(long) * 8);

		lSet = m_plData[lIndex];
		lSet>>=lShift;

		return (lSet&1);
	}

	void negate(void){
		// true <=> false for each entry
		long l,lMax;

		lMax = m_lBits / sizeof(long) / 8 + 1;

		for(l=0;l < lMax; l++){
			m_plData[l] = ~m_plData[l];
		}
	}

	/*CBitField operator~(){
	CBitField ret;

	ret = *this;

	ret.negate();

	return ret;
	}*/

	CBitField &operator &= (const CBitField &param){
		long l,lMax;
		if(m_lBits<=param.m_lBits){					// take the smallest bitfield
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
		if(m_lBits<=param.m_lBits){					// take the smallest bitfield
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

	CBitField &operator ^= (const CBitField &param){
		long l,lMax;
		if(m_lBits<=param.m_lBits){					// take the smallest bitfield
			lMax = m_lBits / sizeof(long) / 8 + 1;
		}
		else{
			lMax = param.m_lBits / sizeof(long) / 8 + 1;
		}
		for(l=0;l < lMax; l++){
			m_plData[l] ^= param.m_plData[l];
		}
		return ((CBitField &)(*this));
	}

	CBitField &operator = (const CBitField &param){
		// copy data from parameter to here
		long lSize = param.m_lBits / sizeof(long) / 8 + 1;
		if(param.m_lBits != m_lBits){
			delete [] m_plData;
			m_plData = new long[lSize];
			m_lBits = param.m_lBits;
		}
		memcpy(m_plData,param.m_plData,sizeof(long)*lSize);

		return ((CBitField &)(*this));
	}

	CBitField &partialCopy(const CBitField &param){
		long lIndex;
		if(param.m_lBits == m_lBits){								// sizes are the same ...
			lIndex = param.m_lBits / (sizeof(long) * 8)+1;			// only copy the data
			memcpy(m_plData,param.m_plData,sizeof(long)*lIndex);
		}
		else if(param.m_lBits > m_lBits){							// copy only the data that fits in there
			lIndex = m_lBits / (sizeof(long) * 8)+1;
			memcpy(m_plData,param.m_plData,sizeof(long)*lIndex);
		}
		else{														// copy as much as possible
			lIndex = param.m_lBits / (sizeof(long) * 8)+1;
			memcpy(m_plData,param.m_plData,sizeof(long)*lIndex);
			/*
			lIndex = param.m_lBits / (sizeof(long) * 8);
			memcpy(m_plData,param.m_plData,sizeof(long)*lIndex);
			for(long lschl = lIndex * sizeof(long) * 8; lschl < param.m_lBits; lschl ++){
				setBit(lschl,param.getBit(lschl));
			}*/
		}
		//m_lBits = param.m_lBits;
		return ((CBitField &)(*this));
	}

	bool operator == (const CBitField &param){
		// check sizes
		if(param.m_lBits != m_lBits)
			return false;

		// dont rely on getBit here, it's too slow

		long lSizeFull = param.m_lBits / sizeof(long) / 8;

		// check first bytes
		for(long l = lSizeFull - 2; l >= 0; l--){
			if(m_plData[l] != param.m_plData[l])
				return false;
		}
		// and check if there are the last bits still unchecked bits at the end
		long lUnchecked = m_lBits - lSizeFull*sizeof(long)*8;
		if(!lUnchecked)		// no more bits to check
			return true;

		long lMask = 0;		// first the mask is 0

		/*for(;lUnchecked > 0;lUnchecked--){
			lMask <<= 1;
			lMask |= 1;
		}*/
		lMask = 1;
		lMask <<= lUnchecked;
		lMask --;

		// lSizeFull-1 is last element of the array
		if((m_plData[lSizeFull-1]&lMask) != (param.m_plData[lSizeFull-1]&lMask))
			return false;

		return true;
	}

	bool operator != (const CBitField &param){
		return !(*this == param);
	}

	CBitProxy operator[](long pos){
		assert(pos>-1 && pos < m_lBits);
		return CBitProxy(*this, pos);
	}

	const CBitProxy operator[](long pos) const{
		assert(pos < m_lBits);
		return CBitProxy(const_cast<CBitField &>(*this), pos);
	}

	long getSet(void){		// just return an index to a bit which is set
		long lSizeFull = m_lBits / sizeof(long) / 8;
		long lStart = 0;
		for(lStart = 0;lStart < lSizeFull; lStart ++){
			if(m_plData[lStart])
				break;
		}
		if(lStart != lSizeFull){
			lStart *= (sizeof(long)*8);
			/*for(;lStart < m_lBits; lStart ++){
				if(getBit(lStart))
					return lStart;
			}*/
			bf_iterator iter;
			iter.initialize(*this);
			iter.setPos(lStart);
			while(iter.inRange()){
				if(iter.getBit())
					return iter.getPos();
				++iter;
			}
		}
		return -1;
	}

	long countSet(void){		// just return an index to a bit which is set
		long lCount = 0;

		bf_iterator iter;
		iter.initialize(*this);
		while(iter.inRange()){
			if(iter.getBit())
				lCount ++;
			++iter;
		}
		return lCount;
	}

	void setSize(long lNewBits){
		if(lNewBits == m_lBits)
			return;

		long lSize = lNewBits / sizeof(long) / 8 + 1;

		delete [] m_plData;
		m_lBits = lNewBits;
		m_plData = new long[lSize];
	}

private:
	long *m_plData;
	long m_lBits;
};

#endif // !defined(__BITFIELD_H)

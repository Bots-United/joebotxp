// CBitField.cpp: implementation of the CBitField class.
//
//////////////////////////////////////////////////////////////////////

#include "BitField.h"

#include <memory.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBitField::~CBitField(){
	if(m_plData){
		delete [] m_plData;
		m_plData = 0;
		m_lBits = 0;
	}
}

void CBitField::load(FILE *fhd){
	fread(&m_lBits,sizeof(long),1,fhd);
	long lSize = m_lBits/sizeof(long)/8+1;
	delete[]m_plData;
	m_plData = new long[lSize];
	fread(m_plData,sizeof(long),lSize,fhd);
}

void CBitField::save(FILE *fhd){
	if(m_lBits && m_plData){
		long lSize = m_lBits/sizeof(long)/8+1;
		fwrite(&m_lBits,sizeof(long),1,fhd);
		fwrite(m_plData,sizeof(long),lSize,fhd);
	}
}
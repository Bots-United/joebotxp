#include <stdio.h>
#include "pathstat.h"

CPathStat::CPathStat(void){
	m_iFailed = 0;
	m_iWorldDamage = 0;
}

CPathStat::~CPathStat(void){
}

void CPathStat::reset(){
	m_iFailed = 0;
}

void CPathStat::save(FILE *fhd){
	unsigned short usTemp;

	usTemp = m_iFailed;
	fwrite(&usTemp,sizeof(unsigned short),1,fhd);
}

void CPathStat::load(FILE *fhd){
	unsigned short usTemp;

	fread(&usTemp,sizeof(unsigned short),1,fhd);

	m_iFailed = usTemp;
}
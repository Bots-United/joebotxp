// RespCurve.cpp: implementation of the CRespCurve class.
//
//////////////////////////////////////////////////////////////////////

#include "RespCurve.h"

#include <memory.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRespCurve::CRespCurve(){
}

CRespCurve::~CRespCurve(){
}

bool CRespCurve::addEntry(float fpx,float fpy){
	RespCurveEntry_t add;

	add.m_fx = fpx;
	add.m_fy = fpy;

	m_ppfSlots.push_back(add);
	return true;
}

float CRespCurve::getValue(float fpx) const{
	if(m_ppfSlots.size()){
		if(fpx <= m_ppfSlots[0].m_fx){
			return m_ppfSlots[0].m_fy;
		}
		
		if(fpx >= m_ppfSlots[m_ppfSlots.size()-1].m_fx){
			return m_ppfSlots[m_ppfSlots.size()-1].m_fy;
		}
		
		for(unsigned int i=1; i < m_ppfSlots.size(); i++){
			if(fpx < m_ppfSlots[i].m_fx){		// we found a bucket :D
				return m_ppfSlots[i-1].m_fy
					+ (m_ppfSlots[i].m_fy - m_ppfSlots[i-1].m_fy)
					* ((fpx - m_ppfSlots[i-1].m_fx)	/ (m_ppfSlots[i].m_fx - m_ppfSlots[i-1].m_fx));
			}
		}
	}
	return 0;
}

float CRespCurve::getMaximum(void) const{
	float fMax = 0;
	unsigned int i;

	for(i = 0; i<m_ppfSlots.size(); i++){
		if(m_ppfSlots[i].m_fy > fMax){
			fMax = m_ppfSlots[i].m_fy;
		}
	}
	return fMax;
}

CRespCurve &CRespCurve::operator =(const CRespCurve &P){
	m_ppfSlots = P.m_ppfSlots;

	return ((CRespCurve &)(*this));
}
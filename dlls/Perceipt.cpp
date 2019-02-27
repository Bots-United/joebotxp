// Perceipt.cpp: implementation of the CPerceipt class.
//
//////////////////////////////////////////////////////////////////////

#include "Perceipt.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern long g_lGlobalCount;

CPerceipt::CPerceipt()
{
	reset();
	g_lGlobalCount ++;
}

CPerceipt::~CPerceipt(){
	/*if(m_lType == PT_COMMAND){
		if(m_lTypeSpec & PTX_ASTARINSIDE){
			delete m_pPointer;
			m_pPointer = 0;
		}
	}*/
	g_lGlobalCount --;
}

void CPerceipt::reset(void){
	/*if(m_lType == PT_COMMAND){
		if(m_lTypeSpec & PTX_ASTARINSIDE){
			delete m_pPointer;
			m_pPointer = 0;
		}
	}*/

	m_fCreate =
	m_fLUpdate =
	m_fDistance = 0.f;
	m_pIEntity = 0;
	m_lType = 0;
	m_lTypeSpec = 0;
	m_bLTMem = false;
	m_pEntity = 0;
	m_fValue = 0.f;
	m_iAddInfo = 0;
	m_pPlayer = 0;
	m_pPointer = 0;
	m_pPointer1 = 0;
	m_VOrigin = Vector(0.f,0.f,0.f);
	m_fLifetime = 3.f;			// default perceipt lifetime is 3 seconds
}

void CPerceipt::kill(void){
	m_fLifetime = 0;
	m_lTypeSpec |= CPerceipt::PTX_INVALID;
}
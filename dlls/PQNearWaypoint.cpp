// PQNearWaypoint.cpp: implementation of the CPQNearWaypoint class.
//
//////////////////////////////////////////////////////////////////////

#include "PQNearWaypoint.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPQINearWaypoint::CPQINearWaypoint()
{
	m_iWPIndex = -1;
	m_fDistance = 0;
}

CPQINearWaypoint::~CPQINearWaypoint()
{

}

bool CPQINearWaypoint::operator < (const CPQINearWaypoint &param){
	return m_fDistance < param.m_fDistance;
}

bool CPQINearWaypoint::operator > (const CPQINearWaypoint &param){
	return m_fDistance > param.m_fDistance;
}
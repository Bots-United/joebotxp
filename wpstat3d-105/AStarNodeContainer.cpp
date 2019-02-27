// AStarNodeContainer.cpp: implementation of the AStarNodeContainer class.
//
//////////////////////////////////////////////////////////////////////
#include <math.h>
#include "AStarNodeContainer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AStarNodeContainer::AStarNodeContainer()
{
	m_p = 0;
}

AStarNodeContainer::~AStarNodeContainer()
{

}

AStarNodeContainer::operator <(const AStarNodeContainer &param){
	return m_p->m_f < param.m_p->m_f;
}

AStarNodeContainer::operator >(const AStarNodeContainer &param){
	return m_p->m_f > param.m_p->m_f;
}

const AStarNodeContainer &AStarNodeContainer::operator =(const AStarNodeContainer &param){
	m_p = param.m_p;

	return ((const AStarNodeContainer &)(*this));
}
// AStarBase.cpp: implementation of the AStarBase class.
//
//////////////////////////////////////////////////////////////////////
#include <math.h>

#include "AStarBase.h"

#include <iostream>
using namespace std;

extern FILE *g_astarmachinelog_fhd;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMemoryPool<AStarNode,256> g_ASN_MPool;

AStarBase::AStarBase()
{
	m_iDestination = -1;
	m_iStart = -1;

	m_pDestinationNode = 0;
	//m_pFirstNode = m_pLastNode = new AStarNode;		// insert dummy node to avoid additional pointer checks
	m_pFirstNode = m_pLastNode = g_ASN_MPool.get();

	m_lMaxRevolutions = 100000000;
	m_lSliceSize = 10000000;
	m_lCSlice2 = 0;
	m_bFinished = true;
}

AStarBase::~AStarBase()
{
	resetNodes();
	//delete m_pFirstNode;
	g_ASN_MPool.free(m_pFirstNode);
	m_pFirstNode = m_pLastNode = 0;
}

void AStarBase::setDestination(int iWPDestination){
	m_iDestination = iWPDestination;
}

void AStarBase::setStart(int iWPStart){
	m_iStart = iWPStart;
}

void AStarBase::setMaxRevolutions(long lMax){
	m_lMaxRevolutions = lMax;
}

void AStarBase::resetNodes(void){
	AStarNode *p,*pd;

	pd = p = m_pFirstNode->m_pNext;
	while(p){
		p = p->m_pNext;
		deleteNode(pd);
		pd = p;
	}
	m_pFirstNode->m_pNext = 0;
	m_pLastNode = m_pFirstNode;

	m_PQClosed.reset();
	m_PQOpen.reset();

	m_lCSlice2 = 0;
	m_bFinished = true;
}

void AStarBase::logResult(void){
#ifdef _LOGRESULTS
	g_astarmachinelog_fhd = fopen("astarresultlog.txt","a");

	fprintf(g_astarmachinelog_fhd,
		"--- Results of AStar Run :\nSearching path from %i to %i\n",m_iStart,m_iDestination);

	if(m_pDestinationNode && m_pDestinationNode->m_iWaypoint == m_iDestination){
		// valid path found
		
		fprintf(g_astarmachinelog_fhd,
			"Found a path after %li revolutions\n",m_lRevolution);
		fprintf(g_astarmachinelog_fhd,
			"Items on open list : %li\nItems on closed list : %li\n",m_PQOpen.m_ln,m_PQClosed.m_ln);
		fprintf(g_astarmachinelog_fhd,
			"heuristic / cost / ratio : %i , %i , %f\n",m_pFirstNode->m_pNext->m_h,m_pDestinationNode->m_g,float(m_pFirstNode->m_pNext->m_h)/float(m_pDestinationNode->m_g));
	}
	else{
		// no valid path found
		fprintf(g_astarmachinelog_fhd,
			"Found no path after %li revolutions\n",m_lRevolution);
		fprintf(g_astarmachinelog_fhd,
			"Items on open list : %li\nItems on closed list : %li\n",m_PQOpen.m_ln,m_PQClosed.m_ln);
		fprintf(g_astarmachinelog_fhd,
			"heuristic / cost / ratio ( last checked waypoint ): %i , %i , %f\n",m_pFirstNode->m_pNext->m_h,m_pLastNode->m_g,float(m_pFirstNode->m_pNext->m_h)/float(m_pLastNode->m_g));
	}

	fclose(g_astarmachinelog_fhd);
#endif
}

void AStarBase::getResult(CWay &Way){
	CWay tempWay;

	int iCurr = _MAX_WAYPOINTS - 1;
	AStarNode *pNode = m_pDestinationNode;

	Way.m_iCount = 0;		// so we can directly return on an error
	Way.m_iActCount = 0;

	while(pNode){
		if(iCurr == 1){
			return;
		}
		tempWay.m_piIndices[iCurr] = pNode->m_iWaypoint;
		--iCurr;

		pNode = pNode->m_pParent;
	}
	Way.m_iCount = _MAX_WAYPOINTS - iCurr - 1;
	memcpy(Way.m_piIndices,&(tempWay.m_piIndices[iCurr+1]),sizeof(int) * Way.m_iCount);
}

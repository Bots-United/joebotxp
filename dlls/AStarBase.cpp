// AStarBase.cpp: implementation of the AStarBase class.
//
//////////////////////////////////////////////////////////////////////

#include "AStarBase.h"

#include <iostream>
using namespace std;

//#define _LOGRESULTS

extern FILE *g_astarmachinelog_fhd;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMemoryPool<AStarNode,2048> g_ASN_MPool;
#ifdef __JOEBOTXPDLL
//extern edict_t *listenserver_edict;
#include "Game.h"
#endif

AStarBase::AStarBase(){
	m_pDestinationNode = 0;
	m_pFirstNode = m_pLastNode = g_ASN_MPool.get();

	m_pLastNode->m_iWaypoint = -1;
	m_pLastNode->m_pNext = 0;
	m_pLastNode->m_pParent = 0;

	m_lMaxRevolutions = 2000000000;
	m_lSliceSize = 10000000;
	m_lCSlice2 = 0;
	m_iCSlice = 0;
	m_bFinished = true;
}

AStarBase::~AStarBase(){
	resetNodes();

	g_ASN_MPool.free(m_pFirstNode);		// free dummy node
	m_pFirstNode = m_pLastNode = 0;		// reset the dummy node pointer
}

void AStarBase::setMaxRevolutions(long lMax){
	m_lMaxRevolutions = lMax;
}

void AStarBase::resetNodes(void){
	AStarNode *p,*pd;

	pd = p = m_pFirstNode->m_pNext;			// delete all nodes by m_pNext pointer
	while(p){
		p = p->m_pNext;
		deleteNode(pd);
		pd = p;
	}
	m_pFirstNode->m_pNext = 0;
	m_pLastNode = m_pFirstNode;

	m_PQClosed.reset();						// erase closed list
	m_PQOpen.reset();						// erase open list

	m_lCSlice2 = 0;							// erase 'slice to' ... no slice to calc atm
	m_bFinished = true;						// well, no job, so we are finished
}

void AStarBase::logResult(void){
#ifdef _LOGRESULTS
	bool bInit = false;
	FILE *fhdtest = fopen("astarresultlog.html","r");
	if(!fhdtest){
		bInit = true;
	}
	else{
		fclose(fhdtest);
	}
	g_astarmachinelog_fhd = fopen("astarresultlog.html","a");

	if(bInit)fprintf(g_astarmachinelog_fhd,"<html><body><table><tr><th>Start<th>Destination<th>Found<th>Revolution<th>Slices<th>open<th>closed<th>heuristic<th>cost<th>ratio");

	fprintf(g_astarmachinelog_fhd,
		"<tr><td><b>%li</b><td><b>%li</b>",getStart(),getDestination());
	fprintf(g_astarmachinelog_fhd,
			"<td>%i<td>%li<td>%i",m_pDestinationNode!=0,m_lRevolution,m_iCSlice);
	fprintf(g_astarmachinelog_fhd,
			"<td>%li<td>%li",m_PQOpen.m_ln,m_PQClosed.m_ln);

	if(m_pDestinationNode){
		// valid path found
		fprintf(g_astarmachinelog_fhd,
			"<td>%i<td>%i<td>%.2f",m_pFirstNode->m_pNext->m_h,m_pDestinationNode->m_g,float(m_pFirstNode->m_pNext->m_h)/float(m_pDestinationNode->m_g));
	}
	else{
		// no valid path found
		fprintf(g_astarmachinelog_fhd,
			"<td>%i<td>%i<td>%.2f",m_pFirstNode->m_pNext->m_h,m_pLastNode->m_g,float(m_pFirstNode->m_pNext->m_h)/float(m_pLastNode->m_g));
	}

	fclose(g_astarmachinelog_fhd);
#endif
}

void AStarBase::getResult(CWay &Way){			// copy astar results to a Way instance
	CWay tempWay;

	int iCurr = _MAX_WAYPOINTS - 1;
	AStarNode *pNode = m_pDestinationNode;

	Way.reset();		// so we can directly return on an error

	while(pNode){
		if(iCurr <= 1){
			return;
		}
		tempWay.m_piIndices[iCurr] = pNode->m_iWaypoint;
		--iCurr;

		pNode = pNode->m_pParent;
	}
	Way.m_iCount = _MAX_WAYPOINTS - 1 - iCurr;
	memcpy(Way.m_piIndices,&(tempWay.m_piIndices[iCurr+1]),sizeof(int) * Way.m_iCount);
}

void AStarBase::getPartialResult(CWay &Way){			// nyi
	getResult(Way);

	if(!Way.m_iCount){
		// try to get at least a part of that path
	}
}

void AStarBase::drawTree(void){		// doesnt work
	drawTreeR(m_pFirstNode->m_pNext->m_iWaypoint,m_pFirstNode->m_pNext);
}

void AStarBase::drawTreeR(int iWP, AStarNode *pNode){
#ifdef __JOEBOTXPDLL
	g_pGame->drawBeam(0,g_pGame->m_pListenserverEdict.getEntity(),g_Map.m_Waypoints[iWP].m_VOrigin,g_Map.m_Waypoints[pNode->m_iWaypoint].m_VOrigin,5,0,Vector(200,0,0),255,0,10);
#endif
	AStarNodeChildren *pChildren = &(pNode->m_Children);
	int iIndex;
	while(pChildren){
		for(iIndex=0; iIndex < _MAX_ASTAR_CHILDREN; iIndex ++){
			if(pChildren->m_ppChildren[iIndex])
				drawTreeR(pNode->m_iWaypoint,pChildren->m_ppChildren[iIndex]);
		}
		pChildren = pChildren->m_pNext;
	}
}
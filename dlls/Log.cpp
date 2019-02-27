// Log.cpp: implementation of the CLog class.
//
//////////////////////////////////////////////////////////////////////

#include "Log.h"

#include <stdio.h>
#include <string.h>
#include "Game.h"
#include "AStarBase.h"
#include "AStarTaskManager.h"
#include <list>
using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLog g_Log("joebot.html");

CLog::CLog(const char *szFilename)
{
	strcpy(m_szFilename,szFilename);
	m_fTime = 0;

	//FILE *fhd1 = fopen(m_szFilename,"r");
	FILE *fhd = fopen(m_szFilename,"w");
	
	if(fhd){
		fprintf(fhd,"\n\n\n<html><body><table><tr>");

		fprintf(fhd,"<th>Time<th>MPCalls<th>MPMaxAlloc<th>MPAlloc<th>Tasks<th>Tot.Rev/s");
		fclose(fhd);
	}
}

CLog::~CLog(){
}

void CLog::process(void){
	if(m_fTime < g_pGame->getTime()){
		m_fTime = g_pGame->getTime() +.1f;
		
		FILE *fhd = fopen(m_szFilename,"a");

		if(fhd){
			fprintf(fhd,"<tr><td>%.0f<td>%li<td>%li<td>%li<td>%li<td>%i",g_pGame->getTime()*10,g_ASN_MPool.m_lCAlloc,g_ASN_MPool.m_lCMax,g_ASN_MPool.m_lCAlloc-g_ASN_MPool.m_lCFree,g_AStarTasks.getCurrentTotalTasks(),g_AStarTasks.getSlicesCPFrame());
			fclose(fhd);
		}
	}
}
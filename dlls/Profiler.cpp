// Profiler.cpp: implementation of the CProfiler class.
//
//////////////////////////////////////////////////////////////////////

#include "Profiler.h"

#include <iostream>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "HPTime.h"

CProfiler g_Profiler;		// global profiler instance

void removeBadChars(char *p){
	while(*p){
		if(*p == ':'
			||*p == ';')
			*p = '_';
		p++;
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction of CProfileSample
//////////////////////////////////////////////////////////////////////

CProfileNode::CProfileNode(){
	m_i64Accumulator = 0;
	m_i64StartTime = 0;
	m_iOpenProfiles = 0;
	m_iProfileInstances = 0;

	m_pParent = 0;
	m_bOpened = false;

	*m_szName = 0;
	m_lNameCheckSum = 0;
}

CProfileNode::~CProfileNode(){
}

CProfileNode * CProfileNode::hasChild(const char *szName){
	vector<CProfileNode *>::iterator iter;

	for(iter = m_ppChildren.begin(); iter != m_ppChildren.end(); iter ++){
		if(!strcmp((*iter)->m_szName,szName)){
			return *iter;
		}
	}
	return 0;
}

void CProfileNode::begin(void){
	++m_iOpenProfiles;							// increment for each call
	++m_iProfileInstances;						// how many times has this been called ?!

	m_i64StartTime = g_HPTime.getTime();		// set start time
}

void CProfileNode::end(void){
	_hptime64 diff = g_HPTime.getTime() - m_i64StartTime;

	m_i64Accumulator += diff;
	if(m_i64MaxTime < diff)		// store minimum and maximum
		m_i64MaxTime = diff;
	if(m_i64MinTime > diff)
		m_i64MinTime = diff;

	--m_iOpenProfiles;			// decrease counter on leaving
}

void CProfileNode::output(char *szBuffer,int iLevel,bool bHTML){
	vector<CProfileNode *>::iterator iter_child;

	if(m_bOpened || !m_ppChildren.size()){
		strcat(szBuffer,"- ");
	}
	else{
		strcat(szBuffer,"+ ");
	}

	char szTest[100]="\0";
	sprintf(szTest,"%s %i %.0f%% (%.1f,%.1f)",m_szName,m_Stats.m_iCalls,m_Stats.m_fPercentage,m_Stats.m_fMin,m_Stats.m_fMax);
	strcat(szBuffer,szTest);
	if(g_Profiler.getCursor() == this){
		strcat(szBuffer,"  ------");
	}
	strcat(szBuffer,"\n");

	if(m_bOpened){
		if(m_ppChildren.size())strcat(szBuffer,"\n");
		for(iter_child=m_ppChildren.begin(); iter_child != m_ppChildren.end(); iter_child ++){
			(*iter_child)->output(szBuffer,iLevel + 1,bHTML);
		}
	}
}

void CProfileNode::calcStats(void){
	m_Stats.m_iCalls = m_iProfileInstances;
	m_Stats.m_i64TotalTime = m_i64Accumulator;
	if(m_pParent)
		m_Stats.m_fPercentage = (float)m_i64Accumulator / (float)m_pParent->m_i64Accumulator*100.f;
	else
		m_Stats.m_fPercentage = 100.f;

	_hptime64 av;

	if(m_iProfileInstances == 0)
		av = m_i64Accumulator;
	else
		av = m_i64Accumulator / (_hptime64)m_iProfileInstances;

	if(m_i64MinTime ==0)
		m_Stats.m_fMin = 1;
	else
		m_Stats.m_fMin = float(av) / float(m_i64MinTime);
	if(av==0)
		m_Stats.m_fMax = 1;
	else
		m_Stats.m_fMax = float(m_i64MaxTime) / float(av);
}

void CProfileNode::resetLogData(void){
	m_iProfileInstances = 0;
	m_i64Accumulator = 0;

	m_i64MaxTime = 0;
	m_i64MinTime = ((_hptime64)1) << 60;
}

void CProfileNode::setName(const char *szName){
	strcpy(m_szName,szName);

	int i;
	m_lNameCheckSum = 0;
	for(i=0;m_szName[i];i++){
		m_lNameCheckSum += m_szName[i];
	}
}

long CProfileNode::getCheckSumRek(void){
	if(m_pParent)
		return m_lNameCheckSum + m_pParent->getCheckSumRek();
	else
		return m_lNameCheckSum;
}

void CProfileNode ::outputHTMLfile(const char *szFilename){
	char szOFilename[80];
	FILE *fhd;
	long lCheckSumTree = getCheckSumRek();

	sprintf(szOFilename,"profiler/%s%s-%p.html",szFilename,m_szName,(void*)lCheckSumTree);
	removeBadChars(szOFilename);
	
	fhd = fopen(szOFilename,"r");
	if(fhd){
		fclose(fhd);
		fhd = fopen(szOFilename,"a");
	}
	else{
		fhd = fopen(szOFilename,"w");
		if(fhd)
			fprintf(fhd,"<html><body><table border=1><tr>\
					<th>Time\
					<th>%%\
					<th>Calls/s\
					<th>Av. Time\
					<th>Min. Time\
					<th>Max. Time\
					 ");
	}
	if(fhd){
		_hptime64 av;

		if(m_iProfileInstances == 0)
			av = m_i64Accumulator;
		else
			av = m_i64Accumulator /	(_hptime64)m_iProfileInstances;

		fprintf(fhd,"<tr><td>%lf<td>%.2f<td>%li<td>%.0lf<td>%.0lf<td>%.0lf",g_HPTime.getTime_d(),m_Stats.m_fPercentage,m_iProfileInstances,double(av),double(m_i64MinTime),double(m_i64MaxTime));
		fclose(fhd);
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction of CProfileItem
//////////////////////////////////////////////////////////////////////

CProfileItem::CProfileItem(char *szName){
	//m_szName = szName;
	strcpy(m_szName,szName);
	
	g_Profiler.begin(m_szName);
}

CProfileItem::~CProfileItem(){
	g_Profiler.end(m_szName);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction of CProfiler
//////////////////////////////////////////////////////////////////////

CProfiler::CProfiler(){
	m_bEnableFileLog = false;
	m_i64LastStat = 0;
	startProfile();

	// create the root node
	vector<CProfileNode>::iterator iter;
	m_pNodes.push_back(m_pCurrNode = m_pRoot = m_pCursor = new CProfileNode);
	strcpy(m_pRoot->m_szName,"/");
}

CProfiler::~CProfiler(){
	end("/");

	while(!m_pNodes.size()){
		delete m_pNodes.back();
		m_pNodes.pop_back();
	}

	m_pCursor = 0;
}

//////////////////////////////////////////////////////////////////////
// Implementation of CProfiler
//////////////////////////////////////////////////////////////////////

void CProfiler::frame(void){
	if(g_HPTime.getTime()-m_i64LastStat > g_HPTime.getFrequency()){
		// calc stats
		vector<CProfileNode *>::iterator iter;

		for(iter = m_pNodes.begin(); iter != m_pNodes.end(); iter ++){
			(*iter)->calcStats();
		}
		if(m_bEnableFileLog)
			outputHTMLfile("profiler");

		for(iter = m_pNodes.begin(); iter != m_pNodes.end(); iter ++){
			(*iter)->resetLogData();
		}

		// store time of last check
		m_i64LastStat = g_HPTime.getTime();
	}
}

bool CProfiler::begin(const char *szName){
	/*if(!strlen(szName))
		*((long*)0)=0;*/
	if(strcmp(m_pCurrNode->m_szName,szName)){	// assuming we use static strings, we can just check the pointers
		CProfileNode *pNode;
		if(pNode = m_pCurrNode->hasChild(szName)){
			// if this name is already a child to the current profiler function
		}
		else{
			vector<CProfileNode>::iterator iter;
			// so we need a new node here
			m_pNodes.push_back(pNode = new CProfileNode);

			m_pCurrNode->m_ppChildren.push_back(pNode);		// add this node as a child
			pNode->m_pParent = m_pCurrNode;					// and set the current node as parent
		}
		if(!pNode->m_szName[0]){							// no name there yet ?
			pNode->setName(szName);
		}

		m_pCurrNode = pNode;	// let's move down the tree ...
		pNode->begin();
	}
	else{
		//it's the same name .... looks like recursion here
		m_pCurrNode->end();
		m_pCurrNode->begin();
	}

	return true;
}

bool CProfiler::end(const char *szName){
	assert ( !strcmp(szName,m_pCurrNode->m_szName) );

	m_pCurrNode->end();

	if(m_pCurrNode->m_pParent)// move up the tree - if there is still some node
		m_pCurrNode = m_pCurrNode->m_pParent;

	return false;
}

void CProfiler::profileDumpOutputToBuffer( char *szBuffer, CProfileNode *pNode ){
	*szBuffer = 0;
	char szPath[200]="\0",szProp[40];
	float fPathProp;
	if(!pNode)
		if(m_pCursor->m_pParent)
			pNode = m_pCursor->m_pParent;
		else
			pNode = m_pCursor;

	fPathProp = createPath(pNode,szPath);
	sprintf(szProp," (%3.2f%%)",fPathProp*100.f);
	strcat(szBuffer,szPath);
	strcat(szBuffer,szProp);
	strcat(szBuffer,"\n\n\n");
	pNode->output(szBuffer,0,false);

	m_i64startProfile = g_HPTime.getTime();
}

float CProfiler::createPath(CProfileNode *p, char *szText){
	float fProp = p->m_Stats.m_fPercentage/100.f;
	if(fProp > 1.f||fProp<0.f)fProp=1;
	if(p->m_pParent)
		fProp *= createPath(p->m_pParent,szText);
	else
		fProp = 1;
	strcat(szText,"\\");
	strcat(szText,p->m_szName);

	return fProp;
}

void CProfiler::cursor_movein(void){
	m_pCursor->m_bOpened = true;
	if(m_pCursor->m_ppChildren.size()){
		m_pCursor = m_pCursor->m_ppChildren[0];
	}
}

void CProfiler::cursor_moveout(void){
	if(m_pCursor->m_pParent){
		m_pCursor->m_pParent->m_bOpened = false;
		m_pCursor = m_pCursor->m_pParent;
	}
}

void CProfiler::cursor_movedown(void){
	if(m_pCursor->m_pParent){
		unsigned int i;

		for(i=0; i < m_pCursor->m_pParent->m_ppChildren.size(); i++){
			if(m_pCursor->m_pParent->m_ppChildren[i] == m_pCursor){
				break;
			}
		}
		if(i < m_pCursor->m_pParent->m_ppChildren.size()-1){
			i ++;
		}
		m_pCursor = m_pCursor->m_pParent->m_ppChildren[i];
	}
}

void CProfiler::cursor_moveup(void){
	if(m_pCursor->m_pParent){
		unsigned int i;

		for(i=0; i < m_pCursor->m_pParent->m_ppChildren.size(); i++){
			if(m_pCursor->m_pParent->m_ppChildren[i] == m_pCursor){
				break;
			}
		}
		if(i){
			i --;
		}
		m_pCursor = m_pCursor->m_pParent->m_ppChildren[i];
	}
}

CProfileNode *CProfiler::getCursor(void){
	return m_pCursor;
}

void CProfiler::startProfile(void){
	m_i64startProfile = g_HPTime.getTime();
}

void CProfiler::outputHTMLfile(const char *szFilename){
	char szOFilename[80];		// filename of overview
	CProfileNode *pNode;
	FILE *fhd=0;

	strcpy(szOFilename,szFilename);
	strcat(szOFilename,".html");

	fhd = fopen(szOFilename,"w");

	pNode = m_pRoot;

	if(fhd)
		fprintf(fhd,"<html><body><table border=1><th>CPU @ %.0lfMhz</table><br>",double(g_HPTime.getFrequency()/(_hptime64)1000000));
	outputHTMLfileRek(szFilename,pNode,fhd);

	fclose(fhd);
}

void CProfiler::outputHTMLfileRek(const char *szFilename,CProfileNode *pNode,FILE *fhd){
	if(fhd){
		char szLinkFilename[80];

		sprintf(szLinkFilename,"%s%s-%p",szFilename,pNode->m_szName,(void*)pNode->getCheckSumRek());
		removeBadChars(szLinkFilename);

		if(pNode->m_pParent){
			fprintf(fhd,"<li><a href=\"profiler/%s.html\">%s</a> (%.1f%% min %.2f, max %.2f)",szLinkFilename,pNode->m_szName,pNode->m_Stats.m_fPercentage,pNode->m_Stats.m_fMin,pNode->m_Stats.m_fMax);
		}
	}
	pNode->outputHTMLfile(szFilename);
	if(pNode->m_ppChildren.size()){
		if(fhd)
			fprintf(fhd,"<ul>");
		unsigned int ui;
		for(ui=0; ui < pNode->m_ppChildren.size(); ui ++){
			outputHTMLfileRek(szFilename,pNode->m_ppChildren[ui],fhd);
		}
		if(fhd)
			fprintf(fhd,"</ul>");
	}
}
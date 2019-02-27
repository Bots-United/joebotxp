// CWPStat.cpp: implementation of the CWPStat class.
//
//////////////////////////////////////////////////////////////////////

#include <math.h>
#include "WPStat.h"
#include "Map.h"
#include "Game.h"

#include <iostream>
using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWPStatItem::CWPStatItem(){
	resetStat();
}

CWPStatItem::~CWPStatItem(){
}

void CWPStatItem::resetStat(void){
	int i;

	for(i=0;i < 8;i++){
		m_plDDamage[i] = 0;
		m_plDDamaged[i] = 0;
	}
	m_lTotalDamage = 0;

	m_lTotalDamaged = 0;

	for(i=0;i < _CWPSTATITEM_KILLTIMESTORAGE;i++){
		m_pfKillTime[i] = -1;
	}
	m_iNKill = 0;

	m_lTraffic = 0;
}

void CWPStatItem::addKill(void){
#ifdef __JOEBOTXPDLL
	m_pfKillTime[m_iNKill] = g_pGame->getTime();
#endif
	m_iNKill ++;
	if(m_iNKill >= _CWPSTATITEM_KILLTIMESTORAGE){
		m_iNKill = 0;
	}
}

void CWPStatItem::addDamage(Vector2D &VDir, int iDamage){
	int index = (getAngle(VDir) + 22.5)/45;
	if(index >= 8)
		index = 0;

	m_plDDamage[index]+=iDamage;
	m_lTotalDamage+=iDamage;
}

void CWPStatItem::addDamaged(Vector2D &VDir, int iDamage){
	int index = (getAngle(VDir) + 22.5)/45;
	if(index >= 8)
		index = 0;

	m_plDDamaged[index]+=iDamage;
	m_lTotalDamaged+=iDamage;
}

void CWPStatItem::save(FILE *fhd){
	fwrite(m_plDDamage,sizeof(long),8,fhd);
	fwrite(&m_lTotalDamage,sizeof(long),1,fhd);

	fwrite(m_plDDamaged,sizeof(long),8,fhd);
	fwrite(&m_lTotalDamaged,sizeof(long),1,fhd);

	fwrite(m_pfKillTime,sizeof(float),_CWPSTATITEM_KILLTIMESTORAGE,fhd);
	fwrite(&m_lTraffic,sizeof(long),1,fhd);
}

void CWPStatItem::load(FILE *fhd){
	fread(m_plDDamage,sizeof(long),8,fhd);
	fread(&m_lTotalDamage,sizeof(long),1,fhd);

	fread(m_plDDamaged,sizeof(long),8,fhd);
	fread(&m_lTotalDamaged,sizeof(long),1,fhd);

	fread(m_pfKillTime,sizeof(float),_CWPSTATITEM_KILLTIMESTORAGE,fhd);
	fread(&m_lTraffic,sizeof(long),1,fhd);
}

float CWPStatItem::getAngle(const Vector2D &V){
	float fR=0;
	if(V.x > 0){
		fR = tanh(V.y/V.x)/3.14159265358979323*180.;
	}
	else if(V.x < 0){
		fR = 180-(tanh(-V.y/V.x)/3.14159265358979323*180.);
	}
	else {
		if(V.y < 0){
			fR = 270;
		}
		else{
			fR = 90;
		}
	}
	return fR;
}
/*
CWPStat::CWPStat()
{
	reset();
}

CWPStat::~CWPStat()
{

}

bool CWPStat::save(const char *szFileName){
	FILE *fhd;
	WPHeader_t header;

	fhd = fopen(szFileName,"wb");

	if(!fhd)
		return false;

	int i, iNum2Save = g_Map.m_Waypoints.m_iNumWaypoints; //g_Map.Waypoints.m_iNumWaypoints

	memset(header.szMapname,0,sizeof(char)*32);
	memset(header.szText,0,sizeof(char)*32);
#ifdef __JOEBOTXPDLL
	strcpy(header.szMapname,g_pGame->getMapname());
#endif
	strcpy(header.szText,"JoeBOT XP StatData File\r\n\r\n");
	header.lVersion = _WPSTAT_VERSION;

	fwrite(&header,sizeof(header),1,fhd);
	fwrite(&m_lMaxTotalDamage,1,sizeof(long),fhd);
	fwrite(&iNum2Save,sizeof(int),1,fhd);
	for(i = 0; i < iNum2Save; i++){
		m_pSData[i].save(fhd);
	}
	fclose(fhd);

	return true;
}

bool CWPStat::load(const char *szFileName){
	FILE *fhd;
	WPHeader_t header;

	fhd = fopen(szFileName,"rb");

	if(!fhd){
		return false;
	}

	int i, iNum2Save;

	fread(&header,sizeof(header),1,fhd);
	if(header.lVersion != _WPSTAT_VERSION){
		fclose(fhd);
		
		return false;
	}
	fread(&m_lMaxTotalDamage,1,sizeof(long),fhd);
	fread(&iNum2Save,sizeof(int),1,fhd);
	for(i = 0; i < iNum2Save; i++){
		m_pSData[i].load(fhd);
	}
	
	fclose(fhd);

	return true;
}

void CWPStat::addDamage(Vector &VDamaged,Vector &VDamageOrigin, int iDamage){
	int iDamagedWP = g_Map.m_Waypoints.getNearest(VDamaged,false,false,150);		// who was damaged
	if(iDamagedWP != -1){
		g_Map.m_WPStat.m_pSData[iDamagedWP].addDamaged((VDamageOrigin - VDamaged).Make2D(),iDamage);
		if(g_Map.m_WPStat.m_pSData[iDamagedWP].m_lTotalDamage > m_lMaxTotalDamage){
			m_lMaxTotalDamage = g_Map.m_WPStat.m_pSData[iDamagedWP].m_lTotalDamage;
		}
	}

	int iDamageWP = g_Map.m_Waypoints.getNearest(VDamageOrigin,false,false,150);	// who does the damage
	if(iDamageWP != -1){
		g_Map.m_WPStat.m_pSData[iDamageWP].addDamage((VDamaged - VDamageOrigin).Make2D(),iDamage);
		if(g_Map.m_WPStat.m_pSData[iDamageWP].m_lTotalDamage > m_lMaxTotalDamage){
			m_lMaxTotalDamage = g_Map.m_WPStat.m_pSData[iDamageWP].m_lTotalDamage;
		}
	}
}

void CWPStat::addKill(Vector &VOrigin){
	int iWP = g_Map.m_Waypoints.getNearest(VOrigin,false,false,150);
	if(iWP != -1){
		g_Map.m_WPStat.m_pSData[iWP].addKill();
	}
}

void CWPStat::addTraffic(int iWP){
	m_pSData[iWP].m_lTraffic ++;
}

void CWPStat::update(int iType,int iAdd){
	if(iType == UD_WAYPOINT_CHANGED){
		// copy data from the nearest waypoint
		int iN = g_Map.m_Waypoints.getNearest(g_Map.m_Waypoints.m_Waypoints[iAdd].m_VOrigin,false,false,10);
		if(iN != -1){
			m_pSData[iAdd] = m_pSData[iN];
		}
		else{
			m_pSData[iAdd].reset();
		}
	}
}

void CWPStat::reset(void){
	m_lMaxTotalDamage = 0;
	int i;
	for(i = 0; i < _MAX_WAYPOINTS; i++){
		m_pSData[i].reset();
	}
}

CWPStatItem &CWPStat::operator[](long l){
	return ((CWPStatItem &)(m_pSData[l]));
}*/
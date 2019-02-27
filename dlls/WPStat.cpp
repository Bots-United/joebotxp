// CWPStat.cpp: implementation of the CWPStat class.
//
//////////////////////////////////////////////////////////////////////

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
	}
	m_lTotalDamage = 0;

	for(i=0;i < 8;i++){
		m_plDDamaged[i] = 0;
	}
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

	m_plDDamage[index&7]+=iDamage;
	m_lTotalDamage+=iDamage;
}

void CWPStatItem::addDamaged(Vector2D &VDir, int iDamage){
	int index = (getAngle(VDir) + 22.5)/45;
	
	m_plDDamaged[index&7]+=iDamage;
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

void CWPStatItem::saveXML(FILE *fhd){
	int i;
	fprintf(fhd,"<statdata>\n");
	fprintf(fhd,"<damage>\n");
	fprintf(fhd,"<total>%li</total>\n",m_lTotalDamage);
	for(i=0; i < 8; i++){
		fprintf(fhd,"<from dir=\"%i\">%li</from>\n",i,m_plDDamage[i]);
	}
	fprintf(fhd,"</damage>\n");
	fprintf(fhd,"<damaged>\n");
	fprintf(fhd,"<total>%li</total>\n",m_lTotalDamaged);
	for(i=0; i < 8; i++){
		fprintf(fhd,"<from dir=\"%i\">%li</from>\n",i,m_plDDamaged[i]);
	}
	fprintf(fhd,"</damaged>\n");
	fprintf(fhd,"<killtime>\n");
	for(i=0; i < _CWPSTATITEM_KILLTIMESTORAGE; i++){
		fprintf(fhd,"%.1f ",m_pfKillTime[i]);
	}
	fprintf(fhd,"</killtime>\n");
	fprintf(fhd,"<traffic>%li</traffic>\n",m_lTraffic);
	fprintf(fhd,"</statdata>\n");
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

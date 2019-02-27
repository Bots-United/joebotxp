// JoeBot XP
// by Johannes Lampel
// Johannes.Lampel@gmx.de
// http://joebot.counter-strike.de

#include "extdll.h"
#include <string.h>
#include <time.h>
#include "Commandfunc.h"
#include "util.h"

#include "Map.h"
#include "BaseBot.h"
#include "Player.h"
#include "Game.h"
#include "AStar.h"
extern char szTemp[200];

void Endround(void);
extern int debug_engine;
extern bool g_bShowProfile;
extern bool g_bShowWPStat;
void getInfo(char *szDebugText,edict_t *pInfo);

/*

  example :
  
	bool bc_whatever(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	}
	
*/

bool bc_waypoint(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	static int iWP1=-1,iWP2=-1;
	if (!strcmp(arg1, "on")||!strcmp(arg1, "show")){
		g_Map.m_Waypoints.m_bDisplay = true;
		g_pGame->userMsg(pEntity,"Waypointsys is shown\n");
		return true;
	}
	else if (!strcmp(arg1, "off")||!strcmp(arg1, "hide")){
		g_Map.m_Waypoints.m_bDisplay = false;
		g_pGame->userMsg(pEntity,"Waypointsys isn't shown\n");
		return true;
	}
	else if (!strcmp(arg1, "showpaths")||!strcmp(arg1, "showpath")){
		g_Map.m_Waypoints.m_bDisplayPaths = true;
		g_pGame->userMsg(pEntity,"Paths are shown\n");
		return true;
	}
	else if (!strcmp(arg1, "hidepaths")||!strcmp(arg1, "hidepath")){
		g_Map.m_Waypoints.m_bDisplayPaths = false;
		g_pGame->userMsg(pEntity,"Paths aren't shown\n");
		return true;
	}
	else if (!strcmp(arg1, "showwaypoints")||!strcmp(arg1, "showwaypoint")){
		g_Map.m_Waypoints.m_bDisplayWaypoints = true;
		g_pGame->userMsg(pEntity,"Waypoints are shown\n");
		return true;
	}
	else if (!strcmp(arg1, "hidewaypoints")||!strcmp(arg1, "hidewaypoints")){
		g_Map.m_Waypoints.m_bDisplayWaypoints = false;
		g_pGame->userMsg(pEntity,"Waypoints aren't shown\n");
		return true;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////
	else if (!strcmp(arg1, "add")){
		g_Map.m_Waypoints.add(pEntity->v.origin);
		return true;
	}
	else if (!strcmp(arg1, "addbasic")){
		g_Map.createBasicWaypoints();
		return true;
	}
	if (!strcmp(arg1, "remove")){
		g_Map.m_Waypoints.remove(g_Map.m_Waypoints.getNearest(pEntity->v.origin));
		return true;
	}
	else if (!strcmp(arg1, "connect1")){
		iWP1 = g_Map.m_Waypoints.getNearest(pEntity->v.origin);
		return true;
	}
	else if (!strcmp(arg1, "connect2")){
		iWP2 = g_Map.m_Waypoints.getNearest(pEntity->v.origin);
		
		g_Map.m_Waypoints.connect(iWP1,iWP2);
		return true;
	}
	else if (!strcmp(arg1, "save")){
		g_Map.save();
		return true;
	}
	else if (!strcmp(arg1, "savexml")){
		char szXMLFile[120];
		sprintf(szXMLFile,"joebot/%s.xml",g_pGame->getMapname());
		g_Map.m_Waypoints.saveXML(szXMLFile);
		return true;
	}
	else if (!strcmp(arg1, "load")){
		g_Map.load();
		return true;
	}
	else if (!strcmp(arg1, "resetstat")){
		g_Map.m_Waypoints.resetStat();
		return true;
	}
	else if (!strcmp(arg1, "info")){
		sprintf(szTemp,"%i/%i",g_Map.m_Waypoints.getNearest(pEntity->v.origin),g_Map.m_Waypoints.m_iNumWaypoints);
		g_pGame->userMsg(pEntity,szTemp);
		return true;
	}
	else{
		sprintf(szTemp,"parameter unknown : \"%s\"\n",arg1);
		g_pGame->userMsg(pEntity,szTemp);
	}
	
	return true;
}

bool bc_search(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	//edict_t *pent = 0;
	CEntity pent;
	char item_name[1000],szBuffer[1000];
	
	sprintf(item_name,"\0");
	sprintf(szBuffer,"------- searching in %s -------------------\n\n\0",g_pGame->getMapname());
	strcat(item_name,szBuffer);
	
	FILE *fhd;
	fhd=fopen("logent  -  search.txt","a");
	fprintf(fhd,"%s\n",item_name);
	fclose(fhd);
	g_pGame->userMsg(pEntity, item_name);
	while ((pent = UTIL_FindEntityInSphere( pent, pEntity->v.origin,100)) != NULL)
	{
		g_pGame->drawBeam(0,pEntity,pEntity->v.origin,pent.getBModelOrigin(),10,10,Vector(255,255,255),255,20);
		
		//float fDistance = (pEntity->v.origin-pent->v.origin).length();
		sprintf(item_name,"\0");
		
		sprintf(szBuffer,"%s\t\0",STRING(pent.getEntity()->v.classname));
		strcat(item_name,szBuffer);
		sprintf(szBuffer,"%s\t\0",STRING(pent.getEntity()->v.globalname));
		strcat(item_name,szBuffer);
		sprintf(szBuffer,"%s\t\0",STRING(pent.getEntity()->v.netname));
		strcat(item_name,szBuffer);
		sprintf(szBuffer,"%s\t\0",STRING(pent.getEntity()->v.target));
		strcat(item_name,szBuffer);
		sprintf(szBuffer,"%s\t\0",STRING(pent.getEntity()->v.targetname));
		strcat(item_name,szBuffer);
		sprintf(szBuffer,"%s\t\0",STRING(pent.getEntity()->v.model));
		strcat(item_name,szBuffer);
		sprintf(szBuffer,"%i\t\0",pent.getEntity()->v.skin);
		strcat(item_name,szBuffer);
		sprintf(szBuffer,"%i\t\0",pent.getEntity()->v.body);
		strcat(item_name,szBuffer);
		sprintf(szBuffer,"%i\t\0",pent.getEntity()->v.effects);
		strcat(item_name,szBuffer);
		sprintf(szBuffer,"%f\t\0",pent.getEntity()->v.velocity.length());
		strcat(item_name,szBuffer);
		
		if(pent.getEntity() ->v.owner){
			sprintf(szBuffer," owner : %s\t\0",STRING(pent.getEntity()->v.owner->v.netname));
			strcat(item_name,szBuffer);
		}
		
		FILE *fhd;
		fhd=fopen("logent  -  search.txt","a");
		fprintf(fhd,"%s\n",item_name);
		fclose(fhd);
		strcat(item_name,"\n");
		g_pGame->userMsg(pEntity, item_name);
	};
	return true;
}

bool bc_addbot(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	g_pGame->createBot(arg1,arg2,arg3,arg4);
	return true;
}

bool bc_testpath(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	int iNear = g_Map.m_Waypoints.getNearest(pEntity->v.origin);
	int iTo = atoi(arg1);
	AStarMachine<AStarBaseGoal> t;
	t.setDestination(iTo);
	t.setStart(iNear);
	if(t.runAStar()){
		g_pGame->drawBeam(0,pEntity,g_Map.m_Waypoints[iNear].m_VOrigin,g_Map.m_Waypoints[iTo].m_VOrigin,10,0,Vector(255,0,0),255,1);
		
		int iLast = -1,i;
		//*((int*)0) = 0;
		AStarNode *p = t.m_pDestinationNode;
		while(p){
			i = p->m_iWaypoint;
			
			if(i!= -1 && iLast != -1){
				g_pGame->drawBeam(0,pEntity,g_Map.m_Waypoints[i].m_VOrigin+Vector(0,0,50),g_Map.m_Waypoints[iLast].m_VOrigin+Vector(0,0,50),10,0,Vector(0,255,0),255,1);
			}
			FILE *fhd;
			fhd = fopen("astarlog.txt","a");
			fprintf(fhd,"%3i,",i);
			fclose(fhd);
			iLast = i;
			p = p->m_pParent;
		}
	}
	return true;
}

bool bc_pupd(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	for(int i=0;i < gpGlobals->maxClients;i ++){
		edict_t *p = INDEXENT(i);
		
		if(p && !(p->free)){
			if(g_pGame->isPlayer(p) == -1){
				g_pGame->addPlayer(p);
			}
		}
	}
	g_pGame->userMsg(pEntity, "naknak!\n");
	
	return true;
}

bool bc_distance(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if(!pEntity)
		return false;
	
	Vector VStart = pEntity->v.origin + pEntity->v.view_ofs;
	//UTIL_MakeVectors(pEntity->v.v_angle);
	Vector VDir = pEntity->v.v_angle.angles2vec();
	Vector VEnd = VStart + VDir * 5000;
	TraceResult tr;
	g_pGame->traceLine(VStart,VEnd,dont_ignore_monsters,dont_ignore_glass,pEntity,&tr);
	
	g_pGame->drawBeam(0,pEntity,VStart,tr.vecEndPos,5,0,Vector(255,255,255),255,0,255);
	
	char szTemp[200];
	sprintf(szTemp,"distance : %f\n",tr.flFraction *5000.f);
	g_pGame->userMsg(pEntity, szTemp);
	
	return true;
}

bool bc_debug(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if(!strcmp(arg1,"engine")){
		debug_engine = 1;
	
		g_pGame->userMsg(pEntity, "debug_engine enabled!\n");

		return true;
	}
	else if(!strcmp(arg1,"beam")){
		int i = 0;

		if(!strcmp(arg2,"all")){
			g_pGame->m_lBeamDebug = 0xffffffff;
			g_pGame->userMsg(pEntity,"debug beam : everything is displayed\n");

			return true;
		}
		else if(!strcmp(arg2,"none")){
			g_pGame->m_lBeamDebug = 0;
			g_pGame->userMsg(pEntity,"debug beam : nothing is displayed\n");

			return true;
		}

		while(strlen(g_szDebugIdent[i])){
			if(!strcmp(arg2,g_szDebugIdent[i])){
				g_pGame->userMsg(pEntity,"debug beam : toggled %s\n",arg2);

				g_pGame->m_lBeamDebug ^= 1<<i;
				if(g_pGame->m_lBeamDebug & 1<<i){
					g_pGame->userMsg(pEntity,"debug beam : %s is enabled\n",arg2);
				}
				else{
					g_pGame->userMsg(pEntity,"debug beam : %s is disabled\n",arg2);
				}
				return true;
			}
			i++;
		}
		g_pGame->userMsg(pEntity,"debug beam : \"%s\" is no known flag\ndebug beam : Valid flags are \"all\" \"none\" ",arg2);
		i=0;
		while(strlen(g_szDebugIdent[i])){
			g_pGame->userMsg(pEntity,"\"%s\" ",g_szDebugIdent[i]);
			i++;
		}
		g_pGame->userMsg(pEntity,"\n");
		return false;
	}
	else if(!strcmp(arg1,"msg")){
		int i = 0;

		if(!strcmp(arg2,"all")){
			g_pGame->m_lMsgDebug = 0xffffffff;
			g_pGame->userMsg(pEntity,"debug msg : everything is displayed\n");

			return true;
		}
		else if(!strcmp(arg2,"none")){
			g_pGame->m_lMsgDebug = 0;
			g_pGame->userMsg(pEntity,"debug msg : nothing is displayed\n");

			return true;
		}

		while(strlen(g_szDebugIdent[i])){
			if(!strcmp(arg2,g_szDebugIdent[i])){
				g_pGame->userMsg(pEntity,"debug msg : toggled %s\n",arg2);

				g_pGame->m_lMsgDebug ^= 1<<i;
				if(g_pGame->m_lMsgDebug & 1<<i){
					g_pGame->userMsg(pEntity,"debug msg : %s is enabled\n",arg2);
				}
				else{
					g_pGame->userMsg(pEntity,"debug msg : %s is disabled\n",arg2);
				}
				return true;
			}
			i++;
		}
		g_pGame->userMsg(pEntity,"debug msg : \"%s\" is no known flag\ndebug msg : Valid flags are \"all\" \"none\" ",arg2);
		i=0;
		while(strlen(g_szDebugIdent[i])){
			g_pGame->userMsg(pEntity,"\"%s\" ",g_szDebugIdent[i]);
			i++;
		}
		g_pGame->userMsg(pEntity,"\n");
		return false;
	}
	else{
		sprintf(szTemp,"parameter unknown : \"%s\"\n",arg1);
		g_pGame->userMsg(pEntity,szTemp);
		g_pGame->userMsg(pEntity,"Usage : debug [beam/msg/engine]\n");
	}
	
	return true;
}

bool bc_endround(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	Endround();
	return true;
}

bool bc_sit(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	CBaseBot *pBot;
	int i;
	char szOutput[100000]="\0",szTemp[300];
	time_t t;
	tm *t1;

	sprintf(szTemp,"time: %.0f\n",g_pGame->getTime());
	strcat(szOutput,szTemp);
	sprintf(szTemp,"Players : %i\n",g_pGame->getMaxCurrentPlayer());
	strcat(szOutput,szTemp);
	sprintf(szTemp,"ASMP : %li %3li %li/%li\n",g_ASN_MPool.m_lCAlloc,g_ASN_MPool.m_lCAlloc-g_ASN_MPool.m_lCFree,g_ASN_MPool.m_lNextFree,g_ASN_MPool.m_lCMax);
	strcat(szOutput,szTemp);
	sprintf(szTemp,"ASTM : #%li/%li\n",g_AStarTasks.getCurrentTotalTasks(),g_AStarTasks.getTotalTasks());
	strcat(szOutput,szTemp);

	strcat(szOutput,"\n\n\n\n");
	
	for(i=g_pGame->m_iMaxCurrentPlayer; i>=0; i--){
		if(pBot = g_pGame->getBot(i)){
			getInfo(szOutput,pBot->getEntity());
			strcat(szOutput,"\n\n\n\n");
		}
	}
	
	FILE *fhd = fopen("sitlog.txt","a");
	t = time(NULL);
	t1 = localtime(&t);
	fprintf(fhd,"\n\n------------------------------------------------------ %s\n\n", asctime(t1));
	fprintf(fhd,szOutput);
	fclose(fhd);
	
	return true;
}

bool bc_toggleprofile(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if(*arg1){
		if(!strcmp(arg1,"cdown")){
			g_Profiler.cursor_movedown();
		}
		else if(!strcmp(arg1,"cup")){
			g_Profiler.cursor_moveup();
		}
		else if(!strcmp(arg1,"cin")){
			g_Profiler.cursor_movein();
		}
		else if(!strcmp(arg1,"cout")){
			g_Profiler.cursor_moveout();
		}
		else if(!strcmp(arg1,"file")){
			if(!strcmp(arg2,"enable")||!strcmp(arg2,"on")){
				g_Profiler.enableFileOutput();
			}
			else if(!strcmp(arg2,"disable")||!strcmp(arg2,"off")){
				g_Profiler.disableFileOutput();
			}
			if(g_Profiler.isFileOutput()){
				g_pGame->userMsg(pEntity, "Logging to file ENABLED!\n");
			}
			else{
				g_pGame->userMsg(pEntity, "Logging to file DISABLED!\n");
			}
		}
		else{
			sprintf(szTemp,"parameter unknown : \"%s\"\n",arg1);
			g_pGame->userMsg(pEntity,szTemp);
			g_pGame->userMsg(pEntity, "Usage : toggleprofile [cdown/cup/cin/cout/file]\n");
		}
	}
	else{
		g_bShowProfile=!g_bShowProfile;
		g_pGame->userMsg(pEntity, "toggled profiling!\n");
	}
	return true;
}

bool bc_dumpstat(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	FILE *fhd = fopen("stats.txt","w");
	fprintf(fhd,"num\tx\ty\tz\tAvDistVWP\tiVWP\tTotalDamage\tTotalDamaged\tTraffic\n\n");

	int i;
	for(i=0; i < g_Map.m_Waypoints.m_iNumWaypoints; i++){
		if(g_Map.m_Waypoints[i].m_lFlags & CWaypoint::DELETED)
			continue;
		fprintf(fhd,"%i\t%f\t%f\t%f\t",i,g_Map.m_Waypoints[i].m_VOrigin.x,g_Map.m_Waypoints[i].m_VOrigin.y,g_Map.m_Waypoints[i].m_VOrigin.z);
		fprintf(fhd,"%f\t%i\t",g_Map.m_Waypoints[i].m_fAvDistVWP,g_Map.m_Waypoints[i].m_iVisibleWaypoints,g_Map.m_Waypoints[i].m_lTotalDamage,g_Map.m_Waypoints[i].m_lTotalDamaged,g_Map.m_Waypoints[i].m_lTraffic);
		fprintf(fhd,"%li\t%li\t%li\n",g_Map.m_Waypoints[i].m_lTotalDamage,g_Map.m_Waypoints[i].m_lTotalDamaged,g_Map.m_Waypoints[i].m_lTraffic);
	}
	char szProf[10000];
	g_Profiler.profileDumpOutputToBuffer(szProf);
	fprintf(fhd,szProf);
	fclose(fhd);
	return true;
}

bool bc_togglestatdata(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	g_bShowWPStat =! g_bShowWPStat;
	return true;
}

/*
bool bc_whatever(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
}
*/

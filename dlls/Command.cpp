// JoeBot XP
// by Johannes Lampel
// Johannes.Lampel@gmx.de
// http://joebot.counter-strike.de

#include "Command.h"
#include "Commandfunc.h"
#include <string.h>

CCommands g_Commands;

CCommand :: CCommand(){
	*m_szName = 0;
	*m_sz2Name = 0;
	m_iType = 0;
	m_pFunc = 0;
	//prev = next = 0;
}

CCommand :: ~CCommand(){
}

// commands
CCommands ::CCommands(){
	AddCommand("waypoint",		"wp",		(void*)bc_waypoint,CM_ALL);
	AddCommand("search",		"s",		(void*)bc_search,CM_ALL);
	AddCommand("addbot",		"a",		(void*)bc_addbot,CM_ALL);
	AddCommand("endround",		"er",		(void*)bc_endround,CM_ALL);
	AddCommand("testpath",		"",			(void*)bc_testpath,CM_ALL);
	AddCommand("playerupdate",			"pupd",			(void*)bc_pupd,CM_ALL);
	AddCommand("distance",		"",			(void*)bc_distance,CM_CONSOLE);

	AddCommand("situation",		"sit",		(void*)bc_sit,CM_ALL);
	AddCommand("toggleprofile",	"tp",		(void*)bc_toggleprofile,CM_ALL);
	AddCommand("togglestatdata","ts",		(void*)bc_togglestatdata,CM_ALL);
	AddCommand("dumpstat",		"ds",		(void*)bc_dumpstat,CM_ALL);
	AddCommand("debug",			"d",		(void*)bc_debug,CM_ALL);
}

CCommands ::~CCommands(){
}

bool CCommands :: AddCommand(const char *n,const char *n2,void *p,int iT){
	CCommand add;

	add.m_iType = iT;
	add.m_pFunc = ((bool (*)(edict_t *p,int iType,const char *arg1,const char *arg2,const char *arg3, const char *arg4))(p));
	strcpy(add.m_szName,n);
	strcpy(add.m_sz2Name,n2);

	m_LCom.push_back(add);

	m_lNum++;
	return true;
}

bool CCommands ::Exec(edict_t *pEntity,int iType,const char *cmd,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	list<CCommand>::iterator iter;

	if(!arg1)arg1="";
	if(!arg2)arg2="";
	if(!arg3)arg3="";
	if(!arg4)arg4="";

	//cout << cmd <<" --- "<<arg1<<" --- "<<arg2<<" --- "<<arg3<<" --- "<<arg4<<endl;

	iter = m_LCom.begin();
	while(iter != m_LCom.end()){
		//cout << p->szName << endl;
		if((strlen(iter->m_szName)
				&&!strcmp(iter->m_szName,cmd))
			||(strlen(iter->m_sz2Name)
				&&!strcmp(iter->m_sz2Name,cmd))){
			if(iter->m_iType & iType){
				return iter->m_pFunc(pEntity,iType,arg1,arg2,arg3,arg4);
			}
		}
		iter ++;
	}
	return false;
}
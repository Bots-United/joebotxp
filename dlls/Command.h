#ifndef _CCOMMAND_H
#define _CCOMMAND_H

#include "extdll.h"

#include <list>
using namespace std;

#define CM_CONSOLE		(1<<0)
#define CM_DEDICATED	(1<<1)
#define CM_SCRIPT		(1<<2)
#define CM_ALL			(1<<0)|(1<<1)|(1<<2)

class CCommand{
	friend class CCommands;
public:
	CCommand();
	~CCommand();

	bool (*m_pFunc) (edict_t *p,int iType,const char *arg1,const char *arg2,const char *arg3, const char *arg4);
	char m_szName[32];
	char m_sz2Name[32];
	int m_iType;
};

class CCommands{
public:
	CCommands();
	~CCommands();
	bool Exec(edict_t*,int,const char*,const char *,const char *,const char *,const char *);	// execute a command issued by param1
	bool AddCommand(const char *,const char *,void *,int);			// add a command to the command list

	long m_lNum;
	list<CCommand> m_LCom;
	//CCommand *LCom;
		//*last;
};

extern CCommands g_Commands;

#endif//_CCOMMAND_H
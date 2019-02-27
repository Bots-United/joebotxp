#ifndef __COMMANDFUNC_H
#define __COMMANDFUNC_H

bool bc_waypoint(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4);
bool bc_search(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4);
bool bc_addbot(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4);
bool bc_endround(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4);
bool bc_sit(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4);

bool bc_testpath(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4);

bool bc_pupd(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4);
bool bc_distance(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4);

bool bc_toggleprofile(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4);
bool bc_togglestatdata(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4);
bool bc_dumpstat(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4);
bool bc_debug(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4);


#endif//__COMMANDFUNC_H
/***
*
*  Copyright (c) 1999, Valve LLC. All rights reserved.
*
*  This product contains software technology licensed from Id 
*  Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*  All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
// JoeBot
// by Johannes Lampel
// Johannes.Lampel@gmx.de
// http://joebot.counter-strike.de

//
// HPB_bot - botman's High Ping Bastard bot
//
// (http://planethalflife.com/botman/)
//
// util.cpp
//

#include "extdll.h"
#include "util.h"

#include "bot.h"
#include "BaseBot.h"

#include "Game.h"
#include "Player.h"

#include "engine.h"
#include <iostream>

extern int mod_id;

int gmsgTextMsg = 0;
int gmsgSayText = 0;
int gmsgShowMenu = 0;

int gmsgBotDebugMsg = 0;

/*Vector UTIL_VecToAngles( const Vector &vec )
{
	float rgflVecOut[3];
	VEC_TO_ANGLES(vec, rgflVecOut);
	return Vector(rgflVecOut);
}*/


// Overloaded to add IGNORE_GLASS
/*void g_pGame->traceLine( const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, IGNORE_GLASS ignoreGlass, edict_t *pentIgnore, TraceResult *ptr )
{
	TRACE_LINE( vecStart, vecEnd, (igmon == ignore_monsters ? TRUE : FALSE) | (ignoreGlass?0x100:0), pentIgnore, ptr );
}


void g_pGame->traceLine( const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, edict_t *pentIgnore, TraceResult *ptr )
{
	TRACE_LINE( vecStart, vecEnd, (igmon == ignore_monsters ? TRUE : FALSE), pentIgnore, ptr );
}*/

/*void UTIL_TraceHull( const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon,int iHullNumber, edict_t *pentIgnore, TraceResult *ptr )
{
	TRACE_HULL( vecStart, vecEnd, (igmon == ignore_monsters ? TRUE : FALSE),iHullNumber, pentIgnore, ptr );
}

void UTIL_TraceHull( const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, IGNORE_GLASS ignoreGlass,int iHullNumber, edict_t *pentIgnore, TraceResult *ptr )
{
	TRACE_HULL( vecStart, vecEnd, (igmon == ignore_monsters ? TRUE : FALSE) | (ignoreGlass?0x100:0),iHullNumber, pentIgnore, ptr );
}*/

/*void UTIL_MakeVectors( const Vector &vecAngles )
{
	MAKE_VECTORS( vecAngles );
}*/


CEntity UTIL_FindEntityInSphere( CEntity pentStart, const Vector &vecCenter, float flRadius )
{
	edict_t  *pentEntity;
	
	pentEntity = FIND_ENTITY_IN_SPHERE( pentStart, vecCenter, flRadius);
	
	if (!FNullEnt(pentEntity))
		return CEntity(pentEntity);
	
	return NULL;
}


CEntity UTIL_FindEntityByString( CEntity pentStart, const char *szKeyword, const char *szValue )
{
	edict_t	*pentEntity;
	
	pentEntity = FIND_ENTITY_BY_STRING( pentStart, szKeyword, szValue );
	
	if (!FNullEnt(pentEntity))
		return CEntity(pentEntity);
	return NULL;
}

CEntity UTIL_FindEntityByClassname( CEntity pentStart, const char *szName )
{
	return UTIL_FindEntityByString( pentStart, "classname", szName );
}

CEntity UTIL_FindEntityByTargetname( CEntity pentStart, const char *szName )
{
	return UTIL_FindEntityByString( pentStart, "targetname", szName );
}


int UTIL_PointContents( const Vector &vec )
{
	return POINT_CONTENTS(vec);
}


void UTIL_SetSize( entvars_t *pev, const Vector &vecMin, const Vector &vecMax )
{
	SET_SIZE( ENT(pev), vecMin, vecMax );
}


void UTIL_SetOrigin( entvars_t *pev, const Vector &vecOrigin )
{
	SET_ORIGIN(ENT(pev), vecOrigin );
}


void ClientPrint( entvars_t *client, int msg_dest, const char *msg_name, const char *param1, const char *param2, const char *param3, const char *param4 )
{
	if (gmsgTextMsg == 0)
		gmsgTextMsg = REG_USER_MSG( "TextMsg", -1 );
	
	MESSAGE_BEGIN( MSG_ONE, gmsgTextMsg, NULL, client );
	
	WRITE_BYTE( msg_dest );
	WRITE_STRING( msg_name );
	
	if ( param1 )
		WRITE_STRING( param1 );
	if ( param2 )
		WRITE_STRING( param2 );
	if ( param3 )
		WRITE_STRING( param3 );
	if ( param4 )
		WRITE_STRING( param4 );
	
	MESSAGE_END();
}
/*
void ClientPrintEx( entvars_t *client, int msg_dest, const char *msg_name, const char *param1, const char *param2, const char *param3, const char *param4 )
{
	if(IS_DEDICATED_SERVER() && client == 0){
		if(msg_name)	cout << msg_name;
		if(param1)		cout << param1;
		if(param2)		cout << param2;
		if(param3)		cout << param3;
		if(param4)		cout << param4;
		cout.flush();
	}
	else
		if(client)
			ClientPrint(client,msg_dest,msg_name,param1,param2,param3,param4);
		else if(g_pGame->m_pListenserverEdict.getEntity())
			ClientPrint(VARS(g_pGame->m_pListenserverEdict.getEntity()),msg_dest,msg_name,param1,param2,param3,param4);
			
}
*/
void UTIL_ClientPrintAll( int msg_dest, const char *msg_name, const char *param1, const char *param2, const char *param3, const char *param4 )
{
	if (gmsgTextMsg == 0)
		gmsgTextMsg = REG_USER_MSG( "TextMsg", -1 );
	
	MESSAGE_BEGIN( MSG_ALL, gmsgTextMsg );
	WRITE_BYTE( msg_dest );
	WRITE_STRING( msg_name );
	
	if ( param1 )
		WRITE_STRING( param1 );
	if ( param2 )
		WRITE_STRING( param2 );
	if ( param3 )
		WRITE_STRING( param3 );
	if ( param4 )
		WRITE_STRING( param4 );
	
	MESSAGE_END();
}

void UTIL_HostSay( edict_t *pEntity, int teamonly, char *message )
{
	int   j;
	char  text[128];
	char *pc;
	int   sender_team, player_team;
	edict_t *client;
	bool bIsBAlive = CEntity(pEntity).isAlive();
	//cout << message << endl;
	
	// make sure the text has content
	for ( pc = message; pc != NULL && *pc != 0; pc++ )
	{
		if ( isprint( *pc ) && !isspace( *pc ) )
		{
			pc = NULL;   // we've found an alphanumeric character,  so text is valid
			break;
		}
	}
	
	if ( pc != NULL )
		return;  // no character found, so say nothing
	
	// turn on color set 2  (color on,  no sound)
	if(bIsBAlive)
		if ( teamonly )
			sprintf( text, "%c(TEAM) %s: ", 2, STRING( pEntity->v.netname ) );
		else
			sprintf( text, "%c%s: ", 2, STRING( pEntity->v.netname ) );
	else
		if ( teamonly )
			sprintf( text, "%c*DEAD*(TEAM) %s: ", 2, STRING( pEntity->v.netname ) );
		else
			sprintf( text, "%c*DEAD*%s: ", 2, STRING( pEntity->v.netname ) );
	
	j = sizeof(text) - 2 - strlen(text);  // -2 for /n and null terminator
	if ( (int)strlen(message) > j )
		message[j] = 0;
	
	strcat( text, message );
	strcat( text, "\n" );
	
	// loop through all players
	// Start with the first player.
	// This may return the world in single player if the client types something between levels or during spawn
	// so check it, or it will infinite loop
	
	if (gmsgSayText == 0)
		gmsgSayText = REG_USER_MSG( "SayText", -1 );
	
	sender_team = UTIL_GetTeam(pEntity);
	
	client = NULL;
	while ( ((client = UTIL_FindEntityByClassname( client, "player" )) != NULL) &&
		(!FNullEnt(client)) ) 
	{
		if ( client == pEntity )  // skip sender of message
			continue;

		if(!bIsBAlive == CEntity(client).isAlive())	// skip sending mess from dead bots to living ones ...
			continue;
		
		player_team = UTIL_GetTeam(client);
		
		if ( teamonly && (sender_team != player_team) )
			continue;
		
		pfnMessageBegin( MSG_ONE, gmsgSayText, NULL, client );
		pfnWriteByte( ENTINDEX(pEntity) );
		pfnWriteString( text );
		pfnMessageEnd();
	}
	
	// print to the sending client
	pfnMessageBegin( MSG_ONE, gmsgSayText, NULL, pEntity );
	pfnWriteByte( ENTINDEX(pEntity) );
	pfnWriteString( text );
	pfnMessageEnd();
}

void UTIL_SayText( const char *pText, edict_t *pEdict )
{
	if (gmsgSayText == 0)
		gmsgSayText = REG_USER_MSG( "SayText", -1 );
	
	MESSAGE_BEGIN( MSG_ONE, gmsgSayText, NULL, pEdict );
	
	WRITE_BYTE( ENTINDEX(pEdict) );
	WRITE_STRING( pText );
	
	MESSAGE_END();
}

/*long UTIL_ClientIndex(edict_t *pEdict){
	int i;
	edict_t *pEnt;
	long lIndex = -1;
	for (i = gpGlobals->maxClients; i ; i--){
		pEnt = INDEXENT(i);
		
		// skip invalid players and skip self (i.e. this bot)
		if ((pEnt) && (!pEnt->free) && pEnt == pEdict){
			lIndex = i;
			break;
		}
	}
	return lIndex;
}*/

bool UTIL_PlayerDecalTrace( TraceResult *pTrace, int playernum, int decalNumber, BOOL bIsCustom )
{
	int index;

	if(decalNumber == -1){
		return false;
	}
	
	/*if (!bIsCustom)
	{
		if ( decalNumber < 0 )
			return;

		index = gDecals[ decalNumber ].index;
		if ( index < 0 )
			return;
	}
	else*/
		index = decalNumber;

	if (pTrace->flFraction == 1.0)
		return false;

	MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY );
		WRITE_BYTE( TE_PLAYERDECAL );
		WRITE_BYTE ( playernum );
		WRITE_COORD( pTrace->vecEndPos.x );
		WRITE_COORD( pTrace->vecEndPos.y );
		WRITE_COORD( pTrace->vecEndPos.z );
		WRITE_SHORT( (short)ENTINDEX(pTrace->pHit) );
		WRITE_BYTE( index );
	MESSAGE_END();
	return true ;
}

#ifdef	DEBUG
edict_t *DBG_EntOfVars( const entvars_t *pev )
{
	if (pev->pContainingEntity != NULL)
		return pev->pContainingEntity;
	ALERT(at_console, "entvars_t pContainingEntity is NULL, calling into engine");
	edict_t* pent = (*g_engfuncs.pfnFindEntityByVars)((entvars_t*)pev);
	if (pent == NULL)
		ALERT(at_console, "DAMN!  Even the engine couldn't FindEntityByVars!");
	((entvars_t *)pev)->pContainingEntity = pent;
	return pent;
}
#endif //DEBUG


// return team number 0 through 3 based what MOD uses for team numbers
int UTIL_GetTeam(edict_t *pEntity)
{
	if (mod_id == CSTRIKE_DLL)
	{
		char *infobuffer;
		char model_name[32];
		
		infobuffer = (*g_engfuncs.pfnGetInfoKeyBuffer)( pEntity );
		strcpy(model_name, (g_engfuncs.pfnInfoKeyValue(infobuffer, "model")));
		
		if ((strcmp(model_name, "terror") == 0) ||  // Phoenix Connektion
			(strcmp(model_name, "arab") == 0) ||    // L337 Krew
			(strcmp(model_name, "artic") == 0) ||   // Artic Avenger
			(strcmp(model_name, "guerilla") == 0))  // Gorilla Warfare
		{
			return 0;
		}
		else if ((strcmp(model_name, "urban") == 0) ||  // Seal Team 6
			(strcmp(model_name, "gsg9") == 0) ||   // German GSG-9
			(strcmp(model_name, "sas") == 0) ||    // UK SAS
			(strcmp(model_name, "gign") == 0) ||   // French GIGN
			(strcmp(model_name, "vip") == 0))      // VIP
		{
			return 1;
		}
		
		return 0;  // return zero if team is unknown
	}
	else
	{
		int team = pEntity->v.team;  // assume all others are 0-3 based
		
		if ((team < 0) || (team > 3))
			team = 0;
		
		return team;
	}
}

/*

bool UTIL_IsVIP(edict_t *pEntity){
	if (mod_id == CSTRIKE_DLL)
	{
		char *infobuffer;
		char model_name[32];
		
		infobuffer = (*g_engfuncs.pfnGetInfoKeyBuffer)( pEntity );
		strcpy(model_name, (g_engfuncs.pfnInfoKeyValue(infobuffer, "model")));
		return !strcmp(model_name, "vip");
	}
	else
		return false;
}*/

void UTIL_ShowMenu( edict_t *pEdict, int slots, int displaytime, bool needmore, char *pText )
{
   if (gmsgShowMenu == 0)
      gmsgShowMenu = REG_USER_MSG( "ShowMenu", -1 );

   pfnMessageBegin( MSG_ONE, gmsgShowMenu, NULL, pEdict );

   pfnWriteShort( slots );
   pfnWriteChar( displaytime );
   pfnWriteByte( needmore );
   pfnWriteString( pText );

   pfnMessageEnd();
}

static unsigned short FixedUnsigned16( float value, float scale )
{
	int output;

	output = value * scale;
	if ( output < 0 )
		output = 0;
	if ( output > 0xFFFF )
		output = 0xFFFF;

	return (unsigned short)output;
}

static short FixedSigned16( float value, float scale )
{
	int output;

	output = value * scale;

	if ( output > 32767 )
		output = 32767;

	if ( output < -32768 )
		output = -32768;

	return (short)output;
}

void UTIL_ShowText( edict_t *pEntity, const hudtextparms_t &textparms, const char *pMessage )
{
	if (!pEntity)
		return;
	
	MESSAGE_BEGIN( MSG_ONE, SVC_TEMPENTITY, NULL, pEntity );
		WRITE_BYTE( TE_TEXTMESSAGE );
		WRITE_BYTE( textparms.channel & 0xFF );

		WRITE_SHORT( FixedSigned16( textparms.x, 1<<13 ) );
		WRITE_SHORT( FixedSigned16( textparms.y, 1<<13 ) );
		WRITE_BYTE( textparms.effect );

		WRITE_BYTE( textparms.r1 );
		WRITE_BYTE( textparms.g1 );
		WRITE_BYTE( textparms.b1 );
		WRITE_BYTE( textparms.a1 );

		WRITE_BYTE( textparms.r2 );
		WRITE_BYTE( textparms.g2 );
		WRITE_BYTE( textparms.b2 );
		WRITE_BYTE( textparms.a2 );

		WRITE_SHORT( FixedUnsigned16( textparms.fadeinTime, 1<<8 ) );
		WRITE_SHORT( FixedUnsigned16( textparms.fadeoutTime, 1<<8 ) );
		WRITE_SHORT( FixedUnsigned16( textparms.holdTime, 1<<8 ) );

		if ( textparms.effect == 2 )
			WRITE_SHORT( FixedUnsigned16( textparms.fxTime, 1<<8 ) );
		
		if ( strlen( pMessage ) < 512 )
		{
			WRITE_STRING( pMessage );
		}
		else
		{
			char tmp[512];
			strncpy( tmp, pMessage, 511 );
			tmp[511] = 0;
			WRITE_STRING( tmp );
		}
	MESSAGE_END();
}

/*
bool IsAlive(edict_t *pEdict)
{
	return ((pEdict->v.deadflag == DEAD_NO) &&
		(pEdict->v.health > 0) && (pEdict->v.movetype != MOVETYPE_NOCLIP));
}

bool TEq(float f1,float f2,float fD){
	if(f1 < f2 + fD
		&& f1 > f2 - fD)
		return true;
	else
		return false;
}*/
/*
int UTIL_ClientsInGame( void )
{
    int iCount = 0;

    for ( int iIndex = 1; iIndex <= gpGlobals->maxClients; iIndex++ )
	{
        edict_t * pPlayer = INDEXENT( iIndex );

        if ( pPlayer == NULL )
            continue;

        if ( FNullEnt( pPlayer ) )
            continue;

        if ( FStrEq( STRING( pPlayer->v.netname ), "" ) )
            continue;

        iCount++;
    }

    return iCount;
}

*/int UTIL_HumansInGame( void )
{
    int iCount = 0;

    for ( int iIndex = 1; iIndex <= gpGlobals->maxClients; iIndex++ )
	{
        edict_t *pPlayer = INDEXENT( iIndex );

        if ( pPlayer == NULL )
            continue;

        if ( FNullEnt( pPlayer ) )
            continue;

        if ( FStrEq( STRING( pPlayer->v.netname ), "" ) )
            continue;

        if ( FBitSet( pPlayer->v.flags, FL_FAKECLIENT ) )
            continue;

        iCount++;
    }

    return iCount;
}

/*bool FInViewCone(const Vector &pOrigin, edict_t *pEdict)
{
	Vector2D vec2LOS;
	float    flDot;
	
	UTIL_MakeVectors ( pEdict->v.angles );
	
	vec2LOS = ( pOrigin - pEdict->v.origin ).make2D();
	vec2LOS = vec2LOS.normalize();
	
	flDot = DotProduct (vec2LOS , gpGlobals->v_forward.make2D() );
	
	if ( flDot > 0.50 )  // 60 degree field of view 
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

bool FVisible( const Vector &vecOrigin, edict_t *pEdict )
{
	TraceResult tr;
	Vector      vecLookerOrigin;
	
	// look through caller's eyes
	vecLookerOrigin = pEdict->v.origin + pEdict->v.view_ofs;
	
	g_pGame->traceLine(vecLookerOrigin, vecOrigin, ignore_monsters, ignore_glass, pEdict, &tr);
	
	if (tr.flFraction != 1.0)
	{
		return FALSE;// Line of sight is not established
	}
	else
	{
		return TRUE;// line of sight is valid.
	}
}

bool FVisibleMT( const Vector &vecOrigin,const Vector &VAim,float fSQ,edict_t *pEdict2I)
{
	const int _RO = 2;
	const int _NT = 3;
	Vector v_right,v_up,v_offset;
	int ix, iy;

	fSQ /= (_NT*2.f);

	UTIL_MakeVectors(UTIL_VecToAngles(VAim - vecOrigin));

	v_right = gpGlobals->v_right;
	v_up = Vector(0,0,1);
	
	for( ix = -_NT; ix <=_NT; ix ++){
		for( iy = -_NT; iy <= _NT; iy ++){
			TraceResult tr;

			v_offset = ix * v_right * fSQ + iy * v_up * fSQ ;
			
			g_pGame->traceLine(vecOrigin+v_offset+Vector(RANDOM_LONG(-_RO,_RO),RANDOM_LONG(-_RO,_RO),RANDOM_LONG(-_RO,_RO)), VAim+v_offset+Vector(RANDOM_LONG(-_RO,_RO),RANDOM_LONG(-_RO,_RO),RANDOM_LONG(-_RO,_RO)), ignore_monsters, ignore_glass, pEdict2I, &tr);
#ifdef DEBUGMESSAGES
			g_pGame->drawBeam(DEBUG_SYSTEM,g_pGame->m_pListenserverEdict.getEntity(),vecOrigin+v_offset, VAim+v_offset,3,0,Vector(250,250,250),250,10);
#endif
			
			if (tr.flFraction != 1.0){
				// couldn't be established
				return false;
			}
			else{
				//continue loop
			}
		}
	}
	return true;
}

bool FVisibleEx(Vector v_start, Vector v_end, bool bignore_doors, bool bignore_breakables, bool bignore_monsters, bool bignore_glass,edict_t *pEdictIgnore){
	TraceResult tr1,tr2;
	char	szDoor[] = "func_door",
			szBreakable[] = "func_breakable";
	
	g_pGame->traceLine(v_start,v_end,bignore_monsters?ignore_monsters:dont_ignore_monsters,bignore_glass?ignore_glass:dont_ignore_glass,pEdictIgnore,&tr1);
	//UTIL_TraceHull(v_start,v_end,bignore_monsters?ignore_monsters:dont_ignore_monsters,bignore_glass?ignore_glass:dont_ignore_glass,iHull,pEdictIgnore,&tr1);

	if(tr1.flFraction == 1.f)
		return true;

	Vector v_dir = (v_end - v_start).normalize();
	
	if(tr1.pHit){
		if(bignore_doors
			&&!strncmp(STRING(tr1.pHit->v.classname),szDoor,9)){
			// it's a door
			// trace vice versa
			g_pGame->traceLine(v_end,v_start,bignore_monsters?ignore_monsters:dont_ignore_monsters,bignore_glass?ignore_glass:dont_ignore_glass,pEdictIgnore,&tr2);
			
			if(tr1.pHit == tr2.pHit)		// the same door
				return true;

			return FVisibleEx(tr1.vecEndPos + v_dir*.001,tr2.vecEndPos - v_dir * .001,bignore_doors,bignore_breakables,bignore_monsters,bignore_glass,pEdictIgnore);
		}
		else if(bignore_breakables
			&&!strncmp(STRING(tr1.pHit->v.classname),szBreakable,14)){
			// it's a breakable
			if(tr1.pHit->v.takedamage == DAMAGE_YES){		// can be destroyed
				g_pGame->traceLine(v_end,v_start,bignore_monsters?ignore_monsters:dont_ignore_monsters,bignore_glass?ignore_glass:dont_ignore_glass,pEdictIgnore,&tr2);
				//UTIL_TraceHull(v_end,v_start,bignore_monsters?ignore_monsters:dont_ignore_monsters,bignore_glass?ignore_glass:dont_ignore_glass,iHull,pEdictIgnore,&tr2);
				if(tr1.pHit == tr2.pHit)		// the same breakable
					return true;

				return FVisibleEx(tr1.vecEndPos + v_dir*.001,tr2.vecEndPos - v_dir * .001,bignore_doors,bignore_breakables,bignore_monsters,bignore_glass,pEdictIgnore);
			}
			else
				return false;
		}
		else
			return false;
	}
	else
		return false;
}*/

void UTIL_SelectItem(edict_t *pEdict, char *item_name)
{
	FakeClientCommand(pEdict, item_name, NULL, NULL);
}

/*Vector VecBModelOrigin(edict_t *pEdict)
{
	return pEdict->v.absmin + (pEdict->v.size * 0.5);
}

Vector VecBModelOrigin(entvars_s *v)
{
	return v->absmin + (v->size * 0.5);
}*/

void UTIL_BuildFileName(char *filename, const char *arg1, const char *arg2)
{
	
	if (mod_id == VALVE_DLL)
		strcpy(filename, "valve");	
	else if (mod_id == TFC_DLL)
		strcpy(filename, "tfc");
	else if (mod_id == CSTRIKE_DLL)
		strcpy(filename, "cstrike");
	else if (mod_id == DOD_DLL)
		strcpy(filename, "dod");
	else if (mod_id == GEARBOX_DLL)
		strcpy(filename, "gearbox");	
	else if (mod_id == NS_DLL)
		strcpy(filename, "ns");	
	else{
		*filename = 0;
		return;
	}

	#ifndef __linux__
      strcat(filename, "\\");
#else
      strcat(filename, "/");
#endif
	
	if ((arg1) && (*arg1) && (arg2) && (*arg2))
   {
      strcat(filename, arg1);

#ifndef __linux__
      strcat(filename, "\\");
#else
      strcat(filename, "/");
#endif

      strcat(filename, arg2);
   }
   else if ((arg1) && (*arg1))
   {
      strcat(filename, arg1);
   }
}

edict_t *UTIL_GetNearestPlayer(edict_t *pEdict,int iTeam,float &fMin,bool bVisible,bool bIVC,float fMax){
	edict_t *pEnt = 0,
		*pNearestEdict=0;
	float fDistance;
	int i;

	fMin = 100000000;
	fMax = fMax * fMax;
	
	for (i = gpGlobals->maxClients; i ; i--){
		pEnt = INDEXENT(i);
		
		// skip invalid players and skip self (i.e. this bot)
		if ((pEnt) && (!pEnt->free) && (pEnt != pEdict))
		{
			if(!CEntity(pEnt).isAlive())
				continue;

			if(iTeam != -1){
				if(UTIL_GetTeam(pEnt) != iTeam)
					continue;
			}
			if(bVisible){
				if(!g_pGame->fVisible(pEnt->v.origin,pEdict))
					continue;
			}
			if(bIVC){
				if(!g_pGame->fInViewCone(pEnt->v.origin,pEdict))
					continue;
			}

			fDistance = (pEdict->v.origin - pEnt->v.origin).squareLength();
			if(fMin > fDistance
				//&& fDistance > 1.0f
				&& fDistance <= fMax){
				fMin = fDistance;
				pNearestEdict = pEnt;
			}
		}
	}
	fMin = sqrt(fMin);		// quare root because fMin should be the real distance
	return pNearestEdict;
}

edict_t *UTIL_BestPlayer(void){
	edict_t *pEnt = 0,
			*pBestEdict=0;
	int iFrag;
	int i;
	
	iFrag = -1;
	
	for (i = gpGlobals->maxClients; i ; i--){
		pEnt = INDEXENT(i);
		
		// skip invalid players and skip self (i.e. this bot)
		if ((pEnt) && (!pEnt->free))
		{
			if(iFrag < pEnt->v.frags){
				iFrag = pEnt->v.frags;
				pBestEdict = pEnt;
			}
		}
	}
	return pBestEdict;
}

/*edict_t *GetRandomPlayer(edict_t *pNot,int iTeam,int iAlive){
	int i;
	edict_t *pFE[32],*pEnt;
	long lCount;
	long lreturn;

	lCount = 0;
	for (i = gpGlobals->maxClients; i ; i--){
		pEnt = INDEXENT(i);
		if ((pEnt) && (!pEnt->free) && (pEnt != pNot)){
			if(iTeam!=-1){
				if(iTeam != UTIL_GetTeam(pEnt))
					continue;
			}
			if(iAlive!=-1){
				if((!IsAlive(pEnt)) && (iAlive == 1))
					continue;
				else if((IsAlive(pEnt)) && (iAlive == 0))
					continue;
			}
			pFE[lCount] = pEnt;
			lCount++;
		}
	}
	
	if(lCount){
		lreturn=RANDOM_LONG(1,lCount)-1;
		return pFE[lreturn];
	}
	else
		return 0;
}*/

#define _GTOSSHEIGHT 300

//
// VecCheckToss - returns the velocity at which an object should be lobbed from vecspot1 to land near vecspot2.
// returns g_vecZero if toss is not feasible.
// 
// float flGravityAdj should normally be 2/3
//
Vector VecCheckToss ( entvars_t *pev, const Vector &vecSpot1, Vector vecSpot2, float flSpeed, float flGravityAdj )
{
	TraceResult		tr;
	Vector			vecMidPoint;// halfway point between Spot1 and Spot2
	Vector			vecApex;// highest point 
	Vector			vecScale;
	Vector			vecGrenadeVel;
	Vector			vecTemp;
	float			flGravity = CVAR_GET_FLOAT("sv_gravity") * flGravityAdj;
	
	if (vecSpot2.z - vecSpot1.z > _GTOSSHEIGHT)
	{
		// to high, fail
		return g_vecZero;
	}
	
	// toss a little bit to the left or right, not right down on the enemy's bean (head). 
	vecSpot2 = vecSpot2 + Vector(1,0,0) * ( RANDOM_FLOAT(-8,8) + RANDOM_FLOAT(-16,16) );
	vecSpot2 = vecSpot2 + Vector(0,1,0) * ( RANDOM_FLOAT(-8,8) + RANDOM_FLOAT(-16,16) );
	
	// calculate the midpoint and apex of the 'triangle'
	// UNDONE: normalize any Z position differences between spot1 and spot2 so that triangle is always RIGHT
	
	// How much time does it take to get there?
	
	// get a rough idea of how high it can be thrown
	vecMidPoint = vecSpot1 + (vecSpot2 - vecSpot1) * 0.5;
	g_pGame->traceLine(vecMidPoint, vecMidPoint + Vector(0,0,_GTOSSHEIGHT),dont_ignore_monsters,dont_ignore_glass, ENT(pev), &tr);
#ifdef DEBUGMESSAGES
	g_pGame->drawBeam(DEBUG_SYSTEM,g_pGame->m_pListenserverEdict.getEntity(),vecMidPoint,vecMidPoint + Vector(0,0,_GTOSSHEIGHT),10,10,Vector(0,200,200),200,10);
#endif
	vecMidPoint = tr.vecEndPos;
	// (subtract 15 so the grenade doesn't hit the ceiling)
	vecMidPoint.z -= 15;
	
	if (vecMidPoint.z < vecSpot1.z || vecMidPoint.z < vecSpot2.z)
	{
		// to not enough space, fail
		return g_vecZero;
	}
	
	// How high should the grenade travel to reach the apex
	float distance1 = (vecMidPoint.z - vecSpot1.z);
	float distance2 = (vecMidPoint.z - vecSpot2.z);
	
	// How long will it take for the grenade to travel this distance
	float time1 = sqrt( distance1 / (0.5f * flGravity) );
	float time2 = sqrt( distance2 / (0.5f * flGravity) );
	
	if (time1 < 0.2)
	{
		// too close
		return g_vecZero;
	}
	
	// how hard to throw sideways to get there in time.
	vecGrenadeVel = (vecSpot2 - vecSpot1) / (time1 + time2);
	// how hard upwards to reach the apex at the right time.
	vecGrenadeVel.z = flGravity * time1 * .5f;
	
	// find the apex
	vecApex  = vecSpot1 + vecGrenadeVel * time1;
	vecApex.z = vecMidPoint.z;
	
	g_pGame->traceLine(vecSpot1, vecApex, dont_ignore_monsters,dont_ignore_glass, ENT(pev), &tr);
#ifdef DEBUGMESSAGES
	g_pGame->drawBeam(DEBUG_SYSTEM,g_pGame->m_pListenserverEdict.getEntity(),vecSpot1, vecApex,10,10,Vector(0,200,200),200,10);
#endif
	if (tr.flFraction != 1.0)
	{
		// fail!
		return g_vecZero;
	}
	
	// UNDONE: either ignore monsters or change it to not care if we hit our enemy
	g_pGame->traceLine(vecSpot2, vecApex,dont_ignore_monsters,dont_ignore_glass, ENT(pev), &tr); 
#ifdef DEBUGMESSAGES
	g_pGame->drawBeam(DEBUG_SYSTEM,g_pGame->m_pListenserverEdict.getEntity(),vecSpot2, vecApex,10,10,Vector(0,200,200),200,10);
#endif
	if (tr.flFraction != 1.0)
	{
		// fail!
		return g_vecZero;
	}
	
	return vecGrenadeVel;
}


//
// VecCheckThrow - returns the velocity vector at which an object should be thrown from vecspot1 to hit vecspot2.
// returns g_vecZero if throw is not feasible.
//
// float flGravityAdj should normally be 2/3
//
Vector VecCheckThrow ( entvars_t *pev, const Vector &vecSpot1, Vector vecSpot2, float flSpeed, float flGravityAdj )
{
	float flGravity = CVAR_GET_FLOAT("sv_gravity") * flGravityAdj;
	
	Vector vecGrenadeVel = (vecSpot2 - vecSpot1);
	
	// throw at a constant time
	float time = vecGrenadeVel.length( ) / flSpeed;
	vecGrenadeVel = vecGrenadeVel * (1.0 / time);
	
	// adjust upward toss to compensate for gravity loss
	vecGrenadeVel.z += flGravity * time * 0.5;
	
	Vector vecApex = vecSpot1 + (vecSpot2 - vecSpot1) * 0.5;
	vecApex.z += 0.5 * flGravity * (time * 0.5) * (time * 0.5);
	
	TraceResult tr;
	g_pGame->traceLine(vecSpot1, vecApex, dont_ignore_monsters,dont_ignore_glass, ENT(pev), &tr);
#ifdef DEBUGMESSAGES
	g_pGame->drawBeam(DEBUG_SYSTEM,g_pGame->m_pListenserverEdict.getEntity(),vecSpot1, vecApex,10,10,Vector(100,200,200),200,10);
#endif

	if (tr.flFraction != 1.0)
	{
		// fail!
		return g_vecZero;
	}
	
	g_pGame->traceLine(vecSpot2, vecApex, ignore_monsters,dont_ignore_glass, ENT(pev), &tr);
#ifdef DEBUGMESSAGES
	g_pGame->drawBeam(DEBUG_SYSTEM,g_pGame->m_pListenserverEdict.getEntity(),vecSpot2, vecApex,10,10,Vector(100,200,200),200,10);
#endif
	if (tr.flFraction != 1.0)
	{
		// fail!
		return g_vecZero;
	}
	
	return vecGrenadeVel;
}

void UTIL_strlwr(char *p){
	while(*p){
		*p=tolower(*p);
		p ++;
	}
}

long UTIL_CountLines(const char *szString){
	long lCount = 0;
	while(*szString){
		if(*szString=='\n')
			lCount++;
		szString ++;
	}
	return lCount;
}

void UTIL_Endround(void){
	int i;
	for(i=0;i<g_pGame->m_iMaxCurrentPlayer;i++){
		if(g_pGame->getBot(i)){
			if(g_pGame->getBot(i)->isAlive()){
				g_pGame->getBot(i)->getEntity()->v.frags++;
				ClientKill(g_pGame->getBot(i)->getEntity());
			}
		}
	}
}

/*bool UTIL_DrawBeam(edict_t *pEntity, Vector start, Vector end, int width,int noise, int red, int green, int blue, int brightness, int speed,int iLife)
{
	if(g_lBeam > 100 || !pEntity)
		return 1;
	g_lBeam++;
	
	MESSAGE_BEGIN(MSG_ONE, SVC_TEMPENTITY, NULL, pEntity);
	WRITE_BYTE( TE_BEAMPOINTS);
	WRITE_COORD(start.x);
	WRITE_COORD(start.y);
	WRITE_COORD(start.z);
	WRITE_COORD(end.x);
	WRITE_COORD(end.y);
	WRITE_COORD(end.z);
	WRITE_SHORT( m_spriteTexture );
	WRITE_BYTE( 1 ); // framestart
	WRITE_BYTE( 10 ); // framerate
	WRITE_BYTE( iLife ); // life in 0.1's
	WRITE_BYTE( width ); // width
	WRITE_BYTE( noise );  // noise
	
	WRITE_BYTE( red );   // r
	WRITE_BYTE( green );   // g
	WRITE_BYTE( blue );   // b
	
	WRITE_BYTE( brightness );   // brightness
	WRITE_BYTE( speed );    // speed
	MESSAGE_END();
	
	return 0;
}*/

/*void UTIL_PrintMessage(const char *sz1,const char *sz2,const char *sz3,const char *sz4){
	ClientPrintEx(VARS(g_pGame->m_pListenserverEdict.getEntity()),HUD_PRINTNOTIFY,sz1,sz2,sz3,sz4);
}*/

void UTIL_TraceToGround(Vector &V){
	TraceResult tr;

	g_pGame->traceLine(V,V+Vector(0,0,-10000),ignore_monsters,ignore_glass,0,&tr);
	V = tr.vecEndPos;
}

void BotDebug(entvars_t *client,Vector &V,const char *szText1,const char *szText2,const char *szText3,const char *szText4,int iindex,int iused){
	if (gmsgBotDebugMsg == 0)
		gmsgBotDebugMsg = REG_USER_MSG( "BotDebug", -1 );

	if (gmsgBotDebugMsg == 0)
		return;

	MESSAGE_BEGIN( MSG_ONE, gmsgBotDebugMsg, NULL, client );

	WRITE_SHORT(iindex);
	WRITE_SHORT(iused);
	
	if(iused){
		WRITE_COORD( V.x );
		WRITE_COORD( V.y );
		WRITE_COORD( V.z );
		
		WRITE_STRING(szText1);
		WRITE_STRING(szText2);
		WRITE_STRING(szText3);
		WRITE_STRING(szText4);
	}
	
	MESSAGE_END();
}

void UTIL_ShortenBV(char *szT){
	char *szTS,*szS,szTemp[256];
	char szHLDM[]="CBV_HLDM_";szTS = szHLDM;

	if(szS = strstr(szT,szTS)){
		strcpy(szTemp,szT);
		strcpy(szT,"_");
		strcat(szT,szTemp+strlen(szTS)*sizeof(char));
	}

	char szNPHLDM[]="CBVNP_HLDM";szTS = szNPHLDM;

	if(szS = strstr(szT,szTS)){
		strcpy(szTemp,szT);
		strcpy(szT,"NP_");
		strcat(szT,szTemp+strlen(szTS)*sizeof(char));
	}
}
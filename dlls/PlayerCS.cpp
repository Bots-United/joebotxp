// PlayerCS.cpp: implementation of the CPlayerCS class.
//
//////////////////////////////////////////////////////////////////////

#include "PlayerCS.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
CPlayerCS::CPlayerCS()
{

}

CPlayerCS::~CPlayerCS()
{

}*/

int CPlayerCS::getTeam(void){
	char *infobuffer;
	char model_name[32];
	
	infobuffer = (*g_engfuncs.pfnGetInfoKeyBuffer)( getEntity() );
	strcpy(model_name, (g_engfuncs.pfnInfoKeyValue(infobuffer, "model")));
	
	/*if (!strcmp(model_name, "terror")	|| // Phoenix Connektion
		!strcmp(model_name, "arab")		|| // L337 Krew
		!strcmp(model_name, "artic")	|| // Artic Avenger
		!strcmp(model_name, "guerilla"))   // Gorilla Warfare
	{
		return TE;
	}
	else */if (!strcmp(model_name, "urban")	|| // Seal Team 6
		!strcmp(model_name, "gsg9")			|| // German GSG-9
		!strcmp(model_name, "sas")			|| // UK SAS
		!strcmp(model_name, "gign")			|| // French GIGN
		!strcmp(model_name, "vip"))		       // VIP
	{
		return CT;
	}
	
	return 0;  // return 0 if team is unknown
}

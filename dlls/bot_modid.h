#ifndef __MODID_H
#define __MODID_H

extern int mod_id;

// define constants used to identify the MOD we are playing...
enum enum_mod_ids{
	VALVE_DLL = 1,
	DOD_DLL,
	CSTRIKE_DLL,
	CSCLASSIC_DLL,
	CSCZ_DLL,
	GEARBOX_DLL,
	FRONTLINE_DLL,
	TFC_DLL,
	NS_DLL,

	MAX_MOD
};
#endif
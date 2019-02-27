// Weapons.h: interface for the CWeapons class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__WEAPONS_H)
#define __WEAPONS_H

#define CWI_USLOTS 5

#include "cdll_dll.h"
#include "RespCurve.h"

/*class CWeaponBitfield{
public:
	CWeaponBitfield(){ m_lValue = 0; };
	CWeaponBitfield(int i){ m_lValue = 1<<i; };

	const CWeaponBitfield & raw(long l){m_lValue = l;return ((const CWeaponBitfield &)(*this));};

	long m_lValue;
};*/

class CBaseBot;

class CWeaponsItem{
public:
	CWeaponsItem();
	~CWeaponsItem();

	bool m_bUsable;
	bool m_bPenetratingWalls;				// is this weapon penetrating
	float m_fDelay;							// delay between 2 consecutive attacks
	CRespCurve m_Usability;
	float m_fPauseTime;						// if recoil is too big, pause this time
	int m_iPreference;						// 'personal' preference for this weapon ( this variable is altered when bot is killing or killed )
	int m_iClipSize;						// clip size
	CRespCurve m_Duck;						// it's useful to duck before shooting ?
	CRespCurve m_Zoom;						//               - || -

	int m_iLoudness;						// how loud is this weapon ?
	int m_iCost;							// if we need to buy weapons in this mod, how much does it cost ?
	int m_iTeam;							// which team can use this weapon ?			// ct1 te0
	float m_fBuyProp;						// probablity to buy this weapon
	void (*m_pFBuy)(CBaseBot *);			// function to buy this weapon
};

class CWeapons
{
public:
	CWeapons();
	virtual ~CWeapons();

	virtual void alloc(void);

	virtual int getBestWeapon(float);
	virtual int getClipSize(int);
	virtual float getBProp(int);
	virtual int getLoudness(int);
	virtual int getTeam(int);
	virtual int getCost(int);
	CWeaponsItem *getCWeapon(void);
	long getCWeaponID(void){return m_iCurrentWId;}
	float getValueWeaponDistance(int,float);
	float getBestDistance(int);
	float getClipPerc(void);						// get the percentage of how the clip is filled up with rounds
	void randomizePrefs(int,int);

	virtual long isPrimary(long){return 0;}
	virtual long isSecondary(long){return 0;}
	virtual long isGrenade(long){return 0;}
	virtual long isExplGrenade(long){return 0;}	// explosive grenade, no flashbang :)
	virtual long isSniper(long){return 0;}
	virtual long isShotgun(long){return 0;}
	virtual long isSubM(long){return 0;}
	virtual long isRifle(long){return 0;}
	virtual long isMachineGun(long){return 0;}

	long hasPrimary(void){return isPrimary(m_iWeapons);}
	long hasSecondary(void){return isSecondary(m_iWeapons);}
	long hasGrenade(void){return isGrenade(m_iWeapons);}
	long hasExplGrenade(void){return isExplGrenade(m_iWeapons);}
	long hasSniper(void){return isSniper(m_iWeapons);}
	long hasShotgun(void){return isShotgun(m_iWeapons);}
	long hasSubM(void){return isSubM(m_iWeapons);}
	long hasRifle(void){return isRifle(m_iWeapons);}
	long hasMachineGun(void){return isMachineGun(m_iWeapons);}
	
	CWeaponsItem *m_pWeapons;

	int m_iWeapons;      // bit map of weapons the bot is carrying
	
	int  m_iCurrentWId;     // weapon ID of current weapon
	int  m_iCurrentWClip;   // amount of ammo in the clip
	int  m_iCurrentWAmmo1;  // amount of ammo in primary reserve
	int  m_iCurrentWAmmo2;  // amount of ammo in secondary reserve

	int m_piAmmo[MAX_AMMO_SLOTS];  // total ammo amounts
	int m_piAmmoClip[32];
};

// weapon ID values for Valve's Half-Life Deathmatch
#define VALVE_WEAPON_CROWBAR       1
#define VALVE_WEAPON_GLOCK         2
#define VALVE_WEAPON_357	       3
#define VALVE_WEAPON_MP5           4
#define VALVE_WEAPON_CHAINGUN      5
#define VALVE_WEAPON_CROSSBOW      6
#define VALVE_WEAPON_SHOTGUN       7
#define VALVE_WEAPON_RPG           8
#define VALVE_WEAPON_GAUSS         9
#define VALVE_WEAPON_EGON         10
#define VALVE_WEAPON_HORNETGUN    11
#define VALVE_WEAPON_HANDGRENADE  12
#define VALVE_WEAPON_TRIPMINE     13
#define VALVE_WEAPON_SATCHEL      14
#define VALVE_WEAPON_SNARK        15

#define VALVE_MAX_NORMAL_BATTERY   100
#define VALVE_HORNET_MAX_CARRY      8


// weapon ID values for Valve's Team Fortress Classic & 1.5
#define TF_WEAPON_UNKNOWN1       1
#define TF_WEAPON_UNKNOWN2       2
#define TF_WEAPON_MEDIKIT        3
#define TF_WEAPON_SPANNER        4
#define TF_WEAPON_AXE            5
#define TF_WEAPON_SNIPERRIFLE    6
#define TF_WEAPON_AUTORIFLE      7
#define TF_WEAPON_SHOTGUN        8
#define TF_WEAPON_SUPERSHOTGUN   9
#define TF_WEAPON_NAILGUN       10
#define TF_WEAPON_SUPERNAILGUN  11
#define TF_WEAPON_GL            12
#define TF_WEAPON_FLAMETHROWER  13
#define TF_WEAPON_RPG           14
#define TF_WEAPON_IC            15
#define TF_WEAPON_UNKNOWN16     16
#define TF_WEAPON_AC            17
#define TF_WEAPON_UNKNOWN18     18
#define TF_WEAPON_UNKNOWN19     19
#define TF_WEAPON_TRANQ         20
#define TF_WEAPON_RAILGUN       21
#define TF_WEAPON_PL            22
#define TF_WEAPON_KNIFE         23


// weapon ID values for Counter-Strike
#define CS_WEAPON_P228           1
#define CS_WEAPON_UNKNOWN1       2
#define CS_WEAPON_SCOUT          3
#define CS_WEAPON_HEGRENADE      4
#define CS_WEAPON_XM1014         5
#define CS_WEAPON_C4             6
#define CS_WEAPON_MAC10          7
#define CS_WEAPON_AUG            8
#define CS_WEAPON_SMOKEGRENADE   9
#define CS_WEAPON_ELITE         10
#define CS_WEAPON_FIVESEVEN     11
#define CS_WEAPON_UMP45         12
#define CS_WEAPON_SG550         13
#define CS_WEAPON_UNKNOWN6      14
#define CS_WEAPON_UNKNOWN7      15
#define CS_WEAPON_USP           16
#define CS_WEAPON_GLOCK18       17
#define CS_WEAPON_AWP           18
#define CS_WEAPON_MP5NAVY       19
#define CS_WEAPON_M249          20
#define CS_WEAPON_M3            21
#define CS_WEAPON_M4A1          22
#define CS_WEAPON_TMP           23
#define CS_WEAPON_G3SG1         24
#define CS_WEAPON_FLASHBANG     25
#define CS_WEAPON_DEAGLE        26
#define CS_WEAPON_SG552         27
#define CS_WEAPON_AK47          28
#define CS_WEAPON_KNIFE         29
#define CS_WEAPON_P90           30


// weapon ID values for Gearbox's OpFor Deathmatch
#define GEARBOX_WEAPON_CROWBAR       1
#define GEARBOX_WEAPON_GLOCK         2
#define GEARBOX_WEAPON_PYTHON        3
#define GEARBOX_WEAPON_MP5           4
#define GEARBOX_WEAPON_CHAINGUN      5
#define GEARBOX_WEAPON_CROSSBOW      6
#define GEARBOX_WEAPON_SHOTGUN       7
#define GEARBOX_WEAPON_RPG           8
#define GEARBOX_WEAPON_GAUSS         9
#define GEARBOX_WEAPON_EGON         10
#define GEARBOX_WEAPON_HORNETGUN    11
#define GEARBOX_WEAPON_HANDGRENADE  12
#define GEARBOX_WEAPON_TRIPMINE     13
#define GEARBOX_WEAPON_SATCHEL      14
#define GEARBOX_WEAPON_SNARK        15
#define GEARBOX_WEAPON_GRAPPLE      16
#define GEARBOX_WEAPON_EAGLE        17
#define GEARBOX_WEAPON_PIPEWRENCH   18
#define GEARBOX_WEAPON_M249         19
#define GEARBOX_WEAPON_DISPLACER    20
#define GEARBOX_WEAPON_UNKNOWN      21
#define GEARBOX_WEAPON_SHOCKRIFLE   22
#define GEARBOX_WEAPON_SPORELAUNCHER 23
#define GEARBOX_WEAPON_SNIPERRIFLE  24
#define GEARBOX_WEAPON_KNIFE        25

// weapons for DOD
#define DOD_WEAPON_AMERKNIFE		1
#define DOD_WEAPON_GERKNIFE			2
#define DOD_WEAPON_COLT				3
#define DOD_WEAPON_LUGER			4
#define DOD_WEAPON_GARAND			5
#define DOD_WEAPON_SCOPEDKAR		6	
#define DOD_WEAPON_THOMPSON			7
#define DOD_WEAPON_MP44				8
#define DOD_WEAPON_SPRING			9
#define DOD_WEAPON_KAR				10
#define DOD_WEAPON_BAR				11
#define DOD_WEAPON_MP40				12
#define DOD_WEAPON_HANDGRENADE		13
#define DOD_WEAPON_STICKGRENADE		14
#define DOD_WEAPON_STICKGRENADE_EX	15
#define DOD_WEAPON_HANDGRENADE_EX	16
#define DOD_WEAPON_MG42				17
#define DOD_WEAPON_30CAL			18
#define DOD_WEAPON_SPADE			19
#define DOD_WEAPON_M1CARBINE		20
#define DOD_WEAPON_MG34				21
#define DOD_WEAPON_GREASEGUN		22
#define DOD_WEAPON_FG42				23
#define DOD_WEAPON_K43				24


// // // // // // natural selection weapons
//MARINE
#define NS_WEAPON_KNIFE 13 
#define NS_WEAPON_PISTOL          14 
#define NS_WEAPON_MACHINEGUN      15 
#define NS_WEAPON_SHOTGUN         16 
#define NS_WEAPON_HEAVYMACHINEGUN 17 
#define NS_WEAPON_WELDINGTORCH    18 
#define NS_WEAPON_MINE            19 
#define NS_WEAPON_GRENADEGUN      20  

//SKULK  
#define NS_WEAPON_BITEGUN          5 
#define NS_WEAPON_PARASITE        10 
#define NS_WEAPON_LEAP            21 
#define NS_WEAPON_XENOCIDE        12  

//LERK  
#define NS_WEAPON_BITE2GUN         6 
#define NS_WEAPON_SPIKEGUN         4 
#define NS_WEAPON_UMBRA           23 
#define NS_WEAPON_SPORE            3  

//GORGE  
#define NS_WEAPON_SPIT             2 
#define NS_WEAPON_HEALTH_SPRAY    27 
#define NS_WEAPON_WEBSPINNER       8  
#define NS_WEAPON_BABBLERGUN      28  

// FADE  
#define NS_WEAPON_SWIPE            7 
#define NS_WEAPON_ACIDROCKETGUN   26 
#define NS_WEAPON_BLINK           11 
#define NS_WEAPON_BILEBOMBGUN     25  

// ONOS  
#define NS_WEAPON_CLAWS            1 
#define NS_WEAPON_PARALYSIS        9 
#define NS_WEAPON_CHARGE          22 
#define NS_WEAPON_PRIMALSCREAM    24 

typedef struct
{
   char szClassname[64];
   int  iAmmo1Id;	// ammo index for primary ammo
   int	iAmmoClip;	// size of clip ( only DOD )
   int  iAmmo1Max;	// max primary ammo
   int  iAmmo2Id;	// ammo index for secondary ammo
   int  iAmmo2Max;	// max secondary ammo
   int  iSlot;		// HUD slot (0 based)
   int  iPosition;	// slot position
   int  iId;		// weapon ID
   int  iFlags;		// flags???
} bot_weapon_t;

extern bot_weapon_t weapon_defs[MAX_WEAPONS];

#endif // !defined(__WEAPONS_H)

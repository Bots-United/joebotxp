// JoeBot
// by Johannes Lampel
// Johannes.Lampel@gmx.de
// http://joebot.counter-strike.de

//
// HPB bot - botman's High Ping Bastard bot
//
// (http://planethalflife.com/botman/)
//
// bot_client.cpp
//

#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "bot.h"
#include "Game.h"
#include "BaseBot.h"
#include "bot_client.h"
#include "Weapons.h"
#include "Map.h"

#include "PerceptionBot.h"

#include "Game.h"
#include "BotCS.h"
#include "BotDOD.h"

#include "Player.h"

#include <iostream>
using namespace std;

// types of damage to ignore...
#define IGNORE_DAMAGE (DMG_CRUSH | DMG_FREEZE | DMG_SHOCK | DMG_DROWN | \
	DMG_NERVEGAS | DMG_RADIATION | DMG_DROWNRECOVER | \
	DMG_ACID | DMG_SLOWBURN | DMG_SLOWFREEZE)

extern edict_t *g_pClientCallEdict;

bot_weapon_t weapon_defs[MAX_WEAPONS]; // array of weapon definitions

// This message is sent when the Counter-Strike VGUI menu is displayed.
void BotClient_CS_VGUI(void *p, int bot_index,int iAdd){
	if(iAdd != _CLIENT_END){
		if ((*(int *)p) == 2)  // is it a team select menu?
			g_pGame->getBot(bot_index)->m_start_action = MSG_CS_TEAM_SELECT;
		
		else if ((*(int *)p) == 26)  // is is a terrorist model select menu?
			g_pGame->getBot(bot_index)->m_start_action = MSG_CS_T_SELECT;
		
		else if ((*(int *)p) == 27)  // is is a counter-terrorist model select menu?	
			g_pGame->getBot(bot_index)->m_start_action = MSG_CS_CT_SELECT;
	}
}

void BotClient_DOD_VGUI(void *p, int bot_index,int iAdd){
	if(iAdd != _CLIENT_END){
		if ((*(int *)p) == 2)  // is it a team select menu?
			g_pGame->getBot(bot_index)->m_start_action = MSG_DOD_TEAM_SELECT;
		
		else if ((*(int *)p) == 10)  // is is a class selection axis menu?
			g_pGame->getBot(bot_index)->m_start_action = MSG_DOD_CLASS_SELECT_AX;
		
		else if ((*(int *)p) == 9)  // is is a class selection allies menu?
			g_pGame->getBot(bot_index)->m_start_action = MSG_DOD_CLASS_SELECT_AL;

		else if ((*(int *)p) == 21)  // is is a class selection allies menu?	// para
			g_pGame->getBot(bot_index)->m_start_action = MSG_DOD_CLASS_SELECT_AL_PARA;	
		else if ((*(int *)p) == 22)  // is is a class selection axis menu?		// para
			g_pGame->getBot(bot_index)->m_start_action = MSG_DOD_CLASS_SELECT_AX_PARA;
		
		else if ((*(int *)p) == 19)  // is is a class selection allies menu of the sergeant?
			g_pGame->getBot(bot_index)->m_start_action = MSG_DOD_WPN_SELECT_SERGEANT;

		else if ((*(int *)p) == 20)  // is is a class selection axis menu for machinegunners ?
			g_pGame->getBot(bot_index)->m_start_action = MSG_DOD_WPN_SELECT_MACHINE;

		else if ((*(int *)p) == 23)  // is is a class selection axis menu for fg42 ?
			g_pGame->getBot(bot_index)->m_start_action = MSG_DOD_WPN_SELECT_FG42;

		else if ((*(int *)p) == 24)  // is is a class selection axis menu for riflemen ?
			g_pGame->getBot(bot_index)->m_start_action = MSG_DOD_WPN_SELECT_RIFLEMAN;

		else if ((*(int *)p) == 25)  // is is a class selection axis menu for grenadiers ?
			g_pGame->getBot(bot_index)->m_start_action = MSG_DOD_WPN_SELECT_GRENADIER;
	}
}

// This message is sent when a menu is being displayed in Counter-Strike.
void BotClient_CS_ShowMenu(void *p, int bot_index,int iAdd){
	static int state = 0;   // current state machine state
	if(iAdd != _CLIENT_END){
		if (state < 3){
			state++;  // ignore first 3 fields of message
			return;
		}
		
		if (strcmp((char *)p, "#Team_Select") == 0){  // team select menu?
			g_pGame->getBot(bot_index)->m_start_action = MSG_CS_TEAM_SELECT;
		}
		else if (strcmp((char *)p, "#Terrorist_Select") == 0){  // T model select?
			g_pGame->getBot(bot_index)->m_start_action = MSG_CS_T_SELECT;
		}
		else if (strcmp((char *)p, "#CT_Select") == 0){  // CT model select menu?
			g_pGame->getBot(bot_index)->m_start_action = MSG_CS_CT_SELECT;
		}
		
		state = 0;  // reset state machine
	}
}


// This message is sent when a client joins the game.  All of the weapons
// are sent with the weapon ID and information about what ammo is used.
void BotClient_Valve_WeaponList(void *p, int bot_index,int iAdd){
	static int state = 0;   // current state machine state
	static bot_weapon_t bot_weapon;
	
	if(iAdd != _CLIENT_END){
		if (state == 0){
			bot_weapon.iAmmoClip = -1;		// just init

			state++;
			strcpy(bot_weapon.szClassname, (char *)p);
		}
		else if (state == 1){
			state++;
			bot_weapon.iAmmo1Id = *(int *)p;  // ammo index 1
		}
		else if (state == 2){
			state++;
			bot_weapon.iAmmo1Max = *(int *)p;  // max ammo1
		}
		else if (state == 3){
			state++;
			bot_weapon.iAmmo2Id = *(int *)p;  // ammo index 2
		}
		else if (state == 4){
			state++;
			bot_weapon.iAmmo2Max = *(int *)p;  // max ammo2
		}
		else if (state == 5){
			state++;
			bot_weapon.iSlot = *(int *)p;  // slot for this weapon
		}
		else if (state == 6){
			state++;
			bot_weapon.iPosition = *(int *)p;  // position in slot
		}
		else if (state == 7){
			state++;
			bot_weapon.iId = *(int *)p;  // weapon ID
		}
		else if (state == 8){
			bot_weapon.iFlags = *(int *)p;  // flags for weapon (WTF???)
			
			// store away this weapon with its ammo information...
			weapon_defs[bot_weapon.iId] = bot_weapon;


		/*	FILE *fhdl;
			fhdl = fopen("test.txt","a");
			fprintf(fhdl,"%i %s\n",bot_weapon.iId,bot_weapon.szClassname);
			fclose(fhdl);*/
			
			state = 0;
		}
	}
	else{
		state = 0;
	}
}

void BotClient_DOD_WeaponList(void *p, int bot_index, int iAdd){
	static int state = 0;   // current state machine state
	static bot_weapon_t bot_weapon;
	
	if(iAdd != _CLIENT_END){
		if (state == 0){
			state++;
			strcpy(bot_weapon.szClassname, (char *)p);
		}
		else if (state == 1){
			state++;
			bot_weapon.iAmmo1Id = *(int *)p;  // ammo index 1
		}
		else if (state == 2){
			state++;
			bot_weapon.iAmmo1Max = *(int *)p;  // max ammo1
		}
		else if (state == 3){
			state++;
			bot_weapon.iAmmo2Id = *(int *)p;  // ammo index 2
		}
		else if (state == 4){
			state++;
			bot_weapon.iAmmo2Max = *(int *)p;  // max ammo2
		}
		else if (state == 5){
			state++;
			bot_weapon.iSlot = *(int *)p;  // slot for this weapon
		}
		else if (state == 6){
			state++;
			bot_weapon.iPosition = *(int *)p;  // position in slot
		}
		else if (state == 7){
			state++;
			bot_weapon.iId = *(int *)p;  // weapon ID
		}
		else if (state == 8){
			state ++;
			bot_weapon.iFlags = *(int *)p;  // flags for weapon (WTF???)
		}
		else if (state == 9){
			bot_weapon.iAmmoClip = *(int *)p;  // clip size
			
			// store away this weapon with it's ammo information...
			weapon_defs[bot_weapon.iId] = bot_weapon;

			FILE *fhd;
			fhd = fopen("weapons.txt","a");
			fprintf(fhd,"%i %s %i\n",bot_weapon.iId,bot_weapon.szClassname,bot_weapon.iAmmoClip);
			fclose(fhd);
			
			state = 0;
		}
	}
}

void BotClient_CS_WeaponList(void *p, int bot_index,int iAdd){
	// this is just like the Valve Weapon List message
	BotClient_Valve_WeaponList(p, bot_index,iAdd);
}

void BotClient_Gearbox_WeaponList(void *p, int bot_index,int iAdd){
	// this is just like the Valve Weapon List message
	BotClient_Valve_WeaponList(p, bot_index,iAdd);
}


// This message is sent when a weapon is selected (either by the bot chosing
// a weapon or by the server auto assigning the bot a weapon).
void BotClient_Valve_CurrentWeapon(void *p, int bot_index,int iAdd){
	static int state = 0;   // current state machine state
	static int iState;
	static int iId;
	static int iClip;
	CBaseBot *pBot;
	
	if(iAdd != _CLIENT_END){
		if (state == 0){
			state++;
			iState = *(int *)p;  // state of the current weapon (WTF???)
		}
		else if (state == 1){
			state++;
			iId = *(int *)p;  // weapon ID of current weapon
		}
		else if (state == 2){
			state = 0;
			
			iClip = *(int *)p;  // ammo currently in the clip for this weapon
			
			if(bot_index != -1){
				if (iId <= 31)
				{
					pBot = g_pGame->getBot(bot_index);
					pBot->m_pWeapons->m_iWeapons |= (1<<iId);  // set this weapon bit
					
					if(pBot->m_pWeapons->getCWeaponID() != -1){
						pBot->m_pWeapons->m_piAmmoClip[pBot->m_pWeapons->getCWeaponID()] =
							pBot->m_pWeapons->m_iCurrentWClip;
					}
					
					if (iState == 1){
						pBot->m_pWeapons->m_iCurrentWId = iId;
						//cout << iId << endl;
						pBot->m_pWeapons->m_iCurrentWClip = iClip;
						
						// update the ammo counts for this weapon...
						pBot->m_pWeapons->m_iCurrentWAmmo1 =
							pBot->m_pWeapons->m_piAmmo[weapon_defs[iId].iAmmo1Id];
						pBot->m_pWeapons->m_iCurrentWAmmo2 =
							pBot->m_pWeapons->m_piAmmo[weapon_defs[iId].iAmmo2Id];
					}
				}
			}
			
			// let's tell all the other bots that a shot was fired
			int i,iWID;
			Vector VForward;
			Vector vecLOS;
			float    flDot;
			float fImpactDistance,fDistanceSquare;
			CPlayer *pPlayer = g_pGame->getPlayer(g_pGame->isPlayer(CEntity(g_pClientCallEdict)));

			if(pPlayer){
				//UTIL_MakeVectors ( pPlayer->getAngles() );
				//VForward = gpGlobals->v_forward;
				VForward = pPlayer->getAngles().angles2vec();
				iWID = g_pGame->weaponModel2ID(pPlayer->getWeaponModel());
				for(i=g_pGame->m_iMaxCurrentPlayer; i>=0; i--){
					if(i == bot_index)
						continue;
					
					if(pBot = g_pGame->getBot(i)){
						if(!pBot->isAlive())
							continue;
						CPerceipt *pNewPerceipt = pBot->m_pPerception->addPerceipt();
						
						pNewPerceipt->m_pPlayer = pPlayer;
						pNewPerceipt->m_VOrigin = pPlayer->getOrigin();
						pNewPerceipt->m_pEntity = CEntity(g_pClientCallEdict);
						pNewPerceipt->m_lType = CPerceipt::PT_SOUND;
						pNewPerceipt->m_fValue = pBot->m_pWeapons->getLoudness(iWID);
						pNewPerceipt->m_lTypeSpec |= (CPerceipt::PTX_SOUND_LOC|CPerceipt::PTX_SOUND_SHOOT);
						pNewPerceipt->m_fDistance = (pBot->getOrigin()-pNewPerceipt->m_VOrigin).length();
						
						if(pPlayer->getTeam() == pBot->m_iTeam){
							pNewPerceipt->m_lTypeSpec |= CPerceipt::PTX_SOUND_TMATE;
						}
						else{
							pNewPerceipt->m_lTypeSpec |= CPerceipt::PTX_SOUND_ENEMY;
						}

						vecLOS = ( pBot->getOrigin() - pPlayer->getOrigin());
						vecLOS = vecLOS.normalize();

						flDot = DotProduct (vecLOS , VForward );

						if(flDot > .5){		// 60° field of view
							pNewPerceipt->m_lTypeSpec |= CPerceipt::PTX_LOOKATME;
							// calculate distance to possible impact :D
							fDistanceSquare = pNewPerceipt->m_fDistance*pNewPerceipt->m_fDistance;
							fImpactDistance = sqrt(fDistanceSquare/(flDot*flDot) - fDistanceSquare);

							// if it may have been near to us, hear it. we do not actually try to trace a line e.g.
							// to see if the bullet could really have hit some epsilon sphere around us
							if(fImpactDistance < 200){
								pNewPerceipt->m_lTypeSpec |= CPerceipt::PTX_SHOOTATME;
							}
						}

						/*if ( flDot > 0.50 ){  // 60 degree field of view ( arccos .5 )
						return (1.f-flDot)* (255*2) + 1;
						}
						else{
							return 0;
						}*/

						/*iInFoV = 0;
						iInFoV = pBot->isInFOVof(pPlayer);
						if(iInFoV){
							pNewPerceipt->m_lTypeSpec |= CPerceipt::PTX_LOOKATME;
						}
						if(iInFoV>250){		// 9°
							pNewPerceipt->m_lTypeSpec |= CPerceipt::PTX_SHOOTATME;
						}*/
					}
				}
			}
		}
	}
}

void BotClient_CS_CurrentWeapon(void *p, int bot_index,int iAdd){
	// this is just like the Valve Current Weapon message
	BotClient_Valve_CurrentWeapon(p, bot_index,iAdd);
}

void BotClient_Gearbox_CurrentWeapon(void *p, int bot_index,int iAdd){
	// this is just like the Valve Current Weapon message
	BotClient_Valve_CurrentWeapon(p, bot_index,iAdd);
}

void BotClient_DOD_CurrentWeapon(void *p, int bot_index, int iAdd){
	// this is just like the Valve Current Weapon message
	BotClient_Valve_CurrentWeapon(p, bot_index,iAdd);
}

// This message is sent whenever ammo ammounts are adjusted (up or down).
void BotClient_Valve_AmmoX(void *p, int bot_index,int iAdd){
	static int state = 0;   // current state machine state
	static int index;
	static int ammount;
	int ammo_index;
	
	if(iAdd != _CLIENT_END){
		if (state == 0){
			state++;
			index = *(int *)p;  // ammo index (for type of ammo)
		}
		else if (state == 1){
			CBaseBot *pBot;
			
			ammount = *(int *)p;  // the ammount of ammo currently available
			
			pBot = g_pGame->getBot(bot_index);
			pBot->m_pWeapons->m_piAmmo[index] = ammount;  // store it away
			
			ammo_index = pBot->m_pWeapons->getCWeaponID();
			
			if(ammo_index != -1){
				// update the ammo counts for this weapon...
				pBot->m_pWeapons->m_iCurrentWAmmo1 =
					pBot->m_pWeapons->m_piAmmo[weapon_defs[ammo_index].iAmmo1Id];
				pBot->m_pWeapons->m_iCurrentWAmmo2 =
					pBot->m_pWeapons->m_piAmmo[weapon_defs[ammo_index].iAmmo2Id];
			}
			state = 0;
		}
	}
	else state = 0;
}

void BotClient_CS_AmmoX(void *p, int bot_index,int iAdd){
	// this is just like the Valve AmmoX message
	BotClient_Valve_AmmoX(p, bot_index,iAdd);
}

void BotClient_Gearbox_AmmoX(void *p, int bot_index,int iAdd){
	// this is just like the Valve AmmoX message
	BotClient_Valve_AmmoX(p, bot_index,iAdd);
}

void BotClient_DOD_AmmoX(void *p, int bot_index,int iAdd){
	// this is just like the Valve AmmoX message
	BotClient_Valve_AmmoX(p, bot_index,iAdd);
}

void BotClient_Valve_SayText(void *p, int bot_index,int iAdd){
	static int state = 0;   // current state machine state
	static short int index;
	static char *szText;
	
	if(iAdd != _CLIENT_END){
		if (state == 0){
			state++;
			index = *(short *)p;
		}
		else if (state == 1){
			/*char *szDP;
			szText = (char *)p;

			char *szSayText;
			edict_t *pSayEdict;

			pSayEdict = INDEXENT(index);
			if(pSayEdict && IsAlive(pSayEdict)){
				szSayText = szLastSayText;
				iLastSayClient = index;
			}
			else{
				szSayText = szDeadLastSayText;
				iDeadLastSayClient = index;
			}

			strcpy(szSayText,szText);

			szDP = strchr(szSayText,':');

			if(szDP  < szSayText + sizeof(char) * 80 )
				memset(szSayText,' ',szDP - szSayText);

			// make it lowercase;
			UTIL_strlwr(szSayText);
			
			//cout << szSayText;
			*/
			state = 0;
		}
	}
}

void BotClient_CS_SayText(void *p, int bot_index,int iAdd){
	// this is just like the Valve SayText message
	BotClient_Valve_SayText(p, bot_index,iAdd);
}

void BotClient_Gearbox_SayText(void *p, int bot_index,int iAdd){
	// this is just like the Valve SayText message
	BotClient_Valve_SayText(p, bot_index,iAdd);
}

void BotClient_DOD_SayText(void *p, int bot_index,int iAdd){
	// this is just like the Valve SayText message
	BotClient_Valve_SayText(p, bot_index,iAdd);
}

// This message is sent when the bot picks up some ammo (AmmoX messages are
// also sent so this message is probably not really necessary except it
// allows the HUD to draw pictures of ammo that have been picked up.  The
// bots don't really need pictures since they don't have any eyes anyway.
void BotClient_Valve_AmmoPickup(void *p, int bot_index,int iAdd){
	static int state = 0;   // current state machine state
	static int index;
	static int ammount;
	int ammo_index;
	
	if(iAdd != _CLIENT_END){
		if (state == 0){
			state++;
			index = *(int *)p;
		}
		else if (state == 1){
			CBaseBot *pBot;
			ammount = *(int *)p;
			
			pBot = g_pGame->getBot(bot_index);
			pBot->m_pWeapons->m_piAmmo[index] = ammount;
			
			ammo_index = pBot->m_pWeapons->getCWeaponID();
			
			// update the ammo counts for this weapon...
			pBot->m_pWeapons->m_iCurrentWAmmo1 =
				pBot->m_pWeapons->m_piAmmo[weapon_defs[ammo_index].iAmmo1Id];
			pBot->m_pWeapons->m_iCurrentWAmmo2 =
				pBot->m_pWeapons->m_piAmmo[weapon_defs[ammo_index].iAmmo2Id];
			
			state = 0;
		}
	}
}

void BotClient_CS_AmmoPickup(void *p, int bot_index,int iAdd){
	// this is just like the Valve Ammo Pickup message
	BotClient_Valve_AmmoPickup(p, bot_index,iAdd);
}

void BotClient_Gearbox_AmmoPickup(void *p, int bot_index,int iAdd){
	// this is just like the Valve Ammo Pickup message
	BotClient_Valve_AmmoPickup(p, bot_index,iAdd);
}

void BotClient_DOD_AmmoPickup(void *p, int bot_index,int iAdd){
	// this is just like the Valve Ammo Pickup message
	BotClient_Valve_AmmoPickup(p, bot_index,iAdd);
}

// This message gets sent when the bot picks up a weapon.
void BotClient_Valve_WeaponPickup(void *p, int bot_index,int iAdd){
	int index;
	
	if(iAdd != _CLIENT_END){
		index = *(int *)p;
		
		// set this weapon bit to indicate that we are carrying this weapon
		g_pGame->getBot(bot_index)->m_pWeapons->m_iWeapons |= (1<<index);
	}
}

void BotClient_CS_WeaponPickup(void *p, int bot_index,int iAdd){
	// this is just like the Valve Weapon Pickup message
	BotClient_Valve_WeaponPickup(p, bot_index,iAdd);
}

void BotClient_Gearbox_WeaponPickup(void *p, int bot_index,int iAdd){
	// this is just like the Valve Weapon Pickup message
	BotClient_Valve_WeaponPickup(p, bot_index,iAdd);
}

void BotClient_DOD_WeaponPickup(void *p, int bot_index,int iAdd){
	// this is just like the Valve Weapon Pickup message
	BotClient_Valve_WeaponPickup(p, bot_index,iAdd);
}

// This message gets sent when the bot picks up an item (like a battery
// or a healthkit)
void BotClient_Valve_ItemPickup(void *p, int bot_index,int iAdd){
	if(iAdd != _CLIENT_END){
	}
}

void BotClient_CS_ItemPickup(void *p, int bot_index,int iAdd){
	// this is just like the Valve Item Pickup message
	BotClient_Valve_ItemPickup(p, bot_index,iAdd);
}

void BotClient_Gearbox_ItemPickup(void *p, int bot_index,int iAdd){
	// this is just like the Valve Item Pickup message
	BotClient_Valve_ItemPickup(p, bot_index,iAdd);
}

void BotClient_DOD_ItemPickup(void *p, int bot_index,int iAdd){
	// this is just like the Valve Item Pickup message
	BotClient_Valve_ItemPickup(p, bot_index,iAdd);
}

// This message gets sent when the bots health changes.
void BotClient_Valve_Health(void *p, int bot_index,int iAdd){
	if(iAdd != _CLIENT_END){
		g_pGame->getBot(bot_index)->m_iHealth = *(int *)p;  // health ammount
	}
}

void BotClient_CS_Health(void *p, int bot_index,int iAdd){
	// this is just like the Valve Health message
	BotClient_Valve_Health(p, bot_index,iAdd);
}

void BotClient_Gearbox_Health(void *p, int bot_index,int iAdd){
	// this is just like the Valve Health message
	BotClient_Valve_Health(p, bot_index,iAdd);
}

void BotClient_DOD_Health(void *p, int bot_index,int iAdd){
	// this is just like the Valve Health message
	BotClient_Valve_Health(p, bot_index,iAdd);
}

// This message gets sent when the bots armor changes.
void BotClient_Valve_Battery(void *p, int bot_index,int iAdd){
	if(iAdd != _CLIENT_END){
		g_pGame->getBot(bot_index)->m_iArmor = *(int *)p;  // armor ammount
	}
}

void BotClient_CS_Battery(void *p, int bot_index,int iAdd){
	// this is just like the Valve Battery message
	BotClient_Valve_Battery(p, bot_index,iAdd);
}

void BotClient_Gearbox_Battery(void *p, int bot_index,int iAdd){
	// this is just like the Valve Battery message
	BotClient_Valve_Battery(p, bot_index,iAdd);
}

void BotClient_DOD_Battery(void *p, int bot_index,int iAdd){
	// this is just like the Valve Battery message
	BotClient_Valve_Battery(p, bot_index,iAdd);
}

// This message gets sent when the bots are getting damaged.
void BotClient_Valve_Damage(void *p, int bot_index,int iAdd){
	static int state = 0;   // current state machine state
	static int damage_armor;
	static int damage_taken;
	static int damage_bits;  // type of damage being done
	static Vector damage_origin;
	
	if(iAdd != _CLIENT_END){
		if (state == 0){
			state++;
			damage_armor = *(int *)p;
		}
		else if (state == 1){
			state++;
			damage_taken = *(int *)p;
		}
		else if (state == 2){
			state++;
			damage_bits = *(int *)p;
		}
		else if (state == 3){
			state++;
			damage_origin.x = *(float *)p;
		}
		else if (state == 4){
			state++;
			damage_origin.y = *(float *)p;
		}
		else if (state == 5){
			damage_origin.z = *(float *)p;
			state = 0;
			
			if(bot_index != -1){
				CBaseBot *pBot = g_pGame->getBot(bot_index);

				if ((damage_armor > 0) || (damage_taken > 0)){
					/*if ((damage_bits & (DMG_FALL | DMG_CRUSH)){  
					// bot received damage by falling down

					}*/
					// ignore certain types of damage...
					if (damage_bits & IGNORE_DAMAGE){
						// let the bot 'feel' something strange is going on ... health is going down, m8 !
						CPerceipt *pNewPerceipt = pBot->m_pPerception->addPerceipt();

						pNewPerceipt->m_VOrigin = damage_origin;
						pNewPerceipt->m_lType = CPerceipt::PT_DAMAGE;
						pNewPerceipt->m_lTypeSpec |= CPerceipt::PTX_DAMAGE_STRANGE;
						pNewPerceipt->m_fDistance = (pBot->getOrigin()-damage_origin).length();
						pNewPerceipt->m_iAddInfo = damage_bits;
					}
					else{
						// let the bot 'feel' that he's attacked :)
						CPerceipt *pNewPerceipt = pBot->m_pPerception->addPerceipt();

						pNewPerceipt->m_VOrigin = damage_origin;
						pNewPerceipt->m_lType = CPerceipt::PT_DAMAGE;
						pNewPerceipt->m_fDistance = (pBot->getOrigin()-damage_origin).length();
						pNewPerceipt->m_iAddInfo = damage_bits;

						g_Map.m_Waypoints.addDamage(pBot->getOrigin(),damage_origin,damage_armor + damage_taken);
					}
				}
			}
			else{
				if ((damage_armor > 0) || (damage_taken > 0))
				{
					// ignore certain types of damage...
					if (damage_bits & IGNORE_DAMAGE)
						return;

					g_Map.m_Waypoints.addDamage(g_pClientCallEdict->v.origin,damage_origin,damage_armor + damage_taken);
				}
			}
		}
	}
}

void BotClient_CS_Damage(void *p, int bot_index,int iAdd){
	// this is just like the Valve Battery message
	BotClient_Valve_Damage(p, bot_index,iAdd);
}

void BotClient_Gearbox_Damage(void *p, int bot_index,int iAdd){
	// this is just like the Valve Battery message
	BotClient_Valve_Damage(p, bot_index,iAdd);
}

void BotClient_DOD_Damage(void *p, int bot_index,int iAdd){
	// this is just like the Valve Battery message
	BotClient_Valve_Damage(p, bot_index,iAdd);
}

// This message gets sent when the bots money ammount changes (for CS)
void BotClient_CS_Money(void *p, int bot_index,int iAdd){
	static int state = 0;   // current state machine state
	if(iAdd != _CLIENT_END){
		if (state == 0){
			state ++;
			
			((CBotCS *)(g_pGame->getBot(bot_index)))->m_iMoney = *(int *)p;  // amount of money
		}
		else{
			state = 0;  // ingore this field
		}
	}
	else{
		state = 0;  // ingore this field
	}
}

void BotClient_CS_StatusI(void *p, int bot_index,int iAdd){
	static int state = 0;   // current state machine state
	static int iTemp;
	static char szText[80];
	
	if(iAdd != _CLIENT_END){
		if(state == 0){
			iTemp = *(int*)p;
			state ++;
		}
		else if (state == 1){
			state ++;

			strcpy(szText,(char*)p);
		}
	}
	else{
		if(!strcmp(szText,"buyzone")){
			((CBotCS*)(g_pGame->getBot(bot_index)))->m_bInBuyZone = (iTemp==1);
		}
		if(!strcmp(szText,"defuser")){
			((CBotCS*)(g_pGame->getBot(bot_index)))->m_bHasDefuser = (iTemp==1);
		}
		if(!strcmp(szText,"c4")){
			((CBotCS*)(g_pGame->getBot(bot_index)))->m_bInBombZone = (iTemp==2);
		}
		
		state = 0;
	}
}

void BotClient_CS_TeamInfo(void *p, int bot_index,int iAdd){
	static int state = 0;   // current state machine state
	static int iTemp;
	static char szText[80];
	
	if(iAdd != _CLIENT_END){
		if(state == 0){
			iTemp = *(int*)p;
			state ++;
		}
		else if (state == 1){
			state ++;

			/*if(!strcmp((char*)p,"TERRORIST")){
				g_pGame->getBot(bot_index)->m_iTeam = TE;
			}
			else if(!strcmp((char*)p,"CT")){
				g_pGame->getBot(bot_index)->m_iTeam = CT;
			}
			else if(!strcmp((char*)p,"UNASSIGNED")){
				g_pGame->getBot(bot_index)->m_iTeam = -1;
			}*/

			g_pGame->getBot(bot_index)->assignTeam(g_pGame->getBot(bot_index)->getTeam());
		}
	}
	else{
		state = 0;
	}
}

void BotClient_DOD_TeamInfo(void *p, int bot_index,int iAdd){
	static int state = 0;   // current state machine state
	static short sTemp;
	static char szText[80];
	
	if(iAdd != _CLIENT_END){
		if(state == 0){
			sTemp = *(short*)p;
			state ++;
		}
		else if (state == 1){
			state  = 0;
			g_pGame->getBot(bot_index)->m_iTeam = *(int*)p;
		}
	}
	else{
		state = 0;
	}
}

void BotClient_CS_Roundtime(void *p, int bot_index,int iAdd){
	static int state = 0;   // current state machine state
	
	if(iAdd != _CLIENT_END){
		if (state == 0){
			state ++;
			
			((CBotCS*)(g_pGame->getBot(bot_index)))->m_fRoundTime = *(short *)p;  // roundtime in min
		}
		else{
			state = 0;  // ingore this field
		}
	}
	else{
		state = 0;
	}
}

void BotClient_CS_StatusV(void *p, int bot_index,int iAdd){						// not yet impl
	static int state = 0;   // current state machine state
	
	if(iAdd != _CLIENT_END){
		if (state == 0){
			state ++;
		}
		else if (state == 1){
			state ++;
		}
		else{
			state = 0;  // ingore this field
		}
	}
	else{
		state = 0;
	}
}

void BotClient_CS_SetFOV(void *p, int bot_index,int iAdd){
	BotClient_Valve_SetFOV(p,bot_index,iAdd);
}

void BotClient_DOD_SetFOV(void *p, int bot_index,int iAdd){
	BotClient_Valve_SetFOV(p,bot_index,iAdd);
}

void BotClient_Valve_SetFOV(void *p, int bot_index,int iAdd){
	static int state = 0;   // current state machine state
	
	if(iAdd != _CLIENT_END){
		if (state == 0){
			g_pGame->getBot(bot_index)->m_iFOV = *(int*)(p);
			state ++;
			//cout << g_pGame->getBot(bot_index)->i_FOV << endl;
		}
		else{
			state = 0;  // ingore this field
		}
	}
	else{
		state = 0;
	}
}

void BotClient_CS_TextMsg(void *p, int bot_index,int iAdd){
	static int state = 0;   // current state machine state
	//static int sNumber = 0;
	static char szStrings[10][32]={"","","","","","","","","",""};
	// assuming 10 is max sentences
	if(iAdd != _CLIENT_END){
		if (state == 0){
			//sNumber = *(int*)p;
			//sNumber++;
			state ++;				// ignore this field
		}
		else{
			strcpy(szStrings[state-1],(char*)p);		// copy string
#ifdef __LOG
			FILE *fhd;fhd=fopen("gotaradiomess.txt","a");fprintf(fhd,"%s\n",szStrings[state -1]);fclose(fhd);
#endif
			state ++;
		}
	}
	else{
#ifdef __LOG
		FILE *fhd;fhd=fopen("gotaradiomess.txt","a");fprintf(fhd,"--------------------");fclose(fhd);
#endif
		if(strcmp(szStrings[0],"#Switch_To_BurstFire") == 0){
			((CBotCS*)(g_pGame->getBot(bot_index)))->m_bGlockBurst = true;
		}
		if(strcmp(szStrings[0],"#Switch_To_SemiAuto") == 0){
			((CBotCS*)(g_pGame->getBot(bot_index)))->m_bGlockBurst = false;
		}
		else if(strcmp(szStrings[0],"#Bomb_Planted") == 0){
//			g_bBombPlanted = true;
//			g_iBombExplode = int(gpGlobals->time + CVAR_GET_FLOAT("mp_c4timer"));
		}
		else if(strcmp(szStrings[0],"#Game_bomb_drop") == 0){
//			g_bBombDropped = true;
		}
		else if(strcmp(szStrings[0],"#Got_bomb") == 0){
//			g_bBombDropped = false;
		}
		else if(strcmp(szStrings[0],"#Terrorists_Win") == 0){
//			g_bBombPlanted = false;
		}
		else if(strcmp(szStrings[0],"#Bomb_Defused") == 0){
//			g_bBombPlanted = false;
		}
		else if(strcmp(szStrings[0],"#CTs_Win") == 0){
//			g_bBombPlanted = false;
		}
		else if(strcmp(szStrings[0],"#Game_Commencing") == 0){
//			g_fGameCommenced = gpGlobals->time;
		}
		else if(strcmp(szStrings[0],"#Cant_buy") == 0){
//			((CBotCS*)(g_pGame->getBot(bot_index)))->f_buytime = gpGlobals->time + 60.0;			// just to avoid too many buy trials
		}
		else if(strcmp(szStrings[0],"#Game_teammate_attack") == 0){
			// get edict of radio using player
/*			edict_t *pEnt = 0;
			CBotCS *pBot = ((CBotCS*)(g_pGame->getBot(bot_index)));
			edict_t *pEdict = pBot->pEdict;
			int i;
			
			pBot->bot_IRadioM.pECalling = 0;		// resetting calling edict
			pBot->bot_IRadioM.lMessage = 0;
			
			for (i = gpGlobals->maxClients; i ; i--){
				pEnt = INDEXENT(i);
				
				// skip invalid players and skip self (i.e. this bot)
				if ((pEnt) && (!pEnt->free) && (pEnt != pEdict))
				{
					if(!IsAlive(pEnt))
						continue;
					if(strcmp(szStrings[1],STRING(pEnt->v.netname)) == 0){// if found ent with that name
						pBot->bot_IRadioM.pECalling = pEnt;					// copy pointer to ent
						
						pBot->bot_IRadioM.f_Time = gpGlobals->time + RANDOM_FLOAT(1.0,2.5);		// this random stuff for faking kind of reaction time to radio commands
						
						pBot->bot_IRadioM.lMessage = RC_TEAMATTACK;
					}
				}
			}*/
		}
		else if(strcmp(szStrings[0],"#Game_radio") == 0){
			// get edict of radio using player
			//edict_t *pEnt = 0;
			CPlayer *pPlayer;
			CBotCS *pBot = ((CBotCS*)(g_pGame->getBot(bot_index)));
			//edict_t *pEdict = pBot->getEntity();
			int i;
			
			//f_LastRadio = gpGlobals->time;			// set last time a radio command was used
			
			pBot->m_Radio.pPCalling = 0;		// resetting calling player
			pBot->m_Radio.lMessage = 0;
			
			for (i = g_pGame->getMaxCurrentPlayer(); i ; i--){
				pPlayer = g_pGame->getPlayer(i);
				
				// skip invalid players and skip self (i.e. this bot)
				if ((pPlayer) /*&& (!pEnt->free)*/ && ((CPlayer*)pPlayer != (CPlayer*)pBot))
				{
					if(!pPlayer->isAlive())
						continue;
					if(strcmp(szStrings[1],pPlayer->getName()) == 0){// if found ent with that name
						//pBot->m_Radio.pECalling = pPlayer->getEntity;					// copy pointer to ent
						pBot->m_Radio.pPCalling = pPlayer;					// copy pointer to player
						
						pBot->m_Radio.f_Time = gpGlobals->time + RANDOM_FLOAT(.75,2.0);		// this random stuff for faking kind of reaction time to radio commands
						
						if(!strcmp(szStrings[2],"#Cover_me")){
							pBot->m_Radio.lMessage = RC_COVER_ME;
						}
						else if(!strcmp(szStrings[2],"#You_take_the_point")){
							pBot->m_Radio.lMessage = RC_YOU_TAKE_THE_POINT;
						}
						else if(!strcmp(szStrings[2],"#Hold_this_position")){
							pBot->m_Radio.lMessage = RC_HOLD_THIS_POSITION;
						}
						else if(!strcmp(szStrings[2],"#Regroup_team")){
							pBot->m_Radio.lMessage = RC_REGROUP_TEAM;
						}
						else if(!strcmp(szStrings[2],"#Follow_me")){
							pBot->m_Radio.lMessage = RC_FOLLOW_ME;
						}
						else if(!strcmp(szStrings[2],"#Taking_fire")){
							pBot->m_Radio.lMessage = RC_TAKING_FIRE;
						}
						else if(!strcmp(szStrings[2],"#Go_go_go")){
							pBot->m_Radio.lMessage = RC_GO_GO_GO;
						}
						else if(!strcmp(szStrings[2],"#Team_fall_back")){
							pBot->m_Radio.lMessage = RC_FALL_BACK;
						}
						else if(!strcmp(szStrings[2],"#Stick_together_team")){
							pBot->m_Radio.lMessage = RC_STICK_TOGETHER;
						}
						else if(!strcmp(szStrings[2],"#Get_in_position_and_wait")){
							pBot->m_Radio.lMessage = RC_GET_IN_POSITION;
						}
						else if(!strcmp(szStrings[2],"#Storm_the_front")){
							pBot->m_Radio.lMessage = RC_STORM_THE_FRONT;
						}
						else if(!strcmp(szStrings[2],"#Report_in_team")){
							pBot->m_Radio.lMessage = RC_REPORT_IN_TEAM;
						}
						else if(!strcmp(szStrings[2],"#Affirmative")||!strcmp(szStrings[2],"#Roger_that")){
							pBot->m_Radio.lMessage = RC_AFFIRMATIVE;
						}
						else if(!strcmp(szStrings[2],"#Enemy_spotted")){
							pBot->m_Radio.lMessage = RC_ENEMY_SPOTTED;
						}
						else if(!strcmp(szStrings[2],"#Need_backup")){
							pBot->m_Radio.lMessage = RC_NEED_BACKUP;
						}
						else if(!strcmp(szStrings[2],"#Sector_clear")){
							pBot->m_Radio.lMessage = RC_SECTOR_CLEAR;
						}
						else if(!strcmp(szStrings[2],"#In_position")){
							pBot->m_Radio.lMessage = RC_IN_POSITION;
						}
						else if(!strcmp(szStrings[2],"#Reporting_in")){
							pBot->m_Radio.lMessage = RC_REPORTING_IN;
						}
						else if(!strcmp(szStrings[2],"#Get_out_of_there")){
							pBot->m_Radio.lMessage = RC_GET_OUT_OF_THERE;
						}
						else if(!strcmp(szStrings[2],"#Negative")){
							pBot->m_Radio.lMessage = RC_NEGATIVE;
						}
						else if(!strcmp(szStrings[2],"#Enemy_down")){
							pBot->m_Radio.lMessage = RC_ENEMY_DOWN;
						}
						else if(!strcmp(szStrings[2],"#Fire_in_the_hole")){
							pBot->m_Radio.lMessage = RC_FIRE_IN_THE_HOLE;
						}
						else if(!strcmp(szStrings[2],"#Hostage_down")){
							pBot->m_Radio.lMessage = RC_HOSTAGE_DOWN;
						}
						else{
							pBot->m_Radio.lMessage = 0;
							FILE *fhd;fhd=fopen("gotaranunknowndiomess.txt","a");fprintf(fhd,"%s\n",szStrings[2]);fclose(fhd);
						}
						break;
					}
				}
			}
		}
		state = 0;			// reset state machine state
		memset(szStrings,0,sizeof(char) * 10 * 32);
		return;
	}
}

void BotClient_Valve_ScreenFade(void *p, int bot_index,int iAdd){
	static int state = 0;   // current state machine state
	static int duration;
	static int hold_time;
	static int fade_flags;
	int length;
	
	if(iAdd != _CLIENT_END){
		if (state == 0)
		{
			state++;
			duration = *(int *)p;
		}
		else if (state == 1)
		{
			state++;
			hold_time = *(int *)p;
		}
		else if (state == 2)
		{
			state++;
			fade_flags = *(int *)p;
		}
		else if (state == 6)
		{
			CBaseBot *pBot;
			state = 0;
			
			length = (duration + hold_time) / 4096;
			if(pBot = g_pGame->getBot(bot_index))
				pBot->m_fBlinded = g_pGame->getTime() + length*2/3 - 1;
		}
		else
		{
			state++;
		}
	}
}

void BotClient_CS_ScreenFade(void *p, int bot_index,int iAdd){
	// this is just like the Valve ScreenFade message
	BotClient_Valve_ScreenFade(p, bot_index,iAdd);
}

void BotClient_DOD_ScreenFade(void *p, int bot_index,int iAdd){
	// this is just like the Valve ScreenFade message
	BotClient_Valve_ScreenFade(p, bot_index,iAdd);
}

// This message gets sent when the bots get killed
void BotClient_Valve_DeathMsg(void *p, int bot_index,int iAdd){
	static int state = 0;   // current state machine state
	static int killer_index;
	static int victim_index;
	static int iHeadShot;
	static edict_t *victim_edict=0,
				*killer_edict=0;
	static int killer_bot_index,victim_bot_index,killer_team,victim_team;
	
	if(iAdd != _CLIENT_END){
		if (state == 0)
		{
			state++;
			killer_index = *(int *)p;  // ENTINDEX() of killer
		}
		else if (state == 1)
		{
			state++;
			victim_index = *(int *)p;  // ENTINDEX() of victim
		}
		else if (state == 2)
		{
			state ++;
			iHeadShot = *(int*)p;		// true when headshot
		}
		else if( state == 3){		// (char*)p would be name of weapon
			//cout << (char*)p << endl;
			char szWeaponclass[32];
			long lKWeapon = -1;
			strcpy(szWeaponclass,"weapon_");
			strcat(szWeaponclass,(char*)p);
			lKWeapon = g_pGame->weaponClass2ID(szWeaponclass);

			state = 0;
			
			if(victim_index >= 0 && victim_index < 32)victim_edict = INDEXENT(victim_index);
			else victim_edict = 0;
			if(victim_index >= 0 && victim_index < 32)killer_edict = INDEXENT(killer_index);
			else killer_edict = 0;

			victim_bot_index = g_pGame->isBot(victim_edict);
			killer_bot_index = g_pGame->isBot(killer_edict);
			
			//index = UTIL_GetBotIndex(victim_edict);

			// is this message about a bot being killed?
			
			if ((killer_index == 0) || (killer_index == victim_index))
			{
				// bot killed by world (worldspawn) or bot killed self...
				if (victim_bot_index != -1){
				}
			}
			else
			{
				// notice bot that has been killed
				if (victim_bot_index != -1){
					g_pGame->getBot(victim_bot_index)->killed(killer_edict,lKWeapon);
				}
				// store information to killer
				
				if(killer_bot_index != -1){
					g_pGame->getBot(killer_bot_index)->kill(victim_edict,lKWeapon);
				}
			}
			
			if(killer_edict != victim_edict){
				// do some stuff for statistics
				//WPStat.AddKill(killer_edict,victim_edict);
				g_Map.m_Waypoints.addKill(victim_edict->v.origin);
			}
			// add this kill as an perception to all bots
			
			CPerceipt *pPerc,OPerc;
			if(victim_edict)
				OPerc.m_VOrigin = victim_edict->v.origin;
			OPerc.m_lType = CPerceipt::PT_KILL;
			OPerc.m_pEntity = victim_edict;
			OPerc.m_pPointer = killer_edict;

			int i;
			CBaseBot *pBot;
			
			for(i=g_pGame->m_iMaxCurrentPlayer-1;i>=0;i--){
				pBot = g_pGame->getBot(i);
				if(pBot){
					pPerc = pBot->m_pPerception->addPerceipt();
					
					pPerc->m_VOrigin = OPerc.m_VOrigin;
					pPerc->m_lType = OPerc.m_lType;
					pPerc->m_pEntity = OPerc.m_pEntity;
					pPerc->m_iAddInfo = OPerc.m_iAddInfo;
					
					pPerc->m_fDistance = (OPerc.m_VOrigin - pBot->getOrigin()).length();
				}
			}
		}
	}
	else
		state = 0;	
}

void BotClient_CS_DeathMsg(void *p, int bot_index,int iAdd){
	// this is just like the Valve DeathMsg message
	BotClient_Valve_DeathMsg(p, bot_index,iAdd);
}

void BotClient_DOD_DeathMsg(void *p, int bot_index,int iAdd){
	//cout << "nak" << endl;
   	static int state = 0;   // current state machine state
	static int killer_index;
	static int victim_index;
	static edict_t *victim_edict,*killer_edict;
	static int killer_bot_index,victim_bot_index,killer_team,victim_team;
	
	if(iAdd != _CLIENT_END){
		if (state == 0)
		{
			state++;
			killer_index = *(int *)p;  // ENTINDEX() of killer
		}
		else if (state == 1)
		{
			state++;
			victim_index = *(int *)p;  // ENTINDEX() of victim
		}
		else if( state == 2){		// (char*)p would be name of weapon
			//cout << (char*)p << endl;
			char szWeaponclass[32];
			long lKWeapon = -1;
			strcpy(szWeaponclass,"weapon_");
			strcat(szWeaponclass,(char*)p);
			lKWeapon = g_pGame->weaponClass2ID(szWeaponclass);

			state = 0;
			
			if(victim_index >= 0 && victim_index < 32)victim_edict = INDEXENT(victim_index);
			else victim_edict = 0;
			if(victim_index >= 0 && victim_index < 32)killer_edict = INDEXENT(killer_index);
			else killer_edict = 0;

			victim_bot_index = g_pGame->isBot(victim_edict);
			killer_bot_index = g_pGame->isBot(killer_edict);
			
			//index = UTIL_GetBotIndex(victim_edict);

			// is this message about a bot being killed?
			
			if ((killer_index == 0) || (killer_index == victim_index))
			{
				// bot killed by world (worldspawn) or bot killed self...
				if (victim_bot_index != -1){
				}
			}
			else
			{
				// notice bot that has been killed
				if (victim_bot_index != -1){
					g_pGame->getBot(victim_bot_index)->killed(killer_edict,lKWeapon);
				}
				// store information to killer
				
				if(killer_bot_index != -1){
					g_pGame->getBot(killer_bot_index)->kill(victim_edict,lKWeapon);
				}
			}
			
			if(killer_edict != victim_edict){
				// do some stuff for statistics
				//WPStat.AddKill(killer_edict,victim_edict);
				g_Map.m_Waypoints.addKill(victim_edict->v.origin);
			}
		}
	}
	else
		state = 0;
}

void BotClient_DOD_Stamina(void *p, int bot_index,int iAdd){
	static int state = 0;   // current state machine state
	if(iAdd != _CLIENT_END){
		if (state == 0){
			state ++;
			
			((CBotDOD * )(g_pGame->getBot(bot_index)))->m_iStamina = *(int *)p;
		}
		else{
			state = 0;  // ingore this field
		}
	}
}

void BotClient_DOD_Speed(void *p, int bot_index,int iAdd){
	static int state = 0;   // current state machine state
	if(iAdd != _CLIENT_END){
		if (state == 0){
			state ++;
			
			g_pGame->getBot(bot_index)->m_pAction->setMaxSpeed(*(int *)p);
		}
		else{
			state = 0;  // ingore this field
		}
	}
	else{
		state = 0;
	}
}

void BotClient_DOD_Slowed(void *p, int bot_index,int iAdd){
	static int state = 0;   // current state machine state
	if(iAdd != _CLIENT_END){
		if (state == 0){
			state ++;
			
			((CBotDOD * )(g_pGame->getBot(bot_index)))->m_bSlowed = *(bool *)p;
		}
		else{
			state = 0;  // ingore this field
		}
	}
	else{
		state = 0;
	}
}

void BotClient_DOD_Bleeding(void *p, int bot_index,int iAdd){
	static int state = 0;   // current state machine state
	if(iAdd != _CLIENT_END){
		if (state == 0){
			state ++;
			
			((CBotDOD *)(g_pGame->getBot(bot_index)))->m_bBleeding = *(bool *)p;
		}
		else{
			state = 0;  // ingore this field
		}
	}
	else{
		state = 0;
	}
}

void BotClient_DOD_Object(void *p, int bot_index,int iAdd){
	/*static int state = 0;   // current state machine state
	static char szString[80];

	if(iAdd != _CLIENT_END){
		if (state == 0){
			strcpy(szString,(char *)p);
			if(!strcmp(szString,"sprites/obj_tnt.spr")){
				((CBotDOD * )(g_pGame->getBot(bot_index)))->bTNT = true;
			}
		}
	}
	else{
		state = 0;
	}*/
}

void BotClient_CS_Flashlight(void *p, int bot_index,int iAdd){
	static int state = 0;   // current state machine state
	static bool bFlashlight;
	static int iStrength;
	if(iAdd != _CLIENT_END){
		if (state == 0){
			state ++;
			
			bFlashlight = *(bool*)p;
		}
		if (state == 1){
			state ++;
			
			iStrength = *(int*)p;
			
			g_pGame->getBot(bot_index)->m_bFlashlight = bFlashlight;
		}
	}
	else{
		state = 0;
	}
}

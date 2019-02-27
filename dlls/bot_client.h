//
// HPB_bot - botman's High Ping Bastard bot
//
// (http://planethalflife.com/botman/)
//
// bot_client.h
//

// extended func call var defs
#define _CLIENT_BYTE	1
#define _CLIENT_CHAR	2
#define _CLIENT_SHORT	3
#define _CLIENT_LONG	4
#define _CLIENT_ANGLE	5
#define _CLIENT_COORD	6
#define _CLIENT_STRING	7
#define _CLIENT_ENTITY	8

#define _CLIENT_END		-1

//void BotClient_TFC_VGUI(void *p, int bot_index);
void BotClient_CS_VGUI(void *p, int bot_index,int iAdd);
void BotClient_CS_ShowMenu(void *p, int bot_index,int iAdd);

void BotClient_Valve_WeaponList(void *p, int bot_index,int iAdd);
//void BotClient_TFC_WeaponList(void *p, int bot_index);
void BotClient_CS_WeaponList(void *p, int bot_index,int iAdd);
void BotClient_Gearbox_WeaponList(void *p, int bot_index,int iAdd);

void BotClient_Valve_CurrentWeapon(void *p, int bot_index,int iAdd);
//void BotClient_TFC_CurrentWeapon(void *p, int bot_index);
void BotClient_CS_CurrentWeapon(void *p, int bot_index,int iAdd);
void BotClient_Gearbox_CurrentWeapon(void *p, int bot_index,int iAdd);

void BotClient_Valve_AmmoX(void *p, int bot_index,int iAdd);
//void BotClient_TFC_AmmoX(void *p, int bot_index,int iAdd);
void BotClient_CS_AmmoX(void *p, int bot_index,int iAdd);
void BotClient_Gearbox_AmmoX(void *p, int bot_index,int iAdd);

void BotClient_Valve_SayText(void *p, int bot_index,int iAdd);
//void BotClient_TFC_SayText(void *p, int bot_index,int iAdd);
void BotClient_CS_SayText(void *p, int bot_index,int iAdd);
void BotClient_Gearbox_SayText(void *p, int bot_index,int iAdd);

void BotClient_Valve_AmmoPickup(void *p, int bot_index,int iAdd);
//void BotClient_TFC_AmmoPickup(void *p, int bot_index);
void BotClient_CS_AmmoPickup(void *p, int bot_index,int iAdd);
void BotClient_Gearbox_AmmoPickup(void *p, int bot_index,int iAdd);

void BotClient_Valve_WeaponPickup(void *p, int bot_index,int iAdd);
//void BotClient_TFC_WeaponPickup(void *p, int bot_index);
void BotClient_CS_WeaponPickup(void *p, int bot_index,int iAdd);
void BotClient_Gearbox_WeaponPickup(void *p, int bot_index,int iAdd);

void BotClient_Valve_ItemPickup(void *p, int bot_index,int iAdd);
//void BotClient_TFC_ItemPickup(void *p, int bot_index);
void BotClient_CS_ItemPickup(void *p, int bot_index,int iAdd);
void BotClient_Gearbox_ItemPickup(void *p, int bot_index,int iAdd);

void BotClient_Valve_Health(void *p, int bot_index,int iAdd);
//void BotClient_TFC_Health(void *p, int bot_index);
void BotClient_CS_Health(void *p, int bot_index,int iAdd);
void BotClient_Gearbox_Health(void *p, int bot_index,int iAdd);

void BotClient_Valve_Battery(void *p, int bot_index,int iAdd);
//void BotClient_TFC_Battery(void *p, int bot_index);
void BotClient_CS_Battery(void *p, int bot_index,int iAdd);
void BotClient_Gearbox_Battery(void *p, int bot_index,int iAdd);

void BotClient_Valve_Damage(void *p, int bot_index,int iAdd);
//void BotClient_TFC_Damage(void *p, int bot_index);
void BotClient_CS_Damage(void *p, int bot_index,int iAdd);
void BotClient_Gearbox_Damage(void *p, int bot_index,int iAdd);

void BotClient_CS_Money(void *p, int bot_index,int iAdd);			// cs specific
void BotClient_CS_StatusI(void *p, int bot_index,int iAdd);
void BotClient_CS_TeamInfo(void *p, int bot_index,int iAdd);
void BotClient_DOD_TeamInfo(void *p, int bot_index,int iAdd);
void BotClient_CS_Roundtime(void *p, int bot_index,int iAdd);
void BotClient_CS_TextMsg(void *p, int bot_index,int iAdd);
void BotClient_CS_StatusV(void *p, int bot_index,int iAdd);

void BotClient_Valve_SetFOV(void *p, int bot_index,int iAdd);
void BotClient_CS_SetFOV(void *p, int bot_index,int iAdd);
void BotClient_DOD_SetFOV(void *p, int bot_index,int iAdd);

void BotClient_Valve_ScreenFade(void *p, int bot_index,int iAdd);
//void BotClient_TFC_ScreenFade(void *p, int bot_index);
void BotClient_CS_ScreenFade(void *p, int bot_index,int iAdd);

void BotClient_CS_DeathMsg(void *p, int bot_index,int iAdd);
void BotClient_Valve_DeathMsg(void *p, int bot_index,int iAdd);


// dod stuff
void BotClient_DOD_VGUI(void *p, int bot_index,int iAdd);
void BotClient_DOD_WeaponList(void *p, int bot_index,int iAdd);
void BotClient_DOD_CurrentWeapon(void *p, int bot_index,int iAdd);
void BotClient_DOD_AmmoX(void *p, int bot_index,int iAdd);
void BotClient_DOD_AmmoPickup(void *p, int bot_index,int iAdd);
void BotClient_DOD_WeaponPickup(void *p, int bot_index,int iAdd);
void BotClient_DOD_ItemPickup(void *p, int bot_index,int iAdd);
void BotClient_DOD_Health(void *p, int bot_index,int iAdd);
void BotClient_DOD_Battery(void *p, int bot_index,int iAdd);
void BotClient_DOD_Damage(void *p, int bot_index,int iAdd);
void BotClient_DOD_DeathMsg(void *p, int bot_index,int iAdd);
void BotClient_DOD_ScreenFade(void *p, int bot_index,int iAdd);
void BotClient_DOD_Stamina(void *p, int bot_index,int iAdd);
void BotClient_DOD_Speed(void *p, int bot_index,int iAdd);
void BotClient_DOD_Slowed(void *p, int bot_index,int iAdd);
void BotClient_DOD_Bleeding(void *p, int bot_index,int iAdd);
void BotClient_DOD_Object(void *p, int bot_index,int iAdd);

void BotClient_CS_Flashlight(void *p, int bot_index,int iAdd);
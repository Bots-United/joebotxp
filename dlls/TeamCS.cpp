#include "TeamCS.h"
#include "BaseBot.h"
#include "Game.h"

CTeamCS::CTeamCS(void){
}

CTeamCS::~CTeamCS(void){
}

void CTeamCS::processMessage(CBaseBot *pBot,int iPlayerIndex,int iType,int iOtherPlayerIndex,int iNWP){
	if(fabs(m_pfRadioUsed[iPlayerIndex] - g_pGame->getTime()) > 5.0){
		if(fabs(g_pGame->getTime() - m_ppfLastMessageSent[iPlayerIndex][iType]) > m_pfTime4Message[iType]){
			switch(iType){
			case RC_IN_POSITION:
				pBot->fakeClientCommand("radio3");
				pBot->fakeClientCommand("menuselect","5");

				m_pfRadioUsed[iPlayerIndex] = g_pGame->getTime();
				break;
			case RC_COVER_ME:
				pBot->fakeClientCommand("radio1");
				pBot->fakeClientCommand("menuselect","1");

				m_pfRadioUsed[iPlayerIndex] = g_pGame->getTime();
				break;
			case RC_FOLLOW_ME:
				pBot->fakeClientCommand("radio1");
				pBot->fakeClientCommand("menuselect","5");

				m_pfRadioUsed[iPlayerIndex] = g_pGame->getTime();
				break;
			case RC_NEED_BACKUP:
				pBot->fakeClientCommand("radio3");
				pBot->fakeClientCommand("menuselect","3");

				m_pfRadioUsed[iPlayerIndex] = g_pGame->getTime();
				break;
			case RC_ENEMY_DOWN:
				pBot->fakeClientCommand("radio3");
				pBot->fakeClientCommand("menuselect","9");

				m_pfRadioUsed[iPlayerIndex] = g_pGame->getTime();
				break;
			case RC_ENEMY_SPOTTED:
				pBot->fakeClientCommand("radio3");
				pBot->fakeClientCommand("menuselect","2");

				m_pfRadioUsed[iPlayerIndex] = g_pGame->getTime();
				break;
			case RC_AFFIRMATIVE:
				pBot->fakeClientCommand("radio3");
				pBot->fakeClientCommand("menuselect","1");

				m_pfRadioUsed[iPlayerIndex] = g_pGame->getTime();
				break;
			case RC_NEGATIVE:
				pBot->fakeClientCommand("radio3");
				pBot->fakeClientCommand("menuselect","8");

				m_pfRadioUsed[iPlayerIndex] = g_pGame->getTime();
				break;
			case RC_GO_GO_GO:
				pBot->fakeClientCommand("radio2");
				pBot->fakeClientCommand("menuselect","1");

				m_pfRadioUsed[iPlayerIndex] = g_pGame->getTime();
				break;
			case RC_YOU_TAKE_THE_POINT:
				pBot->fakeClientCommand("radio1");
				pBot->fakeClientCommand("menuselect","2");

				m_pfRadioUsed[iPlayerIndex] = g_pGame->getTime();
				break;
			case RC_SECTOR_CLEAR:
				pBot->fakeClientCommand("radio3");
				pBot->fakeClientCommand("menuselect","4");

				m_pfRadioUsed[iPlayerIndex] = g_pGame->getTime();
				break;
			default:
				break;
			}
		}
	}

	CBaseTeam::processMessage(pBot,iPlayerIndex,iType,iOtherPlayerIndex,iNWP);
}
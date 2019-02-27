#include "PerceptionSquad.h"

CPerceptionSquad::CPerceptionSquad(void){
}

CPerceptionSquad::~CPerceptionSquad(void){
}

void CPerceptionSquad::run(void){
	PROFILE("CPerceptionSquad::run");
	process();
	merge();

	m_fLastRun = g_pGame->getTime();
}
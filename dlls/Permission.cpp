#include "permission.h"
#include "game.h"

CPermission::CPermission(void){
}

CPermission::~CPermission(void){
}

void CPermission::reset(void){
	unsigned int ui = (unsigned int)m_LPermissions.size()-1;
	for(; ui >= (unsigned int)0; ui --){
		m_LPermissions[ui].reset();
	}
}

bool CPermission::requestPermission(void *pID, unsigned int iType, float fTime){
	checkSize(iType);		// check if enough permission elements exist. if not add some

	if(m_LPermissions[iType].m_id == pID
		&& getTime() < m_LPermissions[iType].m_fTimeReserved){			// pID has already this permission
		return true;
	}
	else if(getTime() > m_LPermissions[iType].m_fTimeReserved){
		// we are allowed to give this permission to pID
		m_LPermissions[iType].m_id = pID;
		m_LPermissions[iType].m_fTimeReserved = getTime();
		return true;
	}
	return false;
}

void CPermission::checkSize(unsigned int iType){
	if(m_LPermissions.size() <= iType){
		// let's add so many elements so we have an iType element
		while(m_LPermissions.size() <= iType)			// <= because size is always +1 of the highest index
			m_LPermissions.push_back(CPermissionItem());
	}
}

bool CPermission::releasePermission(void *pID, unsigned int iType){
	checkSize(iType);

	if(m_LPermissions[iType].m_id == pID){			// pID has this permission - he may delete it
		m_LPermissions[iType].m_id = 0;
		m_LPermissions[iType].m_fTimeReserved = 0;
		return true;
	}
	return false;
}

float CPermission::getTime(void){
	return g_pGame->getTime();
}
#include "Waypoint.h"

#include <iostream>
using namespace std;

#include "Game.h"

CPath::CPath(){
	m_pNext = 0;
	for(int i = 0; i < _MAX_PATHS;i ++){
		m_piPathTo[i] = -1;
	}
}

CPath::~CPath()
{
	if(m_pNext){
		delete m_pNext;
		m_pNext = 0;
	}
}

bool CPath::addConnectionTo(int iConnectTo,CPathStat *pPStat){
	if(!m_pNext){		// if there is no next path instance
		for(int i=0; i < _MAX_PATHS;i ++){
			if(m_piPathTo[i] == -1){
				m_piPathTo[i] = iConnectTo;
				if(pPStat)
					m_pStats[i] = *pPStat;
				return true;
			}
		}
		m_pNext = new CPath;		// create a new path instance and add it there
	}

	return m_pNext->addConnectionTo(iConnectTo,pPStat);	// add this path to the next paths
}

/*bool CPath::removeConnectionTo(int iRemoveConnection){
	for(int i=0; i < _MAX_PATHS;i ++){
		if(m_piPathTo[i] == iRemoveConnection){
			int iLastConnection = getLastConnection();
			if(iLastConnection != iRemoveConnection){		// if it's not the last one
				m_piPathTo[i] = iLastConnection;		// copy last connection here
				removeLastConnection();		// and copy it here ...
			}
			else{
				m_piPathTo[i] = -1;
			}
			return true;
		}
	}
	if(m_pNext)
		return m_pNext->removeConnectionTo(iRemoveConnection);
	else
		return false;
}*/

int CPath::getLastConnection(void){
	int iReturn;
	if(m_pNext){
		iReturn = m_pNext->getLastConnection();		// if there's a next paths, look there

		if(iReturn != -1)
			return iReturn;
		else{
			// this means the last element is empty
			// therefore we can delete it
			delete m_pNext;
			m_pNext = 0;
		}
	}

	// check if we have the last element ourselves
	int i;
	for(i=0; i < _MAX_PATHS;i ++){
		if(m_piPathTo[i] == -1){
			break;
		}
	}
	if(i!=0)	// if not zero, we can substract one ... otherwise we got no last element
		return m_piPathTo[i-1];
	else
		return -1;
}

int CPath::getConnectionCount(void){
	int iCount = 0;

	int i;
		for(i=0; i < _MAX_PATHS;i ++){
			if(m_piPathTo[i] != -1){
				iCount ++;
			}
		}

	if(m_pNext){
		iCount += m_pNext->getConnectionCount();
	}
	
	return iCount;
}

int CPath::getConnection(int iCount){
	if(iCount >= _MAX_PATHS){
		if(m_pNext)
			return m_pNext->getConnection(iCount - _MAX_PATHS);
		else
			return -1;
	}
	else
		return m_piPathTo[iCount];
}

/*bool CPath::removeLastConnection(void){
	if(m_pNext){
		if(m_pNext->removeLastConnection()){
			delete m_pNext;
//			cout << "del path"<<endl;
			m_pNext = 0;
		}
		return false;
	}
	else{
		int i;
		for(i=0; i < _MAX_PATHS;i ++){
			if(m_piPathTo[i] == -1){
				if(i!=0){
					i --;				// go one path back
					m_piPathTo[i] = -1;	// and delete it
				}
				break;
			}
		}
		return (i==0);		// if i==0, return true, i.e. delete these paths
	}
}*/

bool CPath::isConnectedTo(int iConnectedTo){
	int i;
	for(i=0; i < _MAX_PATHS;i ++){
		if(m_piPathTo[i] == iConnectedTo){		// go thru all paths ...
			return true;
		}
	}
	if(m_pNext)
		return m_pNext->isConnectedTo(iConnectedTo);
	else
		return false;								// nothing there ?
}

void CPath::reset(void){
	if(m_pNext){
		delete m_pNext;
		m_pNext = 0;
	}
	for(int i = 0; i < _MAX_PATHS;i ++){
		m_piPathTo[i] = -1;
	}
}

void CPath::resetStat(void){
	for(int i = 0; i < _MAX_PATHS;i ++){
		m_pStats[i].reset();
	}
	if(m_pNext)
		m_pNext->resetStat();
}

void CPath::save(FILE *fhd){
	int iConnectionCount = getConnectionCount();

	fwrite(&iConnectionCount,sizeof(int),1,fhd);
	savePaths(fhd);
}

void CPath::savePaths(FILE *fhd){
	int i;

	for(i=0; i < _MAX_PATHS;i ++){
		if(m_piPathTo[i] != -1){
			fwrite(&(m_piPathTo[i]),sizeof(int),1,fhd);
			/*ustemp = m_piFailed[i];
			fwrite(&ustemp,sizeof(unsigned short),1,fhd);*/
			m_pStats[i].save(fhd);
		}
	}

	if(m_pNext)
		m_pNext->savePaths(fhd);
}

void CPath::saveXML(FILE *fhd){
	int iConnectionCount = getConnectionCount();
	fprintf(fhd,"<connections>\n");
	fprintf(fhd,"<number>%i</number>",iConnectionCount);
	savePathsXML(fhd);
	fprintf(fhd,"\n</connections>\n");
}

void CPath::savePathsXML(FILE *fhd){
	int i;
	for(i=0; i < _MAX_PATHS;i ++){
		if(m_piPathTo[i] != -1){
			fprintf(fhd,"%i ", m_piPathTo[i]);
			fprintf(fhd,"(%i) ", m_pStats[i].m_iFailed);
		}
	}

	if(m_pNext)
		m_pNext->savePathsXML(fhd);
}

void CPath::load(FILE *fhd){
	int iConnectionCount,i;
	CPathStat PSTemp;

	reset();
	fread(&iConnectionCount,sizeof(int),1,fhd);
	//cout << iConnectionCount << endl;

	for(;iConnectionCount > 0; iConnectionCount--){
		fread(&i,sizeof(int),1,fhd);

		if(i<0 || i > g_Map.m_Waypoints.m_iNumWaypoints){
			g_pGame->debugMsg(DEBUG_SYSTEM,"JoeBOT : loading paths; %i is no valid wp index\n",i);
			i=-1;
		}

		PSTemp.load(fhd);
		addConnectionTo(i,&PSTemp);
	}
}

int *CPath::getFailedEntry(int iWP){
	int i;
	for(i=0; i < _MAX_PATHS;i ++){
		if(m_piPathTo[i] == iWP){
			return &(m_pStats[i].m_iFailed);
		}
	}
	if(m_pNext)
		return m_pNext->getFailedEntry(iWP);
	else
		return 0;
}

int CPath::increasedFailedEntry(int iWP){
	int *pi;

	pi = getFailedEntry(iWP);

	if(pi)
		return (*pi)++;
	else
		return -1;
}

CPath_iterator::CPath_iterator(){
	m_pPath = 0;
	m_iIndex = 0;
}

CPath_iterator::~CPath_iterator(){
}
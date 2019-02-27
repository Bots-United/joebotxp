#include "Waypoint.h"
#include "Map.h"

#include <iostream>
using namespace std;

#include "util.h"

#include "Game.h"

CWaypointGraph::CWaypointGraph(){
	m_iNumWaypoints = 0;
	memset(&(m_ppVisTable[0]),0,sizeof(CBitField *)*_MAX_WAYPOINTS);
	m_fMaxDistance2Display = 300;

	m_bDisplay = false;
	m_bDisplayWaypoints = true;
	m_bDisplayPaths = true;

	resetStat();
}

CWaypointGraph::~CWaypointGraph(){
	int i;
	for(i=0;i < _MAX_WAYPOINTS;i++){
		if(m_ppVisTable[i]){
			delete []m_ppVisTable[i];
			m_ppVisTable[i] = 0;
		}
	}
}

int CWaypointGraph::add(const Vector &VOrigin){
	int i = 0;

	while(!(m_pWaypoints[i].m_lFlags&CWaypoint::DELETED)){			// is there somewhere a deleted waypoint in between ?
		i++;
		if(i>=m_iNumWaypoints)
			break;
		if(i >= _MAX_WAYPOINTS)		// dont exceed array size
			return -1;
	}
	if(i >= m_iNumWaypoints){
		m_iNumWaypoints ++;
	}
	m_pWaypoints[i].m_VOrigin = VOrigin;
	m_pWaypoints[i].init();

	addToHashTable(i);

	update(UD_WAYPOINT_CHANGED,i);

	return i;
}

bool CWaypointGraph::connect(int i1,int i2){
	if(i1 >= m_iNumWaypoints
		||i2 >= m_iNumWaypoints
		||i1<0
		||i2<0){
		return false;
	}

	if(i1!=i2)
		return m_pWaypoints[i1].addConnectionTo(i2);
	else
		return false;
}

bool CWaypointGraph::disconnect(int i1,int i2){
	if(!isValid(i1) ||
		!isValid(i2)){
		return false;
	}

	if(i1!=i2)
		return m_pWaypoints[i1].removeConnectionTo(i2);
	else
		return false;
}

void CWaypointGraph::remove(int iRemove){
	assert(isValid(iRemove));

	if(m_pWaypoints[iRemove].m_lFlags & CWaypoint::DELETED)
		return;

	int i;
	for(i=0;i < m_iNumWaypoints; i++){
		m_pWaypoints[i].removeConnectionTo(iRemove);			// remove all connections to this waypoint
	}
	m_pWaypoints[iRemove].resetPaths();							// clear all own paths
	m_pWaypoints[iRemove].m_lFlags |= CWaypoint::DELETED;		// set flag that this waypoint is deleted
	if(iRemove+1 == m_iNumWaypoints)
		m_iNumWaypoints--;

	delete m_ppVisTable[iRemove];
	m_ppVisTable[iRemove] = 0;

	// remove from hash table
	removeFromHashtable(iRemove);
}

void CWaypointGraph::drawTo(edict_t *pEntity){
	// draw the waypoints to the user ... 
#ifdef __HLENGINE
	if(!pEntity)
		return;
	if(!m_bDisplay)
		return;

	int iWP;
	float fDistance,fPDistance;
	bool bDBReturn = true;

	for(iWP = 0; iWP < m_iNumWaypoints;iWP ++){
		if(m_pWaypoints[iWP].m_lFlags & CWaypoint::DELETED)
			continue;

		if(g_pGame->getTime() < m_pWaypoints[iWP].m_fDisplayNext)
			continue;

		fDistance = (pEntity->v.origin - m_pWaypoints[iWP].m_VOrigin).length();

		if(fDistance > m_fMaxDistance2Display)
			continue;

		if(!g_pGame->fInViewCone(m_pWaypoints[iWP].m_VOrigin,pEntity))
			continue;

		if(!g_pGame->fVisible(m_pWaypoints[iWP].m_VOrigin,pEntity))
			continue;

		if(m_bDisplayWaypoints)
			bDBReturn = m_pWaypoints[iWP].drawTo(pEntity);
		
		if(bDBReturn && m_bDisplayPaths){
			CPath_iterator iter_path;
			int iConnection;
			iter_path = m_pWaypoints[iWP];
			for(; (iConnection=iter_path.getTo()) >=0 ; ++iter_path){
				fPDistance = (pEntity->v.origin - m_pWaypoints[iConnection].m_VOrigin).length();
				if(/*iter_path.getStat()->m_iFailed >0  ||*/ 1)
					g_pGame->drawBeam(0,pEntity,m_pWaypoints[iWP].m_VOrigin+(fDistance>fPDistance?0.5:1)*Vector(0,0,20),
						m_pWaypoints[iConnection].m_VOrigin+(fDistance>fPDistance?0.5:1)*Vector(0,0,20),
						10,0,Vector(fDistance>fPDistance?255:0,fDistance>fPDistance?0:255,0),200,0);
			}
		}
		else{
			//m_fMaxDistance2Display -= 2.0f;
		}
	}
#endif
}

void CWaypointGraph::update(int iType,int iAdd){
	// this is called each time something has to be updated ...

	if(iType == UD_WAYPOINT_CHANGED){
		// some waypoint stuff changed ...
		if(!m_ppVisTable[iAdd]){		// already allocated right bitfield ?
			m_ppVisTable[iAdd] = new CBitField(iAdd+1);
		}
		vis_recalc(iAdd);
		
		// update the stat data
		// copy data from the nearest waypoint
		int iN = getNearest(m_pWaypoints[iAdd].m_VOrigin,false,false,10);
		m_pWaypoints[iAdd].resetStat();
		if(iN != -1){
			//m_pWaypoints[iAdd] = m_pWaypoints[iN];
			m_pWaypoints[iAdd].copyStat(m_pWaypoints[iN]);
			m_pWaypoints[iAdd].m_lTraffic *= .6f;				// reduce traffic, because there might not be much
		}
		else{
			// reset it if there is no near waypoint
		}

		checkAutoPath(iAdd);
	}
}

void CWaypointGraph::vis_recalc(int iWP){
#ifdef __JOEBOTXPDLL
	bool bVisib;
	
	int iWP2 = 0;
	for(;iWP2 < iWP; iWP2++){	// check till iWP ... again the reason is the triangle form of the vis table
		bVisib = g_pGame->fVisibleEx(m_pWaypoints[iWP].m_VOrigin,m_pWaypoints[iWP2].m_VOrigin,
			true,true);
		
		if(m_ppVisTable[iWP])
			m_ppVisTable[iWP]->setBit(iWP2,bVisib);
		if(bVisib){
			m_pWaypoints[iWP2].addVisibleWP(&m_pWaypoints[iWP]);		// iWP2 is visible by iWP1
			m_pWaypoints[iWP].addVisibleWP(&m_pWaypoints[iWP2]);		// <->
		}
	}
	for(;iWP2 < m_iNumWaypoints;iWP2++){
		bVisib = g_pGame->fVisibleEx(m_pWaypoints[iWP].m_VOrigin,m_pWaypoints[iWP2].m_VOrigin,
			true,true);
		
		if(m_ppVisTable[iWP2])
			m_ppVisTable[iWP2]->setBit(iWP,bVisib);
		if(bVisib){
			m_pWaypoints[iWP2].addVisibleWP(&m_pWaypoints[iWP]);		// iWP2 is visible by iWP1
			m_pWaypoints[iWP].addVisibleWP(&m_pWaypoints[iWP2]);		// <->
		}
	}
#endif
}

void CWaypointGraph::addToHashTable(int iWP){
	int iHash = getHashcode(m_pWaypoints[iWP].m_VOrigin);

	m_pLHash[iHash].push_back(iWP);
}

bool CWaypointGraph::removeFromHashtable(int iRemove){
	// let's first assume that the origin stored at this waypoint is still right
	int iHash = getHashcode(m_pWaypoints[iRemove].m_VOrigin);
	vector<int>::iterator iter;

	for(iter = m_pLHash[iHash].begin(); iter != m_pLHash[iHash].end(); iter ++){// look into the bucket and delete it
		if(*iter == iRemove){
			m_pLHash[iHash].erase(iter);
			return true;
		}
	}
	// not found : so maybe the origin isnt still right ?!
	// let's loop thru all and see if we can get it
	for(iHash = 0; iHash < 4096; iHash ++){
		for(iter = m_pLHash[iHash].begin(); iter != m_pLHash[iHash].end(); iter ++){
			if(*iter == iRemove){
				m_pLHash[iHash].erase(iter);
				return true;
			}
		}
	}
	return false;
}

bool CWaypointGraph::getVisible(int iWP1, int iWP2){
	if(!isValid(iWP1)				// is this call valid ?
		||!isValid(iWP2))
		return false;

	if(iWP2 > iWP1){// we gotta swap if the second index is higher than the first, cause the vis table is like a triangle
		int iT = iWP2;
		iWP2 = iWP1;
		iWP1 = iT;
	}
	if(m_ppVisTable[iWP1])
		return m_ppVisTable[iWP1]->getBit(iWP2);
	else
		return false;
}

int CWaypointGraph::getNearest(const Vector &VOrigin,bool bVisible, bool bReachable, float fMin, float fMax,long lFlags){
	//
	// note that this function only searches for the nearest waypoint within a distance of min. 1,5*8192/64 = 192
	// and maximum of 255 units.
	//
	float fDistance, fNearest = 1E30;
	int iMinIndex = -1;

	vector<int>::iterator iter,end;
	int iHash = getHashcode(VOrigin);		// get hashcode for that location
	
	int iOX,iOY,iXBase,iYBase;

	fNearest = fMax = fMax * fMax;
	fMin = fMin * fMin;

	for(iOY = -1; iOY <= 1; iOY ++){		// and check the buckets around there
		iYBase = iHash + iOY * 64;
		if(iYBase < 0 || iYBase > 4095 )
			continue;
		for(iOX = -1; iOX <= 1; iOX ++){
			iXBase = iYBase + iOX;

			if(iXBase < 0 || iXBase > 4095 )
				continue;

			end = m_pLHash[iXBase].end();
			for(iter=m_pLHash[iXBase].begin();iter != end; iter ++){
				fDistance = (VOrigin - m_pWaypoints[*iter].m_VOrigin).squareLength();
				
				if(fDistance < fNearest && fDistance >= fMin){
					if(bVisible){		// if we have to check if it's visible
#ifdef __JOEBOTXPDLL
						if(!g_pGame->fVisibleEx(VOrigin,m_pWaypoints[*iter].m_VOrigin)){
							continue;
						}
#endif
					}
					if(bReachable){		// so do we gotta check if it's reachable ?!
						if(!g_Map.getReachable(VOrigin,m_pWaypoints[*iter].m_VOrigin))
							continue;
					}
					if( (m_pWaypoints[*iter].m_lFlags & lFlags) != lFlags)
						continue;

					fNearest = fDistance;
					iMinIndex = *iter;
				}
			}
		}
	}

	if(iMinIndex == -1){
		return getNearesto(VOrigin,bVisible,bReachable,sqrt(fMin),sqrt(fMax),lFlags);
		/*// no waypoint found : we have to continue this search at the boundaries of the searched buckets :)
		int iCheck = 2;		// we checked 'radius' 0 and 1, now we gotta check >= 2
		int iys,iye,ixs,ixe,i,is,ie;

		while(iCheck < 64){

			if((iys = iHashY - iCheck) >= 0){		// top row ( and not outside borders )
				ixs = iHashX - iCheck;
				if(ixs<0) ixs = 0;
				
				ixe = iHashX + iCheck;
				if(ixe > 64) ixe = 64;
				
				is = iys * 64 + ixs;
				ie = iys * 64 + ixe;

				for(i=is; i < ie; i++){
					// check distances
					end = m_pLHash[i].end();
					for(iter=m_pLHash[i].begin();iter != end; iter ++){
						fDistance = (VOrigin - m_pWaypoints[*iter].m_VOrigin).SquareLength();
						
						if(fDistance < fNearest && fDistance > fMin){
							if(bVisible){		// if we have to check if it's visible
#ifdef __JOEBOTXPDLL
								if(!FVisibleEx(VOrigin,m_pWaypoints[*iter].m_VOrigin)){
									continue;
								}
#endif
							}
							if(bReachable){		// so do we gotta check if it's reachable ?!
							}
							
							fNearest = fDistance;
							iMinIndex = *iter;
						}
					}
				}
			}
			if((iys = iHashY + iCheck) < 64){		// bottom row ( and not outside borders )
				ixs = iHashX - iCheck;
				if(ixs<0) ixs = 0;
				
				ixe = iHashX + iCheck;
				if(ixe > 64) ixe = 64;

				is = iys * 64 + ixs;
				ie = iys * 64 + ixe;
				
				for(i=is; i < ie; i++){
					// check distances
					end = m_pLHash[i].end();
					for(iter=m_pLHash[i].begin();iter != end; iter ++){
						fDistance = (VOrigin - m_pWaypoints[*iter].m_VOrigin).SquareLength();
						
						if(fDistance < fNearest && fDistance > fMin){
							if(bVisible){		// if we have to check if it's visible
#ifdef __JOEBOTXPDLL
								if(!FVisibleEx(VOrigin,m_pWaypoints[*iter].m_VOrigin)){
									continue;
								}
#endif
							}
							if(bReachable){		// so do we gotta check if it's reachable ?!
							}
							
							fNearest = fDistance;
							iMinIndex = *iter;
						}
					}
				}
			}

			if((ixs = iHashX - iCheck) >= 0){
				iys = iHashY - iCheck + 1;			// +1 because we dont wanna check the corners twice
				if(iys < 1) iys = 1;

				iye = iHashY + iCheck - 1;
				if(iye > 63) iye = 63;

				is = ixs + iys * 64;
				ie = ixs + iye * 64;
				
				for(i=is; i < ie; i+=64){
					// check distances
					end = m_pLHash[i].end();
					for(iter=m_pLHash[i].begin();iter != end; iter ++){
						fDistance = (VOrigin - m_pWaypoints[*iter].m_VOrigin).SquareLength();
						
						if(fDistance < fNearest && fDistance > fMin){
							if(bVisible){		// if we have to check if it's visible
#ifdef __JOEBOTXPDLL
								if(!FVisibleEx(VOrigin,m_pWaypoints[*iter].m_VOrigin)){
									continue;
								}
#endif
							}
							if(bReachable){		// so do we gotta check if it's reachable ?!
							}
							
							fNearest = fDistance;
							iMinIndex = *iter;
						}
					}
				}
			}
			if((ixs = iHashX + iCheck) < 64){
				iys = iHashY - iCheck + 1;			// +1 because we dont wanna check the corners twice
				if(iys < 1) iys = 1;

				iye = iHashY + iCheck - 1;
				if(iye > 63) iye = 63;

				is = ixs + iys * 64;
				ie = ixs + iye * 64;
				
				for(i=is; i < ie; i+=64){
					// check distances
					end = m_pLHash[i].end();
					for(iter=m_pLHash[i].begin();iter != end; iter ++){
						fDistance = (VOrigin - m_pWaypoints[*iter].m_VOrigin).SquareLength();
						
						if(fDistance < fNearest && fDistance > fMin){
							if(bVisible){		// if we have to check if it's visible
#ifdef __JOEBOTXPDLL
								if(!FVisibleEx(VOrigin,m_pWaypoints[*iter].m_VOrigin)){
									continue;
								}
#endif
							}
							if(bReachable){		// so do we gotta check if it's reachable ?!
							}
							
							fNearest = fDistance;
							iMinIndex = *iter;
						}
					}
				}
			}

			if(iMinIndex != -1)
				break;

			// next round :)
			iCheck ++;
		}*/
	}

	return iMinIndex;
}

int CWaypointGraph::getNearesto(const Vector &VOrigin,bool bVisible, bool bReachable, float fMin, float fMax,long lFlags){
	// 
	// search the nearest waypoint the traditional way ... a lot slower  ( 10x ) than the funtion without the o,
	// but not as limited. You should better use the other funtion though, is you know it's sufficient !
	//
	float fDistance, fNearest = 100000000;
	int iMinIndex = -1;

	list<int>::iterator iter,end;

	fMin = fMin * fMin;
	fNearest = fMax = fMax * fMax;

	for(int iWP = 0; iWP < m_iNumWaypoints;iWP ++){				// loop thru
		if(m_pWaypoints[iWP].m_lFlags & CWaypoint::DELETED)		// all valid waypoints
			continue;

		if(lFlags && !(m_pWaypoints[iWP].m_lFlags & lFlags))		// check if we are only allowed to check certain waypoint types
			continue;

		fDistance = (VOrigin - m_pWaypoints[iWP].m_VOrigin).squareLength();	// get the square distance ( saves us a sqrt call )

		if(fDistance < fMin)		// too near ... continue
			continue;

		if(fDistance < fNearest){	// hey cool, the nearest waypoint so far
			if(bVisible){			// check visibility ... is this waypoints ok ?
#ifdef __JOEBOTXPDLL
				if(!g_pGame->fVisibleEx(VOrigin,m_pWaypoints[iWP].m_VOrigin)){
					continue;
				}
#endif
			}
			if(bReachable){
			}

			fNearest = fDistance;
			iMinIndex = iWP;
		}
	}

	return iMinIndex;
}

int CWaypointGraph::getNearestB(const Vector &VOrigin,CBitField *pBField,bool bVisible, bool bReachable, float fMin, float fMax,long lFlags){
	// 
	// search the nearest waypoint the traditional way ... a lot slower  ( 10x ) than the funtion without the o,
	// but no as limited. You should better use the other funtion though, is you know it's sufficient !
	// This function also looks at a bitfield to define a mask which waypoints to search
	//
	float fDistance, fNearest = 100000000;
	int iMinIndex = -1;

	list<int>::iterator iter,end;

	fMin = fMin * fMin;
	fNearest = fMax = fMax * fMax;

	for(int iWP = 0; iWP < m_iNumWaypoints;iWP ++){				// loop thru
		if(m_pWaypoints[iWP].m_lFlags & CWaypoint::DELETED)		// all valid waypoints
			continue;

		if(pBField && !pBField->getBit(iWP))
			continue;

		if(lFlags && !(m_pWaypoints[iWP].m_lFlags & lFlags))		// check if we are only allowed to check certain waypoint types
			continue;

		fDistance = (VOrigin - m_pWaypoints[iWP].m_VOrigin).squareLength();	// get the square distance ( saves us a sqrt call )

		if(fDistance < fMin)		// too near ... continue
			continue;

		if(fDistance < fNearest){	// hey cool, the nearest waypoint so far
			if(bVisible){			// check visibility ... is this waypoints ok ?
#ifdef __JOEBOTXPDLL
				if(!g_pGame->fVisibleEx(VOrigin,m_pWaypoints[iWP].m_VOrigin)){
					continue;
				}
#endif
			}
			if(bReachable){
				if(!g_Map.getReachable(VOrigin,m_pWaypoints[iWP].m_VOrigin))
					continue;
			}

			fNearest = fDistance;
			iMinIndex = iWP;
		}
	}

	return iMinIndex;
}

void CWaypointGraph::setVisibleMask(CBitField *pMask,int iWP){
	pMask->zero();
	/*if(iWP >= 0 && iWP < m_iNumWaypoints){
		for(int i = 0; i < m_iNumWaypoints;i ++){				// loop thru
			if(getVisible(iWP,i)){
				pMask->setBit(i,true);
			}
		}
	}*/
	/* --- */
	pMask->partialCopy(*m_ppVisTable[iWP]);
	for(int ischl = iWP; ischl < m_iNumWaypoints; ischl ++){
		if(getVisible(iWP,ischl)){
			pMask->setBit(ischl,true);
		}
	}
}

void CWaypointGraph::broadenMask(CBitField *pMask,int iDepth){
	// mark also all connections to set wps in this mask
	iDepth--;
	if(iDepth)
		broadenMask(pMask,iDepth);

	for(int ischl = 0; ischl < m_iNumWaypoints; ischl ++){
		if(g_Map.m_Waypoints.isValid(ischl)
			&& pMask->getBit(ischl)){
			// this bit was set, so add all the waypoints we have paths to
			CPath_iterator path_iter;
			int iTo;
			path_iter = g_Map.m_Waypoints[ischl];

			iTo = path_iter.getTo();
			while(iTo != -1){
				pMask->setBit(iTo,true);
				++path_iter;
				iTo = path_iter.getTo();
			}
		}
	}
}

void CWaypointGraph::setDistanceMask(CBitField *pMask,float fFactor, float fBDistance, Vector &VOrigin){
	// set flags for each waypoint in the pMask BitField class. fFactor can be used to select the waypoints 
	// within the fBDistance ( fFactor = 1 ) or outside that distance ( fFactor = -1 )
	pMask->zero();
	fBDistance *= fFactor;
	float fDistance;
	for(int i = 0; i < m_iNumWaypoints;i ++){				// loop thru
		fDistance = (VOrigin - m_pWaypoints[i].m_VOrigin).length();
		if(fDistance * fFactor < fBDistance){			// fBDistance *= fFactor; already done above
			pMask->setBit(i,true);
		}
	}
}

bool CWaypointGraph::save(const char *szFileName){
	FILE *fhd;
	WPHeader_t header;
	int iWP;

	fhd = fopen(szFileName,"wb");

	/*if(!fhd)
		return false;*/

	memset(header.szMapname,0,sizeof(char)*32);		// reset some strings
	memset(header.szText,0,sizeof(char)*32);
#ifdef __HLENGINE
	strcpy(header.szMapname,g_pGame->getMapname());	// copy map name
#endif
	strcpy(header.szText,"JoeBOT XP Waypoint File\r\n\r\n");	// set a header text
	header.lVersion = WP_VERSION;					// and store the version so we dont get shit when loading different waypoint version or we can distinguish and call different loading functions

	fwrite(&header,sizeof(WPHeader_t),1,fhd);		// write this to the file
	fwrite(&m_iNumWaypoints,sizeof(int),1,fhd);
	fwrite(&m_lMaxTotalDamage,1,sizeof(long),fhd);

	//cout << " ---------------------------------------------- "<<m_iNumWaypoints<<endl;

	for(iWP = 0;iWP < m_iNumWaypoints; iWP ++){
		m_pWaypoints[iWP].save(fhd);
		if(!(m_pWaypoints[iWP].m_lFlags & CWaypoint::DELETED)){	// only store vis table entries for not deleted waypoints
			if(m_ppVisTable[iWP]){
				m_ppVisTable[iWP]->save(fhd);
			}
		}
	}

	fclose(fhd);

	return true;
}

bool CWaypointGraph::saveXML(const char *szFileName){
	FILE *fhd;
	WPHeader_t header;
	int iWP;

	fhd = fopen(szFileName,"w");

	/*if(!fhd)
		return false;*/

	memset(header.szMapname,0,sizeof(char)*32);		// reset some strings
	memset(header.szText,0,sizeof(char)*32);
#ifdef __HLENGINE
	strcpy(header.szMapname,g_pGame->getMapname());	// copy map name
#endif
	strcpy(header.szText,"JoeBOT XP Waypoint File\r\n\r\n");	// set a header text
	header.lVersion = WP_VERSION;					// and store the version so we dont get shit when loading different waypoint version or we can distinguish and call different loading functions

	fprintf(fhd,"<!-- %s / %li -->\n",header.szText,header.lVersion);
	fprintf(fhd,"<map><name>%s</name>\n",header.szMapname);
	fprintf(fhd,"<wpcount>%li</wpcount>\n",m_iNumWaypoints);
	//fwrite(&m_iNumWaypoints,sizeof(int),1,fhd);
	//fwrite(&m_lMaxTotalDamage,1,sizeof(long),fhd);

	//cout << " ---------------------------------------------- "<<m_iNumWaypoints<<endl;

	for(iWP = 0;iWP < m_iNumWaypoints; iWP ++){
		m_pWaypoints[iWP].saveXML(fhd,iWP);
		/*if(!(m_pWaypoints[iWP].m_lFlags & CWaypoint::DELETED)){	// only store vis table entries for not deleted waypoints
			if(m_ppVisTable[iWP]){
				m_ppVisTable[iWP]->save(fhd);
			}
		}*/
	}
	fprintf(fhd,"</map>\n\n\n");

	fclose(fhd);

	return true;
}

bool CWaypointGraph::load(const char *szFileName){
	FILE *fhd;
	WPHeader_t header;
	int iWP;

	fhd = fopen(szFileName,"rb");

	if(!fhd){
#ifdef __JOEBOTXPDLL
		g_pGame->userMsg(0,"waypoints not found : %s",szFileName);
#else
		cerr << "file not found : " << szFileName << endl;
#endif
		return false;
	}

	fread(&header,sizeof(WPHeader_t),1,fhd);		// get the header

	if(header.lVersion != WP_VERSION){				// just quit if the version isnt the same like the default one of this bot version
		fclose(fhd);
#ifdef __JOEBOTXPDLL
		g_pGame->userMsg(0,"wrong waypoint file version : %s",szFileName);
#endif
		return false;
	}

#ifdef __JOEBOTXPDLL
	g_pGame->userMsg(0,"Loading waypoint file : %s",szFileName);
#endif

	fread(&m_iNumWaypoints,sizeof(int),1,fhd);		// get the number of waypoints
	fread(&m_lMaxTotalDamage,1,sizeof(long),fhd);

	for(iWP = 0;iWP < m_iNumWaypoints; iWP ++){		// and load each one
		m_pWaypoints[iWP].load(fhd);
		if(!(m_pWaypoints[iWP].m_lFlags & CWaypoint::DELETED)){	// and skip the deleted waypoints for the vis table this time too
			m_ppVisTable[iWP] = new CBitField(1);
			m_ppVisTable[iWP]->load(fhd);

			addToHashTable(iWP);					// for fast nearest waypoint lookup, tell the hashtable about this entry
		}
		else{
			m_ppVisTable[iWP] = 0;
		}
	}

	fclose(fhd);

	return true;
}

void CWaypointGraph::reset(void){
	/*int i;
	for(i = m_iNumWaypoints-1;i>-1;i--){
		m_pWaypoints[i].resetPaths();
		m_pWaypoints[i].m_lFlags |= CWaypoint::DELETED;
	}
	m_iNumWaypoints = 0;*/
	int i;
	for(i = m_iNumWaypoints-1;i>-1;i--){
		remove(i);
	}
	m_iNumWaypoints = 0;		// if something went wrong ...
}

void CWaypointGraph::checkAutoPath(int iThisWP){		// check if there can be some new paths added
	float fDistance;

	int i;
	if(iThisWP != -1){
		for(i=0; i<m_iNumWaypoints;i++){
			if(iThisWP == i)
				continue;

			if(m_pWaypoints[i].m_lFlags & CWaypoint::DELETED)
				continue;

			if(!getVisible(iThisWP,i))
				continue;

			if(m_pWaypoints[iThisWP].isConnectedTo(i))
				continue;

			fDistance = (m_pWaypoints[i].m_VOrigin
				-m_pWaypoints[iThisWP].m_VOrigin).squareLength();

			if(fDistance > 250.f*250.f)
				continue;

			if(!g_Map.getReachable(m_pWaypoints[iThisWP].m_VOrigin,m_pWaypoints[i].m_VOrigin))
				continue;

			m_pWaypoints[iThisWP].addConnectionTo(i);
		}
	}
}

int CWaypointGraph::getMaximum(CBitField *pValid, int iType, float fFactor, float fOffset){

	// !!! Offset is only used for WG_SEARCH_KILLTIME !!!

	int i,iMax=-1,ii;
	float fMax = -1e10;
	float fTemp=0,fLocalTemp;

	for(i=0; i<m_iNumWaypoints;i++){
		if(pValid){
			if(!pValid->getBit(i))
				continue;
		}

		if(m_pWaypoints[i].m_lFlags & CWaypoint::DELETED)
			continue;

		switch(iType){
		case WG_SEARCH_AVDISTVWP:			// Average Distance to visible waypoints
			fTemp = m_pWaypoints[i].m_fAvDistVWP * fFactor;
			break;
		case WG_SEARCH_IVWP:				// Number of visible waypoints
			fTemp = m_pWaypoints[i].m_iVisibleWaypoints * fFactor;
			break;
		case WG_SEARCH_KILLTIME:		// time of a kill
			fLocalTemp = -1e10;
			for(ii=0; ii < _CWPSTATITEM_KILLTIMESTORAGE; ii++){
				if(m_pWaypoints[i].m_pfKillTime[ii] != -1){		// there is an entry
					fLocalTemp = (m_pWaypoints[i].m_pfKillTime[ii] - fOffset ) * fFactor;
					if(fLocalTemp > fTemp)
						fTemp = fLocalTemp;
				}
			}
			break;
		case WG_SEARCH_DAMAGE:		// damage done from waypoint
			fTemp = (float)m_pWaypoints[i].m_lTotalDamage * fFactor;
			break;
		case WG_SEARCH_DAMAGED:		// damage done to waypoint
			fTemp = (float)m_pWaypoints[i].m_lTotalDamaged * fFactor;
			break;
		case WG_SEARCH_TRAFFIC:		// traffic here
			fTemp = (float)m_pWaypoints[i].m_lTraffic * fFactor;
			break;
		default:
			break;
		}
		if(fTemp > fMax){
			fMax = fTemp;
			iMax = i;
		}
	}
	return iMax;
}

// statistic data waypoint member functions

void CWaypointGraph::addDamage(Vector &VDamaged,Vector &VDamageOrigin, int iDamage){
	int iDamagedWP = g_Map.m_Waypoints.getNearest(VDamaged,false,false,150);		// who was damaged
	Vector2D vec_2d;
	if(iDamagedWP != -1){
		vec_2d = (VDamageOrigin - VDamaged).make2D();
		m_pWaypoints[iDamagedWP].addDamaged(vec_2d,iDamage);
		if(m_pWaypoints[iDamagedWP].m_lTotalDamage > m_lMaxTotalDamage){
			m_lMaxTotalDamage = m_pWaypoints[iDamagedWP].m_lTotalDamage;
		}
	}

	int iDamageWP = g_Map.m_Waypoints.getNearest(VDamageOrigin,false,false,150);	// who does the damage
	if(iDamageWP != -1){
		vec_2d = (VDamaged - VDamageOrigin).make2D();
		m_pWaypoints[iDamageWP].addDamage(vec_2d,iDamage);
		if(m_pWaypoints[iDamageWP].m_lTotalDamage > m_lMaxTotalDamage){
			m_lMaxTotalDamage = m_pWaypoints[iDamageWP].m_lTotalDamage;
		}
	}
}

void CWaypointGraph::addKill(Vector &VOrigin){
	int iWP = g_Map.m_Waypoints.getNearest(VOrigin,false,false,150);
	if(iWP != -1){
		m_pWaypoints[iWP].addKill();
	}
}

void CWaypointGraph::addTraffic(int iWP){
	m_pWaypoints[iWP].m_lTraffic ++;
}

void CWaypointGraph::resetStat(void){
	m_lMaxTotalDamage = 0;
	int i;
	for(i = 0; i < _MAX_WAYPOINTS; i++){
		m_pWaypoints[i].reset();
	}
}
#include <math.h>
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
	m_bDisplayPaths = false;
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
	edict_t *p_Ent=0;

	while(!(m_Waypoints[i].m_lFlags&CWaypoint::DELETED)){			// is there somewhere a deleted waypoint in between ?
		i++;
		if(i>=m_iNumWaypoints)
			break;
		if(i >= _MAX_WAYPOINTS)		// dont exceed array size
			return -1;
	}
	if(i >= m_iNumWaypoints){
		m_iNumWaypoints ++;
	}
	m_Waypoints[i].m_VOrigin = VOrigin;

	m_Waypoints[i].init();

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
		return m_Waypoints[i1].addConnectionTo(i2);
	else
		return false;
}

bool CWaypointGraph::disconnect(int i1,int i2){
	if(i1 >= m_iNumWaypoints
		||i2 >= m_iNumWaypoints
		||i1<0
		||i2<0){
		return false;
	}

	if(i1!=i2)
		return m_Waypoints[i1].removeConnectionTo(i2);
	else
		return false;
}

void CWaypointGraph::remove(int iRemove){
	assert(iRemove < m_iNumWaypoints);

	if(m_Waypoints[iRemove].m_lFlags & CWaypoint::DELETED)
		return;

	int i;
	for(i=0;i < m_iNumWaypoints; i++){
		m_Waypoints[i].removeConnectionTo(iRemove);			// remove all connections to this waypoint
	}
	m_Waypoints[iRemove].resetPaths();							// clear all own paths
	m_Waypoints[iRemove].m_lFlags |= CWaypoint::DELETED;		// set flag that this waypoint is deleted
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

	int iWP,iP;
	float fDistance,fPDistance;
	bool bDBReturn = true;
	//m_fMaxDistance2Display += .01f;

	for(iWP = 0; iWP < m_iNumWaypoints;iWP ++){
		if(m_Waypoints[iWP].m_lFlags & CWaypoint::DELETED)
			continue;

		if(g_pGame->getTime() < m_Waypoints[iWP].m_fDisplayNext)
			continue;

		fDistance = (pEntity->v.origin - m_Waypoints[iWP].m_VOrigin).Length();

		if(fDistance > m_fMaxDistance2Display)
			continue;

		if(!FInViewCone(m_Waypoints[iWP].m_VOrigin,pEntity))
			continue;

		if(!FVisible(m_Waypoints[iWP].m_VOrigin,pEntity))
			continue;

		if(m_bDisplayWaypoints)
			bDBReturn = m_Waypoints[iWP].drawTo(pEntity);
		
		if(bDBReturn && m_bDisplayPaths){
			int iConnection;
			for(iP = m_Waypoints[iWP].getConnectionCount()-1; iP >=0 ; iP--){
				iConnection = m_Waypoints[iWP].getConnection(iP);
				if(iConnection != -1){
					fPDistance = (pEntity->v.origin - m_Waypoints[iConnection].m_VOrigin).Length();
					UTIL_DrawBeam(pEntity,m_Waypoints[iWP].m_VOrigin+(fDistance>fPDistance?0.5:1)*Vector(0,0,20),m_Waypoints[iConnection].m_VOrigin+(fDistance>fPDistance?0.5:1)*Vector(0,0,20),10,0,fDistance>fPDistance?255:0,fDistance>fPDistance?0:255,0,200,0);
				}
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
		if(!m_ppVisTable[iAdd]){
			m_ppVisTable[iAdd] = new CBitField(iAdd+1);
		}
		vis_recalc(iAdd);
		
		// update the stat data
		// copy data from the nearest waypoint
		int iN = g_Map.m_Waypoints.getNearest(g_Map.m_Waypoints.m_Waypoints[iAdd].m_VOrigin,false,false,10);
		if(iN != -1){
			m_Waypoints[iAdd] = m_Waypoints[iN];
		}
		else{
			// reset it if there is no near waypoint
			m_Waypoints[iAdd].resetStat();
		}

		checkAutoPath(iAdd);
	}
}

void CWaypointGraph::vis_recalc(int iWP){
#ifdef __JOEBOTXPDLL
	bool bVisib;
	
	int iWP2 = 0;
	for(;iWP2 < iWP; iWP2++){	// check till iWP ... again the reason is the triangle form of the vis table
		bVisib = FVisibleEx(m_Waypoints[iWP].m_VOrigin,m_Waypoints[iWP2].m_VOrigin,
			false,false,false,false,0);
		
		if(m_ppVisTable[iWP])
			m_ppVisTable[iWP]->setBit(iWP2,bVisib);
		if(bVisib){
			m_Waypoints[iWP2].addVisibleWP(&m_Waypoints[iWP]);		// iWP2 is visible by iWP1
			m_Waypoints[iWP].addVisibleWP(&m_Waypoints[iWP2]);		// <->
		}
	}
	for(;iWP2 < m_iNumWaypoints;iWP2++){
		bVisib = FVisibleEx(m_Waypoints[iWP].m_VOrigin,m_Waypoints[iWP2].m_VOrigin,
			false,false,false,false,0);
		
		if(m_ppVisTable[iWP2])
			m_ppVisTable[iWP2]->setBit(iWP,bVisib);
		if(bVisib){
			m_Waypoints[iWP2].addVisibleWP(&m_Waypoints[iWP]);		// iWP2 is visible by iWP1
			m_Waypoints[iWP].addVisibleWP(&m_Waypoints[iWP2]);		// <->
		}
	}
#endif
}

void CWaypointGraph::addToHashTable(int iWP){
	int iHash = getHashcode(m_Waypoints[iWP].m_VOrigin);

	m_pLHash[iHash].push_back(iWP);
}

bool CWaypointGraph::removeFromHashtable(int iRemove){
	int iHash = getHashcode(m_Waypoints[iRemove].m_VOrigin);
	list<int>::iterator iter;
	
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
	if(iWP1>m_iNumWaypoints				// is this call valid ?
		||iWP2>m_iNumWaypoints
		||iWP1<0
		||iWP2<0)
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
	float fDistance, fNearest = 10000000000;
	int iMinIndex = -1;

	list<int>::iterator iter,end;
	int iHash = getHashcode(VOrigin);		// get hashcode for that location
	/*int iHashX = iHash % 64;
	int iHashY = iHash / 64;*/
	int iOX,iOY,iXBase,iYBase;

	fNearest = fMax = fMax * fMax;
	fMin = fMin * fMin;

	for(iOY = -1; iOY <= 1; iOY ++){		// and check the buckets around there
		iYBase = iHash + iOY * 64;
		/*if(iXBase < 0 || iXBase > 4095 )
			continue;*/
		for(iOX = -1; iOX <= 1; iOX ++){
			iXBase = iYBase + iOX;

			if(iXBase < 0 || iXBase > 4095 )
				continue;

			end = m_pLHash[iXBase].end();
			for(iter=m_pLHash[iXBase].begin();iter != end; iter ++){
				fDistance = (VOrigin - m_Waypoints[*iter].m_VOrigin).squareLength();
				
				if(fDistance < fNearest && fDistance > fMin){
					if(bVisible){		// if we have to check if it's visible
#ifdef __JOEBOTXPDLL
						if(!FVisibleEx(VOrigin,m_Waypoints[*iter].m_VOrigin)){
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

	if(iMinIndex == -1){
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
						fDistance = (VOrigin - m_Waypoints[*iter].m_VOrigin).SquareLength();
						
						if(fDistance < fNearest && fDistance > fMin){
							if(bVisible){		// if we have to check if it's visible
#ifdef __JOEBOTXPDLL
								if(!FVisibleEx(VOrigin,m_Waypoints[*iter].m_VOrigin)){
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
						fDistance = (VOrigin - m_Waypoints[*iter].m_VOrigin).SquareLength();
						
						if(fDistance < fNearest && fDistance > fMin){
							if(bVisible){		// if we have to check if it's visible
#ifdef __JOEBOTXPDLL
								if(!FVisibleEx(VOrigin,m_Waypoints[*iter].m_VOrigin)){
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
						fDistance = (VOrigin - m_Waypoints[*iter].m_VOrigin).SquareLength();
						
						if(fDistance < fNearest && fDistance > fMin){
							if(bVisible){		// if we have to check if it's visible
#ifdef __JOEBOTXPDLL
								if(!FVisibleEx(VOrigin,m_Waypoints[*iter].m_VOrigin)){
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
						fDistance = (VOrigin - m_Waypoints[*iter].m_VOrigin).SquareLength();
						
						if(fDistance < fNearest && fDistance > fMin){
							if(bVisible){		// if we have to check if it's visible
#ifdef __JOEBOTXPDLL
								if(!FVisibleEx(VOrigin,m_Waypoints[*iter].m_VOrigin)){
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
	// but no as limited. You should better use the other funtion though, is you know it's sufficient !
	//
	float fDistance, fNearest = 100000000;
	int iMinIndex = -1;

	list<int>::iterator iter,end;
	int iHash = getHashcode(VOrigin);


	fMin = fMin * fMin;
	fNearest = fMax = fMax * fMax;

	for(int iWP = 0; iWP < m_iNumWaypoints;iWP ++){				// loop thru
		if(m_Waypoints[iWP].m_lFlags & CWaypoint::DELETED)		// all valid waypoints
			continue;

		if(lFlags && !(m_Waypoints[iWP].m_lFlags & lFlags))		// check if we are only allowed to check certain waypoint types
			continue;

		fDistance = (VOrigin - m_Waypoints[iWP].m_VOrigin).squareLength();	// get the square distance ( saves us a sqrt call )

		if(fDistance < fMin)		// too near ... continue
			continue;

		if(fDistance < fNearest){	// hey cool, the nearest waypoint so far
			if(bVisible){			// check visibility ... is this waypoints ok ?
#ifdef __JOEBOTXPDLL
				if(!FVisibleEx(VOrigin,m_Waypoints[iWP].m_VOrigin)){
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
	//
	float fDistance, fNearest = 100000000;
	int iMinIndex = -1;

	list<int>::iterator iter,end;
	int iHash = getHashcode(VOrigin);


	fMin = fMin * fMin;
	fNearest = fMax = fMax * fMax;

	for(int iWP = 0; iWP < m_iNumWaypoints;iWP ++){				// loop thru
		if(m_Waypoints[iWP].m_lFlags & CWaypoint::DELETED)		// all valid waypoints
			continue;

		if(pBField && !pBField->getBit(iWP))
			continue;

		if(lFlags && !(m_Waypoints[iWP].m_lFlags & lFlags))		// check if we are only allowed to check certain waypoint types
			continue;

		fDistance = (VOrigin - m_Waypoints[iWP].m_VOrigin).squareLength();	// get the square distance ( saves us a sqrt call )

		if(fDistance < fMin)		// too near ... continue
			continue;

		if(fDistance < fNearest){	// hey cool, the nearest waypoint so far
			if(bVisible){			// check visibility ... is this waypoints ok ?
#ifdef __JOEBOTXPDLL
				if(!FVisibleEx(VOrigin,m_Waypoints[iWP].m_VOrigin)){
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

void CWaypointGraph::setVisibleMask(CBitField *pMask,int iWP){
	pMask->zero();
	if(iWP >= 0 && iWP < m_iNumWaypoints){
		for(int i = 0; i < m_iNumWaypoints;i ++){				// loop thru
			if(getVisible(iWP,i))
				pMask->setBit(i,true);
		}
	}
}

bool CWaypointGraph::save(const char *szFileName){
	FILE *fhd;
	//WPHeader_t header;
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

	cout << " ---------------------------------------------- "<<m_iNumWaypoints<<endl;

	for(iWP = 0;iWP < m_iNumWaypoints; iWP ++){
		m_Waypoints[iWP].save(fhd);
		if(!(m_Waypoints[iWP].m_lFlags & CWaypoint::DELETED)){	// only store vis table entries for not deleted waypoints
			if(m_ppVisTable[iWP]){
				m_ppVisTable[iWP]->save(fhd);
			}
		}
	}

	fclose(fhd);

	return true;
}

bool CWaypointGraph::load(const char *szFileName){
	FILE *fhd;
	//WPHeader_t header;
	int iWP;

	fhd = fopen(szFileName,"rb");

	if(!fhd){
#ifdef __JOEBOTXPDLL
		UTIL_PrintMessage("waypoints not found : ",szFileName,"\n");
#else
		cerr << "file not found : " << szFileName << endl;
#endif
		return false;
	}

	fread(&header,sizeof(WPHeader_t),1,fhd);		// get the header

	if(header.lVersion != WP_VERSION){				// just quit if the version isnt the same like the default one of this bot version
		fclose(fhd);
		cerr << " false waypoint version" << endl;
		return false;
	}

	fread(&m_iNumWaypoints,sizeof(int),1,fhd);		// get the number of waypoints
	fread(&m_lMaxTotalDamage,1,sizeof(long),fhd);

	for(iWP = 0;iWP < m_iNumWaypoints; iWP ++){		// and load each one
		m_Waypoints[iWP].load(fhd);
		if(!(m_Waypoints[iWP].m_lFlags & CWaypoint::DELETED)){	// and skip the deleted waypoints for the vis table this time too
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
		m_Waypoints[i].resetPaths();
		m_Waypoints[i].m_lFlags |= CWaypoint::DELETED;
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

			if(m_Waypoints[i].m_lFlags & CWaypoint::DELETED)
				continue;

			if(!getVisible(iThisWP,i))
				continue;

			if(m_Waypoints[i].isConnectedTo(iThisWP))
				continue;

			fDistance = (m_Waypoints[i].m_VOrigin
				-m_Waypoints[iThisWP].m_VOrigin).squareLength();

			if(fDistance > 210*210)
				continue;

			if(!g_Map.getReachable(m_Waypoints[i].m_VOrigin,m_Waypoints[iThisWP].m_VOrigin))
				continue;

			m_Waypoints[iThisWP].addConnectionTo(i);
		}
	}
}

int CWaypointGraph::getMaximum(CBitField *pValid, int iType, float fFactor){
	int i,iMax=-1;
	float fMax = -1e10;
	float fTemp;

	for(i=0; i<m_iNumWaypoints;i++){
		if(pValid){
			if(!pValid->getBit(i))
				continue;
		}

		if(m_Waypoints[i].m_lFlags & CWaypoint::DELETED)
			continue;

		switch(iType){
		case WG_SEARCH_AVDISTVWP:
			fTemp = m_Waypoints[i].m_fAvDistVWP * fFactor;
			break;
		case WG_SEARCH_IVWP:
			fTemp = m_Waypoints[i].m_iVisibleWaypoints * fFactor;
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
	if(iDamagedWP != -1){
		m_Waypoints[iDamagedWP].addDamaged((VDamageOrigin - VDamaged).make2D(),iDamage);
		if(m_Waypoints[iDamagedWP].m_lTotalDamage > m_lMaxTotalDamage){
			m_lMaxTotalDamage = m_Waypoints[iDamagedWP].m_lTotalDamage;
		}
	}

	int iDamageWP = g_Map.m_Waypoints.getNearest(VDamageOrigin,false,false,150);	// who does the damage
	if(iDamageWP != -1){
		m_Waypoints[iDamageWP].addDamage((VDamaged - VDamageOrigin).make2D(),iDamage);
		if(m_Waypoints[iDamageWP].m_lTotalDamage > m_lMaxTotalDamage){
			m_lMaxTotalDamage = m_Waypoints[iDamageWP].m_lTotalDamage;
		}
	}
}

void CWaypointGraph::addKill(Vector &VOrigin){
	int iWP = g_Map.m_Waypoints.getNearest(VOrigin,false,false,150);
	if(iWP != -1){
		m_Waypoints[iWP].addKill();
	}
}

void CWaypointGraph::addTraffic(int iWP){
	m_Waypoints[iWP].m_lTraffic ++;
}

void CWaypointGraph::resetStat(void){
	m_lMaxTotalDamage = 0;
	int i;
	for(i = 0; i < _MAX_WAYPOINTS; i++){
		m_Waypoints[i].reset();
	}
}
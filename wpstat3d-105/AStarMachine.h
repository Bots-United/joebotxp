// AStarMachine.h: interface for the AStarMachine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__ASTARMACHINE_H)
#define __ASTARMACHINE_H

#include "AStarBase.h"

#include "Line.h"
#include "Cube.h"

extern CObject **Objects;
extern long lObjects;
extern float rD;
extern FILE *g_astarmachinelog_fhd;
extern bool g_bDisplayAStarObj;

template <class tAStarGoal>
class AStarMachine : public AStarBase
{
public:
	AStarMachine(){
	}
	virtual ~AStarMachine(){
	}
	
	virtual void linkChild(AStarNode *pNode,const AStarNode *pChild){
		int g = pNode->m_g +
			m_Goal.getCost(pNode->m_iWaypoint,pChild->m_iWaypoint);
		
		AStarNode *pCheck;
		int iCheck;
		
		assert(pChild->m_iWaypoint > -1 && pChild->m_iWaypoint < _MAX_WAYPOINTS);
		
		if(iCheck = m_PQOpen.getIndex(pChild->m_iWaypoint)){
			assert(iCheck <= m_PQOpen.m_ln && iCheck >= 0);
#ifdef LOG
			g_astarmachinelog_fhd = fopen("astarlog.txt","a");fprintf(g_astarmachinelog_fhd,"%3li: ",m_lRevolution);fprintf(g_astarmachinelog_fhd,"is already on the open list #%i\n",pChild->m_iWaypoint);fclose(g_astarmachinelog_fhd);
#endif
			// it's already on the open list ...
			pCheck = m_PQOpen.getItem(iCheck)->m_p;
			
			pNode->m_Children.addChild(pCheck);
			if(g < pCheck->m_g){
#ifdef LOG
				g_astarmachinelog_fhd = fopen("astarlog.txt","a");fprintf(g_astarmachinelog_fhd,"%3li: ",m_lRevolution);fprintf(g_astarmachinelog_fhd,"updating #%i because the current g is better than the g of this waypoint on the open list\n",pCheck->m_iWaypoint);fprintf(g_astarmachinelog_fhd,"- setting #%i parent from #%i to #%i\n",pCheck->m_iWaypoint,pCheck->m_pParent->m_iWaypoint,pNode->m_iWaypoint);fclose(g_astarmachinelog_fhd);
#endif
				pCheck->m_pParent = pNode;
				pCheck->m_g = g;
				pCheck->m_f = g + pCheck->m_h;
				
				m_PQOpen.updated(iCheck);
			}
		}
		else if(iCheck = m_PQClosed.getIndex(pChild->m_iWaypoint)){
			assert(iCheck <= m_PQClosed.m_ln && iCheck >= 0);
#ifdef LOG
			g_astarmachinelog_fhd = fopen("astarlog.txt","a");fprintf(g_astarmachinelog_fhd,"%3li: ",m_lRevolution);fprintf(g_astarmachinelog_fhd,"is already on the closed list #%i\n",pChild->m_iWaypoint);fclose(g_astarmachinelog_fhd);
#endif
			// it's already on the closed list ...
			pCheck = m_PQClosed.getItem(iCheck)->m_p;
			
			pNode->m_Children.addChild(pCheck);
			
			if(g < pCheck->m_g){
#ifdef LOG
				g_astarmachinelog_fhd = fopen("astarlog.txt","a");fprintf(g_astarmachinelog_fhd,"%3li: ",m_lRevolution);fprintf(g_astarmachinelog_fhd,"updating #%i because the current g is better than the g of this waypoint on the closed list\n",pCheck->m_iWaypoint);	fprintf(g_astarmachinelog_fhd,"- setting #%i parent from #%i to #%i\n",pCheck->m_iWaypoint,pCheck->m_pParent->m_iWaypoint,pNode->m_iWaypoint);fclose(g_astarmachinelog_fhd);
#endif
				pCheck->m_pParent = pNode;
				pCheck->m_g = g;
				pCheck->m_f = g + pCheck->m_h;
				
				updateParents(pCheck);
			}
		}
		else{
			// it's neither on the open nor on the closed list ...
			AStarNode *pNewChild;
			pNewChild = newNode(pChild->m_iWaypoint);
			
			pNewChild->m_pParent = pNode;
			pNewChild->m_g = g;
			pNewChild->m_h = m_Goal.getDistance(pNewChild->m_iWaypoint,m_iDestination);
			pNewChild->m_f = pNewChild->m_h + g;
			
			AStarNodeContainer PnC;
			PnC.m_p = pNewChild;
			
			pNode->m_Children.addChild(pNewChild);
			m_PQOpen.insert(PnC);
			if(g_bDisplayAStarObj){
				Objects[lObjects] = new CLine;
				((CLine *)Objects[lObjects])->x1 = g_Map.m_Waypoints.m_Waypoints[pNode->m_iWaypoint].m_VOrigin.x/1000;
				((CLine *)Objects[lObjects])->y1 = g_Map.m_Waypoints.m_Waypoints[pNode->m_iWaypoint].m_VOrigin.y/1000;
				((CLine *)Objects[lObjects])->z1 = g_Map.m_Waypoints.m_Waypoints[pNode->m_iWaypoint].m_VOrigin.z/1000 + .01;
				
				((CLine *)Objects[lObjects])->x2 = g_Map.m_Waypoints.m_Waypoints[pChild->m_iWaypoint].m_VOrigin.x/1000;
				((CLine *)Objects[lObjects])->y2 = g_Map.m_Waypoints.m_Waypoints[pChild->m_iWaypoint].m_VOrigin.y/1000;
				((CLine *)Objects[lObjects])->z2 = g_Map.m_Waypoints.m_Waypoints[pChild->m_iWaypoint].m_VOrigin.z/1000 + .01;
				
				((CLine *)Objects[lObjects])->r = .0f;
				((CLine *)Objects[lObjects])->g = .9f;
				((CLine *)Objects[lObjects])->b = .0f;
				
				lObjects++;
			}
#ifdef LOG
			g_astarmachinelog_fhd = fopen("astarlog.txt","a");fprintf(g_astarmachinelog_fhd,"%3li: ",m_lRevolution);fprintf(g_astarmachinelog_fhd,"added to open list : #%i\n",PnC.m_p->m_iWaypoint);fclose(g_astarmachinelog_fhd);
#endif
		}
	}
	
	virtual void updateParents(AStarNode *pNode){
		//return;
		int ipNodeGi;
#ifdef LOG
		g_astarmachinelog_fhd = fopen("astarlog.txt","a");fprintf(g_astarmachinelog_fhd,"%3li: ",m_lRevolution);fprintf(g_astarmachinelog_fhd,"||| starting update parents for #%i\n",pNode->m_iWaypoint);fclose(g_astarmachinelog_fhd);
#endif
		AStarNode *kid,*pParent;
		AStarNodeChildren *pChildren = &(pNode->m_Children);
		int i;
		
		while(pChildren){
			for(i=pChildren->m_iChildren-1; i>=0 ; i--){
				kid = pChildren->m_ppChildren[i];
#ifdef LOG
				g_astarmachinelog_fhd = fopen("astarlog.txt","a");fprintf(g_astarmachinelog_fhd,"%3li: ",m_lRevolution);fprintf(g_astarmachinelog_fhd,"||| checking #%i\n",kid->m_iWaypoint);fclose(g_astarmachinelog_fhd);
#endif
				ipNodeGi = pNode->m_g+m_Goal.getCost(pNode->m_iWaypoint,kid->m_iWaypoint);
				if(ipNodeGi < kid->m_g){
#ifdef LOG
					g_astarmachinelog_fhd = fopen("astarlog.txt","a");fprintf(g_astarmachinelog_fhd,"%3li: ",m_lRevolution);fprintf(g_astarmachinelog_fhd,"||| ||| updating #%i ( #%i to #%i )\n",kid->m_iWaypoint,kid->m_pParent->m_iWaypoint,pNode->m_iWaypoint);fclose(g_astarmachinelog_fhd);
#endif
					kid->m_g = ipNodeGi;
					kid->m_f = kid->m_g + kid->m_h;
					kid->m_pParent = pNode;
					
					push(kid);
				}
			}
			
			pChildren = pChildren->m_pNext;
		}
#ifdef LOG
		g_astarmachinelog_fhd = fopen("astarlog.txt","a");fprintf(g_astarmachinelog_fhd,"%3li: ",m_lRevolution);fprintf(g_astarmachinelog_fhd,"||| emptying list\n");fclose(g_astarmachinelog_fhd);
#endif
		while(!empty()){
			pParent = top();
			if(g_bDisplayAStarObj){

				static float fZOffset = 0.01f;

				Objects[lObjects] = new CCube;
				((CCube *)Objects[lObjects])->x = g_Map.m_Waypoints.m_Waypoints[pParent->m_iWaypoint].m_VOrigin.x/1000;
				((CCube *)Objects[lObjects])->y = g_Map.m_Waypoints.m_Waypoints[pParent->m_iWaypoint].m_VOrigin.y/1000;
				((CCube *)Objects[lObjects])->z = g_Map.m_Waypoints.m_Waypoints[pParent->m_iWaypoint].m_VOrigin.z/1000+fZOffset;

				//fZOffset += .001;
				
				((CCube *)Objects[lObjects])->rx = rD-.005;
				((CCube *)Objects[lObjects])->ry = rD-.005;
				((CCube *)Objects[lObjects])->rz = rD-.005;
				
				((CCube *)Objects[lObjects])->r = 1;
				((CCube *)Objects[lObjects])->g = 0;
				((CCube *)Objects[lObjects])->b = 0;
				
				lObjects++;
			}
			pop();
			
			pChildren = &(pParent->m_Children);
			while(pChildren){
				for(i=pChildren->m_iChildren-1; i>=0 ; i--){
					kid = pChildren->m_ppChildren[i];
#ifdef LOG
					g_astarmachinelog_fhd = fopen("astarlog.txt","a");fprintf(g_astarmachinelog_fhd,"%3li: ",m_lRevolution);fprintf(g_astarmachinelog_fhd,"||| checking #%i\n",kid->m_iWaypoint);fclose(g_astarmachinelog_fhd);
#endif
					ipNodeGi = pParent->m_g+m_Goal.getCost(pParent->m_iWaypoint,kid->m_iWaypoint);
					if(ipNodeGi < kid->m_g){
#ifdef LOG
						g_astarmachinelog_fhd = fopen("astarlog.txt","a");fprintf(g_astarmachinelog_fhd,"%3li: ",m_lRevolution);fprintf(g_astarmachinelog_fhd,"||| ||| updating #%i ( #%i to #%i )\n",kid->m_iWaypoint,kid->m_pParent->m_iWaypoint,pParent->m_iWaypoint);fclose(g_astarmachinelog_fhd);
#endif
						kid->m_g = ipNodeGi;
						kid->m_f = kid->m_g + kid->m_h;
						kid->m_pParent = pParent;
						
						push(kid);
					}
				}
				
				pChildren = pChildren->m_pNext;
			}
		}
#ifdef LOG
		g_astarmachinelog_fhd = fopen("astarlog.txt","a");fprintf(g_astarmachinelog_fhd,"%3li: ",m_lRevolution);fprintf(g_astarmachinelog_fhd,"||| end update parents for #%i\n",pNode->m_iWaypoint);fclose(g_astarmachinelog_fhd);
#endif
	}
	
	virtual bool runAStar(void){
#ifdef LOG
		g_astarmachinelog_fhd = fopen("astarlog.txt","a");fprintf(g_astarmachinelog_fhd,"\n\nStarting AStar from %i to %i\n",m_iStart,m_iDestination);fclose(g_astarmachinelog_fhd);
#endif
		int iConnection,iConnectedWP,
			iCurrentWP;
		CWaypoint *pWP;
		AStarNode *P,*B,ASNTemp;
		AStarNodeContainer PC;
		
		if(m_bFinished){			// if the last job was finished, this is a new job
			if(m_iDestination == -1 || m_iStart == -1)
				return false;
			
			if(g_bDisplayAStarObj){
				Objects[lObjects] = new CCube;
				((CCube *)Objects[lObjects])->x = g_Map.m_Waypoints.m_Waypoints[m_iDestination].m_VOrigin.x/1000;
				((CCube *)Objects[lObjects])->y = g_Map.m_Waypoints.m_Waypoints[m_iDestination].m_VOrigin.y/1000;
				((CCube *)Objects[lObjects])->z = g_Map.m_Waypoints.m_Waypoints[m_iDestination].m_VOrigin.z/1000;
				
				((CCube *)Objects[lObjects])->rx = rD+.005;
				((CCube *)Objects[lObjects])->ry = rD+.005;
				((CCube *)Objects[lObjects])->rz = rD+.005;
				
				((CCube *)Objects[lObjects])->r = 1;
				((CCube *)Objects[lObjects])->g = 0;
				((CCube *)Objects[lObjects])->b = 0;
				
				lObjects++;
				Objects[lObjects] = new CCube;
				((CCube *)Objects[lObjects])->x = g_Map.m_Waypoints.m_Waypoints[m_iStart].m_VOrigin.x/1000;
				((CCube *)Objects[lObjects])->y = g_Map.m_Waypoints.m_Waypoints[m_iStart].m_VOrigin.y/1000;
				((CCube *)Objects[lObjects])->z = g_Map.m_Waypoints.m_Waypoints[m_iStart].m_VOrigin.z/1000;
				
				((CCube *)Objects[lObjects])->rx = rD+.005;
				((CCube *)Objects[lObjects])->ry = rD+.005;
				((CCube *)Objects[lObjects])->rz = rD+.005;
				
				((CCube *)Objects[lObjects])->r = 0;
				((CCube *)Objects[lObjects])->g = 0;
				((CCube *)Objects[lObjects])->b = 1;
				
				lObjects++;
			}

			resetNodes();
			
			m_iBreakReason = BR_NONE;			// no break reason by default
			
			m_lRevolution = 0;
			m_lCSlice2 = m_lSliceSize;
			
			P = newNode(m_iStart);
			
			PC.m_p = P;
			
			P->m_g = m_Goal.getCost(m_iStart,m_iStart);
			P->m_h = m_Goal.getDistance(m_iStart,m_iDestination);
			P->m_f = P->m_g + P->m_h;
			
			m_PQOpen.insert(PC);
		}
		else{
			m_lCSlice2 += m_lSliceSize;
		}
#ifdef LOG
		g_astarmachinelog_fhd = fopen("astarlog.txt","a");fprintf(g_astarmachinelog_fhd,"%3li: ",m_lRevolution);fprintf(g_astarmachinelog_fhd,"added to open list : #%i\n",PC.m_p->m_iWaypoint);fclose(g_astarmachinelog_fhd);
#endif
		
		while(true){
			if(m_lRevolution > m_lCSlice2){
				m_iBreakReason = BR_MAXSLICE;
				//logResult();
				//cout << "slicebreak" << endl;
				m_bFinished = false;
				return true;
			}

			// check if this is enough
			if(m_lRevolution > m_lMaxRevolutions){		// is the maximum revolution count reached ?
				m_iBreakReason = BR_MAXREV;
				m_bFinished = true;
				logResult();
				return false;
			}

			if(m_PQOpen.empty()){
				// no more open nodes - no path has been found
				m_bFinished = true;
				logResult();
				return false;
			}
			PC = m_PQOpen.getTop();
#ifdef LOG
			g_astarmachinelog_fhd = fopen("astarlog.txt","a");fprintf(g_astarmachinelog_fhd,"%3li: ",m_lRevolution);fprintf(g_astarmachinelog_fhd,"got from open list : #%i\n",PC.m_p->m_iWaypoint);fclose(g_astarmachinelog_fhd);
#endif
			B = PC.m_p;
			if(B->m_iWaypoint == m_iDestination){
				// a path has been found
				m_bFinished = true;
				m_pDestinationNode = B;
				logResult();
#ifdef LOG
				g_astarmachinelog_fhd = fopen("astarlog.txt","a");fprintf(g_astarmachinelog_fhd,"%3li: ",m_lRevolution);fprintf(g_astarmachinelog_fhd,"reached goal : %i\n",B->m_iWaypoint);fclose(g_astarmachinelog_fhd);
#endif
				return true;
			}
			iCurrentWP = B->m_iWaypoint;
			pWP = &(g_Map.m_Waypoints.m_Waypoints[iCurrentWP]);
			for(iConnection = pWP->getConnectionCount()-1; iConnection >= 0;iConnection--){
				iConnectedWP = pWP->getConnection(iConnection);
#ifdef LOG
				if(!pWP->isConnectedTo(iConnectedWP)){
					g_astarmachinelog_fhd = fopen("astarlog.txt","a");
					fprintf(g_astarmachinelog_fhd,"%3li: ",m_lRevolution);fprintf(g_astarmachinelog_fhd,"error Waypoint is not connected to ???\n");
					fclose(g_astarmachinelog_fhd);
				}
#endif
#ifdef LOG
				if(iConnectedWP == iCurrentWP){
					g_astarmachinelog_fhd = fopen("astarlog.txt","a");
					fprintf(g_astarmachinelog_fhd,"%3li: ",m_lRevolution);fprintf(g_astarmachinelog_fhd,"error Waypoint is connected to himself #%i\n",PC.m_p->m_iWaypoint);
					fclose(g_astarmachinelog_fhd);
				}
				if(iConnectedWP == -1){
					g_astarmachinelog_fhd = fopen("astarlog.txt","a");
					fprintf(g_astarmachinelog_fhd,"%3li: ",m_lRevolution);fprintf(g_astarmachinelog_fhd,"error Waypoint is connected to -1 #%i\n",PC.m_p->m_iWaypoint);
					fclose(g_astarmachinelog_fhd);
					continue;
				}
#endif
				
				if(m_Goal.isOpen(iCurrentWP,iConnectedWP)){		// is this move valid by goal ?
					ASNTemp.m_iWaypoint = iConnectedWP;
					
					linkChild(B,&ASNTemp);
				}
			}
			m_PQClosed.insertUnsorted(PC);
			if(g_bDisplayAStarObj){
				Objects[lObjects] = new CCube;
				((CCube *)Objects[lObjects])->x = g_Map.m_Waypoints.m_Waypoints[PC.m_p->m_iWaypoint].m_VOrigin.x/1000;
				((CCube *)Objects[lObjects])->y = g_Map.m_Waypoints.m_Waypoints[PC.m_p->m_iWaypoint].m_VOrigin.y/1000;
				((CCube *)Objects[lObjects])->z = g_Map.m_Waypoints.m_Waypoints[PC.m_p->m_iWaypoint].m_VOrigin.z/1000;
				
				((CCube *)Objects[lObjects])->rx = rD+.001;
				((CCube *)Objects[lObjects])->ry = rD+.001;
				((CCube *)Objects[lObjects])->rz = rD+.001;
				
				((CCube *)Objects[lObjects])->r = 0;
				((CCube *)Objects[lObjects])->g = 1;
				((CCube *)Objects[lObjects])->b = 0;
				
				lObjects++;
			}
			//UTIL_DrawBeam(listenserver_edict,g_Map.m_Waypoints.m_Waypoints[PC.m_p->m_iWaypoint].m_VOrigin+Vector(0,0,50),g_Map.m_Waypoints.m_Waypoints[PC.m_p->m_iWaypoint].m_VOrigin+Vector(0,0,0),10,0,0,255,0,255,1);
#ifdef LOG
			g_astarmachinelog_fhd = fopen("astarlog.txt","a");fprintf(g_astarmachinelog_fhd,"%3li: ",m_lRevolution);fprintf(g_astarmachinelog_fhd,"added to closed list : #%i\n",PC.m_p->m_iWaypoint);fclose(g_astarmachinelog_fhd);
#endif
			m_lRevolution ++;
		}
	}
	// data
	
	tAStarGoal m_Goal;
};

#endif // !defined(__ASTARMACHINE_H)

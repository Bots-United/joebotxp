//#include "waypoint.h"
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#include "pqueue.h"
#include "astar.h"
#include "map.h"
#include "hptime.h"

#include <windows.h>

#define _P 8192		// resolution for time table
#define _PPI 100
#define _PI 2

#define _STEP 1		// step size for looping thru wps

#define TMPP 100			// time measurements per path

long g_lCacheFuck = 100000;

void fuckthecache(void){
	if(g_lCacheFuck){
		static float pf[131072];
		static l,lo;
		
		for(int i = 0; i < g_lCacheFuck; i++){
			lo = l;
			l += 17;
			l = l & 131071;
			
			pf[l] += pf[lo];
		}
	}
}

AStarMachine<AStarBaseGoal> t;

void benchit(void){
	cout << "CPU @" << (double)(g_HPTime.getFrequency()/1000)/1000 <<"MHz"<< endl<<endl;
	/*CRespCurve test(5);

	test.addEntry(0,10);
	test.addEntry(10,100);
	test.addEntry(20,0);

	cout << test.getMaximum() << endl;
	cout << test.getValue(100) << endl;

	return;*/
	//g_Map.load("cs_office");
	//g_Map.load("performance");
	//g_Map.load("crossfire");

	long ls, ld;
	long lLoop = 0;
	CPQueue<__int64> runtime;		// runtime per path
	CPQueue<int> runtimepi;		// runtime per revolution/iteration
	CPQueue<int> iter;		// runtime per revolution/iteration
	__int64 lTime=-1,lLTime=0,i64tTime;
	long lCount,lTemp,lOut;
	long lWpNum = g_Map.m_Waypoints.m_iNumWaypoints;
	__int64 *plTime = new __int64[lWpNum * lWpNum * TMPP],lRawTime,iAvCTime,iAvCTimeT;
	long *plIter = new long[lWpNum * lWpNum],lTMPP;

	UINT64 t1,t2,tAR,tC1,/*tC2,*/tCT;

	cout << "benchmarking : ";cout.flush();
	for(ls=0;ls<lWpNum * lWpNum * TMPP;ls++)plTime[ls]=0;
	
	
	t1 = g_HPTime.getTime();
	t2 = g_HPTime.getTime();
	//while((t2-t1)/g_HPTime.getFrequency() < 10){
	lLoop = 1;
	
	//	cout << "Loop : "<<lLoop<<endl;
	for(lTMPP = 0; lTMPP < TMPP; lTMPP ++){
		cout<<lTMPP%10;cout.flush();
		for(ls = 0; ls < lWpNum; ls += _STEP){
			if(g_Map.m_Waypoints[ls].m_lFlags&CWaypoint::DELETED){
				continue;
			}
			for(ld = 0; ld < lWpNum; ld += _STEP){
				if(g_Map.m_Waypoints[ld].m_lFlags&CWaypoint::DELETED){
					continue;
				}

				t.resetNodes();

				tAR = g_HPTime.getTime();
				
				t.setStart(ls);
				t.setDestination(ld);
				
				t.runAStar();
				//g_Map.m_Waypoints.getNearest(g_Map.m_Waypoints[ls].m_VOrigin);
				//cout << long(plTime[lWpNum*ls+ld]) << " - ";
				plTime[lTMPP * lWpNum * lWpNum + lWpNum*ls + ld] += (g_HPTime.getTime()-tAR);
				
				fuckthecache();
				
				plIter[lWpNum*ls+ld] = t.m_lRevolution;
				//cout << long(plTime[lWpNum*ls+ld]) << endl;
			}
			//cout << ls << " : " << ld << " ... ";cout.flush()
		}
	}
	t2 = g_HPTime.getTime();
	/*		lLoop ++;
}*/
	cout << endl <<"finished benchmarking ... " << endl;

	// search lowest pathfinding time among the measurements to avoid measuring context switches etc.
	cout << "cleaning data ..." << endl;
	__int64 i64lowest,max = (__int64)2000000000*(__int64)2000000000,i64test;
	for(ls = 0; ls < lWpNum; ls += _STEP){
		if(g_Map.m_Waypoints[ls].m_lFlags&CWaypoint::DELETED){
			continue;
		}
		for(ld = 0; ld < lWpNum; ld += _STEP){
			if(g_Map.m_Waypoints[ld].m_lFlags&CWaypoint::DELETED){
				continue;
			}
			i64lowest = max;

			for(lTMPP = 0; lTMPP < TMPP; lTMPP ++){
				i64test = plTime[lTMPP * lWpNum * lWpNum + lWpNum*ls + ld];
				if(i64test<i64lowest)
					i64lowest = i64test;
			}
			plTime[0 * lWpNum * lWpNum + lWpNum*ls + ld] = i64lowest;		// set the first table to this lowest value, so we dont need any special code about multible measurements in the following lines
		}
	}

	cout << "inserting time per path in PQueue ";
	lOut = 0;
	for(ls = 0; ls < lWpNum; ls += _STEP){
		if(lOut < ls * 10 / lWpNum){		// just write a .
			lOut = ls * 10 / lWpNum;
			cout << ".";cout.flush();
		}
		for(ld = 0; ld < lWpNum; ld += _STEP){
			runtime.insert(plTime[lWpNum*ls+ld]);
		}
	}

	cout << endl << "inserting time per revolution in PQueue ";
	lOut = 0;
	for(ls = 0; ls < lWpNum; ls += _STEP){
		if(lOut < ls * 10 / lWpNum){		// just write a .
			lOut = ls * 10 / lWpNum;
			cout << ".";cout.flush();
		}
		for(ld = 0; ld < lWpNum; ld += _STEP){
			if(plIter[lWpNum*ls+ld])
				runtimepi.insert(plTime[lWpNum*ls+ld]/plIter[lWpNum*ls+ld]);
			iter.insert(plIter[lWpNum*ls+ld]);
		}
	}
	cout <<endl<< "calculating calculation time ";
	tC1 = 0;						// get the time how long all this stuff needed to compensate the result
	
	for(lTemp=0; lTemp < lLoop; lTemp++){
		for(lTMPP = 0; lTMPP < TMPP; lTMPP ++){
			for(ls = 0; ls < lWpNum; ls += _STEP){
				if(g_Map.m_Waypoints[ls].m_lFlags&CWaypoint::DELETED){
					continue;
				}
				for(ld = 0; ld < lWpNum; ld += _STEP){
					if(g_Map.m_Waypoints[ld].m_lFlags&CWaypoint::DELETED){
						continue;
					}
					tAR = g_HPTime.getTime();

					tCT = g_HPTime.getTime();

					t.setStart(ls);
					t.setDestination(ld);
					
					//t.runAStar();
					//g_Map.m_Waypoints.getNearest(g_Map.m_Waypoints[ls].m_VOrigin);
					//cout << long(plTime[lWpNum*ls+ld]) << " - ";
					plTime[lTMPP * lWpNum * lWpNum + lWpNum*ls + ld] += (g_HPTime.getTime()-tAR);
					
					fuckthecache();
					
					plIter[lWpNum*ls+ld] = t.m_lRevolution;

					tCT = g_HPTime.getTime() - tCT;
					tC1 += tCT;

					//cout << long(plTime[lWpNum*ls+ld]) << endl;
				}
				//cout << ls << " : " << ld << " ... ";cout.flush()
			}
		}
	}
	cout << endl;

	iAvCTime = (tC1)/(lWpNum*lWpNum/_STEP/_STEP*lLoop);		// compensating time per loop
	iAvCTimeT = (tC1)/(lWpNum*lWpNum/_STEP/_STEP);			// compensating time in total ( for all loops on one connection )

	/*while(!runtime.empty()){
		l64Temp = runtime.getTop();
		runtimecomp.insert(l64Temp);
}*/
	FILE *fhd=fopen("log.html","a");
	fprintf(fhd,"<table></tr><td>");
	fprintf(fhd,"\n\n\n<br><br><hr><br><br>\n\n\n<td><table>");
	fprintf(fhd,"<tr><th>Runtime in ms<br>(Res: %lf )<th>Runtime in cpu cycles<th>Number of searches<th>Loops : %li<br>clf %li<br>STEP %i<br>TMPP %i<br>WPNUM %li",double(_P) / (double)(__int64)(g_HPTime.getFrequency())*1000,lLoop,g_lCacheFuck,_STEP,TMPP,lWpNum);
	lCount = 0;
	lTime = 0;
	i64tTime = 0;
	while(!runtime.empty()){		// output how long each pathfinding process took
		lRawTime = runtime.getTop()/(__int64)lLoop;
		i64tTime += lRawTime;
		if(lRawTime != -1){
			lTime = (lRawTime - iAvCTime)/(_P)*(_P);
			
			//if(lTime!=-1)fprintf(fhd,"%lf\n",double(lTime+1)/double(lLoop)/(__int64)(g_HPTime.getFrequency())*1000);
			
			if(lTime != lLTime){
				if(lTime != lLTime + _P){
					for(long l = lLTime; l < lTime ; l += _P){
						fprintf(fhd,"<tr><td>%lf",double(l)/(double)(__int64)(g_HPTime.getFrequency())*1000);
						fprintf(fhd,"<td>%li",l);
						fprintf(fhd,"<td>%li",0);
					}
				}
				printf("%lf ms ( %li )\n",double(lTime)/(double)(__int64)(g_HPTime.getFrequency())*1000,lCount);
				fprintf(fhd,"<tr><td>%lf",double(lTime)/(double)(__int64)(g_HPTime.getFrequency())*1000);
				fprintf(fhd,"<td>%li",lRawTime - iAvCTimeT);
				fprintf(fhd,"<td>%li",lCount);
				lCount = 1;
			}
			else
				lCount ++;
			
			lLTime = lTime;
		}
	}
	fprintf(fhd,"</table>");


	fprintf(fhd,"\n\n\n<br><hr>\n\n\n<td><table>");
	fprintf(fhd,"<tr><th>average cycles per iteration<th>Number of searches");
	lCount = 0;
	lTime = 0;
	while(!runtimepi.empty()){		// output how long each pathfinding process took
		lRawTime = runtimepi.getTop()/(__int64)lLoop;
		if(lRawTime != -1){
			lTime = lRawTime/(_PPI)*(_PPI);
			
			//if(lTime!=-1)fprintf(fhd,"%lf\n",double(lTime+1)/double(lLoop)/(__int64)(g_HPTime.getFrequency())*1000);
			
			if(lTime != lLTime){
				/*if(lTime != lLTime + _P){
					for(long l = lLTime; l < lTime ; l += _P){
						fprintf(fhd,"<tr><td>%li",l);
						fprintf(fhd,"<td>%li",0);
					}
				}*/
				fprintf(fhd,"<tr><td>%li",lRawTime);
				fprintf(fhd,"<td>%li",lCount);
				lCount = 1;
			}
			else
				lCount ++;
			
			lLTime = lTime;
		}
	}
	fprintf(fhd,"</table>");


	fprintf(fhd,"\n\n\n<br><hr>\n\n\n<td><table>");
	fprintf(fhd,"<tr><th>iterations per path<th>Number of searches");
	lCount = 0;
	lTime = 0;
	while(!iter.empty()){		// output how long each pathfinding process took
		lRawTime = iter.getTop()/(__int64)lLoop;
		if(lRawTime != -1){
			lTime = lRawTime/(_PI)*(_PI);
			
			//if(lTime!=-1)fprintf(fhd,"%lf\n",double(lTime+1)/double(lLoop)/(__int64)(g_HPTime.getFrequency())*1000);
			
			if(lTime != lLTime){
				/*if(lTime != lLTime + _P){
					for(long l = lLTime; l < lTime ; l += _P){
						fprintf(fhd,"<tr><td>%li",l);
						fprintf(fhd,"<td>%li",0);
					}
				}*/
				fprintf(fhd,"<tr><td>%li",lRawTime);
				fprintf(fhd,"<td>%li",lCount);
				lCount = 1;
			}
			else
				lCount ++;
			
			lLTime = lTime;
		}
	}
	fprintf(fhd,"</table>");


	fprintf(fhd,"</table>");
	fclose(fhd);
	cout << "\n\n";

	/*while(!runtimepi.empty()){
		cout << runtimepi.getTop() << endl;
	}*/

	float d,dpl,dadpac;
	cout <<"compensating " << long(tC1) <<" ("<<long(iAvCTime)<<"("<<long(iAvCTimeT)<<")"<<"/ASC)"<< " of " << (double)(__int64)(t2-t1) << " ticks" <<endl;
	d = double((__int64)(t2-t1 - (tC1)))/(__int64)(g_HPTime.getFrequency());
	dpl = d/double(lLoop)/double(TMPP);
	dadpac = dpl/double(g_Map.m_Waypoints.m_iNumWaypoints*g_Map.m_Waypoints.m_iNumWaypoints/_STEP/_STEP) * 1000000;

	cout << double((__int64)(i64tTime))/(__int64)(g_HPTime.getFrequency())/double(lLoop)/double(TMPP)/double(g_Map.m_Waypoints.m_iNumWaypoints*g_Map.m_Waypoints.m_iNumWaypoints/_STEP/_STEP) * 1000000 << endl;

	cout << lLoop << " Loop(s)"<<endl;
	cout << endl << endl << d << " secs elapsed - " << dpl << " secs per loop"<< endl;
	cout << "that's " << dadpac << " usecs ( average ) per astar call" << endl;
	cout << endl << g_ASN_MPool.m_lCAlloc << " - "  << g_ASN_MPool.m_lCFree <<" ("<< g_ASN_MPool.m_lCAlloc-g_ASN_MPool.m_lCFree <<")"<< " - " << g_ASN_MPool.m_lCMax << endl;

	delete plTime;
}
#include <vector>
void main(void){
	cout << "precaching" << endl;
	g_Map.load("de_dust");
	cout << g_Map.m_Waypoints.m_iNumWaypoints << " waypoints loaded"<<endl;


	long lWpNum = g_Map.m_Waypoints.m_iNumWaypoints,lOut = 0;
	for(long ls = 0; ls < lWpNum; ls += (lWpNum/50)){
		if(lOut < ls * 11 / lWpNum){		// just write a .
			lOut = ls * 11 / lWpNum;
			cout << ".";cout.flush();
		}
		if(g_Map.m_Waypoints[ls].m_lFlags&CWaypoint::DELETED)continue;
		for(long ld = 0; ld < lWpNum; ld +=10){
			if(g_Map.m_Waypoints[ld].m_lFlags&CWaypoint::DELETED)continue;
			t.setStart(ls);
			t.setDestination(ld);
			t.runAStar();
		}
	}
	cout <<endl<< "end of precaching" << endl;
	for(int i = 0; i < 1; i++){
		benchit();
		if(g_lCacheFuck)
			g_lCacheFuck *= 10;
		else
			g_lCacheFuck = 1;
	}
}

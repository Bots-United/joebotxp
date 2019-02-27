// Profiler.h: interface for the CProfiler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__CPROFILER_H)
#define __CPROFILER_H

#include "HPTime.h"

//
// (c) Johannes Lampel a.k.a. @$3.1415rin
// This code is part of JoeBOT XP.
//
// Feel free to use this code in your own projects.
//

//
// This in game realtime profiling system is based on the ideas of an article of Steve Rabin in Game Programming Gems 1
//

#include <vector>
using namespace std;

class CProfileStats{
public:
	int m_iCalls;
	_hptime64 m_i64TotalTime;
	float m_fPercentage;
	float m_fMin,m_fMax;
};

class CProfileNode{
	friend class CProfiler;
public:
	CProfileNode();
	~CProfileNode();

	CProfileNode *hasChild(const char *);

	void begin(void);
	void end(void);

	void outputHTMLfile(const char *szFilename);

	void calcStats(void);
	void resetLogData(void);
	void output(char *,int,bool);			// outputs the profiler data to some char[]
	void setName(const char *);
	long getCheckSumRek(void);

protected:
	unsigned int m_iProfileInstances;       //# of times ProfileBegin called
	int m_iOpenProfiles;                    //# of times ProfileBegin w/o ProfileEnd
	_hptime64 m_i64StartTime;                  //The current open profile start time
	_hptime64 m_i64Accumulator;                //All samples this frame added together
	_hptime64 m_i64MaxTime;
	_hptime64 m_i64MinTime;

	CProfileNode *m_pParent;
	vector<CProfileNode *> m_ppChildren;    // children of this node

	char m_szName[80];                  // name of current profiler node
	long m_lNameCheckSum;

	CProfileStats m_Stats;
	bool m_bOpened;						// is this node open ?
};

// just a class for the instances created for profiling
class CProfileItem{
public:
	CProfileItem(char *);
	~CProfileItem();
protected:
	char m_szName[80];
};

// main profiler class
//
// we have one global instance of this class. local instances of CProfileItem ( those which are actually used to measure
// time ) call this class on construction and destruction
//
class CProfiler{
public:
	CProfiler();
	virtual ~CProfiler();
	void startProfile(void);			// should be called at the beginning of all ... although it doesnt do much yet

	void frame(void);				// called each frame to reset data

	bool begin(const char*);            // begin profile item
	bool end(const char *);             // end profile item

	void outputHTMLfile(const char *);
	void outputHTMLfileRek(const char *,CProfileNode *,FILE *);

	void profileDumpOutputToBuffer( char *szBuffer, CProfileNode *pNode = 0 );      // output information to a text array
	float createPath(CProfileNode *,char *);    // calculate the percentage of the calculation we have here

	void cursor_movein(void);
	void cursor_moveout(void);
	void cursor_movedown(void);
	void cursor_moveup(void);

	void enableFileOutput(void){
		m_bEnableFileLog = true;
	}
	void disableFileOutput(void){
		m_bEnableFileLog = false;
	}
	bool isFileOutput(void){
		return m_bEnableFileLog;
	}

	CProfileNode *getCursor(void);

protected:
	bool m_bEnableFileLog;

	_hptime64 m_i64endProfile;
	_hptime64 m_i64startProfile;

	vector<CProfileNode *> m_pNodes;
	CProfileNode *m_pCurrNode;
	CProfileNode *m_pRoot;
	CProfileNode *m_pCursor;

	_hptime64 m_i64LastStat;                   // when was the last stat copy ?
};

extern CProfiler g_Profiler;

#define PROFILE(a) CProfileItem _profileiteminstance__profile(a)
//#define PROFILE(a)

#endif // !defined(__CPROFILER_H)

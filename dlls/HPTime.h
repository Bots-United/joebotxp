// Time.h: interface for the CTime class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__HPTIME_H)
#define __HPTIME_H

//
// (c) Johannes Lampel a.k.a. @$3.1415rin
// This code is part of JoeBOT XP.
//
// Feel free to use this code in your own projects.
//

//
// The base of the cpu frequency measurement stuff is copied from somewhere, I guess it was from somewhere like
// codeproject.com, unfortunately I dunno anymore who was the original author of that pieces of code ... *runningaway*
//
// So first, thanks to the unkown.
// Additional thanks to William van der Sterren, who put me on the track towards rdtsc
//

//
// I recently started to port this code for gcc, so it may be again buggy.
// I especially havent tested yet the CTime::CCPUSpeed::GetCyclesDifference function where are some bitshifting
// operations which are used to be buggy, at least when I code such stuff without having much time /@$3.1415rin 01/2004
//

typedef	void (*DELAY_FUNC)(unsigned int uiMS);
#ifdef __GNUC__
	typedef long long _hptime64;
#else
	typedef  __int64 _hptime64;
#endif

#define RDTSC_INSTRUCTION		_asm _emit 0x0f _asm _emit 0x31
#define	CPUSPEED_I32TO64(x, y)		(((_hptime64) x << 32) + y)

class CTime  {
	class CCPUSpeed {
	public:
		CCPUSpeed ();
		~CCPUSpeed ();
		
		// Variables.
		_hptime64 m_i64CPUSpeed;

		// Functions.
		_hptime64 GetCyclesDifference (DELAY_FUNC, unsigned int);
		
	//private:
		// Functions.
		static void Delay (unsigned int);
		static void DelayOverhead (unsigned int);		
	};
public:
	CTime();
	virtual ~CTime();

	_hptime64 getTime(void);
	double getTime_d(void);
	void markTick(void);
	_hptime64 getElapsed(void);
	double getElapsed_d(void);
	_hptime64 getMarkerDiff(void);
	double getMarkerDiff_d(void);
	_hptime64 getFrequency(void){
		//return m_i64Frequency;
		return m_CPUSpeed.m_i64CPUSpeed;
	}
protected:
	double getExactTimewoCPU(void);

	//_hptime64 m_i64Frequency;
	_hptime64 m_i64Marker;
	_hptime64 m_i64MarkerDiff;
	CCPUSpeed m_CPUSpeed;
#ifdef _WIN32
	double m_dStdFrequency;
#endif
};

extern CTime g_HPTime;

#endif // !defined(__HPTIME_H)

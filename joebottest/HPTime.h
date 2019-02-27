// Time.h: interface for the CTime class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__HPTIME_H)
#define __HPTIME_H

typedef	void (*DELAY_FUNC)(unsigned int uiMS);

#define RDTSC_INSTRUCTION			_asm _emit 0x0f _asm _emit 0x31
#define	CPUSPEED_I32TO64(x, y)		(((__int64) x << 32) + y)

class CTime  {
	class CCPUSpeed {
	public:
		CCPUSpeed ();
		~CCPUSpeed ();
		
		// Variables.
		__int64 m_i64CPUSpeed;
		
		// Functions.
		__int64 __cdecl GetCyclesDifference (DELAY_FUNC, unsigned int);
		
	//private:
		// Functions.
		static void Delay (unsigned int);
		static void DelayOverhead (unsigned int);		
	};
public:
	CTime();
	virtual ~CTime();

	_int64 getTime(void);
	double getTime_d(void);
	void markTick(void);
	_int64 getElapsed(void);
	double getElapsed_d(void);
	_int64 getMarkerDiff(void);
	double getMarkerDiff_d(void);
	_int64 getFrequency(void){
		//return m_i64Frequency;
		return m_CPUSpeed.m_i64CPUSpeed;
	}
protected:
	double getExactTimewoCPU(void);

	//_int64 m_i64Frequency;
	_int64 m_i64Marker;
	_int64 m_i64MarkerDiff;
	CCPUSpeed m_CPUSpeed;
#ifdef _WIN32
	double m_dStdFrequency;
#endif
};

extern CTime g_HPTime;

#endif // !defined(__HPTIME_H)

// HPTime.cpp: implementation of the CTime class.
//
//////////////////////////////////////////////////////////////////////

#include "HPTime.h"

#ifdef _WIN32
#include <windows.h>
#include <time.h>
#else
#include <time.h>
#include <stdio.h>
#include <sys/timeb.h>
#endif

// system timer is at 1193182 Hz

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTime g_HPTime;

#ifdef __GNUC__
timeb g_StartTimeb;
#endif

CTime::CTime(){
#ifdef _WIN32
	//g_StartTime = ((double)timeGetTime()) / 1000.0f;
	
	UINT64 f;
	QueryPerformanceFrequency((LARGE_INTEGER *)&f);
	m_dStdFrequency = (double) (_hptime64)f;
#else
	ftime(&g_StartTimeb);
#endif
	markTick();
}

double CTime::getExactTimewoCPU( void )
{
#ifdef _WIN32
	UINT64 l;
	QueryPerformanceCounter((LARGE_INTEGER *)&l);

	return ((double) (_hptime64)l / m_dStdFrequency);
#else
	timeb tp;

	ftime(&tp);

	return tp.time - g_StartTimeb.time +
	  (tp.millitm/1000.0) - (g_StartTimeb.millitm/1000.0);
#endif
}

void CTime::markTick(void){
	_hptime64 i64time = getTime();
	m_i64MarkerDiff = i64time - m_i64Marker;
	m_i64Marker = i64time;
}

_hptime64 CTime::getElapsed(void){
	return (getTime() - m_i64Marker);
}

_hptime64 CTime::getMarkerDiff(void){
	return m_i64MarkerDiff;
}

_hptime64 CTime::getTime(){
	_hptime64 rdtsc_time;
#ifdef __GNUC__
	__asm__ __volatile__( "rdtsc" : "=A" (rdtsc_time));
#else
	__asm
	{ 
		rdtsc
		mov dword ptr [rdtsc_time],eax
		mov dword ptr [rdtsc_time+4],edx
	}
#endif
	return rdtsc_time;
}

double CTime::getTime_d(void){
	return (double)getTime() / (double)getFrequency();
}

double CTime::getElapsed_d(void){
	return (double)getElapsed() / (double)getFrequency();
}

double CTime::getMarkerDiff_d(void){
	return (double)getMarkerDiff() / (double)getFrequency();
}

CTime::~CTime(){
}

CTime::CCPUSpeed::CCPUSpeed (){
	unsigned int uiRepetitions = 1;
	unsigned int uiMSecPerRepetition;
	_hptime64	i64Total = 0, i64Overhead = 0;

#ifdef _WIN32
	uiMSecPerRepetition = 50;
#else
	uiMSecPerRepetition = 1000;
#endif

	for (unsigned int nCounter = 0; nCounter < uiRepetitions; nCounter ++) {
		i64Total += GetCyclesDifference (CTime::CCPUSpeed::Delay, uiMSecPerRepetition);
		i64Overhead += GetCyclesDifference (CTime::CCPUSpeed::DelayOverhead, uiMSecPerRepetition);
	}

	// Calculate the MHz speed.
	i64Total -= i64Overhead;
	i64Total *= 1000;
	i64Total /= uiRepetitions;
	i64Total /= uiMSecPerRepetition;

	// Save the CPU speed.
	m_i64CPUSpeed = i64Total;
}

CTime::CCPUSpeed::~CCPUSpeed (){
}

_hptime64 CTime::CCPUSpeed::GetCyclesDifference (DELAY_FUNC DelayFunction, unsigned int uiParameter){
	unsigned int edx1, eax1;
	unsigned int edx2, eax2;
#ifdef __GNUC__
	long long l1,l2;
	__asm__ __volatile__( "rdtsc" : "=A" (l1));
	DelayFunction(uiParameter);
	__asm__ __volatile__( "rdtsc" : "=A" (l2));

	edx1 = l1;
	eax1 = l1>>32;

	edx1 = l2;
	eax1 = l2>>32;

#else
	// Calculate the frequency of the CPU instructions.
	try {
		__asm {
			push uiParameter		; push parameter param
			mov ebx, DelayFunction	; store func in ebx

			RDTSC_INSTRUCTION

			mov esi, eax			; esi = eax
			mov edi, edx			; edi = edx

			call ebx				; call the delay functions

			RDTSC_INSTRUCTION

			pop ebx

			mov edx2, edx			; edx2 = edx
			mov eax2, eax			; eax2 = eax

			mov edx1, edi			; edx2 = edi
			mov eax1, esi			; eax2 = esi
		}
	}

	// A generic catch-all just to be sure...
	catch (...) {
		return -1;
	}
#endif

	return (CPUSPEED_I32TO64 (edx2, eax2) - CPUSPEED_I32TO64 (edx1, eax1));
}

void CTime::CCPUSpeed::Delay (unsigned int uiMS)		// delay in milliseconds
{
#ifdef _WIN32
	LARGE_INTEGER Frequency, StartCounter, EndCounter;
	_hptime64 x;

	// Get the frequency of the high performance counter.
	if (!QueryPerformanceFrequency (&Frequency)) return;
	x = Frequency.QuadPart / 1000 * uiMS;

	// Get the starting position of the counter.
	QueryPerformanceCounter (&StartCounter);

	do {
		// Get the ending position of the counter.	
		QueryPerformanceCounter (&EndCounter);
	} while (EndCounter.QuadPart - StartCounter.QuadPart < x);
#else
	clock_t s,e;
	s = clock();
	e = s + CLOCKS_PER_SEC * uiMS / 1000;

	do{
		s = clock();
	}while(s<e);
#endif
}

void CTime::CCPUSpeed::DelayOverhead (unsigned int uiMS)
{
#ifdef _WIN32
	LARGE_INTEGER Frequency, StartCounter, EndCounter;
	_hptime64 x;

	// Get the frequency of the high performance counter.
	if (!QueryPerformanceFrequency (&Frequency)) return;
	x = Frequency.QuadPart / 1000 * uiMS;

	// Get the starting position of the counter.
	QueryPerformanceCounter (&StartCounter);
	
	do {
		// Get the ending position of the counter.
		QueryPerformanceCounter (&EndCounter);
	} while (EndCounter.QuadPart - StartCounter.QuadPart == x);
#else
	clock_t s,e;
	s = clock();
	e = s + CLOCKS_PER_SEC *  uiMS / 1000;

	do{
		s = clock();
	}while(s==e);
#endif
}

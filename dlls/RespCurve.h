// RespCurve.h: interface for the CRespCurve class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__RespCurve_H)
#define __RespCurve_H

struct RespCurveEntry_t{
	float m_fx;
	float m_fy;
};

#include <vector>
using namespace std;

class CRespCurve  
{
public:
	CRespCurve();
	virtual ~CRespCurve();

	bool addEntry(float,float);
	float getValue(float) const;
	float getMaximum(void) const;
	
	CRespCurve &operator =(const CRespCurve &);
protected:
	vector<RespCurveEntry_t> m_ppfSlots;
};

#endif // !defined(__RespCurve_H)

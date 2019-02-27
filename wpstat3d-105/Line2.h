// Line2.h: interface for the CLine2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINE2_H__264AB101_A5D8_4E56_AD8D_C3A3E915BBD3__INCLUDED_)
#define AFX_LINE2_H__264AB101_A5D8_4E56_AD8D_C3A3E915BBD3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Object.h"

class CLine2 : public CObject  
{
public:
	virtual void Draw(void);
	CLine2();
	virtual ~CLine2();

	void set1Color3fv(float *pf){r1=pf[0];g1=pf[1];b1=pf[2];}
	void set2Color3fv(float *pf){r2=pf[0];g2=pf[1];b2=pf[2];}

	float x1,y1,z1,x2,y2,z2,r1,g1,b1,r2,g2,b2;
};

#endif // !defined(AFX_LINE2_H__264AB101_A5D8_4E56_AD8D_C3A3E915BBD3__INCLUDED_)

// Line.h: interface for the CLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINE_H__264AB101_A5D8_4E56_AD8D_C3A3E915BBD3__INCLUDED_)
#define AFX_LINE_H__264AB101_A5D8_4E56_AD8D_C3A3E915BBD3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Object.h"

class CLine : public CObject  
{
public:
	virtual void Draw(void);
	CLine();
	virtual ~CLine();

	void setColor3fv(float *pf){r=pf[0];g=pf[1];b=pf[2];}

	float x1,y1,z1,x2,y2,z2,r,g,b;
};

#endif // !defined(AFX_LINE_H__264AB101_A5D8_4E56_AD8D_C3A3E915BBD3__INCLUDED_)

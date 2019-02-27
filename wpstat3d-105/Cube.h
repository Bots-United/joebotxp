// Cube.h: interface for the CCube class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUBE_H__FC4A667D_0DA5_4580_B6D0_DC80ABE551C4__INCLUDED_)
#define AFX_CUBE_H__FC4A667D_0DA5_4580_B6D0_DC80ABE551C4__INCLUDED_

#include "Object.h"

class CCube : public CObject  
{
public:
	virtual void Draw(void);
	CCube();
	virtual ~CCube();
	void setColor3fv(float *pf){r=pf[0];g=pf[1];b=pf[2];}

	float x,y,z,r,g,b,rx,ry,rz;
};

#endif // !defined(AFX_CUBE_H__FC4A667D_0DA5_4580_B6D0_DC80ABE551C4__INCLUDED_)

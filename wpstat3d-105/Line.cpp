// Line.cpp: implementation of the CLine class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>		// Header File For Windows
#include <stdio.h>			// Header File For Standard Input/Output
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library
#include "Line.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLine::CLine()
{

}

CLine::~CLine()
{

}

void CLine::Draw()
{
	glBegin(GL_LINES);								// Drawing Using Triangles

		float f[3] = {r,g,b};
		float fa[3] = {r/2.f,g/2.f,b/2.f};

		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,f);
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,f);

		/*float xn,yn,zn,xi=5,yi=2,zi=6;
		xn = (y1-y2)*zi-(z1-z2)*yi;
		yn = (x1-x2)*zi-xi*(z1-z2);
		zn = (x1-x2)*yi-xi*(y1-y2);
		glNormal3f(xn,yn,zn);*/
		glColor3f(r,g,b);
		glVertex3f(x1,y1,z1);
		glColor3f(r,g,b);
		glVertex3f(x2,y2,z2);
	glEnd();	
}

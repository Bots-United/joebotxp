// Cube.cpp: implementation of the CCube class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>		// Header File For Windows
#include <stdio.h>			// Header File For Standard Input/Output
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library

#include "Cube.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCube::CCube()
{

}

CCube::~CCube()
{

}

void CCube::Draw()
{
	glBegin(GL_QUADS);									// Draw A Quad
		float f[3] = {r,g,b};
		float fh[3] = {r,g,b};

		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,f);
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,fh);

		glColor3f(r,g,b);
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glVertex3f(x+rx,y+ry,z-rz);					// Top Right Of The Quad (Top)
		glVertex3f(x-rx,y+ry,z-rz);					// Top Left Of The Quad (Top)
		glVertex3f(x-rx,y+ry,z+rz);					// Bottom Left Of The Quad (Top)
		glVertex3f(x+rx,y+ry,z+rz);					// Bottom Right Of The Quad (Top)z
		glColor3f(r,g,b);
		glNormal3f( 0.0f, 0.0f,-1.0f);
		glVertex3f(x+rx,y-ry,z+rz);					// Top Right Of The Quad (Bottom)
		glVertex3f(x-rx,y-ry,z+rz);					// Top Left Of The Quad (Bottom)
		glVertex3f(x-rx,y-ry,z-rz);					// Bottom Left Of The Quad (Bottom)
		glVertex3f(x+rx,y-ry,z-rz);					// Bottom Right Of The Quad (Bottom)
		glColor3f(r,g,b);
		glNormal3f( 0.0f, 1.0f, 0.0f);
		glVertex3f(x+rx,y+ry,z+rz);					// Top Right Of The Quad (Front)
		glVertex3f(x-rx,y+ry,z+rz);					// Top Left Of The Quad (Front)
		glVertex3f(x-rx,y-ry,z+rz);					// Bottom Left Of The Quad (Front)
		glVertex3f(x+rx,y-ry,z+rz);					// Bottom Right Of The Quad (Front)
		glColor3f(r,g,b);
		glNormal3f( 0.0f,-1.0f, 0.0f);
		glVertex3f(x+rx,y-ry,z-rz);					// Top Right Of The Quad (Back)
		glVertex3f(x-rx,y-ry,z-rz);					// Top Left Of The Quad (Back)
		glVertex3f(x-rx,y+ry,z-rz);					// Bottom Left Of The Quad (Back)
		glVertex3f(x+rx,y+ry,z-rz);					// Bottom Right Of The Quad (Back)
		glColor3f(r,g,b);
		glNormal3f( 1.0f, 0.0f, 0.0f);
		glVertex3f(x-rx,y+ry,z+rz);					// Top Right Of The Quad (Left)
		glVertex3f(x-rx,y+ry,z-rz);					// Top Left Of The Quad (Left)
		glVertex3f(x-rx,y-ry,z-rz);					// Bottom Left Of The Quad (Left)
		glVertex3f(x-rx,y-ry,z+rz);					// Bottom Right Of The Quad (Left)
		glColor3f(r,g,b);
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glVertex3f(x+rx,y+ry,z-rz);					// Top Right Of The Quad (Right)
		glVertex3f(x+rx,y+ry,z+rz);					// Top Left Of The Quad (Right)
		glVertex3f(x+rx,y-ry,z+rz);					// Bottom Left Of The Quad (Right)
		glVertex3f(x+rx,y-ry,z-rz);					// Bottom Right Of The Quad (Right)
	glEnd();											// Done Drawing The Quad
}

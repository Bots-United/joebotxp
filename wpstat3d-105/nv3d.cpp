#include <windows.h>		// Header File For Windows
#include <winuser.h>		// Header File For Windows
#include <math.h>
#include <time.h>
#include <stdio.h>			// Header File For Standard Input/Output
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library

#include "CFrameTimer.h"

#include "dialog.h"
#include "resource.h"

//#include "NNSim/NeuralNet.h"

#include "Cube.h"
#include "Line.h"
#include "Line2.h"

#include "NV3dfile.h"

#define _ZEILENAB -1

OPENFILENAME ofn;

#include "Map.h"
#include "BWP.h"

const char szOverviewPath[10][80]={
"c:\\spiele\\half-life\\cstrike\\overviews\\",
"c:\\spiele\\half-life\\valve\\overviews\\",
"c:\\spiele\\half-life\\dod\\overviews\\",
"c:\\spiele\\half-life\\tfc\\overviews\\",
"c:\\spiele\\half-life\\gearbox\\overviews\\",
"e:\\Half-Life\\cstrike\\overviews\\",
"\0"};
bool g_bshowbwp = true;

HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

bool	keys[256];			// Array Used For The Keyboard Routine
bool	okeys[256];
bool	active=TRUE;		// Window Active Flag Set To TRUE By Default
bool	fullscreen=FALSE;	// Fullscreen Flag Set To Fullscreen Mode By Default
bool	alias = false;
bool	light =false;				// Lighting ON/OFF
bool	fog = true;
bool	lp;					// L Pressed?
bool	lpa;				// A Pressed?
bool	lpo;				// ...
bool	fp;					// F Pressed?
bool bDisplayConnections = true;
bool bDisplayLOS = false;
bool bDisplayOverview = true;
bool bDisplayNeurons = true;		// here the neurons are the waypoints :)
bool bDisplayOrigin = false;
bool bDisplayText = true;			// no aequivalent here
bool bDisplayDesc = true;
bool bDisplayFPS = true;

bool g_bDisplayAStarObj = true;

bool bColoredConnections = true;

CFrameTimer CFTimer;

float fDangerV= .3f;

int iStart = 11;
int iEnd = 12;

int iTOD = ID_DISPLAY_DEFAULT;

#define _LD 2
#define _SENSIMOUSE .2f

//CNeuralNetBProp NN;

CObject **Objects = 0;
long lObjects = 0;

#include "AStarGoalCover.h"

#include "astarmachine.h"
#include "astarbasegoal.h"

AStarMachine<AStarGoalCover> t;

float tX=0,tY=0,tZ=0;
float tXS=0,tYS=0,tZS=5;

GLfloat rD = .02f;

GLfloat	xrot =45;				// X Rotation
GLfloat	yrot  =-45;				// Y Rotation
GLfloat	xrots;				// X Rotation
GLfloat	yrots;				// Y Rotation
GLfloat xspeed;				// X Rotation Speed
GLfloat yspeed;				// Y Rotation Speed
GLfloat fOverviewZ = -.01f;
GLfloat fOverviewR = 0;
GLfloat fOZoom = 1.12f*2;
GLfloat fOOffset[3]={-.103f,1.011f,-.193f};
//GLfloat	z=-15.0f;			// Depth Into The Screen

GLfloat LightAmbient[]=		{ 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[]=		{ 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[]=	{ 0.0f, 0.0f, -20.0f, -10.0f };

GLuint	fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR };	// Storage For Three Types Of Fog
GLuint	fogfilter = 1;								// Which Fog Mode To Use 
GLfloat	fogColor[4] = {.0f,.0f,.0f,1.0f};		// Fog Color

GLuint	filter;				// Which Filter To Use
//GLuint	texture[3];			// Storage For 3 Textures

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

GLuint	base;
GLYPHMETRICSFLOAT gmf[256];	// Storage For Information About Our Outline Font Characters

GLuint texture[1];

char szOverview[80]="\0";
int screenie_number = 0;

GLbyte data[1024][1024][4];

void createColor(float *pf,float fV){
	fV *= 6;
	pf[0] = fV/2.f;
	pf[1] = fV>2.f?(fV-2.f)/2.f:0;
	pf[2] = fV>4.f?(fV-4.f)/2.f:0;

	if(pf[0]>1)pf[0]=1;
	if(pf[1]>1)pf[1]=1;
	if(pf[2]>1)pf[2]=1;
}

int TakeScreenshot(void){
	char current_screenie_path[80];
	GLubyte *temp_bits; // holds RGBA data
	GLubyte *bitmap_bits; // holds RGB data
	int viewport[4];
	int bmp_width;
	int bmp_height;
	
	sprintf( current_screenie_path, "c:/screenshots/shot%d-%li.bmp", screenie_number,time(NULL));
	
	glGetIntegerv( GL_VIEWPORT, viewport );
	
	bmp_width = viewport[2];
	bmp_height = viewport[3];
	
	temp_bits = new GLubyte[ ( bmp_width * bmp_height ) * 4 ];
	bitmap_bits = new GLubyte[ ( bmp_width * bmp_height ) * 3 ];
	
	// we need to copy the data from the frame buffer into the temp_bits
	// then copy the RGB info from that into bitmap_bits
	glReadPixels( 0, 0, bmp_width, bmp_height, GL_RGBA, GL_UNSIGNED_BYTE, temp_bits );
	
	int b = 0;
	for( int t = 0; t < bmp_width * bmp_height * 4; t += 4, b += 3 )
	{
		// eliminate A and reverse RGB to BGR
		bitmap_bits[b] = temp_bits[t+2];
		bitmap_bits[b+1] = temp_bits[t+1];
		bitmap_bits[b+2] = temp_bits[t];
	}
	
	delete[] temp_bits;
	
	BITMAPFILEHEADER header;
	BITMAPINFOHEADER info;
	
	header.bfType = 'MB';
	header.bfOffBits = sizeof( header ) + sizeof( info );
	header.bfSize = sizeof( header ) + sizeof( info ) + ( bmp_width * bmp_height * 3 );
	header.bfReserved1 = 0;
	header.bfReserved2 = 0;
	
	info.biBitCount = 24;
	info.biClrImportant = 0;
	info.biClrUsed = 0;
	info.biCompression = 0;
	info.biHeight = bmp_height;
	info.biWidth = bmp_width;
	info.biPlanes = 1;
	info.biSize = sizeof( info );
	info.biSizeImage = bmp_width * bmp_height * 3;
	info.biXPelsPerMeter = 2952;
	info.biYPelsPerMeter = 2952;
	
	OFSTRUCT file_data;
	int file_handle;
	
	if( ( file_handle = OpenFile( current_screenie_path, &file_data, OF_WRITE | OF_CREATE ) ) == -1 )
	{
		//Error( "ERROR: cannot create new screenshot file." );
		return -1;
	}
	
	// write data to bitmap
	_lwrite( file_handle, (char*)&header, sizeof( header ) );
	_lwrite( file_handle, (char*)&info, sizeof( info ) );
	_lwrite( file_handle, (char*)bitmap_bits, bmp_width * bmp_height * 3 );
	
	_lclose( file_handle );
	
	delete[] bitmap_bits;
	delete[] temp_bits;
	
	// now update current_screenie_path for the next time this function is called
	screenie_number++;
	//sprintf( current_screenie_path, "screenshots/shot%d.bmp", screenie_number );
	//last_screenie_time = GetTickCount();
	//LogInit( "Screenshot taken.\n" );
	
	return 1;
}

AUX_RGBImageRec *LoadBMP(char *Filename)				// Loads A Bitmap Image
{
	FILE *File=NULL;									// File Handle
	
	if (!Filename)										// Make Sure A Filename Was Given
	{
		return NULL;									// If Not Return NULL
	}
	
	if(strlen(Filename))
		File=fopen(Filename,"r");							// Check To See If The File Exists
	else
		File = 0;
	
	if (File)											// Does The File Exist?
	{
		fclose(File);									// Close The Handle
		return auxDIBImageLoad(Filename);				// Load The Bitmap And Return A Pointer
	}
	
	return NULL;										// If Load Failed Return NULL
}

int LoadGLTextures()									// Load Bitmaps And Convert To Textures
{
	int Status=FALSE;									// Status Indicator
	
	AUX_RGBImageRec *TextureImage[1];					// Create Storage Space For The Texture
	
	memset(TextureImage,0,sizeof(void *)*1);           	// Set The Pointer To NULL
	
	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	if (TextureImage[0]=LoadBMP(szOverview))
	{
		Status=TRUE;									// Set The Status To TRUE
		
		glGenTextures(1, &texture[0]);					// Create The Texture
		
		memset(data,0,sizeof(GLbyte)*1024*1024*4);
		//memcpy(data,TextureImage[0]->data,sizeof(GLbyte)*TextureImage[0]->sizeX*TextureImage[0]->sizeY*3);
		// remove fuckin' green
		long lx,ly;
		GLbyte *p;
		for(lx=0;lx < 1024;lx++){
			for(ly=0;ly < 1024;ly++){
				p = data[lx][ly];
				if(lx<768){
					p[0] = TextureImage[0]->data[3*1024*lx+3*ly];
					p[1] = TextureImage[0]->data[3*1024*lx+3*ly+1];
					p[2] = TextureImage[0]->data[3*1024*lx+3*ly+2];
				}
				p[3] = -127;
				if(p[0]==0
					&&p[2]==0
					&&p[1]<0){
					p[1] = 0;
					p[3] = 0;
				}
			}
		}
		
		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		
		char szOverviewInfo[80];		// get the related .txt file
		char *szChange;
		strcpy(szOverviewInfo,szOverview);
		//MessageBox(0,szOverviewInfo,"",MB_OK);
		strlwr(szOverviewInfo);
		szChange=strstr(szOverviewInfo,".bmp");
		if(szChange){
			strcpy(szChange,".txt");
			char szFile[1024];
			FILE *fhd;
			fhd = fopen(szOverviewInfo,"r");
			if(!fhd)
				return false;
			fread(szFile,sizeof(char),1024,fhd);
			fclose(fhd);
			szChange = strstr(szFile,"ZOOM");
			sscanf(szChange,"ZOOM\t%f",&fOZoom);
			szChange = strstr(szFile,"ORIGIN");
			sscanf(szChange,"ORIGIN\t%f\t%f\t%f",&fOOffset[0],&fOOffset[1],&fOOffset[2]);
			fOOffset[0] /= 1000.f;
			fOOffset[1] /= 1000.f;
			fOOffset[2] /= 1000.f;
			szChange = strstr(szFile,"ROTATED");
			sscanf(szChange,"ROTATED\t%f",&fOverviewR);
			fOverviewR *= 90;
			szChange = strstr(szFile,"HEIGHT");
			sscanf(szChange,"HEIGHT\t%f",&fOverviewZ);
			fOverviewZ /= 1000.f;
			fOverviewZ -= fOOffset[2];
			
			sprintf(szFile,"%f - %f %f %f - %f - %f",fOZoom,fOOffset[0],fOOffset[1],fOOffset[2],fOverviewR,fOverviewZ);
			SetWindowText(hWnd,szFile);
		}
	}
	
	if (TextureImage[0])									// If Texture Exists
	{
		if (TextureImage[0]->data)							// If Texture Image Exists
		{
			free(TextureImage[0]->data);					// Free The Texture Image Memory
		}
		
		free(TextureImage[0]);								// Free The Image Structure
	}
	
	return Status;										// Return The Status
}

void CheckOverview(void){
	*szOverview = 0;
	int i = 0;
	FILE *fhd;
	const char *szPath = &(szOverviewPath[i][0]);

	while(*szPath){
		char szOverviewTest[256];
		strcpy(szOverviewTest,szPath);
		strcat(szOverviewTest,g_Map.m_Waypoints.header.szMapname);
		strcat(szOverviewTest,".bmp");
		
		if(fhd = fopen(szOverviewTest,"r")){
			fclose(fhd);
			strcpy(szOverview,szOverviewTest);
			break;
		}
		i++;
		szPath = &(szOverviewPath[i][0]);
	}
	
	LoadGLTextures();
}

GLvoid BuildFont(GLvoid)								// Build Our Bitmap Font
{
	HFONT	font;										// Windows Font ID
	
	base = glGenLists(256);								// Storage For 256 Characters
	
	font = CreateFont(	-12,							// Height Of Font
		0,								// Width Of Font
		0,								// Angle Of Escapement
		0,								// Orientation Angle
		FW_NORMAL,						// Font Weight
		FALSE,							// Italic
		FALSE,							// Underline
		FALSE,							// Strikeout
		ANSI_CHARSET,					// Character Set Identifier
		OUT_TT_PRECIS,					// Output Precision
		CLIP_DEFAULT_PRECIS,			// Clipping Precision
		ANTIALIASED_QUALITY,			// Output Quality
		FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
		//"Comic Sans MS");				// Font Name
		//"Times New Roman");				// Font Name
		"Tahoma");				// Font Name
	
	SelectObject(hDC, font);							// Selects The Font We Created
	
	wglUseFontOutlines(	hDC,							// Select The Current DC
		0,								// Starting Character
		255,							// Number Of Display Lists To Build
		base,							// Starting Display Lists
		1.f,							// Deviation From The True Outlines
		0.05f,							// Font Thickness In The Z Direction
		WGL_FONT_POLYGONS,				// Use Polygons, Not Lines
		gmf);							// Address Of Buffer To Recieve Data
}

GLvoid KillFont(GLvoid)									// Delete The Font
{
	glDeleteLists(base, 256);								// Delete All 256 Characters
}

GLvoid glPrint(bool bMid,const char *fmt, ...)					// Custom GL "Print" Routine
{
	glPushMatrix();
	float		length=0;								// Used To Find The Length Of The Text
	char		text[256];								// Holds Our String
	va_list		ap;										// Pointer To List Of Arguments
	
	if (fmt == NULL)									// If There's No Text
		return;											// Do Nothing
	
	va_start(ap, fmt);									// Parses The String For Variables
	vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text
	
	if(bMid){
		for (unsigned int loop=0;loop<(strlen(text));loop++)	// Loop To Find Text Length
		{
			length+=gmf[text[loop]].gmfCellIncX;			// Increase Length By Each Characters Width
		}
		
		glTranslatef(-length/2,0.0f,0.0f);					// Center Our Text On The Screen
	}
	
	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(base);									// Sets The Base Character to 0
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib();										// Pops The Display List Bits
	glPopMatrix();
}

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}
	
	glViewport(0,0,width,height);						// Reset The Current Viewport
	
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
	
	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
	
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
/*if (!LoadGLTextures())								// Jump To Texture Loading Routine
{
return FALSE;									// If Texture Didn't Load Return FALSE
}*/
	
	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	if(light)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	
	glColor4f(1.0f,1.0f,1.0f,1.0f);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	
	CFTimer.Start(60);
	
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		// Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		// Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);	// Position The Light
	glEnable(GL_LIGHT1);								// Enable Light One
	
	glFogi(GL_FOG_MODE, fogMode[fogfilter]);			// Fog Mode
	glFogfv(GL_FOG_COLOR, fogColor);					// Set Fog Color
	glFogf(GL_FOG_DENSITY, 0.06f);						// How Dense Will The Fog Be
	glHint(GL_FOG_HINT, GL_DONT_CARE);					// Fog Hint Value
	glFogf(GL_FOG_START, 30.0f);							// Fog Start Depth
	glFogf(GL_FOG_END, 80.0f);							// Fog End Depth
	glEnable(GL_FOG);									// Enables GL_FOG
	
	BuildFont();
	
	return TRUE;										// Initialization Went OK
}

int DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
{
	long lschl;
	CFTimer.Frame();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	
	float fText[3] = {.9f,.9f,.9f};
	float faText[3] = {.4f,.4f,.4f};
	
	//return true;
	glLoadIdentity();									// Reset The View
	glTranslatef(-tXS,-tYS,-tZS);
	
	glRotatef(xrot,1.0f,0.0f,0.0f);
	glRotatef(yrot,0.0f,1.0f,0.0f);
	
	glTranslatef(-tX,-tY,-tZ);
	
	//	glBindTexture(GL_TEXTURE_2D, texture[filter]);
	
	glColor4f(1.0f,1.0f,1.0f,1.0f);
	glDisable(GL_BLEND);
	for(lschl =0 ;lschl < lObjects; lschl ++){
		Objects[lschl]->Draw();
	}
	glEnable(GL_BLEND);
	
	if(bDisplayOverview){	
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		//glTranslatef(fOOffset[0]-2.15f/fOZoom,fOOffset[1],fOOffset[2]);
		glTranslatef(fOOffset[0],fOOffset[1],fOOffset[2]);
		glScalef(1.f/fOZoom,1.f/fOZoom,1.f/fOZoom);
		glRotatef(fOverviewR,0.0f,0.0f,1.0f);
		glTranslatef(256.f/1024.f*4.096f,0,0);
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glColor4f(1,1,1,1);
		
		float f[3] = {1,1,1};
		float fh[3] = {1,1,1};
		
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,f);
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,fh);
		glTexCoord2f(1,0);	glVertex3f(-4.096f,-4.096f,fOverviewZ);
		glTexCoord2f(0,0);	glVertex3f(-4.096f,4.096f,fOverviewZ);
		glTexCoord2f(0,1);	glVertex3f(4.096f,4.096f,fOverviewZ);
		glTexCoord2f(1,1);	glVertex3f(4.096f,-4.096f,fOverviewZ);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// show texts
	glClear(GL_DEPTH_BUFFER_BIT);
	if(bDisplayFPS||bDisplayText){
		glDisable(GL_FOG);
		if(bDisplayFPS){
			static float ffps = CFTimer.GetFramesPerSec(),frx=0,fry=0,fs=0,r,g,b;
			static long ltime = time(NULL);
			if(ltime - time(NULL)){
				ltime = time(NULL);
				ffps = CFTimer.GetFramesPerSec();
			}
			glLoadIdentity();									// Reset The View
			
			glTranslatef(0.0f,3.5f,-10.f);
			
			r = ffps < 75.f ? 1-ffps/75.f:0;
			g = ffps > 0.f ? (ffps)/75.0f:0;
			g>1?g=1:g=g;
			b =1.f-r-g;
			//glColor3f(r,g,b);
			
			float f[4] = {r,g,b,1};
			float fa[4] = {r/2.f,g/2.f,b/2.f,1};
			
			glColor4fv(f);
			
			glScalef(.3f,.3f,.3f);
			
			glPrint(true,"%.1ffps(%i,%li)(%i/%i)(%.2f) %i %i %li %li %.3f",ffps,g_Map.m_Waypoints.m_iNumWaypoints,lObjects,t.m_iStart,t.m_iDestination,fDangerV,t.m_iBreakReason,t.m_bFinished,g_ASN_MPool.m_lNextFree,g_Map.m_Waypoints.m_lMaxTotalDamage,t.m_Goal.m_fOffsetCosts);
			float fCB;
			glBegin(GL_TRIANGLE_STRIP);
			float fColor[3];
			for(fCB=0; fCB <= 12;fCB+=.5){
				createColor(fColor,fCB/12.f);
				
				glColor3fv(fColor);
				glVertex3f(fCB-6,-.25,0);
				glVertex3f(fCB-6,-.5,0);
			}
			glEnd();
			glBegin(GL_LINES);
			glColor3f(1,1,1);
			glVertex3f(-6,-.5,0);
			glVertex3f(-6,0,0);
			glVertex3f(6,-.5,0);
			glVertex3f(6,0,0);
			glEnd();
		}
		if(fog)
			glEnable(GL_FOG);
		else
			glDisable(GL_FOG);
	}
	
	xrot+=xspeed * CFTimer.GetSecsPerFrame();
	yrot+=yspeed * CFTimer.GetSecsPerFrame();
	
	static long lCA = 0;
	if(!lCA){
		if(!t.m_bFinished){
			t.runAStar();
			//TakeScreenshot();
		}
		else{
			t.resetNodes();
		}
		lCA = 1;
	}
	lCA --;
	
	return TRUE;										// Keep Going
}

GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}
	
	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		
		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}
	
	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}
	
	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}
	
	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
*	title			- Title To Appear At The Top Of The Window				*
*	width			- Width Of The GL Window Or Fullscreen Mode				*
*	height			- Height Of The GL Window Or Fullscreen Mode			*
*	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
*	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/

BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag,char *szCmdLine)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height
	
	fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag
	
	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= "IDR_MENU1";									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name
	
	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}
	
	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
		
		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","LSNNSIVI3D GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}
	
	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}
	
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size
	
	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								szCmdLine)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}
	
	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
			1,											// Version Number
			PFD_DRAW_TO_WINDOW |						// Format Must Support Window
			PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
			PFD_DOUBLEBUFFER,							// Must Support Double Buffering
			PFD_TYPE_RGBA,								// Request An RGBA Format
			bits,										// Select Our Color Depth
			0, 0, 0, 0, 0, 0,							// Color Bits Ignored
			0,											// No Alpha Buffer
			0,											// Shift Bit Ignored
			0,											// No Accumulation Buffer
			0, 0, 0, 0,									// Accumulation Bits Ignored
			16,											// 16Bit Z-Buffer (Depth Buffer)  
			0,											// No Stencil Buffer
			0,											// No Auxiliary Buffer
			PFD_MAIN_PLANE,								// Main Drawing Layer
			0,											// Reserved
			0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}
	
	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}
	
	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}
	
	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}
	
	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}
	
	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen
	
	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}
	
	return TRUE;									// Success
}
static HMENU hMenu;
LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
						 UINT	uMsg,			// Message For This Window
						 WPARAM	wParam,			// Additional Message Information
						 LPARAM	lParam)			// Additional Message Information
{
	static bool lbdown = false;
	static bool lbdownctrl = false;
	static bool lbdownshift = false;
	static bool rbdown = false;
	static long lxpos,lypos;
	static long lxposrd,lyposrd;
	static float fxrotrd,fyrotrd,fzrd;
	static long lxposld,lyposld;
	static float tXld,tYld;
	static float tXSld,tYSld;
	static float fxrotld,fyrotld,fzld;
	static char	szFileName[_MAX_PATH]="",
		szTitleName[_MAX_FNAME+_MAX_EXT]="",
		szBuffer[200];
	static char	szFileNameS[_MAX_PATH]="",
		szTitleNameS[_MAX_FNAME+_MAX_EXT]="";
	static char	szFileNameO[_MAX_PATH]="",
		szTitleNameO[_MAX_FNAME+_MAX_EXT]="";
	
	switch (uMsg)									// Check For Windows Messages
	{
	case WM_CREATE:
		hMenu = GetMenu(hWnd);
		FileInitialize(hWnd);
		
		lstrcpy (szFileName, (PSTR)
			(((LPCREATESTRUCT) lParam)->lpCreateParams)) ;
		//MessageBox(hWnd,szFileName,"",MB_OK);
		if (strlen (szFileName) > 0)
		{
			GetFileTitle (szFileName, szTitleName, sizeof (szTitleName)) ;
			
			szFileName[strlen(szFileName)-1]='\0';

			DelObjects();
			if(strstr(szFileName,".wjx"))
				*strstr(szFileName,".wjx") = 0;
			g_Map.reset();
			//MessageBox(hWnd,szFileName+1,"",MB_OK);
			g_Map.load(szFileName+1);

			strcpy(szFileNameS,szFileName);

			char *szTmp = szFileNameS;
			while(*szTmp)szTmp++;

			InitObjects();
		}
		else{
		}
		
		SetWindowText(hWnd,szBuffer);
		
		//InitObjects();
		break;
	case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			CheckOverview();					// check if we gotta load some textures
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=TRUE;						// Program Is Active
			}
			else
			{
				active=FALSE;						// Program Is No Longer Active
			}
			
			return 0;								// Return To The Message Loop
		}
	case WM_COMMAND:
		hMenu = GetMenu(hWnd);
		
		switch(LOWORD(wParam))
		{
		case ID_FILE_QUIT:
			PostQuitMessage(0);
			break;
		case ID_FILE_LOAD:
			*szFileName=0;
			FileOpenDlg(hWnd,szFileName,szTitleName);
			
			if(strlen(szFileName))
			{
				//SetWindowText(hwndWait,buffer);
				DelObjects();
				if(strstr(szFileName,".wjx"))
					*strstr(szFileName,".wjx") = 0;
				g_Map.reset();
				g_Map.load(szFileName);
				
				CheckOverview();
				
				strcpy(szFileNameS,szFileName);
				
				InitObjects();
				
				//DestroyWindow(hwndWait);
			}
			break;
			
		case ID_FILE_LOADOVERVIEW:
			*szFileNameO=0;
			FileOpenDlg(hWnd,szFileNameO,szTitleNameO);
			
			if(strlen(szFileNameO))
			{		
				//SetWindowText(hwndWait,buffer);
				strcpy(szOverview,szFileNameO);
				if(!LoadGLTextures())
					MessageBeep(1);
				
				//DestroyWindow(hwndWait);
			}
			break;
			
		case ID_FILE_SAVEFILE:
			*szFileName=0;
			*szTitleName=0;
			FileSaveDlg(hWnd,szFileName,szTitleName);
			if(strlen(szFileName)){
				//NN.SaveFile(szFileName);
			}
			break;
			
			/*case ID__ABOUT:
			DialogBox(hInstance,"ABOUT",hWnd,DLGPROC(AboutDlgProc));
			return 0;*/
			
		case ID_VIEW_SHOWHIDECONNECTIONS:
			bDisplayConnections = !bDisplayConnections;
			if(!bDisplayConnections){
				CheckMenuItem(hMenu,ID_VIEW_SHOWHIDECONNECTIONS,MF_UNCHECKED);
			}
			else
				CheckMenuItem(hMenu,ID_VIEW_SHOWHIDECONNECTIONS,MF_CHECKED);
			DelObjects();
			InitObjects();
			return 0;

		case ID_VIEW_COLORCONN:
			bColoredConnections = !bColoredConnections;
			if(!bColoredConnections){
				CheckMenuItem(hMenu,ID_VIEW_COLORCONN,MF_UNCHECKED);
			}
			else
				CheckMenuItem(hMenu,ID_VIEW_COLORCONN,MF_CHECKED);
			DelObjects();
			InitObjects();
			return 0;
			
		case ID_VIEW_ASTARSTUFF:
			g_bDisplayAStarObj = !g_bDisplayAStarObj;
			if(!g_bDisplayAStarObj){
				CheckMenuItem(hMenu,ID_VIEW_ASTARSTUFF,MF_UNCHECKED);
			}
			else
				CheckMenuItem(hMenu,ID_VIEW_ASTARSTUFF,MF_CHECKED);
			DelObjects();
			InitObjects();
			return 0;
			
		case ID_VIEW_LOS:
			bDisplayLOS = !bDisplayLOS;
			if(!bDisplayLOS){
				CheckMenuItem(hMenu,ID_VIEW_LOS,MF_UNCHECKED);
			}
			else
				CheckMenuItem(hMenu,ID_VIEW_LOS,MF_CHECKED);
			DelObjects();
			InitObjects();
			return 0;
			
		case ID_VIEW_OVERVIEW:
			bDisplayOverview = !bDisplayOverview;
			if(!bDisplayOverview){
				CheckMenuItem(hMenu,ID_VIEW_OVERVIEW,MF_UNCHECKED);
			}
			else
				CheckMenuItem(hMenu,ID_VIEW_OVERVIEW,MF_CHECKED);
			if(bDisplayOverview){
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			}
			else{
				glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
			}
			return 0;
			
		case ID_VIEW_SHOWHIDENEURONS:
			bDisplayNeurons = !bDisplayNeurons;
			if(!bDisplayNeurons){
				CheckMenuItem(hMenu,ID_VIEW_SHOWHIDENEURONS,MF_UNCHECKED);
			}
			else
				CheckMenuItem(hMenu,ID_VIEW_SHOWHIDENEURONS,MF_CHECKED);
			DelObjects();
			InitObjects();
			return 0;
		case ID_VIEW_DESC:
			bDisplayDesc= !bDisplayDesc;
			if(!bDisplayDesc){
				CheckMenuItem(hMenu,ID_VIEW_DESC,MF_UNCHECKED);
			}
			else
				CheckMenuItem(hMenu,ID_VIEW_DESC,MF_CHECKED);
			return 0;
		case ID_VIEW_FPS:
			bDisplayFPS= !bDisplayFPS;
			if(!bDisplayFPS){
				CheckMenuItem(hMenu,ID_VIEW_FPS,MF_UNCHECKED);
			}
			else
				CheckMenuItem(hMenu,ID_VIEW_FPS,MF_CHECKED);
			return 0;
		case ID_VIEW_ORIGIN:
			bDisplayOrigin = !bDisplayOrigin;
			if(!bDisplayOrigin){
				CheckMenuItem(hMenu,ID_VIEW_ORIGIN,MF_UNCHECKED);
			}
			else
				CheckMenuItem(hMenu,ID_VIEW_ORIGIN,MF_CHECKED);
			DelObjects();
			InitObjects();
			return 0;
		case ID_VIEW_TEXT:
			bDisplayText = !bDisplayText;
			if(!bDisplayText){
				CheckMenuItem(hMenu,ID_VIEW_TEXT,MF_UNCHECKED);
			}
			else
				CheckMenuItem(hMenu,ID_VIEW_TEXT,MF_CHECKED);
			return 0;
		case ID_VIEW_DEFAULT:
			tX=0;tY=0;tZ=0;
			tXS=0;tYS=0;tZS=15;
			
			xrot=45;
			yrot=-45;
			xspeed=0;
			yspeed=0;
			return 0;
		case ID_RENDER_ANTIALIASING:
			alias=!alias;
			
			if(alias)
				glEnable(GL_LINE_SMOOTH);
			else
				glDisable(GL_LINE_SMOOTH);
			if(!alias){
				CheckMenuItem(hMenu,ID_RENDER_ANTIALIASING,MF_UNCHECKED);
			}
			else
				CheckMenuItem(hMenu,ID_RENDER_ANTIALIASING,MF_CHECKED);
			return 0;
			
		case ID_RENDER_FOG:
			fog =! fog;
			
			if(fog)
				glEnable(GL_FOG);
			else
				glDisable(GL_FOG);
			if(!fog){
				CheckMenuItem(hMenu,ID_RENDER_FOG,MF_UNCHECKED);
			}
			else
				CheckMenuItem(hMenu,ID_RENDER_FOG,MF_CHECKED);
			return 0;
			
		case ID_RENDER_LIGHT:
			light=!light;
			if (!light)
			{
				glDisable(GL_LIGHTING);
			}
			else
			{
				glEnable(GL_LIGHTING);
			}
			if(!light){
				CheckMenuItem(hMenu,ID_RENDER_LIGHT,MF_UNCHECKED);
			}
			else
				CheckMenuItem(hMenu,ID_RENDER_LIGHT,MF_CHECKED);
			return 0;
			
		case ID_DISPLAY_DEFAULT:
		case ID_DISPLAY_DAMAGE:
		case ID_DISPLAY_DAMAGED:
		case ID_DISPLAY_AVVDIST:
		case ID_DISPLAY_IVWP:
		case ID_DISPLAY_TRAFFIC:
		case ID_DISPLAY_FAILED:
			CheckMenuItem(hMenu,iTOD,MF_UNCHECKED);
			iTOD = LOWORD(wParam);
			CheckMenuItem(hMenu,iTOD,MF_CHECKED);
			DelObjects();
			InitObjects();
			break;
		};
		break;
		
		case WM_SYSCOMMAND:							// Intercept System Commands
			{
				switch (wParam)							// Check System Calls
				{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
					return 0;							// Prevent From Happening
				}
				break;									// Exit
			}
		case WM_LBUTTONDOWN:
			xspeed = 0;
			yspeed = 0;
			if(keys[VK_CONTROL]){
				lbdownctrl = true;
			}
			if(keys[VK_SHIFT]){
				lbdownshift = true;
			}
			else
				lbdown = true;
			
			lxposld = lxpos;
			lyposld = lypos;
			fxrotld = xrot;
			fyrotld = yrot;
			tYSld = tYS;
			tXSld = tXS;
			fzld = tZS;
			tYld = tY;
			tXld = tX;
			
			break;
		case WM_LBUTTONUP:
			lbdownshift = false;
			lbdownctrl = false;
			lbdown = false;
			break;
			
		case WM_RBUTTONDOWN:
			//xspeed = 0;
			//yspeed = 0;
			rbdown = true;
			
			lxposrd = lxpos;
			lyposrd = lypos;
			fxrotrd = xrot;
			fyrotrd = yrot;
			fzrd = tZS;
			
			break;
		case WM_RBUTTONUP:
			rbdown = false;
			break;
			
		case 0x020A:
			if(short(HIWORD(wParam)) > 0){
				tZS += 1;
			}
			else
				tZS -= 1;
			break;
			
		case WM_MOUSEMOVE:
			lxpos = LOWORD(lParam);  // horizontal position of cursor 
			lypos = HIWORD(lParam);  // vertical position of cursor 
			if(lbdown){
				yrot = fyrotld + (lxpos - lxposld) * _SENSIMOUSE;
				xrot = fxrotld + (lypos - lyposld) * _SENSIMOUSE;
			}
			if(lbdownctrl){
				tYS = tYSld + (lypos - lyposld) * _SENSIMOUSE * .1f;
				tXS = tXSld - (lxpos - lxposld) * _SENSIMOUSE * .1f;
			}
			if(lbdownshift){
				tY = tYld + (lypos - lyposld) * _SENSIMOUSE * .1f;
				tX = tXld - (lxpos - lxposld) * _SENSIMOUSE * .1f;
			}
			if(rbdown){
				tZS = fzrd + (lypos - lyposrd) * 1.0f * _SENSIMOUSE;
			}
			break;
			
		case WM_CLOSE:								// Did We Receive A Close Message?
			{
				PostQuitMessage(0);						// Send A Quit Message
				return 0;								// Jump Back
			}
			
		case WM_KEYDOWN:							// Is A Key Being Held Down?
			{
				memcpy(okeys,keys,sizeof(bool) * 256);
				keys[wParam] = TRUE;					// If So, Mark It As TRUE
				return 0;								// Jump Back
			}
			
		case WM_KEYUP:								// Has A Key Been Released?
			{
				keys[wParam] = FALSE;					// If So, Mark It As FALSE
				return 0;								// Jump Back
			}
			
		case WM_SIZE:								// Resize The OpenGL Window
			{
				ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
				return 0;								// Jump Back
			}
	}
	
	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

void DelObjects(void){
	long lschl;
	if(Objects){
		for(lschl =0 ;lschl < lObjects;lschl ++){
			delete Objects[lschl];
		}
		delete [] Objects;
	}
	Objects = 0;
	lObjects = 0;
	
	t.resetNodes();
}

void InitObjects(void){
	Objects = new CObject *[1000000];	// just a lot of objects - why do we got virtual mem ? ;)
	lObjects = 0;
	float fC[_MAX_WAYPOINTS][3];
	float fColorGrey[3] ={.5,.5,.5};
	float fColorWhite[3] ={1,0,0};
	
	//	long lN,lL,lNO,lLO,lNumoW=0,lC;
	
	if(bDisplayOrigin){
		Objects[lObjects] = new CLine;
		((CLine *)Objects[lObjects])->x1 = 0;
		((CLine *)Objects[lObjects])->y1 = 0;
		((CLine *)Objects[lObjects])->z1 = 0;
		
		((CLine *)Objects[lObjects])->x2 = 100;
		((CLine *)Objects[lObjects])->y2 = 0;
		((CLine *)Objects[lObjects])->z2 = 0;
		
		((CLine *)Objects[lObjects])->r = 1;
		((CLine *)Objects[lObjects])->g = 0;
		((CLine *)Objects[lObjects])->b = 0;
		
		lObjects ++;
		
		Objects[lObjects] = new CLine;
		((CLine *)Objects[lObjects])->x1 = 0;
		((CLine *)Objects[lObjects])->y1 = 0;
		((CLine *)Objects[lObjects])->z1 = 0;
		
		((CLine *)Objects[lObjects])->x2 = 0;
		((CLine *)Objects[lObjects])->y2 = 100;
		((CLine *)Objects[lObjects])->z2 = 0;
		
		((CLine *)Objects[lObjects])->r = 0;
		((CLine *)Objects[lObjects])->g = 1;
		((CLine *)Objects[lObjects])->b = 0;
		
		lObjects ++;
		Objects[lObjects] = new CLine;
		((CLine *)Objects[lObjects])->x1 = 0;
		((CLine *)Objects[lObjects])->y1 = 0;
		((CLine *)Objects[lObjects])->z1 = 0;
		
		((CLine *)Objects[lObjects])->x2 = 0;
		((CLine *)Objects[lObjects])->y2 = 0;
		((CLine *)Objects[lObjects])->z2 = 100;
		
		((CLine *)Objects[lObjects])->r = 0;
		((CLine *)Objects[lObjects])->g = 0;
		((CLine *)Objects[lObjects])->b = 1;
		
		lObjects ++;
	}
	long lschl,lschl1;
	for(lschl = 0;lschl < g_Map.m_Waypoints.m_iNumWaypoints; lschl ++){
		if (g_Map.m_Waypoints.m_Waypoints[lschl].m_lFlags & CWaypoint::DELETED)
			continue;  // skip any deleted waypoints
		switch(iTOD){
		case ID_DISPLAY_DAMAGE:
			createColor(fC[lschl],g_Map.m_Waypoints[lschl].m_lTotalDamage/float(g_Map.m_Waypoints.m_lMaxTotalDamage));
			break;
		case ID_DISPLAY_DAMAGED:
			createColor(fC[lschl],g_Map.m_Waypoints[lschl].m_lTotalDamaged/float(g_Map.m_Waypoints.m_lMaxTotalDamage));
			break;
		case ID_DISPLAY_AVVDIST:
			createColor(fC[lschl],(g_Map.m_Waypoints.m_Waypoints[lschl].m_fAvDistVWP-200) / 1000.f);
			break;
		case ID_DISPLAY_IVWP:
			createColor(fC[lschl],(float)g_Map.m_Waypoints.m_Waypoints[lschl].m_iVisibleWaypoints *4.f / (float)g_Map.m_Waypoints.m_iNumWaypoints);
			break;
		case ID_DISPLAY_TRAFFIC:
			createColor(fC[lschl],(float)g_Map.m_Waypoints.m_Waypoints[lschl].m_lTraffic / 2000);
			break;
		case ID_DISPLAY_FAILED:
			{
				int iMaxFailed=0;
				CPath_iterator iter_path;

				iter_path = &(g_Map.m_Waypoints.m_Waypoints[lschl]);
				for(;iter_path.getTo() != -1;++iter_path){
					if(iter_path.getFailed() > iMaxFailed)
						iMaxFailed = iter_path.getFailed();
				}
				if(iMaxFailed)
					createColor(fC[lschl],iMaxFailed / 5.f);
				else{
					fC[lschl][0] = .5f;
					fC[lschl][1] = .5f;
					fC[lschl][2] = .5f;
				}

			}
			break;
		default :
			fC[lschl][0] = 1;
			fC[lschl][1] = 0;
			fC[lschl][2] = 0;
			break;
		}
	}
	
	for(lschl = 0;lschl < g_Map.m_Waypoints.m_iNumWaypoints; lschl ++){
		if (g_Map.m_Waypoints.m_Waypoints[lschl].m_lFlags & CWaypoint::DELETED)
			continue;  // skip any deleted waypoints
		if(bDisplayNeurons){
			Objects[lObjects] = new CCube;
			((CCube *)Objects[lObjects])->x = g_Map.m_Waypoints.m_Waypoints[lschl].m_VOrigin.x/1000;
			((CCube *)Objects[lObjects])->y = g_Map.m_Waypoints.m_Waypoints[lschl].m_VOrigin.y/1000;
			((CCube *)Objects[lObjects])->z = g_Map.m_Waypoints.m_Waypoints[lschl].m_VOrigin.z/1000;
			
			((CCube *)Objects[lObjects])->rx = rD;
			((CCube *)Objects[lObjects])->ry = rD;
			((CCube *)Objects[lObjects])->rz = rD;
			
			((CCube *)Objects[lObjects])->setColor3fv(fC[lschl]);
			
			lObjects ++;
		}
		
		if (bDisplayConnections)
		{
			CPath_iterator iter_path;
			int iConnection;
			/*for(i = 0; i < g_Map.m_Waypoints.m_Waypoints[lschl].getConnectionCount(); i++){
				if((iConnection=g_Map.m_Waypoints.m_Waypoints[lschl].getConnection(i)) != -1){*/
			iter_path = &(g_Map.m_Waypoints[lschl]);
			for(;iter_path.getTo() != -1;++iter_path){
				iConnection = iter_path.getTo();

				Objects[lObjects] = new CLine2;
				((CLine2 *)Objects[lObjects])->x1 = g_Map.m_Waypoints.m_Waypoints[lschl].m_VOrigin.x/1000;
				((CLine2 *)Objects[lObjects])->y1 = g_Map.m_Waypoints.m_Waypoints[lschl].m_VOrigin.y/1000;
				((CLine2 *)Objects[lObjects])->z1 = g_Map.m_Waypoints.m_Waypoints[lschl].m_VOrigin.z/1000;

				((CLine2 *)Objects[lObjects])->x2 = g_Map.m_Waypoints.m_Waypoints[iConnection].m_VOrigin.x/1000;
				((CLine2 *)Objects[lObjects])->y2 = g_Map.m_Waypoints.m_Waypoints[iConnection].m_VOrigin.y/1000;
				((CLine2 *)Objects[lObjects])->z2 = g_Map.m_Waypoints.m_Waypoints[iConnection].m_VOrigin.z/1000;

				if(iTOD == ID_DISPLAY_FAILED){
					if(iter_path.getFailed()){
						float fPColor[3];
						createColor(fPColor,iter_path.getFailed()/5.f);
						((CLine2 *)Objects[lObjects])->set1Color3fv(fPColor);
						((CLine2 *)Objects[lObjects])->set2Color3fv(fPColor);
					}
					else{
						((CLine2 *)Objects[lObjects])->set1Color3fv(fColorGrey);
						((CLine2 *)Objects[lObjects])->set2Color3fv(fColorGrey);
					}
				}
				else{
					if(bColoredConnections){
						((CLine2 *)Objects[lObjects])->set1Color3fv(fC[lschl]);
						((CLine2 *)Objects[lObjects])->set2Color3fv(fC[iConnection]);
					}
					else{
						((CLine2 *)Objects[lObjects])->set1Color3fv(fColorGrey);
						((CLine2 *)Objects[lObjects])->set2Color3fv(fColorGrey);
					}
				}

				lObjects ++;
			}
			/*	}
			}*/
		}
		
		if(bDisplayLOS){
			for(lschl1 = lschl+1;lschl1 < g_Map.m_Waypoints.m_iNumWaypoints; lschl1 ++){
				if (g_Map.m_Waypoints.m_Waypoints[lschl1].m_lFlags & CWaypoint::DELETED)
					continue;  // skip any deleted waypoints
				if(lschl1 == lschl)
					continue;
				
				if(g_Map.m_Waypoints.getVisible(lschl,lschl1)){
					Objects[lObjects] = new CLine;
					((CLine *)Objects[lObjects])->x1 = g_Map.m_Waypoints.m_Waypoints[lschl].m_VOrigin.x/1000;
					((CLine *)Objects[lObjects])->y1 = g_Map.m_Waypoints.m_Waypoints[lschl].m_VOrigin.y/1000;
					((CLine *)Objects[lObjects])->z1 = g_Map.m_Waypoints.m_Waypoints[lschl].m_VOrigin.z/1000;
					
					((CLine *)Objects[lObjects])->x2 = g_Map.m_Waypoints.m_Waypoints[lschl1].m_VOrigin.x/1000;
					((CLine *)Objects[lObjects])->y2 = g_Map.m_Waypoints.m_Waypoints[lschl1].m_VOrigin.y/1000;
					((CLine *)Objects[lObjects])->z2 = g_Map.m_Waypoints.m_Waypoints[lschl1].m_VOrigin.z/1000;
					
					((CLine *)Objects[lObjects])->r = .0;
					((CLine *)Objects[lObjects])->g = .0;
					((CLine *)Objects[lObjects])->b = 1;
					
					lObjects ++;
				}
			}
		}
	}

	if(g_Map.m_Waypoints.m_iNumWaypoints){
		/*CBWP bwp;
		
		int i,ii,iS = 13;
		
		g_bshowbwp = false;
		for(ii = 0; ii < iS; ii++){
			for(i=ii; i< g_Map.m_Waypoints.m_iNumWaypoints; i+=iS){
				//cout << i<< endl;
				bwp.addWaypoint(&(g_Map.m_Waypoints.m_Waypoints[i]));
			}
		}
		g_bshowbwp = true;
		bwp.traceNode(g_Map.m_Waypoints.m_Waypoints[iStart].m_VOrigin);*/
	}
	
	if(g_Map.m_Waypoints.m_iNumWaypoints){		
		t.m_Goal.m_fNCoverValue = fDangerV;
		t.m_Goal.reset();
		t.m_Goal.addThreadWP ( 13 );
		
		/*{
			Objects[lObjects] = new CCube;
			((CCube *)Objects[lObjects])->x = g_Map.m_Waypoints.m_Waypoints[199].m_VOrigin.x/1000;
			((CCube *)Objects[lObjects])->y = g_Map.m_Waypoints.m_Waypoints[199].m_VOrigin.y/1000;
			((CCube *)Objects[lObjects])->z = g_Map.m_Waypoints.m_Waypoints[199].m_VOrigin.z/1000;
			
			((CCube *)Objects[lObjects])->rx = rD+.02;
			((CCube *)Objects[lObjects])->ry = rD+.02;
			((CCube *)Objects[lObjects])->rz = rD+.02;
			
			((CCube *)Objects[lObjects])->r=((CCube *)Objects[lObjects])->g=((CCube *)Objects[lObjects])->b=1;
			
			lObjects ++;}*/
		
		t.setStart(iStart);
		t.setDestination(iEnd);
		//fcloseall();
		t.m_lSliceSize = 1;
		t.runAStar();
	}
}

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
				   HINSTANCE	hPrevInstance,		// Previous Instance
				   LPSTR		lpCmdLine,			// Command Line Parameters
				   int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;								// Bool Variable To Exit Loop
	
	// Ask The User Which Screen Mode They Prefer
	fullscreen=FALSE;							// Windowed Mode
	
	// Create Our OpenGL Window
	if (!CreateGLWindow("Johannes' NNVISU3D",640,480,16,fullscreen,lpCmdLine))
	{
		return 0;									// Quit If Window Was Not Created
	}
	
	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if ((active && !DrawGLScene()) || keys[VK_ESCAPE])	// Active?  Was There A Quit Received?
			{
				done=TRUE;							// ESC or DrawGLScene Signalled A Quit
			}
			else									// Not Time To Quit, Update Screen
			{
				SwapBuffers(hDC);					// Swap Buffers (Double Buffering)
				if (keys['L'] && !okeys['L'])
				{
					if(!light){
						CheckMenuItem(hMenu,ID_RENDER_LIGHT,MF_UNCHECKED);
					}
					else
						CheckMenuItem(hMenu,ID_RENDER_LIGHT,MF_CHECKED);
					lp=TRUE;
					light=!light;
					if (!light)
					{
						glDisable(GL_LIGHTING);
					}
					else
					{
						glEnable(GL_LIGHTING);
					}
					okeys['L'] = keys['L'];
				}
				if (keys['O'] && !okeys['O'])
				{
					lpo=TRUE;
					fog =! fog;
					
					if(fog)
						glEnable(GL_FOG);
					else
						glDisable(GL_FOG);
					
					if(!fog){
						CheckMenuItem(hMenu,ID_RENDER_FOG,MF_UNCHECKED);
					}
					else
						CheckMenuItem(hMenu,ID_RENDER_FOG,MF_CHECKED);
					
					okeys['O'] = keys['O'];
				}
				
				if (keys['A'] && !okeys['A']){
					iEnd --;
					if(iEnd < 0)
						iEnd = 0;
					DelObjects();
					InitObjects();
					
					okeys['A'] = keys['A'];
				}
				if (keys['S'] && !okeys['S']){
					iEnd ++;
					DelObjects();
					InitObjects();
					
					okeys['S'] = keys['S'];
				}
				
				if (keys['Q'] && !okeys['Q']){
					iStart --;
					if(iStart < 0)
						iStart = 0;
					DelObjects();
					InitObjects();
					
					okeys['Q'] = keys['Q'];
				}
				if (keys['W'] && !okeys['W']){
					iStart ++;
					DelObjects();
					InitObjects();
					
					okeys['W'] = keys['W'];
				}
				
				if (keys['D'] && !okeys['D']){
					fDangerV += .1f;
					DelObjects();
					InitObjects();
					
					okeys['D'] = keys['D'];
				}
				if (keys['F'] && !okeys['F']){
					fDangerV -= .1f;
					DelObjects();
					InitObjects();
					
					okeys['F'] = keys['F'];
				}
				
				if (keys['F'] && !fp){
					fp=TRUE;
					filter+=1;
					if (filter>2)
					{
						filter=0;
					}
				}
				if (keys[VK_PRIOR]){
					tZS-=0.2f;
				}
				if (keys[VK_NEXT]){
					tZS+=0.2f;
				}
				if (keys['Y']){
					fOverviewZ-=0.02f;
				}
				if (keys['X']){
					fOverviewZ-=0.002f;
				}
				if (keys['C']){
					fOverviewZ+=0.002f;
				}
				if (keys['V']){
					fOverviewZ+=0.02f;
				}
				if (keys[VK_UP]){
					xspeed-=1.f;
				}
				if (keys[VK_DOWN]){
					xspeed+=1.f;
				}
				if (keys[VK_RIGHT]){
					yspeed+=1.f;
				}
				if (keys[VK_LEFT]){
					yspeed-=1.f;
				}
				
				if (keys[VK_F1])						// Is F1 Being Pressed?
				{
					keys[VK_F1]=FALSE;					// If So Make Key FALSE
					KillGLWindow();						// Kill Our Current Window
					fullscreen=!fullscreen;				// Toggle Fullscreen / Windowed Mode
					// Recreate Our OpenGL Window
					if (!CreateGLWindow("Johannes' LSNNVISU3D",1024,768,32,fullscreen,""))
					{
						return 0;						// Quit If Window Was Not Created
					}
				}
			}
		}
	}
	
	// Shutdown
	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}

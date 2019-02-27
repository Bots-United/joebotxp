/***
*
*	Copyright (c) 1999, 2000 Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
#ifndef VECTOR_H
#define VECTOR_H

#include "MathLib.h"

//=========================================================
// 2DVector - used for many pathfinding and many other 
// operations that are treated as planar rather than 3d.
//=========================================================
class Vector2D
{
public:
	inline Vector2D(void)									{ }
	inline Vector2D(float X, float Y)						{ x = X; y = Y; }
	inline Vector2D operator+(const Vector2D& v)	const	{ return Vector2D(x+v.x, y+v.y);	}
	inline Vector2D operator-(const Vector2D& v)	const	{ return Vector2D(x-v.x, y-v.y);	}
	inline Vector2D operator*(float fl)				const	{ return Vector2D(x*fl, y*fl);	}
	inline Vector2D operator/(float fl)				const	{ return Vector2D(x/fl, y/fl);	}
	
	inline float length(void)						const	{ return sqrt(x*x + y*y );		}

	inline Vector2D normalize ( void ) const
	{
		Vector2D vec2;

		float flLen = length();
		if ( flLen == 0 )
		{
			return Vector2D( 0, 0 );
		}
		else
		{
			flLen = 1.f / flLen;
			return Vector2D( x * flLen, y * flLen );
		}
	}

	vec_t	x, y;
};

inline float DotProduct(const Vector2D& a, const Vector2D& b) { return( a.x*b.x + a.y*b.y ); }
inline Vector2D operator*(float fl, const Vector2D& v)	{ return v * fl; }

// angle mod and angle normalize based on pmb's version - http://racc.bots-united.com
inline float clamp0360(const float a)
{
   // this function adds or substracts 360 enough times needed to the given angle in
   // order to set it into the range [0, 360) and returns the resulting angle. Letting
   // the engine have a hand on angles that are outside these bounds may cause the
   // game to freeze by screwing up the engine math code.

   return (360.f / 65536.f) * ((int)(a * (65536.f / 360.f)) & 65535);
}

inline float clamp180180(const float angle)
{
   // this function adds or substracts 360 enough times needed to the given angle in
   // order to set it into the range [-180, 180) and returns the resulting angle. Letting
   // the engine have a hand on angles that are outside these bounds may cause the game
   // to freeze by screwing up the engine math code.

   return (360.f / 65536.f) * ((int)((angle + 180.f) * (65536.f / 360.f)) & 65535) - 180.f;
}

//=========================================================
// 3D Vector
//=========================================================
class Vector						// same data-layout as engine's vec3_t,
{								//		which is a vec_t[3]
public:
	// Construction/destruction
	inline Vector(void)								{ }
	inline Vector(float X, float Y, float Z)		{ x = X; y = Y; z = Z;						}
	inline Vector(const Vector& v)					{ x = v.x; y = v.y; z = v.z;				} 
	inline Vector(float rgfl[3])					{ x = rgfl[0]; y = rgfl[1]; z = rgfl[2];	}

	// Operators
	inline Vector operator-(void) const				{ return Vector(-x,-y,-z);				}
	inline int operator==(const Vector& v) const	{ return x==v.x && y==v.y && z==v.z;	}
	inline int operator!=(const Vector& v) const	{ return !(*this==v);					}
	inline Vector operator+(const Vector& v) const	{ return Vector(x+v.x, y+v.y, z+v.z);	}
	inline Vector operator-(const Vector& v) const	{ return Vector(x-v.x, y-v.y, z-v.z);	}
	inline Vector operator*(const float fl) const	{ return Vector(x*fl, y*fl, z*fl);		}
	inline Vector operator/(float fl) const			{ fl=1.f/fl; return Vector(x*fl, y*fl, z*fl);		}

	inline Vector &operator+=(const Vector& v)		{ x+=v.x;y+=v.y;z+=v.z; return ((Vector &)(*this));}
	inline Vector &operator-=(const Vector& v)		{ x-=v.x;y-=v.y;z-=v.z; return ((Vector &)(*this));}
	inline Vector &operator*=(const float fl)		{ x*=fl;y*=fl;z*=fl; return ((Vector &)(*this));}
	inline Vector &operator/=(float fl)				{ fl=1.f/fl;x*=fl;y*=fl;z*=fl; return ((Vector &)(*this));}

	// angle related functions
	//
	// Clamp angles using specified function
	inline Vector &clamp(float (*fClampFunc)(const float))
		{x=fClampFunc(x);y=fClampFunc(y);z=fClampFunc(z); return ((Vector &)(*this));}
	// often only yaw and pitch have to be clamped, therefore another specific function to save one function call
	inline Vector &clamp2D(float (*fClampFunc)(const float))
		{x=fClampFunc(x);y=fClampFunc(y); return ((Vector &)(*this));}

	inline Vector vec2angles(void) const
	{
		float yaw, pitch;

		if (x == 0 && y == 0)
		{
			yaw = 0;
			pitch = (z > 0) ? 90.f : 270.f;
		}
		else
		{
			yaw = atan2f(y, x) * (180.f / M_PI);
			pitch = atan2f(z, length2D()) * (180.f / M_PI);
		}

		return Vector(pitch, yaw, 0);
	}

	inline Vector angles2vec( void ) const
	{
		float sp, cp, sy, cy;
		float angle;

		angle = x * (M_PI / 180.f);
		SinCos(angle, &sp, &cp);
		angle = y * (M_PI / 180.f);
		SinCos(angle, &sy, &cy);
		
		return Vector(cp * cy,cp * sy,-sp);
	}
	inline Vector angles2vec_up( void ) const
	{
		float sp, cp, sy, cy, sr, cr;
		float angle;

		angle = x * (M_PI / 180.f);
		SinCos(angle, &sp, &cp);
		angle = y * (M_PI / 180.f);
		SinCos(angle, &sy, &cy);
		angle = z * (M_PI / 180.f);
		SinCos(angle, &sr, &cr);

		return Vector(cr * sp * cy + sr * sy, cr * sp * sy - sr * cy, cr * cp );
	}
	inline Vector angles2vec_right( void ) const
	{
		float sp, cp, sy, cy, sr, cr;
		float angle;

		angle = x * (M_PI / 180.f);
		SinCos(angle, &sp, &cp);
		angle = y * (M_PI / 180.f);
		SinCos(angle, &sy, &cy);
		angle = z * (M_PI / 180.f);
		SinCos(angle, &sr, &cr);

		return Vector(-sr * sp * cy + cr * sy, -sr * sp * sy - cr * cy, -sr * cp);
	}
	
	// Methods
	inline void copyToArray(float* rgfl) const		{ rgfl[0] = x, rgfl[1] = y, rgfl[2] = z; }
	inline float length(void) const					{ return sqrt(x*x + y*y + z*z); }
	inline float squareLength(void) const			{ return (x*x + y*y + z*z); }
	inline float squareLength2D(void) const			{ return (x*x + y*y); }
	inline float CBLength(void) const				{ return fabs(x)+fabs(y)+fabs(z); }		// manhattan distance
	operator float *()								{ return &x; } // Vectors will now automatically convert to float * when needed
	operator const float *() const					{ return &x; } // Vectors will now automatically convert to float * when needed
	inline Vector normalize(void) const
	{
		float flLen = length();
		if (flLen == 0) return Vector(0,0,1); // ???? why
		flLen = 1.f / flLen;
		return Vector(x * flLen, y * flLen, z * flLen);
	}

	inline Vector2D make2D ( void ) const
	{
		Vector2D	Vec2;

		Vec2.x = x;
		Vec2.y = y;

		return Vec2;
	}
	inline float length2D(void) const					{ return sqrt(x*x + y*y); }

	// Members
	vec_t x, y, z;
};
inline Vector operator*(float fl, const Vector& v)	{ return v * fl; }
inline float DotProduct(const Vector& a, const Vector& b) { return(a.x*b.x+a.y*b.y+a.z*b.z); }
inline Vector CrossProduct(const Vector& a, const Vector& b) { return Vector( a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x ); }

#endif


#ifndef __MATHLIB_H
#define __MATHLIB_H

#define M_PI 3.141592653589793238462643383279502884197169399375105820974944

// code from whistler, see http://forums.bots-united.com/showthread.php?t=1997
void inline SinCos( float rad, float *flSin, float *flCos )
{
#ifdef __linux__
   register double __cosr, __sinr;
   __asm __volatile__ ("fsincos" : "=t" (__cosr), "=u" (__sinr) : "0" (rad));
   *flSin = __sinr;
   *flCos = __cosr;
#else
#ifndef _MSC_VER
   *flSin = sin(rad);
   *flCos = cos(rad);
#else
   __asm
   {
      fld DWORD PTR[rad]
      fsincos
      mov edx, DWORD PTR[flCos]
      mov eax, DWORD PTR[flSin]
      fstp DWORD PTR[edx]
      fstp DWORD PTR[eax]
   }
#endif
#endif
}

/*inline float UTIL_VecToYaw( const Vector &vec )
{
   // the purpose of this function is to convert a spatial location determined by the vector
   // passed in into an absolute Y angle (yaw) from the origin of the world.

   if (vec.x == 0 && vec.y == 0)
      return 0;
   else
      return atan2(vec.y, vec.x) * (180 / M_PI);
}*/

#endif
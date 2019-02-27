#ifndef __STUDIOAPI_H
#define __STUDIOAPI_H

#include <studio.h>		// MAXSTUDIOBONES

// Typedef for Server_GetBlendingInterface() from Eric Smith on the hlcoders 
// mailing list.
typedef int (*GETBLENDAPI_FN) (int version,	struct sv_blending_interface_s **ppinterface,struct engine_studio_api_s *pstudio,float (*rotationmatrix)[3][4],	float (*bonetransform)[MAXSTUDIOBONES][3][4]);

#endif /* STUDIOAPI_H */

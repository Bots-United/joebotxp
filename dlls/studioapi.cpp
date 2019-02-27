#include "extdll.h"
#include "studioapi.h"

//
// thx to botman for point this out
//
// check out
// http://www.mail-archive.com/hlcoders@list.valvesoftware.com/msg01224.html
// http://www.mail-archive.com/hlcoders@list.valvesoftware.com/msg02724.html

extern HINSTANCE h_Library;

DLLEXPORT Server_GetBlendingInterface(int version,
		struct sv_blending_interface_s **ppinterface,
		struct engine_studio_api_s *pstudio,
		float (*rotationmatrix)[3][4],
		float (*bonetransform)[MAXSTUDIOBONES][3][4]) 
{
	static GETBLENDAPI_FN getblend=NULL;
	static int missing=0;

	if(missing) {
		return(0);
	}
	if(!getblend) {
		getblend = (GETBLENDAPI_FN) GetProcAddress(h_Library,"Server_GetBlendingInterface");
	}
	if(!getblend) {
		missing=1;
		return(0);
	}
	return((getblend)(version, ppinterface, pstudio, rotationmatrix, 
			bonetransform));
}

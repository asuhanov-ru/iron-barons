//
//  cdll_int.c
//
// this implementation handles the linking of the engine to the DLL
//

#include "hud.h"
#include "cl_util.h"
#include "netadr.h"
#undef INTERFACE_H
#include "../public/interface.h"
//#include "vgui_schememanager.h"

extern "C"
{
#include "pm_shared.h"
}

#include <string.h>

#ifdef _WIN32
#include "winsani_in.h"
#include <windows.h>
#include "winsani_out.h"
#endif
#include "Exports.h"

cl_enginefunc_t gEngfuncs;
CHud gHUD;

cldll_func_dst_t *g_pcldstAddrs;

int CL_DLLEXPORT Initialize( cl_enginefunc_t *pEnginefuncs, int iVersion )
{
	gEngfuncs = *pEnginefuncs;

//	RecClInitialize(pEnginefuncs, iVersion);

	if (iVersion != CLDLL_INTERFACE_VERSION)
		return 0;

	memcpy(&gEngfuncs, pEnginefuncs, sizeof(cl_enginefunc_t));

	EV_HookEvents();

	CL_LoadParticleMan();

	// get tracker interface, if any
	return 1;
}

/*
==========================
	HUD_Init

Called whenever the client connects
to a server.  Reinitializes all 
the hud variables.
==========================
*/

void CL_DLLEXPORT HUD_Init( void )
{
//	RecClHudInit();
	InitInput();
	gHUD.Init();
	Scheme_Init();
}


/*
==========================
	HUD_VidInit

Called when the game initializes
and whenever the vid_mode is changed
so the HUD can reinitialize itself.
==========================
*/

int CL_DLLEXPORT HUD_VidInit( void )
{
//	RecClHudVidInit();
	gHUD.VidInit();

	VGui_Startup();

	return 1;
}

/*
==========================
	HUD_Redraw

called every screen frame to
redraw the HUD.
===========================
*/

int CL_DLLEXPORT HUD_Redraw( float time, int intermission )
{
//	RecClHudRedraw(time, intermission);

	gHUD.Redraw( time, intermission );

	return 1;
}

/*
==========================
	HUD_UpdateClientData

called every time shared client
dll/engine data gets changed,
and gives the cdll a chance
to modify the data.

returns 1 if anything has been changed, 0 otherwise.
==========================
*/

int CL_DLLEXPORT HUD_UpdateClientData(client_data_t *pcldata, float flTime )
{
//	RecClHudUpdateClientData(pcldata, flTime);

	IN_Commands();

	return gHUD.UpdateClientData(pcldata, flTime );
}

/*
==========================
	HUD_Reset

Called at start and end of demos to restore to "non"HUD state.
==========================
*/

void CL_DLLEXPORT HUD_Reset( void )
{
//	RecClHudReset();

	gHUD.VidInit();
}

void CL_DLLEXPORT HUD_PlayerMoveInit( struct playermove_s *ppmove )
{
//	RecClClientMoveInit(ppmove);

	PM_Init( ppmove );
}

char CL_DLLEXPORT HUD_PlayerMoveTexture( char *name )
{
//	RecClClientTextureType(name);

	return PM_FindTextureType( name );
}

void CL_DLLEXPORT HUD_PlayerMove( struct playermove_s *ppmove, int server )
{
//	RecClClientMove(ppmove, server);

	PM_Move( ppmove, server );
}






extern "C" void CL_DLLEXPORT F(void *pv)
{
	cldll_func_t *pcldll_func = (cldll_func_t *)pv;

	// Hack!
	g_pcldstAddrs = ((cldll_func_dst_t *)pcldll_func->pHudVidInitFunc);

    cldll_func_t cldll_func = 
	{
	Initialize,
	HUD_Init,
	HUD_VidInit,
	HUD_Redraw,
	HUD_UpdateClientData,
	HUD_Reset,
	HUD_PlayerMove,
	HUD_PlayerMoveInit,
	HUD_PlayerMoveTexture,
	IN_ActivateMouse,
	IN_DeactivateMouse,
	IN_MouseEvent,
	IN_ClearStates,
	IN_Accumulate,
	CL_CreateMove,
	CL_IsThirdPerson,
	CL_CameraOffset,
	KB_Find,
	CAM_Think,
	V_CalcRefdef,
	HUD_AddEntity,
	HUD_CreateEntities,
	HUD_DrawNormalTriangles,
	HUD_DrawTransparentTriangles,
	HUD_StudioEvent,
	HUD_PostRunCmd,
	HUD_Shutdown,
	HUD_TxferLocalOverrides,
	HUD_ProcessPlayerState,
	HUD_TxferPredictionData,
	Demo_ReadBuffer,
	HUD_ConnectionlessPacket,
	HUD_GetHullBounds,
	HUD_Frame,
	HUD_Key_Event,
	HUD_TempEntUpdate,
	HUD_GetUserEntity,
	HUD_VoiceStatus,
	HUD_DirectorMessage,
	HUD_GetStudioModelInterface,
	HUD_ChatInputPosition,
	};

	*pcldll_func = cldll_func;
}
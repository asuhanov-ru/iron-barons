//
// hud.cpp
//
// implementation of CHud class
//

#include "hud.h"
#include "cl_util.h"

hud_player_info_t	 g_PlayerInfoList[MAX_PLAYERS+1];	 // player info from the engine
extra_player_info_t  g_PlayerExtraInfo[MAX_PLAYERS+1];   // additional player info sent directly to the client dll




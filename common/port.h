/*
port.h -- Portability Layer for Windows types
Copyright (C) 2015 Alibek Omarov

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#pragma once
#ifndef PORT_H
#define PORT_H

#include "build.h"

#if !_WIN32
    #error
#else // WIN32
	#define open _open
	#define read _read
	#define alloca _alloca

	#define HSPRITE WINAPI_HSPRITE
		#define WIN32_LEAN_AND_MEAN
		#include <winsock2.h>
		#include <windows.h>
	#undef HSPRITE

	#define OS_LIB_PREFIX ""
	#define OS_LIB_EXT "dll"
	#define VGUI_SUPPORT_DLL "../vgui_support." OS_LIB_EXT
	#define HAVE_DUP
#endif //WIN32

#ifndef IB_LOW_MEMORY
#define IB_LOW_MEMORY 0
#endif

#include <stdlib.h>
#include <string.h>
#include <limits.h>

#if defined IB_SDL && !defined REF_DLL
#include <SDL.h>
#endif

#endif // PORT_H

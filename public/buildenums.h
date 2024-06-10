/*
build.h - compile-time build information
Copyright (C) 2023 Alibek Omarov

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
#ifndef BUILDENUMS_H
#define BUILDENUMS_H

#include "build.h"

// This header defines the enumeration values that can be passed to Q_build*
// functions and get current value through IB_PLATFORM, IB_ARCHITECTURE and
// IB_ARCHITECTURE_ABI defines

//================================================================
//
//           OPERATING SYSTEM DEFINES
//
//================================================================
#define PLATFORM_WIN32      1
#define PLATFORM_LINUX      2
#define PLATFORM_FREEBSD    3
#define PLATFORM_ANDROID    4
#define PLATFORM_APPLE      5
#define PLATFORM_NETBSD     6
#define PLATFORM_OPENBSD    7
#define PLATFORM_EMSCRIPTEN 8
#define PLATFORM_DOS4GW     9
#define PLATFORM_HAIKU      10
#define PLATFORM_SERENITY   11
#define PLATFORM_IRIX       12
#define PLATFORM_NSWITCH    13
#define PLATFORM_PSVITA     14

#if _WIN32
	#define IB_PLATFORM PLATFORM_WIN32
#elif IB_ANDROID
	#define IB_PLATFORM PLATFORM_ANDROID
#elif IB_LINUX
	#define IB_PLATFORM PLATFORM_LINUX
#elif IB_APPLE
	#define IB_PLATFORM PLATFORM_APPLE
#elif IB_FREEBSD
	#define IB_PLATFORM PLATFORM_FREEBSD
#elif IB_NETBSD
	#define IB_PLATFORM PLATFORM_NETBSD
#elif IB_OPENBSD
	#define IB_PLATFORM PLATFORM_OPENBSD
#elif IB_EMSCRIPTEN
	#define IB_PLATFORM PLATFORM_EMSCRIPTEN
#elif IB_DOS4GW
	#define IB_PLATFORM PLATFORM_DOS4GW
#elif IB_HAIKU
	#define IB_PLATFORM PLATFORM_HAIKU
#elif IB_SERENITY
	#define IB_PLATFORM PLATFORM_SERENITY
#elif IB_IRIX
	#define IB_PLATFORM PLATFORM_IRIX
#elif IB_NSWITCH
	#define IB_PLATFORM PLATFORM_NSWITCH
#elif IB_PSVITA
	#define IB_PLATFORM PLATFORM_PSVITA
#else
	#error
#endif

//================================================================
//
//           CPU ARCHITECTURE DEFINES
//
//================================================================
#define ARCHITECTURE_X86     1
#define ARCHITECTURE_AMD64   2
#define ARCHITECTURE_ARM     3
#define ARCHITECTURE_MIPS    4
#define ARCHITECTURE_JS      6
#define ARCHITECTURE_E2K     7
#define ARCHITECTURE_RISCV   8
#define ARCHITECTURE_PPC     9

#if IB_AMD64
	#define IB_ARCHITECTURE ARCHITECTURE_AMD64
#elif IB_X86
	#define IB_ARCHITECTURE ARCHITECTURE_X86
#elif IB_ARM
	#define IB_ARCHITECTURE ARCHITECTURE_ARM
#elif IB_MIPS
	#define IB_ARCHITECTURE ARCHITECTURE_MIPS
#elif IB_JS
	#define IB_ARCHITECTURE ARCHITECTURE_JS
#elif IB_E2K
	#define IB_ARCHITECTURE ARCHITECTURE_E2K
#elif IB_RISCV
	#define IB_ARCHITECTURE ARCHITECTURE_RISCV
#elif IB_PPC
	#define IB_ARCHITECTURE ARCHITECTURE_PPC
#else
	#error
#endif

//================================================================
//
//           ENDIANNESS DEFINES
//
//================================================================
#define ENDIANNESS_LITTLE  1
#define ENDIANNESS_BIG     2

#if IB_LITTLE_ENDIAN
	#define IB_ENDIANNESS ENDIANNESS_LITTLE
#elif IB_BIG_ENDIAN
	#define IB_ENDIANNESS ENDIANNESS_BIG
#else
	#error
#endif

//================================================================
//
//           APPLICATION BINARY INTERFACE
//
//================================================================
#define BIT( n )		( 1U << ( n ))

#define ARCH_ARM_VER_MASK   ( BIT( 5 ) - 1 )
#define ARCH_ARM_VER_SHIFT  0
#define ARCH_ARM_HARDFP     BIT( 5 )

#define ARCH_RISCV_FP_SOFT   0
#define ARCH_RISCV_FP_SINGLE 1
#define ARCH_RISCV_FP_DOUBLE 2

#if IB_ARCHITECTURE == ARCHITECTURE_ARM
	#if IB_ARM_HARDFP
		#define IB_ARCHITECTURE_ABI ( ARCH_ARM_HARDFP | IB_ARM )
	#else
		#define IB_ARCHITECTURE_ABI ( IB_ARM )
	#endif
#elif IB_ARCHITECTURE == ARCHITECTURE_RISCV
	#if IB_RISCV_SOFTFP
		#define IB_ARCHITECTURE_ABI ARCH_RISCV_FP_SOFT
	#elif IB_RISCV_SINGLEFP
		#define IB_ARCHITECTURE_ABI ARCH_RISCV_FP_SINGLE
	#elif IB_RISCV_DOUBLEFP
		#define IB_ARCHITECTURE_ABI ARCH_RISCV_FP_DOUBLE
	#else
		#error
	#endif
#else
	#define IB_ARCHITECTURE_ABI 0 // unused
#endif


#endif // BUILDENUMS_H

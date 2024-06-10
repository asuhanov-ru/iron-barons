// basic typedefs
#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include "build.h"
#include <wchar.h> // off_t

#include <sys/types.h> // off_t
#include <stdint.h>
#include <assert.h>

typedef unsigned char byte;
typedef int		sound_t;
typedef float		vec_t;
typedef vec_t		vec2_t[2];
typedef vec_t		vec3_t[3];
typedef vec_t		vec4_t[4];
typedef vec_t		quat_t[4];
typedef byte		rgba_t[4];	// unsigned byte colorpack
typedef byte		rgb_t[3];		// unsigned byte colorpack
typedef vec_t		matrix3x4[3][4];
typedef vec_t		matrix4x4[4][4];

typedef uint32_t        poolhandle_t;

#undef true
#undef false

#ifndef __cplusplus
typedef enum { false, true }	qboolean;
#else
typedef int qboolean;
#endif

typedef uint64_t longtime_t;

#define MAX_STRING		256	// generic string
#define MAX_INFO_STRING	256	// infostrings are transmitted across network
#define MAX_SERVERINFO_STRING	512	// server handles too many settings. expand to 1024?
#define MAX_LOCALINFO_STRING	32768	// localinfo used on server and not sended to the clients
#define MAX_SYSPATH		1024	// system filepath
#define MAX_VA_STRING	1024	// string length returned by va()
#define MAX_PRINT_MSG	8192	// how many symbols can handle single call of Con_Printf or Con_DPrintf
#define MAX_TOKEN		2048	// parse token length
#define MAX_MODS		512	// environment games that engine can keep visible
#define MAX_USERMSG_LENGTH	2048	// don't modify it's relies on a client-side definitions

#define BIT( n )		( 1U << ( n ))
#define BIT64( n )		( 1ULL << ( n ))
#define SetBits( iBitVector, bits )	((iBitVector) = (iBitVector) | (bits))
#define ClearBits( iBitVector, bits )	((iBitVector) = (iBitVector) & ~(bits))
#define FBitSet( iBitVector, bit )	((iBitVector) & (bit))

#ifndef __cplusplus
#ifdef NULL
#undef NULL
#endif

#define NULL		((void *)0)
#endif

// color strings
#define IsColorString( p )	( p && *( p ) == '^' && *(( p ) + 1) && *(( p ) + 1) >= '0' && *(( p ) + 1 ) <= '9' )
#define ColorIndex( c )	((( c ) - '0' ) & 7 )

// Platform specific
#if defined( _MSC_VER )
	#define EXPORT         __declspec( dllexport )
#else
	#define EXPORT
#endif

#define GAME_EXPORT
#define NORETURN
#define NONNULL
#define _format( x )
#define ALLOC_CHECK( x )
#define RENAME_SYMBOL( x )

#if defined( __has_builtin )
	#if __has_builtin( __builtin_expect )
		#define unlikely( x ) __builtin_expect( x, 0 )
		#define likely( x )   __builtin_expect( x, 1 )
	#else
		#define unlikely( x ) ( x )
		#define likely( x )   ( x )
	#endif
#endif

#if __STDC_VERSION__ >= 202311L || __cplusplus >= 201103L // C23 or C++ static_assert is a keyword
	#define STATIC_ASSERT_( ignore, x, y ) static_assert( x, y )
	#define STATIC_ASSERT  static_assert
#elif __STDC_VERSION__ >= 201112L // in C11 it's _Static_assert
	#define STATIC_ASSERT_( ignore, x, y ) _Static_assert( x, y )
	#define STATIC_ASSERT  _Static_assert
#else
	#define STATIC_ASSERT_( id, x, y ) extern int id[( x ) ? 1 : -1]
	// need these to correctly expand the line macro
	#define STATIC_ASSERT_3( line, x, y ) STATIC_ASSERT_( static_assert_ ## line, x, y )
	#define STATIC_ASSERT_2( line, x, y ) STATIC_ASSERT_3( line, x, y )
	#define STATIC_ASSERT( x, y ) STATIC_ASSERT_2( __LINE__, x, y )
#endif

// _restrict
#if !defined( __cplusplus ) && __STDC_VERSION__ >= 199101L 
	// not C++ and C99 or newer
	#define IB_RESTRICT restrict
#elif _MSC_VER || __GNUC__ || __clang__
	// compiler-specific extensions
	#define IB_RESTRICT __restrict
#else
	#define IB_RESTRICT // nothing
#endif

// Endianes
#define LittleLong(x) (x)
#define LittleLongSW(x)
#define LittleShort(x) (x)
#define LittleShortSW(x)
#define LittleFloat(x) (x)

typedef unsigned int	dword;
typedef unsigned int	uint;
typedef char		string[MAX_STRING];
typedef struct file_s	file_t;		// normal file
typedef struct stream_s	stream_t;		// sound stream for background music playing
typedef off_t fs_offset_t;

typedef int fs_size_t; // return type of _read, _write funcs

typedef struct dllfunc_s
{
	const char	*name;
	void		**func;
} dllfunc_t;

typedef struct dll_info_s
{
	const char	*name;	// name of library
	const dllfunc_t	*fcts;	// list of dll exports
	qboolean		crash;	// crash if dll not found
	void		*link;	// hinstance of loading library
} dll_info_t;

typedef void (*setpair_t)( const char *key, const void *value, const void *buffer, void *numpairs );
typedef void *(*pfnCreateInterface_t)( const char *, int * );

// config strings are a general means of communication from
// the server to all connected clients.
// each config string can be at most CS_SIZE characters.

#define MAX_QPATH		64	// max length of a game pathname
#define MAX_OSPATH		260	// max length of a filesystem pathname
#define CS_SIZE		64	// size of one config string
#define CS_TIME		16	// size of time string

#endif // COMMON_TYPES_H
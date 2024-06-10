#ifndef _CORE_H_
#define _CORE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "common_types.h"

//
// core.c
//
typedef void( *pfnChangeGame )( const char *progname );

int EXPORT Engine_Main( int argc, char **argv, const char *progname, int bChangeGame, pfnChangeGame func );

#ifdef __cplusplus
}
#endif

#endif // _CORE_H_
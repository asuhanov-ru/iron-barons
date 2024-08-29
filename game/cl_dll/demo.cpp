#include "hud.h"
#include "cl_util.h"
#include "demo.h"
#include "demo_api.h"
#include <memory.h>
#include "Exports.h"

int g_demosniper = 0;
int g_demosniperdamage = 0;
float g_demosniperorg[3];
float g_demosniperangles[3];
float g_demozoom;

/*
=====================
Demo_ReadBuffer

Engine wants us to parse some data from the demo stream
=====================
*/
void CL_DLLEXPORT Demo_ReadBuffer( int size, unsigned char *buffer )
{
//	RecClReadDemoBuffer(size, buffer);

	int type;
	int i = 0;

	type = *( int * )buffer;
	i += sizeof( int );
	switch ( type )
	{
	case TYPE_SNIPERDOT:
		g_demosniper = *(int * )&buffer[ i ];
		i += sizeof( int );
		
		if ( g_demosniper )
		{
			g_demosniperdamage = *( int * )&buffer[ i ];
			i += sizeof( int );

			g_demosniperangles[ 0 ] = *(float *)&buffer[i];
			i += sizeof( float );
			g_demosniperangles[ 1 ] = *(float *)&buffer[i];
			i += sizeof( float );
			g_demosniperangles[ 2 ] = *(float *)&buffer[i];
			i += sizeof( float );
			g_demosniperorg[ 0 ] = *(float *)&buffer[i];
			i += sizeof( float );
			g_demosniperorg[ 1 ] = *(float *)&buffer[i];
			i += sizeof( float );
			g_demosniperorg[ 2 ] = *(float *)&buffer[i];
			i += sizeof( float );
		}
		break;
	case TYPE_ZOOM:
		g_demozoom = *(float * )&buffer[ i ];
		i += sizeof( float );
		break;
	default:
		gEngfuncs.Con_DPrintf( "Unknown demo buffer type, skipping.\n" );
		break;
	}
}

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"

#define MAX_CLIENTS 32
/// USER-DEFINED SERVER MESSAGE HANDLERS

int CHud :: MsgFunc_ResetHUD(const char *pszName, int iSize, void *pbuf )
{
	ASSERT( iSize == 0 );

	// clear all hud data
	HUDLIST *pList = m_pHudList;

	while ( pList )
	{
		if ( pList->p )
			pList->p->Reset();
		pList = pList->pNext;
	}

	// reset sensitivity
	m_flMouseSensitivity = 0;

	// reset concussion effect
	m_iConcussionEffect = 0;

	return 1;
}
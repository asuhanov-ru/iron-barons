#include<VGUI_Cursor.h>
#include<VGUI_Frame.h>
#include<VGUI_Label.h>
#include<VGUI_Surface.h>
#include<VGUI_BorderLayout.h>
#include<VGUI_Panel.h>
#include<VGUI_ImagePanel.h>
#include<VGUI_Button.h>
#include<VGUI_ActionSignal.h>
#include<VGUI_InputSignal.h>
#include<VGUI_MenuSeparator.h>
#include<VGUI_TextPanel.h>
#include<VGUI_LoweredBorder.h>
#include<VGUI_LineBorder.h>
#include<VGUI_Scheme.h>
#include<VGUI_Font.h>
#include<VGUI_App.h>
#include<VGUI_BuildGroup.h>

#include "hud.h"
#include "cl_util.h"
#include "camera.h"
#include "kbutton.h"
#include "cvardef.h"
#include "usercmd.h"
#include "const.h"
#include "camera.h"
#include "in_defs.h"
#include "parsemsg.h"
#include "pm_shared.h"
#include "keydefs.h"
#include "demo.h"
#include "demo_api.h"

#include "vgui_int.h"
#include "vgui_IronBaronsViewport.h"


#include "shake.h"
#include "screenfade.h"

extern int g_iVisibleMouse;
class CCommandMenu;
int g_iPlayerClass;
int g_iTeamNumber;
int g_iUser1 = 0;
int g_iUser2 = 0;
int g_iUser3 = 0;

// Scoreboard positions
#define SBOARD_INDENT_X			XRES(104)
#define SBOARD_INDENT_Y			YRES(40)

// low-res scoreboard indents
#define SBOARD_INDENT_X_512		30
#define SBOARD_INDENT_Y_512		30

#define SBOARD_INDENT_X_400		0
#define SBOARD_INDENT_Y_400		20

void IN_ResetMouse( void );
// extern CMenuPanel *CMessageWindowPanel_Create( const char *szMOTD, const char *szTitle, int iShadeFullscreen, int iRemoveMe, int x, int y, int wide, int tall );
extern float * GetClientColor( int clientIndex );

using namespace vgui;

// Team Colors
int iNumberOfTeamColors = 5;
int iTeamColors[5][3] =
{
	{ 255, 170, 0 },	// HL orange (default)
	{ 125, 165, 210 },	// Blue
	{ 200, 90, 70 },	// Red
	{ 225, 205, 45 },	// Yellow
	{ 145, 215, 140 },	// Green
};


// Used for Class specific buttons
char *sTFClasses[] =
{
	"",
	"SCOUT",
	"SNIPER",
	"SOLDIER",
	"DEMOMAN",
	"MEDIC",
	"HWGUY",
	"PYRO",
	"SPY",
	"ENGINEER",
	"CIVILIAN",
};

char *sLocalisedClasses[] = 
{
	"#Civilian",
	"#Scout",
	"#Sniper",
	"#Soldier",
	"#Demoman",
	"#Medic",
	"#HWGuy",
	"#Pyro",
	"#Spy",
	"#Engineer",
	"#Random",
	"#Civilian",
};

char *sTFClassSelection[] = 
{
	"civilian",
	"scout",
	"sniper",
	"soldier",
	"demoman",
	"medic",
	"hwguy",
	"pyro",
	"spy",
	"engineer",
	"randompc",
	"civilian",
};


//-----------------------------------------------------------------------------
// Purpose: Called everytime a new level is started. Viewport clears out it's data.
//-----------------------------------------------------------------------------
void IronBaronsViewport::Initialize( void )
{
	// Force each menu to Initialize
/* 	if (m_pTeamMenu)
	{
		m_pTeamMenu->Initialize();
	}
	if (m_pClassMenu)
	{
		m_pClassMenu->Initialize();
	}
	if (m_pScoreBoard)
	{
		m_pScoreBoard->Initialize();
		HideScoreBoard();
	}
	if (m_pSpectatorPanel)
	{
		// Spectator menu doesn't need initializing
		m_pSpectatorPanel->setVisible( false );
	} */

	// Make sure all menus are hidden
	//HideVGUIMenu();
	//HideCommandMenu();

	// Clear out some data
	m_iGotAllMOTD = true;
	m_iRandomPC = false;
	m_flScoreBoardLastUpdated = 0;
	m_flSpectatorPanelLastUpdated = 0;

	// reset player info
	g_iPlayerClass = 0;
	g_iTeamNumber = 0;

	strcpy(m_sMapName, "");
	//strcpy(m_szServerName, "");
	for (int i = 0; i < 5; i++)
	{
		m_iValidClasses[i] = 0;
		//strcpy(m_sTeamNames[i], "");
	}

	App::getInstance()->setCursorOveride( App::getInstance()->getScheme()->getCursor(Scheme::scu_none) );
}

//================================================================
IronBaronsViewport::IronBaronsViewport(int x,int y,int wide,int tall) : Panel(x,y,wide,tall), m_SchemeManager(wide,tall)
{
	gViewPort = this;
	m_iInitialized = false;
	//m_pTeamMenu = NULL;
	//m_pClassMenu = NULL;
	//m_pScoreBoard = NULL;
	//m_pSpectatorPanel = NULL;
	//m_pCurrentMenu = NULL;
	//m_pCurrentCommandMenu = NULL;

	Initialize();
	//addInputSignal( new CViewPortInputHandler );

	int r, g, b, a;
	
	//Scheme* pScheme = App::getInstance()->getScheme();

	// primary text color
	// Get the colors
	//!! two different types of scheme here, need to integrate
	/* SchemeHandle_t hPrimaryScheme = m_SchemeManager.getSchemeHandle( "Primary Button Text" );
	{
		// font
		pScheme->setFont( Scheme::sf_primary1, m_SchemeManager.getFont(hPrimaryScheme) );

		// text color
		m_SchemeManager.getFgColor( hPrimaryScheme, r, g, b, a );
		pScheme->setColor(Scheme::sc_primary1, r, g, b, a );		// sc_primary1 is non-transparent orange

		// background color (transparent black)
		m_SchemeManager.getBgColor( hPrimaryScheme, r, g, b, a );
		pScheme->setColor(Scheme::sc_primary3, r, g, b, a );

		// armed foreground color
		m_SchemeManager.getFgArmedColor( hPrimaryScheme, r, g, b, a );
		pScheme->setColor(Scheme::sc_secondary2, r, g, b, a );

		// armed background color
		m_SchemeManager.getBgArmedColor( hPrimaryScheme, r, g, b, a );
		pScheme->setColor(Scheme::sc_primary2, r, g, b, a );

		//!! need to get this color from scheme file
		// used for orange borders around buttons
		m_SchemeManager.getBorderColor( hPrimaryScheme, r, g, b, a );
		// pScheme->setColor(Scheme::sc_secondary1, r, g, b, a );
		pScheme->setColor(Scheme::sc_secondary1, 255*0.7, 170*0.7, 0, 0);
	}

	// Change the second primary font (used in the scoreboard)
	SchemeHandle_t hScoreboardScheme = m_SchemeManager.getSchemeHandle( "Scoreboard Text" );
	{
		pScheme->setFont(Scheme::sf_primary2, m_SchemeManager.getFont(hScoreboardScheme) );
	}
	
	// Change the third primary font (used in command menu)
	SchemeHandle_t hCommandMenuScheme = m_SchemeManager.getSchemeHandle( "CommandMenu Text" );
	{
		pScheme->setFont(Scheme::sf_primary3, m_SchemeManager.getFont(hCommandMenuScheme) );
	}

	App::getInstance()->setScheme(pScheme); */

	// VGUI MENUS
	//CreateTeamMenu();
	//CreateClassMenu();
	//CreateSpectatorMenu();
	//CreateScoreBoard();
	// Init command menus
	m_iNumMenus = 0;
	//m_iCurrentTeamNumber = m_iUser1 = m_iUser2 = m_iUser3 = 0;

	//m_StandardMenu = CreateCommandMenu("commandmenu.txt", 0, CMENU_TOP, false, CMENU_SIZE_X, BUTTON_SIZE_Y, 0 );
	//m_SpectatorOptionsMenu = CreateCommandMenu("spectatormenu.txt", 1, PANEL_HEIGHT, true, CMENU_SIZE_X, BUTTON_SIZE_Y / 2, 0 );	// above bottom bar, flat design
	//m_SpectatorCameraMenu = CreateCommandMenu("spectcammenu.txt", 1, PANEL_HEIGHT, true, XRES( 200 ), BUTTON_SIZE_Y / 2, ScreenWidth - ( XRES ( 200 ) + 15 ) );	// above bottom bar, flat design

	//m_PlayerMenu =  m_iNumMenus;
	//m_iNumMenus++;

	//float flLabelSize = ( (ScreenWidth - (XRES ( CAMOPTIONS_BUTTON_X ) + 15)) - XRES ( 24 + 15 ) ) - XRES( (15 + OPTIONS_BUTTON_X + 15) + 38 );

	//m_pCommandMenus[m_PlayerMenu] = new CCommandMenu(NULL, 1, 
	//								XRES( ( 15 + OPTIONS_BUTTON_X + 15 ) + 31 ),PANEL_HEIGHT, flLabelSize,300);	
	//m_pCommandMenus[m_PlayerMenu]->setParent(this);
	//m_pCommandMenus[m_PlayerMenu]->setVisible(false);
	//m_pCommandMenus[m_PlayerMenu]->m_flButtonSizeY = BUTTON_SIZE_Y /2;
	//m_pCommandMenus[m_PlayerMenu]->m_iSpectCmdMenu = 1;

	//UpdatePlayerMenu(m_PlayerMenu);

	//CreateServerBrowser();


}

// Direct Key Input
int	IronBaronsViewport::KeyInput( int down, int keynum, const char *pszCurrentBinding )
{
	// Enter gets out of Spectator Mode by bringing up the Team Menu
	//if (m_iUser1 && gEngfuncs.Con_IsVisible() == false )
	//{
	//	if ( down && (keynum == K_ENTER || keynum == K_KP_ENTER) )
	//		ShowVGUIMenu( MENU_TEAM );
	//}

	// Open Text Window?
	//if (m_pCurrentMenu && gEngfuncs.Con_IsVisible() == false)
	//{
	//	int iMenuID = m_pCurrentMenu->GetMenuID();

		// Get number keys as Input for Team/Class menus
	//	if (iMenuID == MENU_TEAM || iMenuID == MENU_CLASS)
	//	{
			// Escape gets you out of Team/Class menus if the Cancel button is visible
	//		if ( keynum == K_ESCAPE )
	//		{
	//			if ( (iMenuID == MENU_TEAM && g_iTeamNumber) || (iMenuID == MENU_CLASS && g_iPlayerClass) )
	//			{
	//				HideTopMenu();
	//				return 0;
	//			}
	//		}

	//		for (int i = '0'; i <= '9'; i++)
	//		{
	//			if ( down && (keynum == i) )
	//			{
	//				SlotInput( i - '0' );
	//				return 0;
	//			}
	//		}
	//	}

		// Grab enter keys to close TextWindows
	//	if ( down && (keynum == K_ENTER || keynum == K_KP_ENTER || keynum == K_SPACE || keynum == K_ESCAPE) )
	//	{
	//		if ( iMenuID == MENU_MAPBRIEFING || iMenuID == MENU_INTRO || iMenuID == MENU_CLASSHELP )
	//		{
	//			HideTopMenu();
	//			return 0;
	//		}
	//	}

		// Grab jump key on Team Menu as autoassign
	//	if ( pszCurrentBinding && down && !strcmp(pszCurrentBinding, "+jump") )
	//	{
	//		if (iMenuID == MENU_TEAM)
	//		{
	//			m_pTeamMenu->SlotInput(5);
	//			return 0;
	//		}
	//	}

	//}

	// if we're in a command menu, try hit one of it's buttons
	//if ( down && m_pCurrentCommandMenu )
	//{
		// Escape hides the command menu
	//	if ( keynum == K_ESCAPE )
	//	{
	//		HideCommandMenu();
	//		return 0;
	//	}

		// only trap the number keys
	//	if ( keynum >= '0' && keynum <= '9' )
	//	{
	//		if ( m_pCurrentCommandMenu->KeyInput(keynum) )
	//		{
				// a final command has been issued, so close the command menu
	//			HideCommandMenu();
	//		}

	//		return 0;
	//	}
	//}

	return 1;
}

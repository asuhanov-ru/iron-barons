
#ifndef IRONBARONSVIEWPORT_H
#define IRONBARONSVIEWPORT_H

#include<VGUI_Panel.h>
#include<VGUI_Frame.h>
#include<VGUI_TextPanel.h>
#include<VGUI_Label.h>
#include<VGUI_Button.h>
#include<VGUI_ActionSignal.h>
#include<VGUI_InputSignal.h>
#include<VGUI_Scheme.h>
#include<VGUI_Image.h>
#include<VGUI_FileInputStream.h>
#include<VGUI_BitmapTGA.h>
#include<VGUI_DesktopIcon.h>
#include<VGUI_App.h>
#include<VGUI_MiniApp.h>
#include<VGUI_LineBorder.h>
#include<VGUI_String.h>
#include<VGUI_ScrollPanel.h>
#include<VGUI_ScrollBar.h>
#include<VGUI_Slider.h>

// custom scheme handling
#include "vgui_SchemeManager.h"

using namespace vgui;

class IronBaronsViewport;

extern IronBaronsViewport *gViewPort;

//==============================================================================
class IronBaronsViewport : public Panel
{
private:
	vgui::Cursor* _cursorNone;
	vgui::Cursor* _cursorArrow;

	int			 m_iInitialized;

/* 	CCommandMenu *m_pCommandMenus[ MAX_MENUS ];
	CCommandMenu *m_pCurrentCommandMenu; */
	float		 m_flMenuOpenTime;
	float		 m_flScoreBoardLastUpdated;
	float		 m_flSpectatorPanelLastUpdated;
	int			 m_iNumMenus;
	int			 m_iCurrentTeamNumber;
	int			 m_iCurrentPlayerClass;
	int			 m_iUser1;
	int			 m_iUser2;
	int			 m_iUser3;

	// VGUI Menus
/* 	void		 CreateTeamMenu( void );
	CMenuPanel*	 ShowTeamMenu( void );
	void		 CreateClassMenu( void );
	CMenuPanel*	 ShowClassMenu( void );
	void		 CreateSpectatorMenu( void ); */
	
	// Scheme handler
	CSchemeManager m_SchemeManager;

	// MOTD
	int		m_iGotAllMOTD;
	// char	m_szMOTD[ MAX_MOTD_LENGTH ];

	//  Command Menu Team buttons
	//CommandButton *m_pTeamButtons[6];
	//CommandButton *m_pDisguiseButtons[5];
	//BuildButton   *m_pBuildButtons[3];
	//BuildButton   *m_pBuildActiveButtons[3];

	// Server Browser
	//ServerBrowser *m_pServerBrowser;

	int					m_iAllowSpectators;

	// Data for specific sections of the Command Menu
	int			m_iValidClasses[5];
	int			m_iIsFeigning;
	int			m_iIsSettingDetpack;
	int			m_iNumberOfTeams;
	int			m_iBuildState;
	int			m_iRandomPC;
	//char		m_sTeamNames[5][MAX_TEAMNAME_SIZE];

	// Localisation strings
	//char		m_sDetpackStrings[3][MAX_BUTTON_SIZE];

	char		m_sMapName[64];

	// helper function to update the player menu entries
	void UpdatePlayerMenu(int menuIndex);

public:
	IronBaronsViewport(int x,int y,int wide,int tall);
	void Initialize( void );

	int		CreateCommandMenu( char * menuFile, int direction, int yOffset, bool flatDesign, float flButtonSizeX, float flButtonSizeY, int xOffset );
	void	CreateScoreBoard( void );
	//void	CreateServerBrowser( void );
	//CommandButton * CreateCustomButton( char *pButtonText, char * pButtonName, int  iYOffset );
	//CCommandMenu *	CreateDisguiseSubmenu( CommandButton *pButton, CCommandMenu *pParentMenu, const char *commandText, int iYOffset, int iXOffset = 0 );

	//void UpdateCursorState( void );
	//void UpdateCommandMenu(int menuIndex);
	//void UpdateOnPlayerInfo( void );
	//void UpdateHighlights( void );
	//void UpdateSpectatorPanel( void );

	int	 KeyInput( int down, int keynum, const char *pszCurrentBinding );
	//void InputPlayerSpecial( void );
	//void GetAllPlayersInfo( void );
	//void DeathMsg( int killer, int victim );

	//void ShowCommandMenu(int menuIndex);
	//void InputSignalHideCommandMenu( void );
	//void HideCommandMenu( void );
	//void SetCurrentCommandMenu( CCommandMenu *pNewMenu );
	//void SetCurrentMenu( CMenuPanel *pMenu );

	//void ShowScoreBoard( void );
	//void HideScoreBoard( void );
	//bool IsScoreBoardVisible( void );

	//bool AllowedToPrintText( void );

	//void ShowVGUIMenu( int iMenu );
	//void HideVGUIMenu( void );
	//void HideTopMenu( void );

	//void ToggleServerBrowser( void );

	//CMenuPanel* CreateTextWindow( int iTextToShow );

	//CCommandMenu *CreateSubMenu( CommandButton *pButton, CCommandMenu *pParentMenu, int iYOffset, int iXOffset = 0 );

	// Data Handlers
	//int GetValidClasses(int iTeam) { return m_iValidClasses[iTeam]; };
	//int GetNumberOfTeams() { return m_iNumberOfTeams; };
	//int GetIsFeigning() { return m_iIsFeigning; };
	//int GetIsSettingDetpack() { return m_iIsSettingDetpack; };
	//int GetBuildState() { return m_iBuildState; };
	//int IsRandomPC() { return m_iRandomPC; };
	//char *GetTeamName( int iTeam ) { return m_sTeamNames[iTeam]; };
	//int GetAllowSpectators() { return m_iAllowSpectators; };

	// Message Handlers
	//int MsgFunc_ValClass(const char *pszName, int iSize, void *pbuf );
	//int MsgFunc_TeamNames(const char *pszName, int iSize, void *pbuf );
	//int MsgFunc_Feign(const char *pszName, int iSize, void *pbuf );
	//int MsgFunc_Detpack(const char *pszName, int iSize, void *pbuf );
	//int MsgFunc_VGUIMenu(const char *pszName, int iSize, void *pbuf );
	//int MsgFunc_MOTD( const char *pszName, int iSize, void *pbuf );
	//int MsgFunc_BuildSt( const char *pszName, int iSize, void *pbuf );
	//int MsgFunc_RandomPC( const char *pszName, int iSize, void *pbuf );
	//int MsgFunc_ServerName( const char *pszName, int iSize, void *pbuf );
	//int MsgFunc_ScoreInfo( const char *pszName, int iSize, void *pbuf );
	//int MsgFunc_TeamScore( const char *pszName, int iSize, void *pbuf );
	//int MsgFunc_TeamInfo( const char *pszName, int iSize, void *pbuf );
	//int MsgFunc_Spectator( const char *pszName, int iSize, void *pbuf );
	//int MsgFunc_AllowSpec( const char *pszName, int iSize, void *pbuf );
	//int MsgFunc_SpecFade( const char *pszName, int iSize, void *pbuf );	
	//int MsgFunc_ResetFade( const char *pszName, int iSize, void *pbuf );	

	// Input
	//bool SlotInput( int iSlot );

	//virtual void paintBackground();

	//CSchemeManager *GetSchemeManager( void ) { return &m_SchemeManager; }
	//ScorePanel *GetScoreBoard( void ) { return m_pScoreBoard; }

	//void *operator new( size_t stAllocateBlock );

public:
	// VGUI Menus
	//CMenuPanel		*m_pCurrentMenu;
	//CTeamMenuPanel	*m_pTeamMenu;
	int						m_StandardMenu;	// indexs in m_pCommandMenus
	int						m_SpectatorOptionsMenu;
	int						m_SpectatorCameraMenu;
	int						m_PlayerMenu; // a list of current player
	//CClassMenuPanel	*m_pClassMenu;
	//ScorePanel		*m_pScoreBoard;
	//SpectatorPanel *		m_pSpectatorPanel;
	//char			m_szServerName[ MAX_SERVERNAME_LENGTH ];
};


#endif


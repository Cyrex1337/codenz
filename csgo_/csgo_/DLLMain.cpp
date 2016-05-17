#include "DLLMain.hpp"

void __stdcall painttraverse( unsigned int panel, bool forcerepaint, bool allowforce )
{
	oPaintTraverse( Interfaces::Panel, panel, forcerepaint, allowforce );

	static unsigned int panel_ = 0;
	if ( !panel_ )
	{
		std::string panelName = Interfaces::Panel->GetName( panel );
		if ( panelName.find( XorStr( "FocusOverlayPanel" ) ) != std::string::npos )
			panel_ = panel;
	}

	if ( panel_ && panel == panel_ )
	{
		g_pDrawing->DrawString( 5, 5, Color::White( ), false, XorStr( "Internalz." ) );
	}
}

void Init( )
{
	Utilities::CreateConsole( "Log" );

	Interfaces::Setup( );

	g_pPanelHook = std::make_shared<Hook>( Interfaces::Panel );

	oPaintTraverse = g_pPanelHook->HookPtr<tPaintTraverse>( 41, painttraverse );
}

BOOL WINAPI DllMain( HMODULE hModule, DWORD dwReason, LPVOID lpReserved )
{
	if ( dwReason == DLL_PROCESS_ATTACH )
	{
		CreateThread( nullptr, 0, ( LPTHREAD_START_ROUTINE ) Init, nullptr, 0, nullptr );
	}

	return TRUE;
}
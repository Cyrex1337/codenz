#include "InterfaceManager.hpp"
#include "Drawing.hpp"

template <typename T>
T* Interfaces::Grab( const char * module, const char * szinterface )
{
	tCreateInterface fn = ( tCreateInterface ) GetProcAddress( GetModuleHandle( module ), XorStr( "CreateInterface" ) );
	return ( T* ) fn( szinterface, nullptr );
}

template <typename T>
T* Interfaces::BruteForce( const char * module, const char * szinterface )
{
	tCreateInterface fn = ( tCreateInterface ) GetProcAddress( GetModuleHandle( module ), XorStr( "CreateInterface" ) );

	char tmp [ 128 ];

	for ( int i = 0; i < 100; i++ )
	{
		sprintf_s( tmp, sizeof( tmp ), "%s0%i", szinterface, i );

		T* return_ = ( T* ) fn( tmp, nullptr );
		if ( return_ ) return return_;

		sprintf_s( tmp, sizeof( tmp ), "%s00%i", szinterface, i );

		return_ = ( T* ) fn( tmp, nullptr );
		if ( return_ ) return return_;
	}

	return nullptr;
}

void Interfaces::Setup( )
{
	Interfaces::Client = Interfaces::Grab<IBaseClientDLL>( "client.dll", "VClient017" );
	Interfaces::PlayerInfoManager = Interfaces::Grab<IPlayerInfoManager>( "server.dll", "PlayerInfoManager002" );
	Interfaces::Globals = Interfaces::PlayerInfoManager->GetGlobalVars( );
	Interfaces::Surface = Interfaces::Grab<ISurface>( "vguimatsurface.dll", "VGUI_Surface031" );
	Interfaces::Panel = Interfaces::Grab<IVPanel>( "vgui2.dll", "VGUI_Panel009" );

	g_pDrawing = std::make_shared<Drawing>( );
}

IBaseClientDLL* Interfaces::Client;
IPlayerInfoManager* Interfaces::PlayerInfoManager;
IGlobalVarsBase* Interfaces::Globals;
ISurface* Interfaces::Surface;
IVPanel* Interfaces::Panel;
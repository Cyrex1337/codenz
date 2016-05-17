#include "Utilities.hpp"

std::shared_ptr<Hook> g_pPanelHook;

void Utilities::CreateConsole( const std::string& title )
{
	FreeConsole( );
	AllocConsole( );
	FILE *conout, *conin;
	freopen_s( &conout, "conout$", "w", stdout );
	freopen_s( &conout, "conout$", "w", stderr );
	freopen_s( &conin, "conin$", "r", stdin );

	if ( !title.empty( ) && title != "" )
		SetConsoleTitle( title.c_str( ) );
}

uintptr_t Utilities::FindPattern( const std::string& module, unsigned char* pattern )
{
	MODULEINFO modInfo;
	if ( !GetModuleInformation( GetCurrentProcess( ), GetModuleHandle( module.c_str( ) ), &modInfo, sizeof( MODULEINFO ) ) )
		return 0;

	auto code = static_cast< unsigned char* >( modInfo.lpBaseOfDll );
	size_t searchSize = modInfo.SizeOfImage;

	auto patt_len = strlen( reinterpret_cast< char* >( pattern ) );

	if ( pattern [ 0 ] == ' ' )
		return 0;

	for ( size_t i = 0; i < searchSize; i++ )
	{
		for ( size_t j = 0, k = 0; j < patt_len && ( i + k < searchSize ); k++ )
		{
			unsigned char tempChar [ 3 ];
			memset( tempChar, 0, sizeof( tempChar ) );

			if ( pattern [ j ] == static_cast< unsigned char >( '?' ) )
			{
				j += 2;
				continue;
			}

			sprintf_s( reinterpret_cast< char* >( tempChar ), sizeof( tempChar ), "%02X", code [ ( i + k ) ] );

			if ( tempChar [ 0 ] != pattern [ j ] || tempChar [ 1 ] != pattern [ j + 1 ] )
				break;

			j += 3;

			if ( j > ( patt_len - 2 ) )
			{
				auto return_ = reinterpret_cast< uintptr_t >( code + i );
				return return_;
			}
		}
	}

	return 0;
}

uintptr_t Utilities::FindPattern_( const std::string& module, byte* bMask, const char* szMask )
{
	auto DataCompare = [] ( const byte* pData, const byte* mask, const char* cmask, byte chLast, size_t iEnd ) -> bool
	{
		if ( pData [ iEnd ] != chLast ) return false;
		for ( ; *cmask; ++cmask, ++pData, ++mask )
		{
			if ( *cmask == 'x' && *pData != *mask )
			{
				return false;
			}
		}

		return true;
	};

	MODULEINFO modInfo;
	if ( !GetModuleInformation( GetCurrentProcess( ), GetModuleHandle( module.c_str( ) ), &modInfo, sizeof( MODULEINFO ) ) )
		return 0;

	byte* pBaseAddress = static_cast< byte* >( modInfo.lpBaseOfDll );
	size_t nLength = modInfo.SizeOfImage;


	auto iEnd = strlen( szMask ) - 1;
	auto chLast = bMask [ iEnd ];

	auto* pEnd = pBaseAddress + nLength - strlen( szMask );
	for ( ; pBaseAddress < pEnd; ++pBaseAddress )
	{
		if ( DataCompare( pBaseAddress, bMask, szMask, chLast, iEnd ) )
		{
			return reinterpret_cast< uintptr_t >( pBaseAddress );
		}
	}

	return 0;
}

Hook::Hook( void* class_ )
{
	m_class = ( void*** ) class_;
	m_originalTable = *( void*** ) class_;
	m_functions = GetFunctionCount( );

	m_shadowTable = new void* [ m_functions ];
	memcpy_s( m_shadowTable, sizeof( void* ) * m_functions, m_originalTable, sizeof( void* ) * m_functions );

	*( void*** ) class_ = m_shadowTable;
}

Hook::~Hook( )
{
	UnhookTable( );
}

void Hook::UnhookTable( )
{
	*( void*** ) m_class = m_originalTable;

	if ( m_shadowTable )
	{
		delete m_shadowTable;
		m_shadowTable = nullptr;
	}
}

int Hook::GetFunctionCount( )
{
	int counter = 0;
	while ( m_originalTable [ counter ] )
		counter++;

	return counter;
}
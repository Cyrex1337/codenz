#pragma once

#include <Windows.h>
#include <iostream>
#include <string>
#include <cstdint>
#include <Psapi.h>

#include <memory>

namespace Utilities
{
	extern void CreateConsole( const std::string& title = "" );
	
	// "45 43 45 55 33 9A FA ? ? ? ? 45 68 21"-like patterns
	extern uintptr_t FindPattern( const std::string& module, unsigned char* pattern );

	// "\x45\x43\x45\x55\x33\x9a\xfa\x00\x00\x00\x00\x45\x68\x21"-like patterns
	extern uintptr_t FindPattern_( const std::string& module, byte* bMask, const char* szMask );
}

class Hook
{
public:
	Hook( void* class_ );
	~Hook( );

	template <typename T>
	T HookPtr( int index, void* new_ )
	{
		m_shadowTable [ index ] = new_;
		return ( T ) m_originalTable [ index ];
	}

	template <typename T>
	T Unhook( int index )
	{
		m_shadowTable [ index ] = m_originalTable [ index ];
		return ( T ) m_originalTable [ index ];
	}

	void UnhookTable( );

private:
	int GetFunctionCount( );
	void** m_shadowTable;
	void*** m_class;
	void** m_originalTable;
	int m_functions;
};

extern std::shared_ptr<Hook> g_pPanelHook;
#pragma once

#include <Windows.h>
#include "XOR.hpp"
#include "SDK.hpp"

using tCreateInterface = void*( *)( const char*, int* );

namespace Interfaces
{
	template <typename T>
	extern T* Grab( const char* module, const char* szinterface );

	template <typename T>
	extern T* BruteForce( const char* module, const char* szinterface );

	void Setup( );

	extern IBaseClientDLL* Client;
	extern IPlayerInfoManager* PlayerInfoManager;
	extern IGlobalVarsBase* Globals;
	extern ISurface* Surface;
	extern IVPanel* Panel;
}
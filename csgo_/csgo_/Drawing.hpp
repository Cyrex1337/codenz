#pragma once

#include "SDK.hpp"
#include <memory>

class Drawing
{
	public:
		Drawing( );
		~Drawing( );

		void	DrawString( int x, int y, Color clrColor, bool bCenter, const char* szText, ... );
	private:
		HFont	hFont;
};

extern std::shared_ptr<Drawing> g_pDrawing;
#include "Drawing.hpp"
#include "InterfaceManager.hpp"

std::shared_ptr<Drawing> g_pDrawing;

Drawing::Drawing( )
{
	hFont = Interfaces::Surface->Create_Font( );
	if ( !hFont )
		return;

	Interfaces::Surface->SetFontGlyphSet( hFont, "Tahoma", 12, 400, 0, 0, FONTFLAG_OUTLINE );
}

Drawing::~Drawing( )
{
	hFont = 0;
}

void Drawing::DrawString( int x, int y, Color clrColor, bool bCenter, const char * szText, ... )
{
	if ( !szText )
		return;

	va_list va_alist = nullptr;
	char szBuffer [ 256 ] = { '\0' };
	wchar_t szString [ 128 ] = { '\0' };

	va_start( va_alist, szText );
	vsprintf_s( szBuffer, szText, va_alist );
	va_end( va_alist );

	MultiByteToWideChar( CP_UTF8, 0, szBuffer, -1, szString, 128 );

	if ( bCenter )
	{
		int Wide = 0, Tall = 0;

		Interfaces::Surface->GetTextSize( hFont, szString, Wide, Tall );

		x -= Wide / 2;
	}

	Interfaces::Surface->DrawSetTextPos( x, y );
	Interfaces::Surface->DrawSetTextFont( hFont );
	Interfaces::Surface->DrawSetTextColor( clrColor );
	Interfaces::Surface->DrawPrintText( szString, strlen( szBuffer ) );
}
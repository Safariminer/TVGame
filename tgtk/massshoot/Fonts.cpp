#include "Fonts.h"
#include <raylib.h>

Font titleFont;
Font textFont;
Font smallFont;

LOADING_FUNC MassShoot::Fonts::LoadFonts()
{
	titleFont = LoadFontEx("massshoot/fonts/title.ttf", 300, 0, 360);
	textFont = LoadFontEx("massshoot/fonts/text.ttf", 300, 0, 360);
	smallFont = LoadFontEx("massshoot/fonts/small.ttf", 300, 0, 360);
}

#include "Fonts.h"
#include <raylib.h>

Font titleFont;
Font textFont;
Font smallFont;

LOADING_FUNC MassShoot::Fonts::LoadFonts()
{
	titleFont = LoadFontEx("massshoot/fonts/title.otf", 300, 0, 360);
	textFont = LoadFontEx("massshoot/fonts/text.otf", 300, 0, 360);
	smallFont = LoadFontEx("massshoot/fonts/small.otf", 300, 0, 360);
}

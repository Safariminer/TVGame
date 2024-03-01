#pragma once
#include <raylib.h>
#include "defines.h"

extern Font titleFont;
extern Font textFont;
extern Font smallFont;

namespace MassShoot {
	namespace Fonts {
		LOADING_FUNC LoadFonts();
	}
}
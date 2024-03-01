#pragma once
#include "defines.h"
#include <iostream>

namespace MassShoot {
	namespace Engine {
		VITAL_FUNC Puts(std::string text);
		VITAL_FUNC Log(int msgType, const char* text, va_list args);
		VITAL_FUNC InitEngine(int width, int height, int fps, std::string title);
		VITAL_FUNC Console();
	}
}
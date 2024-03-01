#pragma once
#include "defines.h"
#include <iostream>

namespace MassShoot {
	namespace Skybox{
		LOADING_FUNC LoadSkybox(std::string filePath);
		RENDERING_FUNC DrawSkybox();
	}
}
#pragma once

#include "defines.h"
#include <iostream>

namespace MassShoot {
	namespace Weapons {
		class WeaponUpgrade {
		public:
			std::string name;
			std::string script;
			int keyID;
		};
		class Weapon {
		public:
			std::string modelPath;
			const char* texturePath;
			int ammo;
			int maxAmmo;
			virtual INPUT_FUNC Shoot() = 0;
		};
		LOADING_FUNC InitModel(Weapon* weapon);
		LOADING_FUNC InitWeaponCamera();
		RENDERING_FUNC DrawWeapon(Weapon* weapon, int offset = 0);
	}
}
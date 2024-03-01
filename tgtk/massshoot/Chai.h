#pragma once
#include "Weapons.h"
#include <vector>
#include "defines.h"

namespace MassShoot {
	namespace Scripting {
		SCRIPTING_FUNC InitChai();
		SCRIPTING_FUNC ChaiEval(std::string code);
	}
	namespace Weapons {
		class WeaponSuite {
		public:
			std::vector<MassShoot::Weapons::WeaponUpgrade> upgrades;
			SCRIPTING_FUNC CheckForUpgradeTriggers();
			SCRIPTING_FUNC Execute(int upgradeId);
		};
	}
}
#include "Chai.h"
#include <chaiscript/chaiscript.hpp>
#include "Engine.h"
#include "chaiLoadMap.h"

#include "unraylibedExterns.h"



void Chai_SetAmmo(int ammo) {
	AmmoSetter = ammo;
}

int Chai_GetAmmo() {
	return AmmoGetter;
}

void Chai_Puts(std::string text) {
	MassShoot::Engine::Puts(text);
}

void Chai_Log(std::string text) {
	va_list args = nullptr;
	MassShoot::Engine::Log(0, text.c_str(), args);
}


chaiscript::ChaiScript chai;


SCRIPTING_FUNC MassShoot::Scripting::InitChai()
{
	chai.add(chaiscript::fun(&Chai_SetAmmo), "MassShoot_SetAmmo");
	chai.add(chaiscript::fun(&Chai_GetAmmo), "MassShoot_GetAmmo");
	chai.add(chaiscript::fun(&Chai_Log), "MassShoot_Log");
	chai.add(chaiscript::fun(&Chai_Puts), "puts");
	chai.add(chaiscript::fun(&Chai_LoadMap), "MassShoot_LoadMap");

}

SCRIPTING_FUNC MassShoot::Scripting::ChaiEval(std::string code)
{
	chai.eval(code);
}

SCRIPTING_FUNC MassShoot::Weapons::WeaponSuite::Execute(int upgradeId)
{
	chai.eval(upgrades.at(upgradeId).script);
}
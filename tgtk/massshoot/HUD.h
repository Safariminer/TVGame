#pragma once
#include "defines.h"
#include "UI.h"

class HeadsUpDisplay : public MassShoot::UI::UIPanel {
public:
	int ammo, maxAmmo;
	UI_FUNC Update();
};
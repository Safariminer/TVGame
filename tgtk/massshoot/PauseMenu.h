#pragma once
#include "UI.h"
#include "defines.h"
#include <iostream>

class PauseMenuTitle : public MassShoot::UI::UIElement {
public:
	UI_FUNC Update();
};

class PauseMenuMapName : public MassShoot::UI::UIElement {
public:
	std::string mapName;
	UI_FUNC Update();
};

class PauseMenu : public MassShoot::UI::UIPanel {
public:
	UI_FUNC Update();
};
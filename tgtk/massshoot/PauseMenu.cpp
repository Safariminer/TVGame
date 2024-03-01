#include "PauseMenu.h"
#include <raylib.h>
#include "defines.h"
#include "Map.h"
#include "Fonts.h"
#include "ExternSuite.h"

UI_FUNC PauseMenuTitle::Update()
{
	DrawTextEx(titleFont, GAMENAME, { 12, 12 }, 50, 0, WHITE);
}
MassShoot::Maps::Map newMap;
UI_FUNC PauseMenuMapName::Update()
{
	DrawTextEx(smallFont, "massshoot/tgtk - massshoot > cocaineondrugs > headdisease", {10, (float)GetScreenHeight() - 32}, 30, 0, WHITE);
}

UI_FUNC PauseMenu::Update()
{
	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), { 50, 50, 50, 150 });
	for (int i = 0; i < suite.upgrades.size(); i++) {
		DrawTextEx(textFont, suite.upgrades.at(i).name.c_str(), { 10, (float)i * 20 + 70 }, 20, 0, WHITE);
	}
}



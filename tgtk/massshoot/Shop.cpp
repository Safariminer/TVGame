#include "Shop.h"
#include <raylib.h>
#include "Fonts.h"
#include <raymath.h>
int damageboosts;

UI_FUNC Shop::FrameFunc()
{
	DrawRectangle(50, 50, GetScreenWidth() - 100, GetScreenHeight() - 100, RAYWHITE);
	DrawTextEx(titleFont, "BIGASSFOREST SHOP", { 50, 50 }, 40, 0, BLACK);
	DrawRectangle(52, 102, GetScreenWidth() - 104, GetScreenHeight() - 154, GRAY);
	DrawTextEx(textFont, "Health up (10$)", { 54, 104 }, 30, 0, BLACK);
	DrawTextEx(textFont, "Damage up (20$)", { 54, 134 }, 30, 0, BLACK);
	DrawTextEx(textFont, TextFormat("Money: %i", money), { 54, 134 }, 30, 0, BLACK);
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		Vector2 mousePos = GetMousePosition();
		if (mousePos.x > 54 && mousePos.y > 104 && mousePos.x < GetScreenWidth() - 54 && mousePos.y < 164) {
			if (mousePos.y < 134 && money >= 10) {
				playerHealth += 25;
				money -= 10;
			}
			else if(money >= 20) {
				damageboosts += 1;
				money -= 20;
			}
		}
	}
}

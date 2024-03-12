#include <iostream>
#include <raylib.h>
#include "defines.h"
#include "Skybox.h"
#include "Camera.h"
#include "Engine.h"
#include "UI.h"
#include "PauseMenu.h"
#include "Weapons.h"
#include "Fonts.h"
#include "HUD.h"
#include "Map.h"
#include "Chai.h"
#include "ExternSuite.h"
#include <fstream>
#include <string>
#include "titles.h"
#include "ExternMap.h"
#include <filesystem>
#include "Shop.h"
#include "Ghost.h"


int menuEnum = 0; // 0 = warning; 1 = mainmenu; 2 = game;
MassShoot::Maps::Map map;
MassShoot::Weapons::WeaponSuite suite;

Sound gunshot;

int AmmoSetter = -1;

int AmmoGetter = 0;

int countdown = 0;
class MKInfinite : public MassShoot::Weapons::Weapon{
	
public:
	INPUT_FUNC Shoot() {
		if(ammo > 0 && countdown == 0){
			ammo--;
			countdown = 5;
			PlaySound(gunshot);
		}
		// SetWindowTitle(TextFormat("BULLETS: %i/%i", ammo, maxAmmo));
	}
};

int monitor = 0;

bool unlockResolution = false;
bool windowed = true;
bool isNight = false;
int nights = 0;
int playerHealth = 100;
bool debugMenu = false;
int main(int argc, char** argv) {
	monitor = GetCurrentMonitor();

	MassShoot::Scripting::InitChai();
	
	
	bool isShopOpen = false;
	if (!std::filesystem::exists("highscore.tgtk")) {
		std::ofstream hsfile("highscore.tgtk");
		hsfile << "0";
		hsfile.close();
	}
	int highscore;
	{
		std::ifstream hsfile("highscore.tgtk");
		hsfile >> highscore;
		hsfile.close();
	}
	// suite.upgrades.push_back({ "Reload", "MassShoot_SetAmmo(100);", KEY_Q });

	MassShoot::Engine::InitEngine(640, 480, 60, GAMENAME);

	TVGame::GhostHandler ghostHandler;
	// ghostHandler.SpawnGhosts(100);

	// MassShoot::Skybox::LoadSkybox("massshoot/textures/skybox.png");
	MassShoot::Camera::InitCamera();
	// MassShoot::Camera::LockCamera();
	Shop shopDisplay;
	MassShoot::Fonts::LoadFonts();
	Texture2D startup = LoadTexture("massshoot/menus/startup.png");
	Texture2D logo = LoadTexture("massshoot/menus/logo.png");
	Texture2D windowedIcon = LoadTexture("massshoot/menus/windowed.png");
	Texture2D unwindowedIcon = LoadTexture("massshoot/menus/unwindowed.png");
	Texture2D townLocator = LoadTexture("massshoot/menus/townlocator.png");
	gunshot = LoadSound("massshoot/sounds/smg.wav");

	PauseMenu pauseMenu;
	PauseMenuTitle pauseMenuTitle;
	PauseMenuMapName pauseMenuMapName;
	pauseMenu.elements.push_back(&pauseMenuTitle);
	pauseMenu.elements.push_back(&pauseMenuMapName);

	HeadsUpDisplay headsUpDisplay;

	bool isPaused = false;

	// Music music = LoadMusicStream("massshoot/music/Motherlode.mp3");
	Music splashMusic = LoadMusicStream("massshoot/music/splash.mp3");
	Music mainMenuMusic = LoadMusicStream("massshoot/music/mainmenu.mp3");
	Sound startGame = LoadSound("massshoot/music/startgame.mp3");
	MassShoot::Weapons::InitWeaponCamera();
	
	Music footsteps = LoadMusicStream("massshoot/sounds/ilikefeet.mp3");
	Music vacuum = LoadMusicStream("massshoot/sounds/vacuum.wav");

	PlayMusicStream(splashMusic);
	
	
	// PlayMusicStream(music);
	
	std::ifstream firstMapData("massshoot/firstmap.txt");
	std::string firstmap;
	while (std::getline(firstMapData, firstmap)) {
		// do nothing
	}
	firstMapData.close();

	RenderTexture2D rttarget = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

	Shader bloom = LoadShader(0, "massshoot/shaders/bloom.fs");

	while (!WindowShouldClose())
	{
		
		SetExitKey(0);
		// map.UpdateLighting(MassShoot::Camera::GetCameraPosition());

		if (IsKeyDown(KEY_LEFT_ALT)) {
			if (IsKeyPressed(KEY_ENTER)) {
				windowed = !windowed;
			}
		}

		if (!unlockResolution && windowed) {
			if (GetScreenWidth() != 640 || GetScreenHeight() != 480) SetWindowSize(640, 480);
		}
		if (!windowed) {
			if (GetScreenWidth() != GetMonitorWidth(GetCurrentMonitor()) || GetScreenHeight() != GetMonitorHeight(GetCurrentMonitor())) {
				SetWindowSize(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
				SetWindowPosition(GetMonitorPosition(GetCurrentMonitor()).x, GetMonitorPosition(GetCurrentMonitor()).y);
			}
		}

		switch(menuEnum){
		case 0:
			UpdateMusicStream(splashMusic);
			BeginDrawing();

			ClearBackground(BLACK);
			// DrawTextEx(titleFont, GAMENAME, {(float)GetScreenWidth()/2-175, (float)GetScreenHeight()/2-200}, 40, 0, WHITE);
			// DrawTextEx(textFont, "This game is currently in development. Gameplay elements are subject\nto change.\n\nPress Enter to continue.", { (float)GetScreenWidth() / 2 - 300, (float)GetScreenHeight() / 2 - 100 }, 20, 0, WHITE);
			DrawTexture(startup, 0, 0, WHITE);
			if(IsKeyPressed(KEY_ENTER)){
				map.LoadMap("background");
				menuEnum = 1;
				StopMusicStream(splashMusic);
				PlayMusicStream(mainMenuMusic);
				
			}
			break;
		case 1:
			// BeginTextureMode(rttarget);
			BeginDrawing();
			MassShoot::Camera::StartCameraFrame();
			// MassShoot::Skybox::DrawSkybox();
			// ClearBackground({ 102, 146, 212, 255 });
			ClearBackground(DARKBLUE);
			// DrawSphere({ 500, -1000, 500 }, 1000, DARKGREEN);
			// map.RenderBoxes();
			MassShoot::Camera::StopCameraFrame();
			// EndTextureMode();
			DrawCircle(GetScreenWidth() / 2, GetScreenHeight(), GetScreenHeight() / 1.5, DARKGREEN);
			DrawTextureEx(townLocator, { (float)GetScreenWidth() / 5, (float)GetScreenHeight() / 2 }, 0, 0.5f, WHITE);
			DrawTextEx(textFont, "Play Game", { (float)GetScreenWidth() / 5 + 100, (float)GetScreenHeight() / 2 }, 20, 0, WHITE);
			DrawTextureEx(townLocator, { (float)GetScreenWidth() - ((float)GetScreenWidth() / 5) *2, (float)GetScreenHeight() / 2 }, 0, 0.5f, WHITE);
			DrawTextEx(textFont, "Quit Game", { (float)GetScreenWidth() - ((float)GetScreenWidth() / 5) * 2 + 100, (float)GetScreenHeight() / 2 - 10}, 20, 0, WHITE);
			
			if (windowed) {
				DrawRectangle(0, 400, 640, 80, WHITE);
				DrawTextureEx(logo, {460, 400}, 0, 0.7, WHITE);
				DrawRectangle(0, 400, 460, 80, BLUE);
				DrawTextEx(textFont, TextFormat("High score: %i nights", highscore), { 12, 432 }, 20, 0, DARKBLUE);
				DrawTextEx(textFont, TextFormat("High score: %i nights", highscore), { 10, 430 }, 20, 0, WHITE);
			}
			else {
				DrawRectangle(0, GetScreenHeight() - 80, GetScreenWidth(), 80, WHITE);
				DrawTextureEx(logo, { (float)GetScreenWidth()-180, (float)GetScreenHeight()-80}, 0, 0.7, WHITE);
				DrawRectangle(0, 0, 180, GetScreenHeight(), BLUE);
				DrawRectangle(0, GetScreenHeight()-80, GetScreenWidth()-180, 80, BLUE);
				DrawTextEx(textFont, TextFormat("High score: %i nights", highscore), { 12, (float)GetScreenHeight() - 48}, 20, 0, DARKBLUE);
				DrawTextEx(textFont, TextFormat("High score: %i nights", highscore), { 10, (float)GetScreenHeight() - 50}, 20, 0, WHITE);
			}
			
			
			UpdateMusicStream(mainMenuMusic);
			// BeginShaderMode(bloom);
			// DrawTextureRec(rttarget.texture, { 0, 0, (float)rttarget.texture.width, -(float)rttarget.texture.height },  { 0, 0 }, WHITE);
			// EndShaderMode();
			// DrawTextEx(titleFont, GAMENAME, { 10, 10 }, 80, 0, BLACK);
			// DrawTextEx(textFont, TextFormat("Press Enter to continue...\nHigh Score: %i nights", highscore), { 10, 100 }, 20, 0, BLACK);
			// DrawRectangle(GetScreenWidth() - 10, GetScreenHeight() - 10, 10, 10, (windowed) ? GREEN : RED);
			/*if (IsKeyPressed(KEY_ENTER)) {
				menuEnum = 2;
				MassShoot::Camera::InitCamera();
				suite.upgrades.clear();
				map.LoadMap(firstmap);
				StopMusicStream(mainMenuMusic);
				MassShoot::Camera::LockCamera();
			}*/
			DrawTexture((windowed) ? windowedIcon : unwindowedIcon, GetScreenWidth() - 32, 0, WHITE);
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				if (GetMouseX() > GetScreenWidth() - 32 && GetMouseY() < 32) windowed = !windowed;
				if (GetMouseX() > GetScreenWidth() / 5 && GetMouseX() < GetScreenWidth() / 5 + 128 &&
					GetMouseY() > (float)GetScreenHeight() / 2 && GetMouseY() < (float)GetScreenHeight() / 2 + 128) {
					menuEnum = 2;
					MassShoot::Camera::InitCamera();
					suite.upgrades.clear();
					map.LoadMap(firstmap);
					StopMusicStream(mainMenuMusic);
					PlaySound(startGame);
					MassShoot::Camera::LockCamera();
				}
				if (GetMouseX() > (float)GetScreenWidth() - ((float)GetScreenWidth() / 5) * 2 && GetMouseX() < (float)GetScreenWidth() - ((float)GetScreenWidth() / 5) * 2 + 128 &&
					GetMouseY() > (float)GetScreenHeight() / 2 - 10 && GetMouseY() < (float)GetScreenHeight() / 2 + 118) {
					return 0;
				}
			}
			break;
		case 2:
			if (IsKeyPressed(KEY_R)) {
				MassShoot::Camera::InitCamera();
				suite.upgrades.clear();
				map.LoadMap(firstmap);
			}
			// UpdateMusicStream(music);
			headsUpDisplay.ammo = 0;
			headsUpDisplay.maxAmmo = 0;
#pragma region
			if (IsKeyPressed(KEY_ESCAPE)) {
				switch (isPaused) {
				case false:
					isPaused = true;
					MassShoot::Camera::UnlockCamera();
					break;
				case true:
					isPaused = false;
					MassShoot::Camera::LockCamera();
					break;
				}

			}
#pragma endregion Pause menu input
#pragma region
			if (map.shaded) BeginTextureMode(rttarget);
			else BeginDrawing();
			MassShoot::Camera::StartCameraFrame();
			ClearBackground({ 102, 146, 212, 255 });
			if(!map.shaded) MassShoot::Skybox::DrawSkybox();
			
			// DrawGrid(10, 1.0f);
			//                                     bhop        new ground collider
			MassShoot::Camera::CameraMovement(map, false, true, true);
			//                                           advanced physics

			map.RenderBoxes();
			map.RenderBillboards();
			
			// DrawRay(GetMouseRay({ (float)GetScreenWidth()/2, (float)GetScreenHeight()/2}, MassShoot::Camera::GetCamera()), RED);
			
			/*if (IsKeyDown(KEY_LEFT_CONTROL)) {
				if (IsKeyPressed(KEY_O)) isShopOpen = true;
				if (IsKeyPressed(KEY_EQUAL)) nights++;
				if (IsKeyPressed(KEY_MINUS)) nights--;
				if (IsKeyPressed(KEY_P)) {
					int option, option2;
					std::cout << "What do you want to do? Skip to night(1), spawn ghosts(2), kill all ghosts(3) >";
					std::cin >> option;
					switch (option) {
					case 1:
						std::cout << "Which night? >";
						std::cin >> option2;
						nights = option2;
						break;
					case 2:
						std::cout << "How many ghosts? >";
						std::cin >> option2;
						ghostHandler.SpawnGhosts(option2);
						break;
					case 3:
						std::cout << "GHOSTINATOR 99000-HX deployed <3\n";
						ghostHandler.KillAllGhosts();
					default:
						std::cout << "Invalid option.\n";
						break;
					}
				}
			}*/

			//if (!isNight) {
			//	if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_L)) {
			//		isNight = true;
			//		MassShoot::Skybox::LoadSkybox("massshoot/textures/skybox3.png");
			//		nights++;
			//		ghostHandler.SpawnGhosts(5 * nights * nights);
			//		// playerHealth = 100;
			//	}
			//}

			ghostHandler.GhostsUpdate(MassShoot::Camera::GetCameraPosition());
			// playerHealth -= ghostHandler.CheckForGhostCollision(MassShoot::Camera::GetCollider());
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
				ghostHandler.CheckForHarmedGhosts(GetMouseRay({ (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 }, MassShoot::Camera::GetCamera()));
			}
			if (ghostHandler.GetGhostCount() == 0 && isNight) {
				MassShoot::Skybox::LoadSkybox("massshoot/textures/skybox2.png");
				isNight = false;
			}
			if (playerHealth <= 0) {
				menuEnum = 3;
			}
			
			MassShoot::Camera::StopCameraFrame();
			if (map.shaded) {
				EndTextureMode();
				BeginShaderMode(bloom);
				DrawTextureRec(rttarget.texture, { 0, 0, (float)rttarget.texture.width, -(float)rttarget.texture.height }, { 0, 0 }, WHITE);
				EndShaderMode();
			}

#pragma endregion Map Rendering
			
			// MassShoot::Weapons::DrawWeapon(&mkinfinite, countdown);
			map.CheckForTriggers(MassShoot::Camera::GetCollider());
			/*suite.CheckForUpgradeTriggers();
			for (int i = 0; i < suite.upgrades.size(); i++) {
				if(IsKeyPressed(suite.upgrades.at(i).keyID))
					suite.Execute(i);
			}
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !isPaused) {
				mkinfinite.Shoot();
			}*/
			if (isNight) {
				DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {30, 30, 60, 100});
			}
			if (isShopOpen) {
				shopDisplay.FrameFunc();
				if (IsKeyPressed(KEY_ESCAPE)) isShopOpen = false;
			}
			else{
				if (isPaused) {
					pauseMenu.FrameFunc();
				}
				if (!isPaused) { 
					headsUpDisplay.FrameFunc();
				}
			}

			if (ghostHandler.GetGhostCount() > 0) {

				DrawTextEx(textFont, TextFormat("Ghosts left: %i\nNight: %i\nHealth: %i\nMoney: %i", ghostHandler.GetGhostCount(), nights, playerHealth, money), { 10, 10 }, 30, 0, BLACK);
				DrawTextEx(textFont, TextFormat("Ghosts left: %i\nNight: %i\nHealth: %i\nMoney: %i", ghostHandler.GetGhostCount(), nights, playerHealth, money), {0, 0}, 30, 0, WHITE);
			}
			else{
				DrawTextEx(textFont, TextFormat("Health: %i\nMoney: %i", playerHealth, money), { 10, 10 }, 30, 0, BLACK);
				DrawTextEx(textFont, TextFormat("Health: %i\nMoney: %i", playerHealth, money), { 0, 0 }, 30, 0, WHITE);
			}
			MassShoot::StoryTools::DisplayTitle(map.title, map.subtitle);
			DrawText("+", (GetScreenWidth() / 2) - 3, (GetScreenHeight() / 2) - 3, 6, WHITE);
			Vector3 camPos = MassShoot::Camera::GetCameraPosition();
			if (camPos.x > 17.5 && camPos.x < 35 && camPos.z > -49 && camPos.z < -38 && !isNight) {
				DrawTextEx(textFont, "Press E to start night...", { (float)GetScreenWidth() / 2 - MeasureTextEx(textFont, "Press E to start night...", 30, 0).x / 2 + 10, (float)GetScreenHeight() - 60 + 10}, 30, 0, BLACK);
				DrawTextEx(textFont, "Press E to start night...", { (float)GetScreenWidth() / 2 - MeasureTextEx(textFont, "Press E to start night...", 30, 0).x / 2, (float)GetScreenHeight() - 60 }, 30, 0, WHITE);
				if (IsKeyPressed(KEY_E)) {
					isNight = true;
					MassShoot::Skybox::LoadSkybox("massshoot/textures/skybox3.png");
					nights++;
					ghostHandler.SpawnGhosts(5 * nights * nights);
				}
			}
			if (camPos.x > 64 && camPos.x < 69 && camPos.z > -49 && camPos.z < -45 && !isNight) {
				DrawTextEx(textFont, "Go online...", { (float)GetScreenWidth() / 2 - MeasureTextEx(textFont, "Go online...", 30, 0).x / 2 + 10, (float)GetScreenHeight() - 60 + 10 }, 30, 0, BLACK);
				DrawTextEx(textFont, "Go online...", { (float)GetScreenWidth() / 2 - MeasureTextEx(textFont, "Go online...", 30, 0).x / 2, (float)GetScreenHeight() - 60 }, 30, 0, WHITE);
				if (IsKeyPressed(KEY_E)) {
					MassShoot::Camera::UnlockCamera();
					isShopOpen = true;
				}
			}
			printf("X: %f | Y: %f | Z: %f\n", camPos.x, camPos.y, camPos.z);

			DrawFPS(1000, 0);
			// MassShoot::Camera::GravityDisplay();
			
			break;
		case 3:
			BeginDrawing();
			ClearBackground({ 12, 37, 25, 255 });
			DrawTextEx(titleFont, "Hijacked.", { 12, 12 }, 72, 0, {100, 0, 0, 255});
			DrawTextEx(titleFont, "Hijacked.", { 10, 10 }, 72, 0, RED);
			DrawTextEx(textFont, TextFormat("Your score: %i\n\nPress ENTER to return to Main Menu...", nights), { 12, 102 }, 20, 0, {100,0,0,255});
			DrawTextEx(textFont, TextFormat("Your score: %i\n\nPress ENTER to return to Main Menu...", nights), { 10, 100 }, 20, 0, RED);
			if (nights > highscore) {
				DrawTextEx(textFont, "HIGH SCORE!", { 12, (float)GetScreenHeight() - 78 }, 70, 0, {100,0,0,255});
				DrawTextEx(textFont, "HIGH SCORE!", { 10, (float)GetScreenHeight() - 80 }, 70, 0, RED);
			}
			
			if (IsKeyPressed(KEY_ENTER)) {
				if(nights > highscore)
				{
					std::ofstream hsfile("highscore.tgtk");
					hsfile << nights;
					hsfile.close();
					highscore = nights;
					isNight = false;
					ghostHandler.KillAllGhosts();
				}
				MassShoot::Camera::InitCamera();
				menuEnum = 1;
				playerHealth = 100;
				nights = 0;
				MassShoot::Camera::UnlockCamera();
			}
			EndDrawing();
			
			break;
		}
		MassShoot::Engine::Console();
			EndDrawing();
			if (countdown > 0) countdown--;

			if (rttarget.texture.width != GetScreenWidth() || rttarget.texture.height != GetScreenHeight()) {
				UnloadRenderTexture(rttarget);
				rttarget = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
			}
	}
	CloseWindow();

}


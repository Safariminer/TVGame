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
int WinMain(int argc, char** argv) {
	monitor = GetCurrentMonitor();
	MassShoot::Scripting::InitChai();

	// suite.upgrades.push_back({ "Reload", "MassShoot_SetAmmo(100);", KEY_Q });

	MassShoot::Engine::InitEngine(640, 480, 60, GAMENAME);

	TVGame::GhostHandler ghostHandler;
	// ghostHandler.SpawnGhosts(100);

	// MassShoot::Skybox::LoadSkybox("massshoot/textures/skybox.png");
	MassShoot::Camera::InitCamera();
	// MassShoot::Camera::LockCamera();

	MassShoot::Fonts::LoadFonts();
	Texture2D startup = LoadTexture("massshoot/menus/startup.png");
	gunshot = LoadSound("massshoot/sounds/smg.wav");

	PauseMenu pauseMenu;
	PauseMenuTitle pauseMenuTitle;
	PauseMenuMapName pauseMenuMapName;
	pauseMenu.elements.push_back(&pauseMenuTitle);
	pauseMenu.elements.push_back(&pauseMenuMapName);

	HeadsUpDisplay headsUpDisplay;

	bool isPaused = false;

	// Music music = LoadMusicStream("massshoot/music/Motherlode.mp3");
	Music mainMenuMusic = LoadMusicStream("massshoot/music/mainmenu.wav");
	MassShoot::Weapons::InitWeaponCamera();
	
	
	
	
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
			BeginDrawing();

			ClearBackground(BLACK);
			// DrawTextEx(titleFont, GAMENAME, {(float)GetScreenWidth()/2-175, (float)GetScreenHeight()/2-200}, 40, 0, WHITE);
			// DrawTextEx(textFont, "This game is currently in development. Gameplay elements are subject\nto change.\n\nPress Enter to continue.", { (float)GetScreenWidth() / 2 - 300, (float)GetScreenHeight() / 2 - 100 }, 20, 0, WHITE);
			DrawTexture(startup, 0, 0, WHITE);
			if(IsKeyPressed(KEY_ENTER)){
				map.LoadMap("background");
				menuEnum = 1;
				PlayMusicStream(mainMenuMusic);
			}
			break;
		case 1:
			BeginTextureMode(rttarget);
			
			MassShoot::Camera::StartCameraFrame();
			MassShoot::Skybox::DrawSkybox();
			ClearBackground({ 102, 146, 212, 255 });
			map.RenderBoxes();
			MassShoot::Camera::StopCameraFrame();
			EndTextureMode();


			BeginDrawing();
			ClearBackground(DARKBLUE);
			UpdateMusicStream(mainMenuMusic);
			// BeginShaderMode(bloom);
			// DrawTextureRec(rttarget.texture, { 0, 0, (float)rttarget.texture.width, -(float)rttarget.texture.height },  { 0, 0 }, WHITE);
			// EndShaderMode();
			DrawTextEx(titleFont, GAMENAME, { 10, 10 }, 80, 0, BLACK);
			DrawTextEx(textFont, "Press Enter to continue...", { 10, 100 }, 20, 0, BLACK);
			DrawRectangle(GetScreenWidth() - 10, GetScreenHeight() - 10, 10, 10, (windowed) ? GREEN : RED);
			if (IsKeyPressed(KEY_ENTER)) {
				menuEnum = 2;
				MassShoot::Camera::InitCamera();
				suite.upgrades.clear();
				map.LoadMap(firstmap);
				StopMusicStream(mainMenuMusic);
				MassShoot::Camera::LockCamera();
			}
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				if (GetMouseX() > GetScreenWidth() - 10 && GetMouseY() > GetScreenHeight() - 10) windowed = !windowed;
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
			
			if (!isNight) {
				if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_L)) {
					isNight = true;
					MassShoot::Skybox::LoadSkybox("massshoot/textures/skybox3.png");
					nights++;
					ghostHandler.SpawnGhosts(10 * nights);
				}
			}
			
			if (isNight) {
				ghostHandler.GhostsUpdate(MassShoot::Camera::GetCameraPosition());
				if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
					ghostHandler.CheckForHarmedGhosts(GetMouseRay({ (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 }, MassShoot::Camera::GetCamera()));
				}
				if (ghostHandler.GetGhostCount() == 0) {
					MassShoot::Skybox::LoadSkybox("massshoot/textures/skybox2.png");
					isNight = false;
				}
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
			if (isPaused) {
				pauseMenu.FrameFunc();
			}
			if (!isPaused) { 
				headsUpDisplay.FrameFunc();
			}
			if (isNight) {

				DrawTextEx(textFont, TextFormat("Ghosts left: %i\nNight: %i", ghostHandler.GetGhostCount(), nights), { 10, 10 }, 50, 0, BLACK);
				DrawTextEx(textFont, TextFormat("Ghosts left: %i\nNight: %i", ghostHandler.GetGhostCount(), nights), {0, 0}, 50, 0, WHITE);
			}
			MassShoot::StoryTools::DisplayTitle(map.title, map.subtitle);

			// Vector3 camPos = MassShoot::Camera::GetCameraPosition();
			// printf("X: %f | Y: %f | Z: %f\n", camPos.x, camPos.y, camPos.z);

			// DrawFPS(200, 0);
			// MassShoot::Camera::GravityDisplay();
			
			break;
		case 3:
			BeginDrawing();
			ClearBackground({ 12, 37, 25, 255 });
			DrawTextEx(titleFont, "Hijacked.", { 12, 12 }, 72, 0, {100, 0, 0, 255});
			DrawTextEx(titleFont, "Hijacked.", { 10, 10 }, 72, 0, RED);
			DrawTextEx(textFont, "Press ENTER to return to Main Menu...", { 12, 102 }, 20, 0, {100,0,0,255});
			DrawTextEx(textFont, "Press ENTER to return to Main Menu...", { 10, 100 }, 20, 0, RED);
			if (IsKeyPressed(KEY_ENTER)) {
				menuEnum = 1;
				playerHealth = 100;
				nights = 0;
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


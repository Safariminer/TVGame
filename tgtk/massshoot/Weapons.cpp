#include "Weapons.h"
#include <raylib.h>
#include <vector>
#include <map>
#include <stdio.h>
#include "rlgl.h"
std::vector<Model> weaponModels;
std::vector<const char*> weaponPaths;
Camera weaponCamera;
LOADING_FUNC MassShoot::Weapons::InitWeaponCamera() {
	weaponCamera = { 0 };
	weaponCamera.position = { 0.0f, 1.0f, -1.0f };    // Camera position
	weaponCamera.target = { 0,1,0 };      // Camera looking at point
	weaponCamera.up = { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
	weaponCamera.fovy = 90.0f;                                // Camera field-of-view Y
	weaponCamera.projection = CAMERA_PERSPECTIVE;
}

LOADING_FUNC MassShoot::Weapons::InitModel(Weapon* weapon)
{
	Model newModel = LoadModel(weapon->modelPath.c_str());
	newModel.materials[0].maps->texture = LoadTexture(weapon->texturePath);
	weaponModels.push_back(newModel);
	weaponPaths.push_back(weapon->modelPath.c_str());
	// weaponModels.at(weaponModels.size() - 1)
}

RENDERING_FUNC MassShoot::Weapons::DrawWeapon(Weapon* weapon, int offset)
{
	bool found = false;
	for (int i = 0; i < weaponPaths.size(); i++) {
		if (strcmp(weaponPaths.at(i), weapon->modelPath.c_str()) == 0) {
			BeginMode3D(weaponCamera);
			DrawModel(weaponModels.at(i), { -0.4f,0.9f, -0.5f - (float)(0.01 * offset)}, 0.05f, WHITE);
			EndMode3D();
			found = true;
			break;
		}
	}
	if (!found) {
		printf("Couldn't find weapon. Did you initialize it?\n");
	}
}

#include "Map.h"
#include <raylib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "Skybox.h"
#include <raymath.h>
#include "rlgl.h"
#include <map>
#include "Fonts.h"
#include "Camera.h"
#include "Chai.h"
#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"
#include "titles.h"
#include "Engine.h"
#include <filesystem>
#include "cameraextern.h"
std::string currMapName;

void DrawCubeTexture(Texture2D texture, Vector3 position, float width, float height, float length, Color color); // Draw cube textured
void DrawCubeTextureRec(Texture2D texture, Rectangle source, Vector3 position, float width, float height, float length, Color color); // Draw cube with a region of a texture

std::map<std::string, Texture2D> mapTextures;
std::map<std::string, Texture2D> mapSprites;
Shader lightingShader;

std::string MassShoot::Maps::Map::GetMapName() {
	return currMapName;
}

std::vector<BoundingBox> upgradeTriggers;
std::vector<std::string> upgradeTriggersNames;

bool loadedSound = false;
Sound upgradeSound;

LOADING_FUNC MassShoot::Maps::Map::LoadMap(std::string mapName)
{
	if (!loadedSound) {
		upgradeSound = LoadSound("massshoot/sounds/upgrade.mp3");
		loadedSound = true;
	}
#ifdef LIGHTING_ENABLED
	lights.clear();
	if (lightingShader.id != NULL) UnloadShader(lightingShader);
	lightingShader = LoadShader("massshoot/shaders/lighting.vs",
		"massshoot/shaders/lighting.fs");
	// Get some required lightingShader locations
	lightingShader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(lightingShader, "viewPos");
	// NOTE: "matModel" location name is automatically assigned on lightingShader loading, 
	// no need to get the location again if using that uniform name
	//lightingShader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(lightingShader, "matModel");

	// Ambient light level (some basic lighting)
	int ambientLoc = GetShaderLocation(lightingShader, "ambient");
	float basicLight[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	SetShaderValue(lightingShader, ambientLoc, basicLight, SHADER_UNIFORM_VEC4);
	

	std::ifstream lightsfile("massshoot/maps/" + mapName + "/lighting.txt");
	int xlights, ylights, zlights, rlights, glights, blights, alights;
	while (lightsfile >> xlights >> ylights >> zlights >> rlights >> glights >> blights >> alights)
	{
		Light light = CreateLight(LIGHT_POINT, { (float)xlights, (float)ylights, (float)zlights}, Vector3Zero(), {(unsigned char)rlights, (unsigned char)glights, (unsigned char)blights, (unsigned char)alights}, lightingShader);
		lights.push_back(light);
	}
	lightsfile.close();
#endif
	std::ifstream textureData("massshoot/maps/" + mapName + "/textures.txt");
	
	std::string textureName, texturePath;
	collisions.clear();
	triggers.clear();
	triggerMaps.clear();
	upgradeTriggers.clear();
	textures.clear();
	sprites.clear();
	billboards.clear();
	// textures.clear();
	while (textureData >> textureName >> texturePath) {
		// MassShoot::Skybox::LoadSkybox(line);
		if (mapTextures.find(textureName) != mapTextures.end()) {
			UnloadTexture(mapTextures.at(textureName));
			mapTextures.erase(textureName);
		}
		mapTextures.insert_or_assign(textureName, LoadTexture(texturePath.c_str()));
	}
	textureData.close();

	std::ifstream spritesData("massshoot/maps/" + mapName + "/sprites.txt");
	// textures.clear();
	while (spritesData >> textureName >> texturePath) {
		// MassShoot::Skybox::LoadSkybox(line);
		if (mapSprites.find(textureName) != mapSprites.end()) {
			UnloadTexture(mapSprites.at(textureName));
			mapSprites.erase(textureName);
		}
		mapSprites.insert_or_assign(textureName, LoadTexture(texturePath.c_str()));
	}
	spritesData.close();

	std::ifstream mapfile("massshoot/maps/" + mapName + "/map.txt");
	float x1, y1, z1, x2, y2, z2;
	while (mapfile >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> textureName)
	{
		BoundingBox newBox;
		newBox.min = { (float)x1, (float)y1, (float)z1 };
		newBox.max = { (float)x2, (float)y2, (float)z2 };
		
		collisions.push_back(newBox);
		textures.push_back(textureName);
	}
	mapfile.close();

	std::ifstream upgradefiles("massshoot/maps/" + mapName + "/upgrades.txt");
	while (upgradefiles >> x1 >> y1 >> z1 >> textureName)
	{
		BoundingBox newBox;
		newBox.min = { (float)x1-1, (float)y1-1, (float)z1-1 };
		newBox.max = { (float)x1+1, (float)y1+1, (float)z1+1 };

		upgradeTriggers.push_back(newBox);
		upgradeTriggersNames.push_back(textureName);
	}
	upgradefiles.close();

	std::ifstream triggerData("massshoot/maps/" + mapName + "/triggers.txt");
	// float x1, y1, z1, x2, y2, z2;
	std::string mapTrigName;
	while (triggerData >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> mapTrigName)
	{
		BoundingBox newBox;
		newBox.min = { (float)x1, (float)y1, (float)z1 };
		newBox.max = { (float)x2, (float)y2, (float)z2 };

		triggers.push_back(newBox);
		triggerMaps.push_back(mapTrigName);
	}
	triggerData.close();

	std::ifstream skyboxData("massshoot/maps/" + mapName + "/skybox.txt");
	std::string line;
	while (std::getline(skyboxData, line)) {
		MassShoot::Skybox::LoadSkybox(line);
	}
	for (int i = 0; i < collisions.size(); i++) {
		printf("%f, %f, %f | %f, %f, %f\n",
			collisions.at(i).min.x,
			collisions.at(i).min.y,
			collisions.at(i).min.z,
			collisions.at(i).max.x,
			collisions.at(i).max.y,
			collisions.at(i).max.z
		);
	}
	std::string spritename;

	std::ifstream billboardsData("massshoot/maps/" + mapName + "/billboards.txt");
	// float x1, y1, z1, x2, y2, z2;
	
	while (billboardsData >> x1 >> y1 >> z1 >> spritename)
	{
		Vector3 newvec = { x1, y1, z1 };

		billboards.push_back(newvec);
		sprites.push_back(spritename);
	}
	triggerData.close();
	skyboxData.close();
	shaded = std::filesystem::exists("massshoot/maps/" + mapName + "/pleaseshademap");
	std::ifstream titlefile("massshoot/maps/" + mapName + "/title.txt");
	std::getline(titlefile, title);
	std::getline(titlefile, subtitle);
	titlefile.close();
	MassShoot::StoryTools::LaunchTitle(title, subtitle);
	MassShoot::Engine::Log(2, TextFormat("Title: %s\nSubtitle: %s\n\n", title.c_str(), subtitle.c_str()), nullptr);
	
}

LIGHTING_FUNC MassShoot::Maps::Map::UpdateLighting(Vector3 cameraPosition) {
#ifdef LIGHTING_ENABLED
	float cameraPos[3] = { cameraPosition.x, cameraPosition.y, cameraPosition.z };
	SetShaderValue(lightingShader, lightingShader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);

	// Update light values (actually, only enable/disable them)
	// for (int i = 0; i < MAX_LIGHTS; i++) UpdateLightValues(shader, lights[i]);
	for (int i = 0; i < lights.size(); i++) {
		UpdateLightValues(lightingShader, lights.at(i));
	}
#endif
}

RENDERING_FUNC MassShoot::Maps::Map::RenderBillboards() {
	for (int i = 0; i < billboards.size(); i++) {
		DrawBillboard(camera, mapSprites.at(sprites.at(i)), billboards.at(i), 10.0f, WHITE);
	}
}


RENDERING_FUNC MassShoot::Maps::Map::RenderBoxes()
{
	// BeginShaderMode(lightingShader);
	for (int i = 0; i < collisions.size(); i++) {
		// DrawBoundingBox(collisions.at(i), BLACK);
		Vector3 size = { 0 };

		size.x = fabsf(collisions.at(i).max.x - collisions.at(i).min.x);
		size.y = fabsf(collisions.at(i).max.y - collisions.at(i).min.y);
		size.z = fabsf(collisions.at(i).max.z - collisions.at(i).min.z);

		Vector3 center = { collisions.at(i).min.x + size.x / 2.0f, collisions.at(i).min.y + size.y / 2.0f, collisions.at(i).min.z + size.z / 2.0f };
		// int color = 255 / collisions.size() * i; {255, (unsigned char)color, 0, 255}
		
		DrawCubeTexture(mapTextures.at(textures.at(i)), center, size.x, size.y, size.z, WHITE);
		
		
	}

	for (int i = 0; i < upgradeTriggers.size(); i++) {
		DrawCube(upgradeTriggers.at(i).min, 1, 1, 1, PURPLE);
	}
   //  EndShaderMode();
#ifdef LIGHTING_ENABLED
	for (int l = 0; l < lights.size(); l++) {
		DrawSphere(lights.at(l).position, 2, lights.at(l).color);
	}
#endif
}

PHYSICS_BOOL MassShoot::Maps::Map::IsColliding(BoundingBox box)
{
	for (int i = 0; i < collisions.size(); i++) {
		if (CheckCollisionBoxes(collisions.at(i), box)) {
			// printf("%i\n", i);
			return true;
		}
	}
	return false;
}

BoundingBox GenerateBoundingBox(Vector3 center, int margin, int height) {
	BoundingBox box;
	box.min.x = center.x - (margin / 2);
	box.max.x = center.x + (margin / 2);
	box.min.y = center.y - (height / 2);
	box.max.y = center.y + (height / 2);
	box.min.z = center.z - (margin / 2);
	box.max.z = center.z + (margin / 2);
	return box;
}

MassShoot::Maps::CollideCollAdvancedResult MassShoot::Maps::Map::CollideCollAdvanced(Vector3 newbox, Vector3 oldbox, int margin, int height)
{
	MassShoot::Maps::CollideCollAdvancedResult result = { oldbox };

	if (!IsColliding(GenerateBoundingBox({ oldbox.x, oldbox.y, newbox.z }, margin, height))) result = { oldbox.x, oldbox.y, newbox.z };
	if (!IsColliding(GenerateBoundingBox({ oldbox.x, newbox.y, oldbox.z }, margin, height))) result = { oldbox.x, newbox.y, oldbox.z };
	if (!IsColliding(GenerateBoundingBox({ oldbox.x, newbox.y, newbox.z }, margin, height))) result = { oldbox.x, newbox.y, newbox.z };
	if (!IsColliding(GenerateBoundingBox({ newbox.x, oldbox.y, oldbox.z }, margin, height))) result = { newbox.x, oldbox.y, oldbox.z };
	if (!IsColliding(GenerateBoundingBox({ newbox.x, oldbox.y, newbox.z }, margin, height))) result = { newbox.x, oldbox.y, newbox.z };
	if (!IsColliding(GenerateBoundingBox({ newbox.x, newbox.y, oldbox.z }, margin, height))) result = { newbox.x, newbox.y, oldbox.z };

	return result;
}

PHYSICS_FUNC MassShoot::Maps::Map::CheckForTriggers(BoundingBox box)
{
	for (int i = 0; i < triggers.size(); i++) {
		if (CheckCollisionBoxes(triggers.at(i), box)) {
			DrawTextEx(textFont, TextFormat("PRESS E TO GO TO %s", triggerMaps.at(i).c_str()), {(float)GetScreenWidth()/2-180, (float)GetScreenHeight() / 2 +140}, 40, 0, WHITE);
			if (IsKeyPressed(KEY_E)) {
				DrawRectangle(0, 0, 300, 70, { 150,150,150,150 });
				MassShoot::Camera::InitCamera();
				std::cout << triggerMaps.at(i);
				DrawTextEx(smallFont, "LOADING...", { 0,0 }, 60, 0, WHITE);
				LoadMap(triggerMaps.at(i));
			}
		}
	}
}

SCRIPTING_FUNC MassShoot::Weapons::WeaponSuite::CheckForUpgradeTriggers()
{
	for (int i = 0; i < upgradeTriggers.size(); i++) {
		if (CheckCollisionBoxes(upgradeTriggers.at(i), MassShoot::Camera::GetCollider())) {
			DrawTextEx(textFont, "PRESS E TO GRAB", { (float)GetScreenWidth() / 2 - 180, (float)GetScreenHeight() / 2 + 140 }, 40, 0, WHITE);
			if (IsKeyPressed(KEY_E)) {
				std::string info, script;
				int key;
				std::ifstream infoData("massshoot/upgrades/" + upgradeTriggersNames.at(i) + "/info.txt");
				while (std::getline(infoData, info)) {
					// truc
				}
				infoData.close();
				std::ifstream scriptData("massshoot/upgrades/" + upgradeTriggersNames.at(i) + "/script.chai");
				std::string scriptline;
				while (std::getline(scriptData, scriptline)) {
					script = script + "\n" + scriptline;
				}
				scriptData.close();
				std::ifstream keyData("massshoot/upgrades/" + upgradeTriggersNames.at(i) + "/key.txt");
				while (keyData >> key) {
					printf("New Key: %i\n", key);
				}
				keyData.close();
				MassShoot::Weapons::WeaponUpgrade newUpgrade;
				newUpgrade.keyID = key;
				newUpgrade.name = info;
				newUpgrade.script = script;
				upgrades.push_back(newUpgrade);
				PlaySound(upgradeSound);
				upgradeTriggers.erase(upgradeTriggers.begin()+i);
				upgradeTriggersNames.erase(upgradeTriggersNames.begin() + i);

			}
		}
	}
}

void DrawCubeTexture(Texture2D texture, Vector3 position, float width, float height, float length, Color color)
{
	float x = position.x;
	float y = position.y;
	float z = position.z;

	// Set desired texture to be enabled while drawing following vertex data
	rlSetTexture(texture.id);

	// Vertex data transformation can be defined with the commented lines,
	// but in this example we calculate the transformed vertex data directly when calling rlVertex3f()
	//rlPushMatrix();
		// NOTE: Transformation is applied in inverse order (scale -> rotate -> translate)
		//rlTranslatef(2.0f, 0.0f, 0.0f);
		//rlRotatef(45, 0, 1, 0);
		//rlScalef(2.0f, 2.0f, 2.0f);

	rlBegin(RL_QUADS);
	rlColor4ub(color.r, color.g, color.b, color.a);
	// Front Face
	rlNormal3f(0.0f, 0.0f, 1.0f);       // Normal Pointing Towards Viewer
	rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width / 2, y - height / 2, z + length / 2);  // Bottom Left Of The Texture and Quad
	rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width / 2, y - height / 2, z + length / 2);  // Bottom Right Of The Texture and Quad
	rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width / 2, y + height / 2, z + length / 2);  // Top Right Of The Texture and Quad
	rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width / 2, y + height / 2, z + length / 2);  // Top Left Of The Texture and Quad
	// Back Face
	rlNormal3f(0.0f, 0.0f, -1.0f);     // Normal Pointing Away From Viewer
	rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width / 2, y - height / 2, z - length / 2);  // Bottom Right Of The Texture and Quad
	rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width / 2, y + height / 2, z - length / 2);  // Top Right Of The Texture and Quad
	rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width / 2, y + height / 2, z - length / 2);  // Top Left Of The Texture and Quad
	rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width / 2, y - height / 2, z - length / 2);  // Bottom Left Of The Texture and Quad
	// Top Face
	rlNormal3f(0.0f, 1.0f, 0.0f);       // Normal Pointing Up
	rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width / 2, y + height / 2, z - length / 2);  // Top Left Of The Texture and Quad
	rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width / 2, y + height / 2, z + length / 2);  // Bottom Left Of The Texture and Quad
	rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width / 2, y + height / 2, z + length / 2);  // Bottom Right Of The Texture and Quad
	rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width / 2, y + height / 2, z - length / 2);  // Top Right Of The Texture and Quad
	// Bottom Face
	rlNormal3f(0.0f, -1.0f, 0.0f);     // Normal Pointing Down
	rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width / 2, y - height / 2, z - length / 2);  // Top Right Of The Texture and Quad
	rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width / 2, y - height / 2, z - length / 2);  // Top Left Of The Texture and Quad
	rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width / 2, y - height / 2, z + length / 2);  // Bottom Left Of The Texture and Quad
	rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width / 2, y - height / 2, z + length / 2);  // Bottom Right Of The Texture and Quad
	// Right face
	rlNormal3f(1.0f, 0.0f, 0.0f);       // Normal Pointing Right
	rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width / 2, y - height / 2, z - length / 2);  // Bottom Right Of The Texture and Quad
	rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width / 2, y + height / 2, z - length / 2);  // Top Right Of The Texture and Quad
	rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width / 2, y + height / 2, z + length / 2);  // Top Left Of The Texture and Quad
	rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width / 2, y - height / 2, z + length / 2);  // Bottom Left Of The Texture and Quad
	// Left Face
	rlNormal3f(-1.0f, 0.0f, 0.0f);    // Normal Pointing Left
	rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width / 2, y - height / 2, z - length / 2);  // Bottom Left Of The Texture and Quad
	rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width / 2, y - height / 2, z + length / 2);  // Bottom Right Of The Texture and Quad
	rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width / 2, y + height / 2, z + length / 2);  // Top Right Of The Texture and Quad
	rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width / 2, y + height / 2, z - length / 2);  // Top Left Of The Texture and Quad
	rlEnd();
	//rlPopMatrix();

	rlSetTexture(0);
}

// Draw cube with texture piece applied to all faces
void DrawCubeTextureRec(Texture2D texture, Rectangle source, Vector3 position, float width, float height, float length, Color color)
{
	float x = position.x;
	float y = position.y;
	float z = position.z;
	float texWidth = (float)texture.width;
	float texHeight = (float)texture.height;

	// Set desired texture to be enabled while drawing following vertex data
	rlSetTexture(texture.id);

	// We calculate the normalized texture coordinates for the desired texture-source-rectangle
	// It means converting from (tex.width, tex.height) coordinates to [0.0f, 1.0f] equivalent 
	rlBegin(RL_QUADS);
	rlColor4ub(color.r, color.g, color.b, color.a);

	// Front face
	rlNormal3f(0.0f, 0.0f, 1.0f);
	rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
	rlVertex3f(x - width / 2, y - height / 2, z + length / 2);
	rlTexCoord2f((source.x + source.width) / texWidth, (source.y + source.height) / texHeight);
	rlVertex3f(x + width / 2, y - height / 2, z + length / 2);
	rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
	rlVertex3f(x + width / 2, y + height / 2, z + length / 2);
	rlTexCoord2f(source.x / texWidth, source.y / texHeight);
	rlVertex3f(x - width / 2, y + height / 2, z + length / 2);

	// Back face
	rlNormal3f(0.0f, 0.0f, -1.0f);
	rlTexCoord2f((source.x + source.width) / texWidth, (source.y + source.height) / texHeight);
	rlVertex3f(x - width / 2, y - height / 2, z - length / 2);
	rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
	rlVertex3f(x - width / 2, y + height / 2, z - length / 2);
	rlTexCoord2f(source.x / texWidth, source.y / texHeight);
	rlVertex3f(x + width / 2, y + height / 2, z - length / 2);
	rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
	rlVertex3f(x + width / 2, y - height / 2, z - length / 2);

	// Top face
	rlNormal3f(0.0f, 1.0f, 0.0f);
	rlTexCoord2f(source.x / texWidth, source.y / texHeight);
	rlVertex3f(x - width / 2, y + height / 2, z - length / 2);
	rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
	rlVertex3f(x - width / 2, y + height / 2, z + length / 2);
	rlTexCoord2f((source.x + source.width) / texWidth, (source.y + source.height) / texHeight);
	rlVertex3f(x + width / 2, y + height / 2, z + length / 2);
	rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
	rlVertex3f(x + width / 2, y + height / 2, z - length / 2);

	// Bottom face
	rlNormal3f(0.0f, -1.0f, 0.0f);
	rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
	rlVertex3f(x - width / 2, y - height / 2, z - length / 2);
	rlTexCoord2f(source.x / texWidth, source.y / texHeight);
	rlVertex3f(x + width / 2, y - height / 2, z - length / 2);
	rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
	rlVertex3f(x + width / 2, y - height / 2, z + length / 2);
	rlTexCoord2f((source.x + source.width) / texWidth, (source.y + source.height) / texHeight);
	rlVertex3f(x - width / 2, y - height / 2, z + length / 2);

	// Right face
	rlNormal3f(1.0f, 0.0f, 0.0f);
	rlTexCoord2f((source.x + source.width) / texWidth, (source.y + source.height) / texHeight);
	rlVertex3f(x + width / 2, y - height / 2, z - length / 2);
	rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
	rlVertex3f(x + width / 2, y + height / 2, z - length / 2);
	rlTexCoord2f(source.x / texWidth, source.y / texHeight);
	rlVertex3f(x + width / 2, y + height / 2, z + length / 2);
	rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
	rlVertex3f(x + width / 2, y - height / 2, z + length / 2);

	// Left face
	rlNormal3f(-1.0f, 0.0f, 0.0f);
	rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
	rlVertex3f(x - width / 2, y - height / 2, z - length / 2);
	rlTexCoord2f((source.x + source.width) / texWidth, (source.y + source.height) / texHeight);
	rlVertex3f(x - width / 2, y - height / 2, z + length / 2);
	rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
	rlVertex3f(x - width / 2, y + height / 2, z + length / 2);
	rlTexCoord2f(source.x / texWidth, source.y / texHeight);
	rlVertex3f(x - width / 2, y + height / 2, z - length / 2);

	rlEnd();

	rlSetTexture(0);
}
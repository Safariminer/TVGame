#include <raylib.h>
#include "rlremoved.h"
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <fstream>
class KwaplanBox {
public:
	float x, y, z, width, height, length;
	std::string textureInTextures;
};
int main() {
	InitWindow(1280, 720, "EdwyPlanner");
	SetTargetFPS(60);
	Camera camera;
	camera.position = { 10, 10, 10 };
	camera.target = { 0,0,0 };
	camera.fovy = 70;
	camera.up = { 0,1,0 };
	camera.projection = CAMERA_PERSPECTIVE;
	std::map<std::string, Texture2D> textures;
	std::string texturefile;
	std::cout << "Select texture file: ";

	std::getline(std::cin, texturefile);
	std::ifstream textureData(texturefile);

	std::string textureName, texturePath;

	while (textureData >> textureName >> texturePath) {
		// MassShoot::Skybox::LoadSkybox(line);
		if (textures.find(textureName) != textures.end()) {
			UnloadTexture(textures.at(textureName));
			textures.erase(textureName);
		}
		textures.insert_or_assign(textureName, LoadTexture(texturePath.c_str()));
	}

	textureData.close();

	std::vector<KwaplanBox> boxes;
	std::vector<KwaplanBox> boxesCancelled;
	std::string mapfile;
	std::cout << "Select map file: ";

	std::getline(std::cin, mapfile);
	std::ifstream mapData(mapfile);

	float x1,y1,z1,x2,y2,z2;

	while (mapData >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> textureName) {
		KwaplanBox newBox;
		newBox.width = x2 - x1;
		newBox.height = y2 - y1;
		newBox.length = z2 - z1;
		newBox.x = x2 - (newBox.width/2);
		newBox.y = y2 - (newBox.height/2);
		newBox.z = z2 - (newBox.length/2);
		newBox.textureInTextures = textureName;
		boxes.push_back(newBox);
	}

	mapData.close();
	std::cout << boxes.size();
	bool stuck = true;
	while (!WindowShouldClose()) {
		SetExitKey(0);

		if (IsKeyPressed(KEY_ESCAPE)) {
			if (stuck) {
				stuck = false;
				EnableCursor();
			}
			else {
				stuck = true;
				DisableCursor();
			}
		}

		BeginDrawing();
		ClearBackground(SKYBLUE);

		UpdateCameraPro(&camera,
			{
			(IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) * 0.1f -      // Move forward-backward
				(IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) * 0.1f,
				(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) * 0.1f -   // Move right-left
				(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) * 0.1f,
				0.0f                                                // Move up-down
		},
			{
			GetMouseDelta().x * 0.05f,                            // Rotation: yaw
				GetMouseDelta().y * 0.05f,                            // Rotation: pitch
				0.0f                                                // Rotation: roll
		},
			0);                              // Move to target (zoom)
		BeginMode3D(camera);
		DrawGrid(10, 10);

		for (int i = 0; i < boxes.size(); i++) {
			// à tes souhaits
			DrawCubeTexture(textures[boxes.at(i).textureInTextures], { (float)boxes.at(i).x, (float)boxes.at(i).y, (float)boxes.at(i).z }, (float)boxes.at(i).width, (float)boxes.at(i).height, (float)boxes.at(i).length, WHITE);
		}

		EndMode3D();
		EndDrawing();


		if (IsKeyDown(KEY_LEFT_CONTROL)) {
			if (IsKeyPressed(KEY_Z)) {
				boxesCancelled.push_back(boxes.at(boxes.size() - 1));
				boxes.pop_back();
			}
			if (IsKeyPressed(KEY_Y)) {
				boxes.push_back(boxesCancelled.at(boxesCancelled.size() - 1));
				boxesCancelled.pop_back();
			}
		}


		if (IsKeyPressed(KEY_ENTER)) {
			KwaplanBox newBox;
			std::string command;
			std::cout << "X:";
			std::cin >> command;
			newBox.x = std::stoi(command);
			std::cout << "Y:";
			std::cin >> command;
			newBox.y = std::stoi(command);
			std::cout << "Z:";
			std::cin >> command;
			newBox.z = std::stoi(command);
			std::cout << "W:";
			std::cin >> command;
			newBox.width = std::stoi(command);
			std::cout << "H:";
			std::cin >> command;
			newBox.height = std::stoi(command);
			std::cout << "L:";
			std::cin >> command;
			newBox.length = std::stoi(command);
			std::cout << "Texture:";
			std::cin >> command;
			newBox.textureInTextures = command;
			boxes.push_back(newBox);
		}
	}
}
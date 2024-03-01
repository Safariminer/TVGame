#include "Ghost.h"
#include <raylib.h>
#include <iostream>

bool GhostHandlerAvailable = false;

Model ghostModel;

void TVGame::Ghost::CalculateNextPos(Vector3 camera) {
	if (position.x < camera.x) position.x += speed;
	if (position.y < camera.y) position.y += speed;
	if (position.z < camera.z) position.z += speed;
	if (position.x > camera.x) position.x -= speed;
	if (position.y > camera.y) position.y -= speed;
	if (position.z > camera.z) position.z -= speed;
}

void TVGame::Ghost::RenderGhost() {
	if (!GhostHandlerAvailable) {
		std::cout << "Error: no ghost handler available for ghost rendering.";
		return;
	}
	DrawModel(ghostModel, position, 1, WHITE);
}

void TVGame::GhostHandler::GhostHandlerInit() {
	GhostHandlerAvailable = true;
	
	ghostModel = LoadModel("massshoot/actors/ghost/ghost.obj");
	ghostModel.materials[0].maps[0].texture = LoadTexture("massshoot/actors/ghost/ghost.png");
}

TVGame::GhostHandler::~GhostHandler() {
	GhostHandlerAvailable = false;
	UnloadModel(ghostModel);
}

void TVGame::GhostHandler::SpawnGhosts(int number) {
	for (int i = 0; i < number; i++) {
		Ghost ghost;
		ghost.health = 100;
		ghost.speed = (float)GetRandomValue(1, 100) / 1000;
		ghost.position = { (float)GetRandomValue(-100, 100), (float)GetRandomValue(10, 100) , (float)GetRandomValue(-100, 100) };
		ghosts.push_back(ghost);
	}
}

void TVGame::GhostHandler::CheckForGhostCollision(BoundingBox playerCollision) {
	// todo: this
}

void TVGame::GhostHandler::CheckForHarmedGhosts(Ray ray) {
	// todo: this
}

void TVGame::GhostHandler::GhostsUpdate(Vector3 camera) {
	for (int i = 0; i < ghosts.size(); i++) {
		ghosts.at(i).CalculateNextPos(camera);
		ghosts.at(i).RenderGhost();
	}
}
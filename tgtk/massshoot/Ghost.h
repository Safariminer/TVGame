#pragma once
#include <raylib.h>
#include <vector>

namespace TVGame {
	class Ghost {
	public:
		int health;
		float speed;
		Vector3 position;
		bool hurt;
		void CalculateNextPos(Vector3 camera);
		void RenderGhost();
	};
	class GhostHandler {
		void GhostHandlerInit();
		std::vector<Ghost> ghosts;
	public:
		GhostHandler() { GhostHandlerInit(); }
		~GhostHandler();
		void SpawnGhosts(int number);
		void KillAllGhosts();
		int CheckForGhostCollision(BoundingBox playerCollision);
		void CheckForHarmedGhosts(Ray ray);
		void GhostsUpdate(Vector3 camera);
		int GetGhostCount();
	};
}
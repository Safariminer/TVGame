#pragma once
#include "defines.h"
#include <raylib.h>
#include <vector>
#include <iostream>
#include <rlights.h>

namespace MassShoot {
	namespace Maps {
		class CollideCollAdvancedResult {
		public:
			Vector3 newPossiblePos;
		};
		class Map {
			std::vector<BoundingBox> collisions;
			std::vector<std::string> textures;
			std::vector<Light> lights;
			std::vector<std::string> triggerMaps;
			std::vector<BoundingBox> triggers;
			std::vector<std::string> sprites;
			std::vector<Vector3> billboards;
		public:
			LOADING_FUNC LoadMap(std::string mapName);
			LIGHTING_FUNC UpdateLighting(Vector3 cameraPosition);
			RENDERING_FUNC RenderBillboards();
			RENDERING_FUNC RenderBoxes();
			PHYSICS_BOOL IsColliding(BoundingBox box);
			CollideCollAdvancedResult CollideCollAdvanced(Vector3 newbox, Vector3 oldbox, int margin, int height);
			PHYSICS_FUNC CheckForTriggers(BoundingBox box);
			std::string GetMapName();
			std::string title, subtitle;
			bool shaded = true;
		};
	}
}
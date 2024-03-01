#pragma once
#include "defines.h"
#include "Map.h"
namespace MassShoot {
	namespace Camera {
		BoundingBox GetCollider();
		LOADING_FUNC InitCamera();
		INPUT_FUNC LockCamera();
		INPUT_FUNC UnlockCamera();
		RENDERING_FUNC GravityDisplay();
		INPUT_FUNC CameraMovement(MassShoot::Maps::Map map, bool enableBhop, bool enableAdvancedPhysics, bool enableNewGroundCollider);
		RENDERING_FUNC StartCameraFrame();
		RENDERING_FUNC StopCameraFrame();
		Vector3 GetCameraPosition();
	}
}
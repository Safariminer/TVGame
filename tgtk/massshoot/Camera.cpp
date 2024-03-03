#include "Camera.h"
#include "defines.h"
#include <raylib.h>
#include <raymath.h>
#include "cameraextern.h"
#include "Fonts.h"

#define BHOPFACTOR 0.025f

bool locked = true;
Camera camera = { 0 };

Vector3 rotation = { 1,0,0 };
Vector3 hRotation = { 1,0,0 };
Vector3 oldCamPos;
Vector3 oldTargPos;

BoundingBox playerCollider;

Music wind;
Sound stomp;
Vector3 gravity = { 0,0,0 };

Camera MassShoot::Camera::GetCamera()
{
	return camera;
}

BoundingBox MassShoot::Camera::GetCollider() {
	return playerCollider;
}

LOADING_FUNC MassShoot::Camera::InitCamera()
{
	gravity = { 0,0,0 };
	camera = { 0 };
	camera.position = { 0.0f, 5.0f, 0.0f };    // Camera position
	camera.target = { 4.0f, 1.0f, 4.0f };      // Camera looking at point
	camera.up = { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
	camera.fovy = 90.0f;                                // Camera field-of-view Y
	camera.projection = CAMERA_PERSPECTIVE;
	/*if (wind.stream.buffer == NULL) {
		wind = LoadMusicStream("massshoot/sounds/wind.wav");
	}
	if (stomp.stream.buffer == NULL) {
		stomp = LoadSound("massshoot/sounds/stomp.wav");
	}
	StopMusicStream(wind);
	PlayMusicStream(wind);*/
}

INPUT_FUNC MassShoot::Camera::LockCamera() {
	locked = true;
	DisableCursor();
}

INPUT_FUNC MassShoot::Camera::UnlockCamera() {
	locked = false;
	EnableCursor();
}

float playerSpeed = 0.25f;

bool gravityActivated = false;

bool previouslyGrounded;

int bhopcooldown = 0;

RENDERING_FUNC MassShoot::Camera::GravityDisplay() {
	DrawTextEx(textFont, TextFormat("gravity.x: %f\ngravity.y: %f\ngravity.z: %f", gravity.x, gravity.y, gravity.z), {0,0}, 20, 0, WHITE);
}

INPUT_FUNC MassShoot::Camera::CameraMovement(MassShoot::Maps::Map map, bool enableBhop = false, bool enableAdvancedPhysics = true, bool enableNewGroundCollider = true)
{
	if (camera.position.y < -1000) InitCamera();
	// UpdateMusicStream(wind);
	// float windVol = 0.1f;
	// windVol = 0.1f + abs(gravity.y);
	// if (windVol > 1.5f) windVol = 1.5f;
	// SetMusicVolume(wind, windVol*0.5f);
	if(locked){
		camera.target = Vector3Add(camera.position, rotation);
		oldCamPos = camera.position;
		oldTargPos = camera.target;
		Vector2 mouseDelta = GetMouseDelta();
		rotation = Vector3RotateByAxisAngle(rotation, { 0,1,0 }, (-mouseDelta.x) / 100);
		hRotation = Vector3RotateByAxisAngle(hRotation, { 0,1,0 }, (-mouseDelta.x) / 100);
		rotation.y -= mouseDelta.y / 100;
		rotation.y = Clamp(rotation.y, -0.99, 0.99);
		rotation = Vector3Normalize(rotation);
		Vector3 newCamPos = camera.position;

		BoundingBox groundCollider;
		groundCollider.max = Vector3Add(newCamPos, { 1,1,1 });
		if(enableNewGroundCollider) groundCollider.max = Vector3Add(newCamPos, { 1,-1,1 });
		groundCollider.min = Vector3Subtract(newCamPos, { 1,3,1 });
		
		bool grounded = map.IsColliding(groundCollider);

		// if (!previouslyGrounded) PlaySound(stomp);

		// if (IsKeyPressed(KEY_G)) gravityActivated = !gravityActivated;

		if (grounded) {
			gravity.y = 0;
			if(enableBhop){
				bhopcooldown++;
				if (bhopcooldown > 5) {
					gravity = Vector3Zero();
				}
			}
		}
		else {
			gravity.y+=0.01f;
		}

		if (IsKeyDown(KEY_W))
			newCamPos = Vector3Add(newCamPos, Vector3Multiply(hRotation, { playerSpeed, playerSpeed, playerSpeed }));

		if (IsKeyDown(KEY_S))
			newCamPos = Vector3Subtract(newCamPos, Vector3Multiply(hRotation, { playerSpeed, playerSpeed, playerSpeed }));


		Vector3 left;
		left = Vector3RotateByAxisAngle(hRotation, { 0,1,0 }, 1.5708f);
		left.y = 0;
		if (IsKeyDown(KEY_A))
			newCamPos = Vector3Add(newCamPos, Vector3Multiply(left, { playerSpeed, playerSpeed, playerSpeed }));
		if (IsKeyDown(KEY_D))
			newCamPos = Vector3Subtract(newCamPos, Vector3Multiply(left, { playerSpeed, playerSpeed, playerSpeed }));

		if (IsKeyDown(KEY_SPACE) && grounded) {
			gravity.y -= 0.25;
			if(enableBhop){
				Vector3 bhopVec = Vector3Zero();
				if(IsKeyDown(KEY_W)) bhopVec = Vector3Multiply(Vector3Subtract(camera.position, camera.target), { BHOPFACTOR, 0, BHOPFACTOR });
				if (IsKeyDown(KEY_S)) bhopVec = Vector3Multiply(Vector3Subtract(camera.position, camera.target), { -BHOPFACTOR, 0, -BHOPFACTOR });
				gravity = Vector3Add(gravity, bhopVec);
				bhopcooldown = 0;
			}
		}

		newCamPos = Vector3Subtract(newCamPos, gravity);

		playerCollider.max = Vector3Add(newCamPos, { 1,0,1 });
		playerCollider.min = Vector3Subtract(newCamPos, { 1,2,1 });
		
		if (!map.IsColliding(playerCollider)) camera.position = newCamPos;
		else {
			if(enableAdvancedPhysics) camera.position = map.CollideCollAdvanced(newCamPos, { camera.position.x, camera.position.y - 1, camera.position.z }, 2, 4).newPossiblePos;
		}
		camera.target = Vector3Add(camera.position, rotation);
		previouslyGrounded = grounded;
	}
}

RENDERING_FUNC MassShoot::Camera::StartCameraFrame()
{
	BeginMode3D(camera);
	ClearBackground(BLACK);
}

RENDERING_FUNC MassShoot::Camera::StopCameraFrame()
{
	EndMode3D();
}

Vector3 MassShoot::Camera::GetCameraPosition()
{
	return camera.position;
}

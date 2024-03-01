#include "chaiLoadMap.h"
#include "ExternMap.h"
#include "Camera.h"
#include "unraylibedExterns.h"
void Chai_LoadMap(std::string mapname)
{
	menuEnum = 2;
	map.LoadMap(mapname);
	MassShoot::Camera::InitCamera();
	MassShoot::Camera::LockCamera();
}

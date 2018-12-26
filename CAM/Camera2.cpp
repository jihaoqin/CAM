#include "Camera2.h"
#include "utility.h"



Camera2::Camera2(glm::vec3 pos, glm::vec3 zDir, glm::vec3 upDir)
{
	camBaseWorld = utility::createMat(pos, zDir, upDir);
	worldBaseCam = glm::inverse(camBaseWorld);
}


Camera2::~Camera2()
{
}

#include "PerspectiveMat.h"
PerspectiveMat::PerspectiveMat():fov(45.0f),ratio(8.0/6.0f),nearPlane(0.1f),farPlane(100.0f) {
}
PerspectiveMat::~PerspectiveMat()
{
}
glm::mat4 PerspectiveMat::getMatrix() {
	update();
	return matrix;
}
void PerspectiveMat::update() {
	matrix = glm::perspective(glm::radians(fov), ratio, nearPlane, farPlane);
}
float PerspectiveMat::getNearPlane() {
	return nearPlane;
}

float PerspectiveMat::getFov()
{
	return fov;
}

void PerspectiveMat::setFov(float value)
{
	fov = value;
	update();
}


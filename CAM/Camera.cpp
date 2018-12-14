#include "Camera.h"



Camera::Camera(glm::vec3 p, glm::vec3 d, glm::vec3 u): pos(p), dir(glm::normalize(d)), worldUp(u), moveSpeed(5.0f), rotateSensity( 2000.0f/(2*3.14))
{
	view = glm::lookAt(pos, pos + dir, worldUp);
	right = glm::normalize(glm::cross(dir, worldUp));
	left = right *(-1.0f);
	pitch = atan(dir.z*(-1), dir.x);
	yaw = atan(glm::length(glm::vec2(dir.x, dir.z)), dir.y);
}


Camera::~Camera()
{
}

glm::mat4 Camera::getViewMat()
{
	view = glm::lookAt(pos, pos + dir, worldUp);
	return view;
}

void Camera::moveForward(float time)
{
	pos += dir * moveSpeed * time;
}

void Camera::moveBackward(float time)
{
	moveForward(-1 * time);
}

void Camera::moveLeft(float time)
{
	pos += left * moveSpeed * time;
}

void Camera::moveRight(float time)
{
	pos += right * moveSpeed * time;
}

void Camera::processMouseMove(float deltaX, float deltaY)
{
	float deltaPitch = deltaX / rotateSensity;
	float deltaYaw = deltaY / rotateSensity * (-1.0f);
	pitch += deltaPitch;
	yaw += deltaYaw;
	if (yaw > glm::radians(89.0f)) {
		yaw = glm::radians(89.0f);
	}
	else if (yaw < glm::radians(-89.0f)) {
		yaw = glm::radians(-89.0f);
	}
	else {
		// do nothing
	}
	dir.x = cos(yaw)*sin(pitch);
	dir.y = sin(yaw);
	dir.z = -1 * cos(yaw)*cos(pitch);
	glm::normalize(dir);
	update();
}

float Camera::atan(float x, float y) {
	glm::vec2 v(x, y);
	glm::normalize(v);
	float rad = glm::acos(v.x);
	if (v.y > 0) {
		// do nothing
	}
	else {
		rad = -1.0f * rad;
	}
	return rad;
}

void Camera::update() {
	glm::normalize(dir);
	right = glm::normalize(glm::cross(dir, worldUp));
	left = right *(-1.0f);
}
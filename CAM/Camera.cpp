#pragma once
#include "Camera.h"
#include <iostream>


Camera::Camera(glm::vec3 p, glm::vec3 d, glm::vec3 u): pos(p), dir(glm::normalize(d)),
worldUp(u), moveSpeed(5.0f), rotateSensity( 800.0f/(2*3.14)) 
{
	update();
}

Camera::~Camera()
{
}

glm::mat4 Camera::getViewMat()
{
	return view;
}

void Camera::moveForward(float time)
{
	pos += dir * moveSpeed * time;
	update();
}

void Camera::moveBackward(float time)
{
	moveForward(-1 * time);
	update();
}

void Camera::moveLeft(float time)
{
	pos += left * moveSpeed * time;
	update();
}

void Camera::moveRight(float time)
{
	pos += right * moveSpeed * time;
	update();
}

void Camera::processMouseMove(float deltaX, float deltaY)
{
	float deltaPitch = deltaX / rotateSensity;
	float deltaYaw = deltaY / rotateSensity * (-1.0f);
#if DEBUG
	std::cout << "Pitch1 = " << glm::degrees(pitch) << ", " << "Yaw1 = " << glm::degrees(yaw) << std::endl;
#endif
	pitch += deltaPitch;
	yaw += deltaYaw;
#if DEBUG
	std::cout << "deltaPitch = " << glm::degrees(deltaPitch) << ", " << "deltaYaw = " << glm::degrees(deltaYaw) << std::endl;
#endif
	if (yaw > glm::radians(89.0f)) {
		yaw = glm::radians(89.0f);
	}
	else if (yaw < glm::radians(-89.0f)) {
		yaw = glm::radians(-89.0f);
	}
	else {
		// do nothing
	}
	std::cout << "Pitch2 = " << glm::degrees(pitch) << ", " << "Yaw2 = " << glm::degrees(yaw) << std::endl;
	dir.x = cos(yaw)*sin(pitch);
	dir.y = sin(yaw);
	dir.z = -1 * cos(yaw)*cos(pitch);
	update();
}

float Camera::atan(float x, float y) {
	glm::vec2 v(x, y);
	v = glm::normalize(v);
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
	dir = glm::normalize(dir);
	std::cout << vec3Str(dir) << std::endl;
	right = glm::normalize(glm::cross(dir, worldUp));
	left = right *(-1.0f);
	pitch = atan(dir.z*(-1), dir.x);
	yaw = atan(glm::length(glm::vec2(dir.x, dir.z)), dir.y);
	view = glm::lookAt(pos, pos + dir, worldUp);
	print();
}

glm::mat4 Camera::getPerspective()
{
	return perspective.getMatrix();
}

void Camera::processScroll(double yOffset) {
	float fov = perspective.getFov();
	if (fov >= 1.0f && fov <= 88.0f)
		perspective.setFov(fov - yOffset);
	else if (fov <= 1.0f)
		perspective.setFov(1.0f);
	else if (fov >= 88.0f)
		perspective.setFov(88.0f);
	else
		;
	update();
}

glm::vec3 Camera::getPos()
{
	return pos;
}

void Camera::print() {
	std::cout << "Camera Information:\n";
	std::cout << "direction = " <<vec3Str(dir)<<"\n";
	std::cout << "[yaw, pitch] = " <<glm::degrees(yaw) << ", "<< glm::degrees(pitch)<<"\n";
}
std::string vec3Str(glm::vec3 value)
{
	std::string str = std::to_string(value.x) + ", " + std::to_string(value.y) + ", " + std::to_string(value.z);
	return str;
}

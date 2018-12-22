#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "PerspectiveMat.h"
#include <string>
class Camera
{
public:
	Camera(glm::vec3 p, glm::vec3 d, glm::vec3 u);
	~Camera();
	glm::mat4 getViewMat();
	void moveForward(float time);
	void moveBackward(float time);
	void moveLeft(float time);
	void moveRight(float time);
	void processMouseMove(float deltaX, float deltaY);
	glm::mat4 getPerspective();
	void processScroll(double);
	glm::vec3 getPos();
	void print();
private:
	//基本量
	glm::vec3 pos;
	glm::vec3 dir;
	glm::vec3 worldUp;
	float moveSpeed;
	float rotateSensity;
	//计算量
	glm::vec3 right;
	glm::vec3 left;
	float pitch;
	float yaw;
	glm::mat4 view;
	PerspectiveMat perspective;
	float atan(float x, float y);
	void update();
};
std::string vec3Str(glm::vec3);
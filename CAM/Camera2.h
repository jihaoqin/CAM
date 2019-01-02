#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Model.h"
#include "PerspectiveMat.h"
class Camera2
{
public:
	Camera2(glm::vec3 pos, glm::vec3 zDir, glm::vec3 upDir);
	Camera2(BoundingBox& b);
	~Camera2();
public:
	void rotateScene(glm::vec3 pos, float rad, glm::vec3 axis);
	glm::vec3 getPos();
	void updateShader(Shader&);
	void bindBoundingBox(BoundingBox b);
	void processMouseMove(float deltaX, float deltaY);
	void processScroll(double yOffset);
private:
	PerspectiveMat perspective;
	BoundingBox box;
	glm::mat4 camBaseWorld;
	glm::mat4 worldBaseCam;
};

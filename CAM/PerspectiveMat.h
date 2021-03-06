#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class PerspectiveMat {
public:
	PerspectiveMat();
	~PerspectiveMat();
	glm::mat4 getMatrix();
	float getNearPlane();
	float getFov();
	void setFov(float);
private:
	float fov;
	float ratio;
	float nearPlane;
	float farPlane;
	glm::mat4 matrix;
	void update();
};

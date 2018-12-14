#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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

	glm::vec3 pos;
	glm::vec3 dir;
	glm::vec3 worldUp;
	glm::vec3 right;
	glm::vec3 left;
	float pitch;
	float yaw;
	float moveSpeed;
	float rotateSensity;
private:
	float atan(float x, float y);
	glm::mat4 view;
	void update();
};


#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Camera2
{
public:
	Camera2(glm::vec3 pos, glm::vec3 zDir, glm::vec3 upDir);
	~Camera2();
private:
	glm::mat4 camBaseWorld;
	glm::mat4 worldBaseCam;
};


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include "Shader.h"
#include "stb_image.h"
#include "Camera.h"
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include "Model.h"
#include <stdlib.h>
#include "Camera2.h"

using namespace std;
using namespace glm;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

glm::vec3 camPos(0.0f, 0.0f, 5.0f);
glm::vec3 dir(0.0f, 0.0f, -1.0f);
glm::vec3 upVec(0.0f, 1.0f, 0.0f);
Camera camera(camPos, dir, upVec);
Camera2 camera2(camPos, dir, upVec);
float deltaT;

float fov = 45.0f;
glm::mat4 myPerspective = glm::perspective(glm::radians(fov), 0.8f/0.6f, 0.1f, 100.0f);
glm::vec2 lastLoc(400, 300);
float pitch = 0;
float yaw = 0;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
	Model m("C:/Users/ÇØ¼ÌºÀ/source/repos/CAM/CAM/ÈýÍ¨¹Ü.STL");
	Shader s1("vertexShader.vs", "pureColorWithLight.fs");
	s1.use();
	//s1.setMat4("perspective", myPerspective);
	glm::mat4 myModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
	s1.setVec3("lineLight.direction", glm::vec3(0.0f, 0.0f, -1.0f));
	s1.setVec3("lineLight.ambient", glm::vec3(0.2f));
	s1.setVec3("lineLight.diffuse", glm::vec3(0.5f));
	s1.setVec3("lineLight.specular", glm::vec3(0.9f));
	s1.setVec3("material.ambient", glm::vec3(0.3f, 0.1f, 0.2f));
	s1.setVec3("material.diffuse", glm::vec3(0.5f, 0.2f, 0.1f));
	s1.setVec3("material.specular", glm::vec3(0.7f));
	s1.setFloat("material.shininess", 32);
	camera2.bindBoundingBox(m.boundingBox());
	

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glEnable(GL_DEPTH_TEST);
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		double beginT = glfwGetTime();
		processInput(window);

		//render
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//camera.updateShader(s1);
		camera2.updateShader(s1);
		m.draw(s1);
		double endT = glfwGetTime();
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		deltaT = endT - beginT;
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.moveForward(deltaT);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.moveBackward(deltaT);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.moveLeft(deltaT);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.moveRight(deltaT);
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
 
void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
	cout << "x, y = " << xPos << "," << yPos ;
	cout << "lastx, lasty = " << lastLoc.x << "," << lastLoc.y << std::endl;
	float deltaX = xPos - lastLoc.x;
	float deltaY = yPos - lastLoc.y;
	//camera.processMouseMove(deltaX, deltaY);
	camera2.processMouseMove(deltaX, deltaY);
	lastLoc.x = xPos;
	lastLoc.y = yPos;
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera2.processScroll(yoffset);
}

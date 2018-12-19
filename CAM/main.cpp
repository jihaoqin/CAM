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

using namespace std;
using namespace glm;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);
GLuint loadTexture(const char*);
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

glm::vec3 camPos(0.0f, 0.0f, 5.0f);
glm::vec3 dir(0.0f, 0.0f, -1.0f);
glm::vec3 upVec(0.0f, 1.0f, 0.0f);
Camera camera(camPos, dir, upVec);
float deltaT;

glm::mat4 model2 = glm::translate(glm::mat4(1.0f),glm::vec3(3.0f, 3.0f, 3.0f));
glm::mat4 myPerspective = glm::perspective(glm::radians(45.0f), 0.8f/0.6f, 0.1f, 100.0f);
vector<glm::mat4> models = {
	translate(rotate(mat4(1.0f), radians(0.0f), vec3(1.0,2.0,3.0)), vec3(0.0,0.0,0.0)),
	translate(rotate(mat4(1.0f), radians(20.0f), vec3(1.0,1.0,0.0)), vec3(2.5,1.0,-2.0)),
	translate(rotate(mat4(1.0f), radians(30.0f), vec3(-1.0,2.0,3.0)), vec3(-1.9,0.9,-2.0)),
	translate(rotate(mat4(1.0f), radians(10.0f), vec3(1.0,-2.0,3.0)), vec3(1.7,2.0,-1.7)),
	translate(rotate(mat4(1.0f), radians(40.0f), vec3(1.0,2.0,-3.0)), vec3(1.8,-0.2,-1.0)),
};
glm::vec2 lastLoc(400, 300);
float pitch = 0;
float yaw = 0;
const float vertices[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,	
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};
vector<Vertex> vertexVec = arrayToVec(sizeof(vertices),vertices);
vector<unsigned int> indexVec = initVecUint(sizeof(vertices)/sizeof(float));

int main()
{
	unsigned char a[] = {0x00, 0x00, 0x00, 0xbf};
	float *p = (float *)a;
	printf("%g \r\n", *p);
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

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
	
	Shader s1("vertexShader.vs", "fragmentShader.fs");
	Shader s2("vertexShader.vs", "light.fs");
<<<<<<< HEAD

	GLuint texture1;
	glGenTextures(1, &texture1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;
	unsigned char *data = stbi_load("container2.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
=======
	GLuint texture1 = loadTexture("container2.png");
>>>>>>> 03aec3b5da4e242e92b0cb2a829fcb0aa6fa4df6
	Texture t1;
	t1.id = texture1;
	t1.type = "texture_diffuse";

	GLuint texture2 = loadTexture("container2_specular.png");
	Texture t2;
	t2.id = texture2;
	t2.type = "texture_specular";
	vector<Texture> textureVec;
	textureVec.push_back(t1);
	textureVec.push_back(t2);
	Mesh m(vertexVec, indexVec, textureVec);
	s1.use();
	s1.setVec3("pointLight.position", glm::vec3(model2 * glm::vec4(0, 0, 0, 1)));
	s1.setVec3("pointLight.ambient", glm::vec3(0.2f));
	s1.setVec3("pointLight.diffuse", glm::vec3(0.5f));
	s1.setVec3("pointLight.specular", glm::vec3(1.0f));
	s1.setVec3("lineLight.direction",vec3(0.0f, 0.0f, -1.0f));
	s1.setVec3("lineLight.ambient",vec3(0.2f));
	s1.setVec3("lineLight.diffuse",vec3(0.5f));
	s1.setVec3("lineLight.specular",vec3(1.0f));
	s1.setFloat("material.shininess", 32);
	s1.setMat4("perspective", myPerspective);

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
<<<<<<< HEAD
		//第一个正方体
		s1.use();
		s1.setMat4("view", camera.getViewMat());
		s1.setVec3("viewPos", camera.pos);
		m.draw(s1);
=======
		//正方体们
		s1.use();
		s1.setMat4("view", camera.getViewMat());
		s1.setVec3("viewPos", camera.pos);
		for (int i = 0; i < models.size(); i++) {
			s1.setMat4("model", models.at(i));
			m.draw(s1);
		}
>>>>>>> 03aec3b5da4e242e92b0cb2a829fcb0aa6fa4df6

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
	float deltaX = xPos - lastLoc.x;
	float deltaY = yPos - lastLoc.y;
	camera.processMouseMove(deltaX, deltaY);
	lastLoc.x = xPos;
	lastLoc.y = yPos;
}

GLuint loadTexture(const char* path) {
	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;
	unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
	GLint format;
	if (nrChannels == 1) {
		format = GL_RED;
	}
	else if (nrChannels == 3) {
		format = GL_RGB;
	}
	else if (nrChannels == 4) {
		format = GL_RGBA;
	}
	else {
		//do nothing;
		std::cout << "Unexpected image foramt. \n";
	}

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	return texture;
}
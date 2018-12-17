#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <string>
#include "Shader.h"
using std::vector;
struct Texture {
	unsigned int id;
	std::string type;
};
struct Vertex {
	glm::vec3 vertex;
	glm::vec3 normal;
	glm::vec2 coordinate;
};
class Mesh
{
public:
	Mesh(vector<Vertex>, vector<unsigned int>, vector<Texture>);
	~Mesh();
	void draw(Shader s);
private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();
	std::vector<Vertex> vertexVec;
	std::vector<unsigned int> indexVec;
	std::vector<Texture> textureVec;
};

vector<Vertex> arrayToVec(unsigned int size, const float a[]);
vector<unsigned int> initVecUint(unsigned int size);

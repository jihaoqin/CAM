#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <string>
using std::vector;
struct Texture {
	unsigned int id;
	std::string type;
};
struct Vertex {
	glm::vec3 vertex;
	glm::vec3 normal;
	glm::vec3 coordinate;
};
class Mesh
{
public:
	Mesh(vector<Vertex>, vector<unsigned int>, vector<Texture>);
	~Mesh();
private:
	void setupMesh();
	std::vector<Vertex> vertexVec;
	std::vector<unsigned int> indexVec;
	std::vector<Texture> textureVec;
};


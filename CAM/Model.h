#pragma once
#include <iostream>
#include <assimp/scene.h>
#include "Mesh.h"
#include <string>
class BoundingBox {
public:
	BoundingBox();
	BoundingBox( std::vector<Mesh>&);
	double xmin;
	double xmax;
	double ymin;
	double ymax;
	void print();
};
class Model
{
public:
	Model(const char* filePath);
	~Model();
	void draw(Shader);
private:
	std::vector<Mesh> meshVec;
	std::string directory;
	glm::mat4 modelMatrix;
	BoundingBox box;
	void loadModel(const aiScene*);
	void processNode(const aiNode *, const aiScene *);
	Mesh processMesh(const unsigned int, const aiScene *);
	unsigned int textureFromFile(const char*);
	vector<Texture> loadTextures(const aiMaterial* material, aiTextureType type);
};



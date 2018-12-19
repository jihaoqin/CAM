#pragma once
#include <iostream>
#include <assimp/scene.h>
#include "Mesh.h"
class Model
{
public:
	Model(const char* filePath);
	~Model();
private:
	std::vector<Mesh> meshVec;
	void loadModel(const aiScene*);
	void processNode(const aiNode *, const aiScene *);
	void processMesh(const unsigned int, const aiScene *);
};


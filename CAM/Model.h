#pragma once
#include <iostream>
#include <assimp/scene.h>
#include "Mesh.h"
class Model
{
public:
	Model(const char* filePath);
	~Model();
	void draw(Shader);
private:
	std::vector<Mesh> meshVec;
	void loadModel(const aiScene*);
	void processNode(const aiNode *, const aiScene *);
	Mesh processMesh(const unsigned int, const aiScene *);
	unsigned int textureFromFile(const char*);
	vector<Texture> loadTextures(const aiMaterial* material, aiTextureType type);
};



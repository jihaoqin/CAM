#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using std::vector;
Model::Model(const char* filePath)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);
	loadModel(scene);
}


Model::~Model()
{
}

void Model::loadModel(const aiScene *s) {
	const aiNode *n = s->mRootNode;
	processNode(n, s);
}

void Model::processNode(const aiNode* n, const aiScene* s) {
	unsigned int num = n->mNumChildren;
	if (0 == num) {
		//do nothing
		return;
	}
	else {
		for (int i = 0; i < num; i++) {
			processNode(n->mChildren[i], s);
		}
		for (int i = 0; i < n->mNumMeshes; i++) {
			processMesh(n->mMeshes[i], s);
		}
	}
}

void Model::processMesh(const unsigned int index, const aiScene *s) {
	
}
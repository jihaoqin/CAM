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
	const aiMesh* m = s->mMeshes[index];
	//process position
	vector<Vertex> verVec;

	for (int i = 0; i < m->mNumVertices; i++) {
		Vertex ver;
		auto& v = m->mVertices[i];
		auto& n = m->mNormals[i];
		ver.vertex = glm::vec3(v.x, v.y, v.z);
		ver.normal = glm::vec3(n.x, n.y, n.z);

		aiVector3D* t = m->mTextureCoords[i];
		if (NULL == t) {
			ver.coordinate = glm::vec2(0.0f, 0.0f);
		}
		else {
			ver.coordinate = glm::vec2(t[0], t[1]);
		}
		verVec.push_back(ver);
	}
	vector<unsigned int> indexs;
	for (int i = 0; i < m->mNumFaces; i++) {
		aiFace& face = m->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++) {
			indexs.push_back(face.mIndices[j]);
		}
	}
	Mesh m(verVec, indexs,);
}
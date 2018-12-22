#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stb_image.h"
#include <string>

using std::vector;
Model::Model(const char* filePath)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);
	//µ½ÕâÀï
	std::string str(filePath);
	str.find_last_of('/');
	std::string directory = filePath;
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}
	if (!scene) {
		std::cout << "Failed to import the model.\n";
	}
	else {
		loadModel(scene);
	}
}


Model::~Model()
{
}

void Model::draw(Shader s) {
	s.use();
	for (int i = 0; i < meshVec.size(); i++) {
		meshVec.at(i).draw(s);
	}
}

void Model::loadModel(const aiScene *s) {
	if (!s) {
		std::cout << "Pointer pointed to model is NULL";
	}
	else {
		const aiNode *n = s->mRootNode;
		if (!n) {
			std::cout << "RootNode is NULL.\n";
		}
		else {
			processNode(n, s);
		}
	}
}

void Model::processNode(const aiNode* n, const aiScene* s) {
	for (int i = 0; i < n->mNumMeshes; i++) {
		meshVec.push_back(processMesh(n->mMeshes[i], s));
	}
	unsigned int num = n->mNumChildren;
	if (0 == num) {
		//do nothing
		return;
	}
	else {
		for (int i = 0; i < num; i++) {
			processNode(n->mChildren[i], s);
		}
	}
}

Mesh Model::processMesh(const unsigned int index, const aiScene *s) {
	const aiMesh* m = s->mMeshes[index];
	//process position
	vector<Vertex> verVec;

	for (int i = 0; i < m->mNumVertices; i++) {
		Vertex ver;
		auto& v = m->mVertices[i];
		auto& n = m->mNormals[i];
		ver.vertex = glm::vec3(v.x, v.y, v.z);
		ver.normal = glm::vec3(n.x, n.y, n.z);

		if (m->mTextureCoords[0]) {
			glm::vec2 tex;
			tex.x = m->mTextureCoords[0][i].x;
			tex.y = m->mTextureCoords[0][i].y;
			ver.coordinate = tex;
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

	vector<Texture> textureVec;
	const aiMaterial *material = s->mMaterials[m->mMaterialIndex];
	vector<Texture> diffuseTex = loadTextures(material, aiTextureType_DIFFUSE);
	textureVec.insert(textureVec.end(), diffuseTex.begin(), diffuseTex.end());
	vector<Texture> specularTex = loadTextures(material, aiTextureType_SPECULAR);
	textureVec.insert(textureVec.end(), specularTex.begin(), specularTex.end());
	vector<Texture> normalTex = loadTextures(material, aiTextureType_HEIGHT);
	textureVec.insert(textureVec.end(), normalTex.begin(), normalTex.end());
	vector<Texture> heightTex = loadTextures(material, aiTextureType_AMBIENT);
	textureVec.insert(textureVec.end(), heightTex.begin(), heightTex.end());

	return Mesh(verVec, indexs,textureVec);
}

vector<Texture> Model::loadTextures(const aiMaterial* material, aiTextureType type) {
	vector<Texture> tex;
	for (int i = 0; i < material->GetTextureCount(type); i++) {
		aiString path;
		material->GetTexture(type, i, &path);
		Texture t;
		t.id = textureFromFile(path.C_Str());
		if (type == aiTextureType_DIFFUSE) {
			t.type = "texture_diffuse";
		}
		else if(type == aiTextureType_SPECULAR) {
			t.type = "texture_specular";
		}
		else if(type == aiTextureType_HEIGHT){
			t.type = "texture_normal";
		}
		else if (type == aiTextureType_AMBIENT) {
			t.type = "texture_height";
		}
		tex.push_back(t);
	}
	return tex;
}

unsigned int Model::textureFromFile(const char* filePath) {
	unsigned int id;
	glGenTextures(1, &id);
	int width, height, nrComponents;
	unsigned char* data = stbi_load(filePath, &width, &height, &nrComponents, 0);
	if (data) {
		GLenum format;
		if (nrComponents == 1) {
			format = GL_RED;
		}
		else if (nrComponents == 3) {
			format = GL_RGB;
		}
		else if (nrComponents == 4) {
			format = GL_RGBA;
		}
		else {
			//do nothing;
			std::cout << "Wrong image foramt!\n";
		}
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "Failed to load texture. \n";
		stbi_image_free(data);
	}
	return id;
}
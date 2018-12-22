#include "Mesh.h"
#include <glad/glad.h>
#include <iostream>



Mesh::Mesh(vector<Vertex> vertexs, vector<unsigned int> indexs, vector<Texture> textures)
{
	this->vertexVec = vertexs;
	this->indexVec = indexs;
	this->textureVec = textures;
	setupMesh();
}

Mesh::~Mesh()
{
}

void Mesh::draw(Shader s) {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	s.use();
	int diffIndex = 1;
	int specuIndex = 1;
	int normalIndex = 1;
	int heightIndex = 1;
	for (int i = 0; i < textureVec.size(); i++) {
		Texture& t = textureVec.at(i);
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, t.id);
		int index;
		if (t.type == "texture_diffuse") {
			index = diffIndex;
			diffIndex += 1;
		}
		else if (t.type == "texture_specular") {
			index = specuIndex;
			specuIndex += 1;
		}
		else if (t.type == "texture_normal") {
			index = normalIndex;
			normalIndex += 1;
		}
		else if (t.type == "texture_height") {
			index = heightIndex;
			heightIndex += 1;
		}
		else {
			std::cout << "Wrong texture type! \n";
		}
		s.setInt(("material." + t.type + std::to_string(index)).c_str(), i);
	}
	glDrawElements(GL_TRIANGLES, indexVec.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::print() {
	for (int i = 0; i < vertexVec.size(); i++) {
		auto vertex = vertexVec.at(i);
		//std::cout << "point = " << vertex.vertex.x << ", " << vertex.vertex.y << ", " << vertex.vertex.z << "\n";
	}
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertexVec.size(), &(vertexVec[0]), GL_STATIC_DRAW);
	Vertex* p = &vertexVec[0];
	//for (int i = 0; i < vertexVec.size(); i++) {
	//	std::cout << "vertexVec["<< i <<"].coordinate = " <<p->vertex.x<<","<<p->vertex.y<<","<<p->vertex.z<<std::endl;
	//	p++;
	//}
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//和nsight里的数据对不上EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indexVec.size(), &(indexVec[0]), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(3*sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(6*sizeof(float)));

}

vector<Vertex> arrayToVec(unsigned int size, const float a[]) {
	int num = size / sizeof(float);
	Vertex x;
	vector<Vertex> v;
	for (int i = 0; i < num; i = i+8) {
		x.vertex = glm::vec3(a[i], a[i+1], a[i+2]);
		x.normal = glm::vec3(a[i+3], a[i+4], a[i+5]);
		x.coordinate = glm::vec2(a[i+6], a[i+7]);
		v.push_back(x);
	}
	return v;
}

vector<unsigned int> initVecUint(unsigned int num) {
	num = num / 8;
	vector<unsigned int> uintVec;
	for (int i = 0; i < num; i++) {
		uintVec.push_back(i);
	}
	return uintVec;
}

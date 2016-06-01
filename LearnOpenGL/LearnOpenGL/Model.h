#pragma once

#include <iostream>
#include <vector>
using namespace std;

#include <GL/glew.h>
#include <glm\glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"
#include "Mesh.h"

class Model
{
public:
	Model(const char* path);
	~Model();
	void Draw(Shader *shader);
private:
	vector<Mesh> m_meshes;
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
};
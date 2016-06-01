#pragma once

#include <vector>
#include <GL\glew.h>
#include <glm\glm.hpp>

using namespace std;

#include "Shader.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec3 Tangent;
	glm::vec2 Texcoords;
};

class Mesh
{
public:
	Mesh(vector<Vertex> vertices, vector<GLuint> indices);
	~Mesh();
	void Draw(Shader *shader);
private:
	vector<Vertex> m_vertices;
	vector<GLuint> m_indices;
	GLuint m_vaoId, m_vboId, m_eboId;
	void setup();
};
#pragma once

#include "Mesh.h"
class ScreenQuadMesh : public Mesh
{
public:
	ScreenQuadMesh():Mesh()
	{
		vector<Vertex> vertices;

		Vertex v0;
		v0.Normal = glm::vec3(0, 0, 0);
		v0.Tangent = glm::vec3(0, 0, 0);
		v0.Position = glm::vec3(-1, -1, 0);
		v0.Texcoords = glm::vec2(0, 0);
		vertices.push_back(v0);

		Vertex v1;
		v1.Normal = glm::vec3(0, 0, 0);
		v1.Tangent = glm::vec3(0, 0, 0);
		v1.Position = glm::vec3(1, -1, 0);
		v1.Texcoords = glm::vec2(1, 0);
		vertices.push_back(v1);

		Vertex v2;
		v2.Normal = glm::vec3(0, 0, 0);
		v2.Tangent = glm::vec3(0, 0, 0);
		v2.Position = glm::vec3(1, 1, 0);
		v2.Texcoords = glm::vec2(1, 1);
		vertices.push_back(v2);

		Vertex v3;
		v3.Normal = glm::vec3(0, 0, 0);
		v3.Tangent = glm::vec3(0, 0, 0);
		v3.Position = glm::vec3(-1, 1, 0);
		v3.Texcoords = glm::vec2(0, 1);
		vertices.push_back(v3);

		vector<GLuint> indices;
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(0);
		indices.push_back(2);
		indices.push_back(3);

		this->Init(vertices, indices);
	}

	~ScreenQuadMesh()
	{

	}
};
#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices)
{
	this->Init(vertices, indices);
}

Mesh::~Mesh()
{
}

void Mesh::Draw(Shader *shader)
{
	glBindVertexArray(this->m_vaoId);
	glDrawElements(GL_TRIANGLES, this->m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::Init(vector<Vertex> vertices, vector<GLuint> indices)
{
	this->m_vertices = vertices;
	this->m_indices = indices;

	this->setup();
}

void Mesh::setup()
{
	glGenVertexArrays(1, &this->m_vaoId);
	glGenBuffers(1, &this->m_vboId);
	glGenBuffers(1, &this->m_eboId);

	glBindVertexArray(this->m_vaoId);
	
	glBindBuffer(GL_ARRAY_BUFFER, this->m_vboId);
	glBufferData(GL_ARRAY_BUFFER, this->m_vertices.size() * sizeof(Vertex), &this->m_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_eboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->m_indices.size() * sizeof(GLuint), &this->m_indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Tangent));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Texcoords));

	glBindVertexArray(0);
}

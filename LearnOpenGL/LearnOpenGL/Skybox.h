#pragma once

#include <SOIL\SOIL.h>

#include "Model.h"
#include "Shader.h"

class Skybox {
public:
	Skybox(vector<const char*> paths) {
		this->m_box = new Model("res/skybox.obj");

		this->m_cubeTex = Textures::LoadCubeImage(paths);

		this->m_shader = new Shader("shaders/pbr/Skybox.vs", "shaders/pbr/Skybox.fs");		
	}

	Skybox(GLuint texCube) {
		this->m_box = new Model("res/skybox.obj");

		this->m_cubeTex = texCube;

		this->m_shader = new Shader("shaders/pbr/Skybox.vs", "shaders/pbr/Skybox.fs");
	}

	~Skybox() {

	}

	void Render() {
		glDisable(GL_CULL_FACE);
		glDepthMask(false);
		this->m_shader->Use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->m_cubeTex);
		glUniform1i(glGetUniformLocation(this->m_shader->Program, "cube_texture"), 0);

		this->m_box->Draw(this->m_shader);
		glEnable(GL_CULL_FACE);
		glDepthMask(true);
	}
	Model *m_box;
	GLuint m_cubeTex;
	Shader *m_shader;
};
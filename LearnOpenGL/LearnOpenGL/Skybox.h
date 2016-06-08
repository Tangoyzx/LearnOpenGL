#pragma once

#include <SOIL\SOIL.h>

#include "Model.h"
#include "Shader.h"

class Skybox {
public:
	Skybox(vector<const char*> paths) {
		this->m_box = new Model("res/skybox.obj");

		int width, height;
		glGenTextures(1, &this->m_cubeTex);
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->m_cubeTex);

		for (int i = 0; i < 6; i++)
		{
			unsigned char * image = SOIL_load_image(paths[i], &width, &height, 0, SOIL_LOAD_RGB);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		this->m_shader = new Shader("shaders/pbr/Skybox.vs", "shaders/pbr/Skybox.fs");		
	}

	~Skybox() {

	}

	void Render() {
		glDisable(GL_CULL_FACE);
		this->m_shader->Use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->m_cubeTex);
		glUniform1i(glGetUniformLocation(this->m_shader->Program, "cube_texture"), 0);

		this->m_box->Draw(this->m_shader);
		glEnable(GL_CULL_FACE);
	}
private:
	Model *m_box;
	GLuint m_cubeTex;
	Shader *m_shader;
};
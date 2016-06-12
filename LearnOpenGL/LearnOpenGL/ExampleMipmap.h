#pragma once

#include "ExampleBase.h"
#include "Skybox.h"
#include "RenderObject.h"

class ExampleMipmap : public ExampleBase
{
public:
	void OnTick() override {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		SetupCommonUbo(this->m_camera, glm::vec4(0.625, 0.25, 0.125, 0));

		this->m_shader->Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->m_tex);
		glUniform1i(glGetUniformLocation(this->m_shader->Program, "tex_diffuse"), 0);
		this->m_renderObj->Render(this->m_shader);

		this->m_shader_1->Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->m_tex);
		glUniform1i(glGetUniformLocation(this->m_shader_1->Program, "tex_diffuse"), 0);
		
		this->m_renderObj_1->Render(this->m_shader_1);
	}

protected:
	void ChildInit() override {

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_BACK);
		glDepthFunc(GL_LEQUAL);

		auto model = new Model("res/plane.obj");
		this->m_renderObj = new RenderObject(model);
		this->m_renderObj_1 = new RenderObject(model);
		this->m_shader = new Shader("shaders/Simple.vs", "shaders/mipmap/Mipmap.fs");
		this->m_shader_1 = new Shader("shaders/Simple.vs", "shaders/mipmap/Mipmap_1.fs");

		this->GenTexture();

		this->m_renderObj->GetTransform()->SetRotation(glm::radians(90.0f), 0, 0);
		this->m_renderObj->GetTransform()->SetTranslation(0, 1, -3);

		this->m_renderObj_1->GetTransform()->SetRotation(glm::radians(90.0f), 0, 0);
		this->m_renderObj_1->GetTransform()->SetTranslation(0, -1, -3);

		this->m_camera->GetTransform()->SetTranslation(0, 0, 0);
	}

	void GenTexture()
	{
		int path_num = 8;

		glGenTextures(1, &this->m_tex);
		glBindTexture(GL_TEXTURE_2D, this->m_tex);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		int width, height;
		
		unsigned char * image = SOIL_load_image("res/skybox/front.jpg", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	RenderObject *m_renderObj, *m_renderObj_1;
	Shader *m_shader, *m_shader_1;
	GLuint m_tex;
};
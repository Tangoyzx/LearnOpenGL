#pragma once

#include <SOIL\SOIL.h>

#include "ExampleBase.h"
#include "Shader.h"
#include "RenderObject.h"
#include "Textures.h"

class ExampleIBL : public ExampleBase
{
public:
	void OnTick() override 
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		this->SetupCommonUbo(this->m_camera, glm::vec4(-1, 1, -1, 0));

		this->m_skybox->Render();

		this->m_shader->Use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->m_texDiffuse);
		glUniform1i(glGetUniformLocation(this->m_shader->Program, "texDiffuse"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, this->m_texSpecular);
		glUniform1i(glGetUniformLocation(this->m_shader->Program, "texSpecular"), 1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, this->m_texLut);
		glUniform1i(glGetUniformLocation(this->m_shader->Program, "texLut"), 2);

		for (int i = 0; i < this->m_roughness.size(); i++)
		{
			glUniform1f(glGetUniformLocation(this->m_shader->Program, "roughness"), this->m_roughness[i]);

			float xx = floor(this->m_roughness[i] * this->m_cubeTexs.size());
			int indexCube = floor(this->m_roughness[i] * this->m_cubeTexs.size());
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_CUBE_MAP, this->m_cubeTexs[indexCube]);
			glUniform1i(glGetUniformLocation(this->m_shader->Program, "texCube"), 3);

			this->m_renderObj->GetTransform()->SetTranslation((i % 4) * 3, floorf(i / 4) * 3, 0);

			this->m_renderObj->Render(this->m_shader);
		}
	}

protected:
	void ChildInit() override 
	{
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_BACK);
		glDepthFunc(GL_LESS);

		auto model = new Model("res/sphere.obj");
		this->m_renderObj = new RenderObject(model);

		this->addCubeTex("res/skybox/d512.dds");
		this->addCubeTex("res/skybox/d256.dds");
		this->addCubeTex("res/skybox/d128.dds");
		this->addCubeTex("res/skybox/d64.dds");
		this->addCubeTex("res/skybox/d32.dds");
		this->addCubeTex("res/skybox/d16.dds");
		this->addCubeTex("res/skybox/d8.dds");
		this->addCubeTex("res/skybox/d4.dds");

		this->m_skybox = new Skybox(this->m_cubeTexs[0]);

		this->m_shader = new Shader("shaders/Simple.vs", "shaders/pbr/UE4.fs");

		this->m_camera->GetTransform()->SetTranslation(0, 0, 4);

		for (int i = 0; i < 19; i++)
		{
			this->m_roughness.push_back(i * 0.05 + 0.05);
		}

		int width, height;
		this->m_texDiffuse = Textures::LoadImage("res/pbr/diffuse.jpg", &width, &height, 0, SOIL_LOAD_RGB);
		this->m_texSpecular = Textures::LoadImage("res/pbr/specular.jpg", &width, &height, 0, SOIL_LOAD_RGB);
		this->m_texLut = Textures::LoadImage("res/lut.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	}

	void addCubeTex(const char* path)
	{
		this->m_cubeTexs.push_back(SOIL_load_OGL_single_cubemap(path, SOIL_DDS_CUBEMAP_FACE_ORDER, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MIPMAPS | SOIL_FLAG_DDS_LOAD_DIRECT));
	}

	Skybox *m_skybox;
	RenderObject *m_renderObj;
	Shader *m_shader;
	GLuint m_texDiffuse, m_texSpecular, m_texLut;
	vector<GLuint> m_cubeTexs;
	vector<float> m_roughness;
};
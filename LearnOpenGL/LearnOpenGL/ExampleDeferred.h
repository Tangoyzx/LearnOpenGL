#pragma once

#include "ExampleBase.h"
#include "ScreenQuadMesh.h"
#include "Shader.h"
#include "RenderObject.h"

class ExampleDeferred : public ExampleBase
{
public:
	void OnTick() override {
		this->RenderGBufferGeometry();
		this->RenderGBufferLighting();
	}
protected:
	ScreenQuadMesh *m_screenQuad;

	Shader *m_gGeometryShader, *m_gLightingShader;

	GLuint m_gBuffer, m_texPosition, m_texNormal, m_texAlbedoSpec, m_rboDepth, m_texDiffuse, m_texSpec;
	
	vector<glm::vec3> m_lightsColor;
	vector<glm::vec4> m_lightsPos;

	vector<RenderObject*> m_renderObjs;
	RenderObject *m_desk;
	
	int SCREEN_WIDTH = 640;
	int SCREEN_HEIGHT = 640;

	void RenderGBufferGeometry()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, this->m_gBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		this->SetupCommonUbo(this->m_camera, glm::vec4(0, 0, 0, 1));
		this->m_gGeometryShader->Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->m_texDiffuse);
		glUniform1i(glGetUniformLocation(this->m_gGeometryShader->Program, "tex_diffuse"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, this->m_texSpec);
		glUniform1i(glGetUniformLocation(this->m_gGeometryShader->Program, "tex_spec"), 1);

		this->m_desk->Render(this->m_gGeometryShader);
		for (int i = 0; i < this->m_renderObjs.size(); i++)
		{
			this->m_renderObjs[i]->Render(this->m_gGeometryShader);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderGBufferLighting()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		this->m_gLightingShader->Use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->m_texPosition);
		glUniform1i(glGetUniformLocation(this->m_gLightingShader->Program, "tex_position"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, this->m_texNormal);
		glUniform1i(glGetUniformLocation(this->m_gLightingShader->Program, "tex_normal"), 1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, this->m_texAlbedoSpec);
		glUniform1i(glGetUniformLocation(this->m_gLightingShader->Program, "tex_albedoSpec"), 2);

		for (GLuint i = 0; i < this->m_lightsColor.size(); i++)
		{
			glUniform4fv(glGetUniformLocation(this->m_gLightingShader->Program, ("lights[" + std::to_string(i) + "].Position").c_str()), 1, &m_lightsPos[i][0]);
			glUniform3fv(glGetUniformLocation(this->m_gLightingShader->Program, ("lights[" + std::to_string(i) + "].Color").c_str()), 1, &m_lightsColor[i][0]);
			glUniform1f(glGetUniformLocation(this->m_gLightingShader->Program, ("lights[" + std::to_string(i) + "].isLinear").c_str()), 0.0f);
		}

		this->m_screenQuad->Draw(this->m_gLightingShader);
	}

	void InitGBuffer()
	{
		glGenFramebuffers(1, &this->m_gBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, this->m_gBuffer);

		glGenTextures(1, &this->m_texPosition);
		glBindTexture(GL_TEXTURE_2D, this->m_texPosition);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->m_texPosition, 0);
		// - Normal color buffer
		glGenTextures(1, &this->m_texNormal);
		glBindTexture(GL_TEXTURE_2D, this->m_texNormal);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, this->m_texNormal, 0);
		// - Color + Specular color buffer
		glGenTextures(1, &this->m_texAlbedoSpec);
		glBindTexture(GL_TEXTURE_2D, this->m_texAlbedoSpec);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, this->m_texAlbedoSpec, 0);

		GLuint attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
		glDrawBuffers(3, attachments);

		glGenRenderbuffers(1, &this->m_rboDepth);
		glBindRenderbuffer(GL_RENDERBUFFER, this->m_rboDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCREEN_WIDTH, SCREEN_HEIGHT);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->m_rboDepth);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		this->m_gGeometryShader = new Shader("shaders/deferred/DeferredGeometry.vs", "shaders/deferred/DeferredGeometry.fs");
		this->m_gLightingShader = new Shader("shaders/deferred/DeferredLighting.vs", "shaders/deferred/DeferredLighting.fs");
	}

	void ChildInit() override
	{
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_BACK);
		glDepthFunc(GL_LESS);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		this->InitGBuffer();

		this->m_screenQuad = new ScreenQuadMesh();

		auto model = new Model("res/sphere.obj");


		for (int i = 0; i < 10; i++) 
		{
			for (int j = 0; j < 10; j++) 
			{
				auto ro = new RenderObject(model);
				ro->GetTransform()->SetTranslation(i * 2 - 10, -3, -j * 2);
				this->m_renderObjs.push_back(ro);
			}
		}

		this->m_desk = new RenderObject(new Model("res/cube.obj"));
		this->m_desk->GetTransform()->SetTranslation(-1, -5, -9);
		this->m_desk->GetTransform()->SetScale(10, 1, 10);

		for (int i = 0; i < 100; i++)
		{
			this->m_lightsPos.push_back(glm::vec4(rand()%20 - 10, rand() % 4 - 5, -rand() % 20, 1));
			this->m_lightsColor.push_back(glm::vec4(rand() % 100*0.01, rand() % 100 * 0.01, rand() % 100 * 0.01, 1));
			//this->m_lightsColor.push_back(glm::vec4(1, 0, 0, 1));
		}


		int width, height;
		this->m_texDiffuse = Textures::LoadImage("res/box_diffuse.png", &width, &height, 0, SOIL_LOAD_RGB);
		this->m_texSpec = Textures::LoadImage("res/box_spec.png", &width, &height, 0, SOIL_LOAD_RGB);
	}
};
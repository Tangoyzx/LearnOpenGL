#pragma once

#include "ExampleBase.h"
#include "ScreenQuadMesh.h"
#include "Shader.h"
#include "RenderObject.h"

class ExampleShadow : public ExampleBase
{
public:
	void OnTick() override {
		this->DrawShadowMap();

		if (false)
		{
			this->ShowShadowMap();
		}
		else
		{
			this->DrawLighting();
		}
	}
protected:
	void ChildInit() override
	{
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_BACK);
		glDepthFunc(GL_LESS);

		this->InitShadowFbo();

		this->lightPos = glm::vec4(4, 10, -3, 1);

		this->m_shadowCam = new Camera(glm::radians(90.0f), 1);
		this->m_shadowCam->GetTransform()->SetRotation(glm::radians(-90.0f), 0, 0);
		this->m_shadowCam->GetTransform()->SetTranslation(this->lightPos);

		auto model = new Model("res/cube.obj");
		this->m_obj1 = new RenderObject(model);
		this->m_obj2 = new RenderObject(model);

		this->m_obj1->GetTransform()->SetTranslation(0, 0, -3);
		this->m_obj1->GetTransform()->SetRotation(glm::radians(45.0f), 0, 0);

		this->m_obj2->GetTransform()->SetTranslation(0, -3, -3);
		this->m_obj2->GetTransform()->SetScale(5, 1, 5);

		this->m_shadowShader = new Shader("shaders/shadow/Shadow.vs", "shaders/shadow/Shadow.fs");
		this->m_showTexShader = new Shader("shaders/ShowQuad.vs", "shaders/ShowQuad.fs");
		this->m_lightShader = new Shader("shaders/shadow/ShadowLighting.vs", "shaders/shadow/ShadowLighting.fs");

		this->m_quad = new ScreenQuadMesh();
	}

	void InitShadowFbo()
	{
		glGenFramebuffers(1, &this->m_fboShadow);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_BACK);
		glDepthFunc(GL_LESS);

		glBindFramebuffer(GL_FRAMEBUFFER, this->m_fboShadow);
		glGenTextures(1, &this->m_texShadow);
		glBindTexture(GL_TEXTURE_2D, this->m_texShadow);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 640, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->m_texShadow, 0);

		GLuint depthBuffer;
		glGenRenderbuffers(1, &depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 640, 640);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void DrawShadowMap()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, this->m_fboShadow);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		this->SetupCommonUbo(this->m_shadowCam, glm::vec4(0, 1, 0, 0));

		this->m_shadowShader->Use();

		this->m_obj1->Render(this->m_shadowShader);
		this->m_obj2->Render(this->m_shadowShader);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void DrawLighting()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		this->SetupCommonUbo(this->m_camera, this->lightPos);

		auto shadowVP = this->m_shadowCam->GetProjectionMatrix() * this->m_shadowCam->GetViewMatrix();

		this->m_lightShader->Use();
		glUniformMatrix4fv(glGetUniformLocation(this->m_lightShader->Program, "shadowVP"), 1, false, glm::value_ptr(shadowVP));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->m_texShadow);
		glUniform1i(glGetUniformLocation(this->m_lightShader->Program, "texShadowMap"), 0);

		this->m_obj1->Render(this->m_lightShader);
		this->m_obj2->Render(this->m_lightShader);
	}

	void ShowShadowMap()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		this->SetupCommonUbo(this->m_camera, glm::vec4(1, 0, 0, 1));

		this->m_showTexShader->Use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->m_texShadow);
		glUniform1i(glGetUniformLocation(this->m_showTexShader->Program, "customTex"), 0);

		this->m_quad->Draw(this->m_showTexShader);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	glm::vec4 lightPos;

	Camera *m_shadowCam;

	GLuint m_fboShadow, m_texShadow;

	RenderObject *m_obj1, *m_obj2;
	Shader *m_shadowShader, *m_showTexShader, *m_lightShader;
	ScreenQuadMesh *m_quad;
};
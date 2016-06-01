#pragma once

#include "ExampleBase.h"
#include "RenderObject.h"
#include "Model.h"
#include "Textures.h"
#include "PostEffectHelper.h"

class Example1 : public ExampleBase {
public:
	RenderObject *m_renderObj;
	Shader *m_shader;
	virtual void OnTick() override {
		glBindFramebuffer(GL_FRAMEBUFFER, this->m_post->m_fboId);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		
		glm::vec3 pos = this->m_camera->GetTransform()->GetTranslation();
		SetupCommonUbo(this->m_camera, glm::vec4(0.625, 0.25, 0.125, 0));

		this->m_shader->Use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->m_parallaxMap);
		glUniform1i(glGetUniformLocation(this->m_shader->Program, "parallax_map"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, this->m_normalMap);
		glUniform1i(glGetUniformLocation(this->m_shader->Program, "normal_map"), 1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, this->m_diffuseMap);
		glUniform1i(glGetUniformLocation(this->m_shader->Program, "diffuse_map"), 2);


		this->m_renderObj->Render(this->m_shader);		
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		this->m_post->Render(this->m_postShader);
	}
protected:
	GLuint m_parallaxMap, m_normalMap, m_diffuseMap;
	PostEffectHelper *m_post;
	Shader *m_postShader;

	void ChildInit() override {
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_BACK);
		glDepthFunc(GL_LEQUAL);
		auto model = new Model("res/cube.obj");
		this->m_renderObj = new RenderObject(model);
		this->m_renderObj->GetTransform()->SetTranslation(0, 0, 0);
		this->m_camera->GetTransform()->SetTranslation(0, 0, 4);
		this->m_shader = new Shader("shaders/TangentVertex.vs", "shaders/ParallaxMapping.fs");

		int width, height;
		this->m_parallaxMap = Textures::LoadImage("res/parallax_2.png", &width, &height, 0, SOIL_LOAD_RGB);
		this->m_normalMap = Textures::LoadImage("res/normal_2.png", &width, &height, 0, SOIL_LOAD_RGB);
		this->m_diffuseMap = Textures::LoadImage("res/diffuse_2.png", &width, &height, 0, SOIL_LOAD_RGB);

		this->m_postShader = new Shader("shaders/PostEffect.vs", "shaders/PostEffect.fs");
		this->m_post = new PostEffectHelper();
	}
};
#pragma once

#include "ExampleBase.h"
#include "ScreenQuadMesh.h"
#include "Shader.h"
#include "RenderObject.h"

class ExampleSSAO : public ExampleBase
{
public:
	void OnTick() override {
		this->RenderGBufferGeometry();
		this->RenderSSAO();
		//this->RenderBlurSSAO();
		//this->RenderGBufferLighting();
	}
protected:
	ScreenQuadMesh *m_screenQuad;

	Shader *m_gGeometryShader, *m_gLightingShader, *m_shaderSSAO, *m_shaderBlurSSAO;

	GLuint m_gBuffer, m_texPosition, m_texNormal, m_texAlbedoSpec, m_rboDepth, m_texDiffuse, m_texSpec;

	vector<glm::vec3> m_lightsColor;
	vector<glm::vec4> m_lightsPos;

	vector<RenderObject*> m_renderObjs;
	RenderObject *m_desk, *m_cube, *m_sphere;

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
		this->m_cube->Render(this->m_gGeometryShader);
		this->m_sphere->Render(this->m_gGeometryShader);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderSSAO()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, this->m_fboSSAO);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		this->m_shaderSSAO->Use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->m_texPosition);
		glUniform1i(glGetUniformLocation(this->m_shaderSSAO->Program, "tex_position"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, this->m_texNormal);
		glUniform1i(glGetUniformLocation(this->m_shaderSSAO->Program, "tex_normal"), 1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, this->m_texNoise);
		glUniform1i(glGetUniformLocation(this->m_shaderSSAO->Program, "tex_noise"), 2);

		for (GLuint i = 0; i < 64; ++i)
		{
			glUniform3fv(glGetUniformLocation(this->m_shaderSSAO->Program, ("samples[" + std::to_string(i) + "]").c_str()), 1, &this->m_ssaoKernel[i][0]);
		}

		this->m_screenQuad->Draw(this->m_shaderSSAO);
	}

	void RenderBlurSSAO()
	{

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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->m_texPosition, 0);
		// - Normal color buffer
		glGenTextures(1, &this->m_texNormal);
		glBindTexture(GL_TEXTURE_2D, this->m_texNormal);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, this->m_texNormal, 0);
		// - Color + Specular color buffer
		glGenTextures(1, &this->m_texAlbedoSpec);
		glBindTexture(GL_TEXTURE_2D, this->m_texAlbedoSpec);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, this->m_texAlbedoSpec, 0);

		GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
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
		this->InitNoise();
		this->InitSSAO();

		this->m_screenQuad = new ScreenQuadMesh();

		auto model = new Model("res/sphere.obj");

		this->m_desk = new RenderObject(new Model("res/cube.obj"));
		this->m_desk->GetTransform()->SetTranslation(-1, -5, -9);
		this->m_desk->GetTransform()->SetScale(3, 1, 3);

		this->m_cube = new RenderObject(new Model("res/cube.obj"));
		this->m_cube->GetTransform()->SetTranslation(-1, -3, -9);

		this->m_sphere = new RenderObject(new Model("res/sphere.obj"));
		this->m_sphere->GetTransform()->SetTranslation(1, -3, -9);

		for (int i = 0; i < 100; i++)
		{
			this->m_lightsPos.push_back(glm::vec4(rand() % 20 - 10, rand() % 4 - 5, -rand() % 20, 1));
			this->m_lightsColor.push_back(glm::vec4(rand() % 100 * 0.01, rand() % 100 * 0.01, rand() % 100 * 0.01, 1));
			//this->m_lightsColor.push_back(glm::vec4(1, 0, 0, 1));
		}


		int width, height;
		this->m_texDiffuse = Textures::LoadImage("res/box_diffuse.png", &width, &height, 0, SOIL_LOAD_RGB);
		this->m_texSpec = Textures::LoadImage("res/box_spec.png", &width, &height, 0, SOIL_LOAD_RGB);
	}

	void InitNoise()
	{
		// Sample kernel
		std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
		std::default_random_engine generator;
		for (GLuint i = 0; i < 64; ++i)
		{
			glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
			sample = glm::normalize(sample);
			sample *= randomFloats(generator);
			GLfloat scale = GLfloat(i) / 64.0;

			// Scale samples s.t. they're more aligned to center of kernel
			scale = lerp(0.1f, 1.0f, scale * scale);
			sample *= scale;
			this->m_ssaoKernel.push_back(sample);
		}

		// Noise texture
		std::vector<glm::vec3> ssaoNoise;
		for (GLuint i = 0; i < 16; i++)
		{
			glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
			ssaoNoise.push_back(noise);
		}
		glGenTextures(1, &this->m_texNoise);
		glBindTexture(GL_TEXTURE_2D, this->m_texNoise);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void InitSSAO()
	{
		glGenFramebuffers(1, &this->m_fboSSAO);
		glBindFramebuffer(GL_FRAMEBUFFER, this->m_fboSSAO);
		glGenTextures(1, &this->m_texSSAO);
		glBindTexture(GL_TEXTURE_2D, this->m_texSSAO);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 640, 640, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->m_texSSAO, 0);

		glGenFramebuffers(1, &this->m_fboBlurSSAO);
		glBindFramebuffer(GL_FRAMEBUFFER, this->m_fboBlurSSAO);
		glGenTextures(1, &this->m_texBlurSSAO);
		glBindTexture(GL_TEXTURE_2D, this->m_texBlurSSAO);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 640, 640, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->m_texBlurSSAO, 0);

		this->m_shaderSSAO = new Shader("shaders/deferred/DeferredLighting.vs", "shaders/deferred/SSAO.fs");
		this->m_shaderBlurSSAO = new Shader("shaders/deferred/DeferredLighting.vs", "shaders/deferred/SSAOBlur.fs");
	}

	float lerp(float a, float b, float t)
	{
		return a + (b - a) * t;
	}

	vector<glm::vec3> m_ssaoKernel;
	GLuint m_texNoise, m_texSSAO, m_texBlurSSAO, m_fboSSAO, m_fboBlurSSAO;
};
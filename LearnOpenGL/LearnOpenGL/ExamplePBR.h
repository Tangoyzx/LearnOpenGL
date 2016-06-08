#pragma once

#include "ExampleBase.h"
#include "Skybox.h"

class ExamplePBR : public ExampleBase
{
public:
	void OnTick() override {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);

		this->SetupCommonUbo(this->m_camera, glm::vec4(3, 2, 5, 0));

		this->m_skybox->Render();

		glDepthMask(GL_TRUE);

		for (int i = 0; i < this->fragment_size; i++)
		{
			this->m_shaders[i]->Use();
			this->m_ros[i]->Render(this->m_shaders[i]);
		}
	}

protected:
	const int fragment_size = 2;
	void ChildInit() override {
		auto paths = vector<const char*>{ "res/skybox/right.jpg", "res/skybox/left.jpg" , "res/skybox/top.jpg", "res/skybox/bottom.jpg" , "res/skybox/back.jpg", "res/skybox/front.jpg" };

		this->m_skybox = new Skybox(paths);

		string fragments[] = { "shaders/pbr/Klay.fs", "shaders/pbr/BlackOP.fs" };

		auto model = new Model("res/sphere.obj");

		const char *vertexPath = "shaders/Simple.vs";
		
		for (int i = 0; i < this->fragment_size; i++)
		{
			auto ro = new RenderObject(model);
			auto shader = new Shader(vertexPath, fragments[i]);

			this->m_ros.push_back(ro);
			this->m_shaders.push_back(shader);
			ro->GetTransform()->SetTranslation(i * 3, 0, -4);
		}


		this->m_camera->GetTransform()->SetTranslation(0, 0, 4);
	}

	Skybox *m_skybox;
	vector<Shader*> m_shaders;
	vector<RenderObject*> m_ros;
};
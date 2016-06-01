#pragma once

#include "Camera.h"
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class ExampleBase
{
public:
	ExampleBase();
	~ExampleBase();
	void Init(Camera *camera, GLuint commonUboId);
	virtual void OnTick();
protected:
	Camera* m_camera;
	GLuint m_commonUboId;
	void SetupCommonUbo(Camera *camera, glm::vec4 &lightPos);
	virtual void ChildInit();
};
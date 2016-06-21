#include "ExampleBase.h"




ExampleBase::ExampleBase()
{
}


ExampleBase::~ExampleBase()
{
}

void ExampleBase::Init(Camera * camera, GLuint commonUboId)
{
	this->m_camera = camera;
	this->m_commonUboId = commonUboId;

	this->ChildInit();
}

void ExampleBase::OnTick()
{
}

void ExampleBase::SetupCommonUbo(Camera *camera, glm::vec4 &lightPos)
{
	glBindBuffer(GL_UNIFORM_BUFFER, this->m_commonUboId);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, 64, glm::value_ptr(camera->GetProjectionMatrix()));
	glBufferSubData(GL_UNIFORM_BUFFER, 64, 64, glm::value_ptr(camera->GetViewMatrix()));
	glBufferSubData(GL_UNIFORM_BUFFER, 128, 16, glm::value_ptr(camera->GetTransform()->GetTranslation()));
	glBufferSubData(GL_UNIFORM_BUFFER, 144, 16, glm::value_ptr(lightPos));
	glBufferSubData(GL_UNIFORM_BUFFER, 160, 16, glm::value_ptr(glm::vec4(camera->GetNear(), camera->GetFar(), camera->getTanHalfFov(), camera->getAspect())));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ExampleBase::ChildInit()
{
}
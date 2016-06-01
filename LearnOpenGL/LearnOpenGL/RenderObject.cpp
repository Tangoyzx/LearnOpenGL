#include "RenderObject.h"



RenderObject::RenderObject(Model * model)
{
	this->m_model = model;
	this->m_transform = new Matrix();
}

RenderObject::~RenderObject()
{
}

void RenderObject::Render(Shader * shader)
{
	//shader->Use();
	GLint modelLoc = glGetUniformLocation(shader->Program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(this->m_transform->glmMat()));

	GLint invModelLoc = glGetUniformLocation(shader->Program, "invModel");
	auto bbb = glm::inverse(this->m_transform->glmMat());
	glUniformMatrix4fv(invModelLoc, 1, GL_FALSE, glm::value_ptr(glm::inverse(this->m_transform->glmMat())));

	this->m_model->Draw(shader);
}

Matrix * RenderObject::GetTransform()
{
	return this->m_transform;
}

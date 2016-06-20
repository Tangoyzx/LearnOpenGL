#include "Camera.h"
#include <glm\gtc\matrix_transform.hpp>


Camera::Camera()
{
	
}

Camera::Camera(float fov, float aspect, float near, float far)
{
	this->m_transform = new Matrix();
	this->m_projection = glm::perspective(fov, aspect, near, far);
	this->m_inverseProjection = glm::inverse(this->m_projection);
	this->m_near = near;
	this->m_far = far;
}


Camera::~Camera()
{
}

Matrix * Camera::GetTransform()
{
	return this->m_transform;
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::inverse(this->m_transform->glmMat());
}

glm::mat4 Camera::GetProjectionMatrix()
{
	return this->m_projection;
}

float Camera::GetNear()
{
	return this->m_near;
}

float Camera::GetFar()
{
	return this->m_far;
}

glm::mat4 Camera::GetInverseProjectionMatrix()
{
	return this->m_inverseProjection;
}

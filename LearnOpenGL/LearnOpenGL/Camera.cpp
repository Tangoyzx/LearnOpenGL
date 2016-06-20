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



	this->GetTransform()->SetTranslation(0, 5, 0);

	auto worldPos = glm::vec4(-2, -3, -far, 1);
	auto viewPos = this->GetViewMatrix() * worldPos;
	auto p_pos = this->m_projection * viewPos;

	auto screenUv = glm::vec2(0, 0);
	auto depth = far;


	float a = -(far + near) / (far - near);
	float b = -(2 * far * near) / (far - near);

	auto newZ = a * -depth + b;
	auto newW = depth;
	
	auto new_p = glm::vec4(screenUv.x * newW, screenUv.y * newW, newZ, newW);
	auto new_view_p = this->m_inverseProjection * new_p;
	auto new_world_p = this->m_transform->glmMat() * new_view_p;

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

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
	this->m_tanHalfFov = glm::tan(fov * 0.5);
	this->m_aspect = aspect;

	auto viewPos = glm::vec4(7, 2, -10, 1);
	auto p_pos = this->m_projection * viewPos;

	auto screenUv = glm::vec2((p_pos.x / p_pos.w) * 0.5 + 0.5, (p_pos.y / p_pos.w) * 0.5 + 0.5);
	auto viewZ = viewPos.z;

	auto halfFov = glm::tan(fov * 0.5f);

	auto viewRay = glm::vec2(screenUv.x * aspect * halfFov, screenUv.y * halfFov);

	auto viewXY = glm::vec2(viewRay.x * viewZ, viewRay.y * viewZ);

	auto screenXY = glm::vec2(screenUv.x * 2 - 1, screenUv.y * 2 - 1);

	auto viewX = screenXY.x * halfFov * aspect * -viewZ;
	auto viewY = screenXY.y * halfFov * -viewZ;

	auto vvv = glm::vec3(viewX, viewY, viewZ);
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

float Camera::getTanHalfFov()
{
	return this->m_tanHalfFov;
}

float Camera::getAspect()
{
	return this->m_aspect;
}

glm::mat4 Camera::GetInverseProjectionMatrix()
{
	return this->m_inverseProjection;
}

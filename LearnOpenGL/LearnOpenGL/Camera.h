#pragma once

#include "Matrix.h"

class Camera
{
public:
	Camera();
	Camera(float fov, float aspect, float near = 0.1f, float far = 50.0f);
	~Camera();
	Matrix* GetTransform();
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();
	bool isUse;
	float GetNear();
	float GetFar();
	float getTanHalfFov();
	float getAspect();
	glm::mat4 GetInverseProjectionMatrix();
private:
	float m_near, m_far, m_tanHalfFov, m_aspect;
	Matrix *m_transform;
	glm::mat4 m_projection, m_inverseProjection;
};
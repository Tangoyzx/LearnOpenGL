#pragma once

#include <glm/glm.hpp>

enum Space {
	WORLD,
	LOCAL
};

class Matrix
{
public:
	Matrix();
	~Matrix();
	void Rotate(float angle, const glm::vec3 axis, Space space = Space::WORLD);
	glm::vec3 forward();
	glm::vec3 right();
	glm::mat4 glmMat();

	void SetTranslation(glm::vec3 value);
	void SetTranslation(float tx, float ty, float tz);
	glm::vec3 GetTranslation() const;

	void SetRotation(glm::vec3 value);
	void SetRotation(float rx, float ry, float rz);
	glm::vec3 GetRotation() const;

	void SetScale(glm::vec3 value);
	void SetScale(float sx, float sy, float sz);
	glm::vec3 GetScale() const;

	void LookAt(glm::vec3 targetPos, glm::vec3 up);
	void SetMatrix(glm::mat4 mat);
	static float limitAngle(float angle, float area);

private:
	glm::mat4 m_matrix;
	glm::mat4 m_rotationMat;
	glm::vec3 m_position, m_rotation, m_scale;
	bool m_isDirty, m_isRotationDirty;
	glm::vec3 getEulerAngles(const glm::mat4 &mat) const;
	void refreshMatrix();
	void refreshRotationMatrix();
};
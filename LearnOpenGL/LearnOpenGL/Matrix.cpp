#include "Matrix.h"
#include <glm/gtc/matrix_transform.hpp>


Matrix::Matrix()
{
	this->m_matrix = glm::mat4(1.0f);
	this->m_rotationMat = glm::mat4(1.0f);
	this->m_position = glm::vec3(0, 0, 0);
	glm::mat4 dd = glm::mat4(1.0f);
	this->m_rotation = glm::vec3(0, 0, 0);
	this->m_scale = glm::vec3(1, 1, 1);
	this->m_isDirty = this->m_isRotationDirty = false;
}

Matrix::~Matrix()
{
}

void Matrix::Rotate(float angle, const glm::vec3 axis, Space space)
{
	this->refreshRotationMatrix();

	if (space == Space::LOCAL) {
		this->m_rotationMat = glm::rotate(this->m_rotationMat, angle, axis);
	}
	else {
		glm::mat4 rotate_matrix = glm::rotate(glm::mat4(1), angle, axis);
		this->m_rotationMat = rotate_matrix * this->m_rotationMat;
	}

	this->m_rotation = getEulerAngles(this->m_rotationMat);
}

glm::vec3 Matrix::forward()
{
	this->refreshMatrix();
	glm::vec4 xxx = this->m_matrix * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
	return glm::vec3(xxx);
}

glm::vec3 Matrix::right()
{
	this->refreshMatrix();
	return glm::vec3(this->m_matrix * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
}

glm::mat4 Matrix::glmMat()
{
	glm::mat4 xxxxx = this->m_rotationMat;
	if (this->m_isDirty) {
		this->refreshMatrix();
	}
	return this->m_matrix;
}

void Matrix::SetTranslation(glm::vec3 value)
{
	if (this->m_position == value)
		return;
	this->m_position = value;
	this->m_isDirty = true;
}

void Matrix::SetTranslation(float tx, float ty, float tz)
{
	if ((this->m_position.x == tx) && (this->m_position.y == ty) && (this->m_position.z == tz))
		return;
	this->m_position.x = tx;
	this->m_position.y = ty;
	this->m_position.z = tz;
	this->m_isDirty = true;
}

glm::vec3 Matrix::GetTranslation() const
{
	return this->m_position;
}

void Matrix::SetRotation(glm::vec3 value)
{
	this->SetRotation(value.x, value.y, value.z);
}

void Matrix::SetRotation(float rx, float ry, float rz)
{
	/*
	rx = limitAngle(rx, glm::radians(90.0f));
	ry = limitAngle(ry, glm::radians(180.0f));
	rz = limitAngle(rz, glm::radians(180.0f));
	if ((this->m_rotation.x == rx) && (this->m_rotation.y == ry) && (this->m_rotation.z == rz))
	return;
	*/
	this->m_rotation.x = rx;
	this->m_rotation.y = ry;
	this->m_rotation.z = rz;
	this->m_isDirty = true;
	this->m_isRotationDirty = true;
}

glm::vec3 Matrix::GetRotation() const
{
	return this->m_rotation;
}

void Matrix::SetScale(glm::vec3 value)
{
	if (this->m_scale == value)
		return;
	this->m_scale = value;
	this->m_isDirty = true;
}

void Matrix::SetScale(float sx, float sy, float sz)
{
	if ((this->m_scale.x == sx) && (this->m_scale.y == sy) && (this->m_scale.z == sz))
		return;
	this->m_scale.x = sx;
	this->m_scale.y = sy;
	this->m_scale.z = sz;
	this->m_isDirty = true;
}

glm::vec3 Matrix::GetScale() const
{
	return this->m_scale;
}

void Matrix::LookAt(glm::vec3 targetPos, glm::vec3 up)
{
	glm::vec3 axis_z = glm::normalize(this->m_position - targetPos);
	//glm::vec3 axis_z = glm::normalize(targetPos - this->m_position);
	glm::vec3 axis_x = glm::normalize(glm::cross(up, axis_z));
	glm::vec3 axis_y = glm::normalize(glm::cross(axis_z, axis_x));

	glm::vec3 euler = glm::vec3();
	euler.x = glm::asin(axis_y.z);
	float cos_x = glm::cos(euler.x);
	euler.y = glm::asin(-axis_x.z / cos_x);
	euler.z = glm::asin(-axis_y.x / cos_x);

	this->SetRotation(euler);
}

void Matrix::SetMatrix(glm::mat4 mat)
{
	this->m_matrix = mat;
	this->m_isDirty = false;

	this->m_rotation = getEulerAngles(mat);
	this->m_isRotationDirty = true;
	this->refreshRotationMatrix();

	glm::mat4 invRot = glm::inverse(this->m_rotationMat);
	glm::mat4 noRotMat = invRot * this->m_matrix;
	this->m_scale = glm::vec3(noRotMat[0].x, noRotMat[1].y, noRotMat[2].z);
	this->m_position = glm::vec3(noRotMat[0].w, noRotMat[1].w, noRotMat[2].w);
}

glm::vec3 Matrix::getEulerAngles(const glm::mat4 & mat) const
{
	glm::vec3 euler = glm::vec3();
	euler.x = glm::asin(mat[1].z);
	float cos_x = glm::cos(euler.x);
	euler.y = glm::acos(mat[2].z / cos_x);
	euler.z = glm::acos(mat[1].y / cos_x);
	return euler;
}

void Matrix::refreshMatrix()
{
	if (!this->m_isDirty)
		return;
	this->refreshRotationMatrix();
	this->m_matrix = glm::translate(glm::mat4(1.0), this->m_position);
	this->m_matrix = this->m_matrix * this->m_rotationMat;
	this->m_matrix = glm::scale(this->m_matrix, this->m_scale);
	this->m_isDirty = false;
}

void Matrix::refreshRotationMatrix()
{
	if (!this->m_isRotationDirty)
		return;
	this->m_rotationMat = glm::rotate(glm::mat4(1.0f), this->m_rotation.y, glm::vec3(0, 1, 0));
	this->m_rotationMat = glm::rotate(this->m_rotationMat, this->m_rotation.x, glm::vec3(1, 0, 0));
	this->m_rotationMat = glm::rotate(this->m_rotationMat, this->m_rotation.z, glm::vec3(0, 0, 1));
	this->m_isRotationDirty = false;
}

float Matrix::limitAngle(float angle, float area)
{
	float area_two = area + area;
	angle = fmod(angle, area_two);

	if (angle > area) {
		angle -= area_two;
	}
	else if (angle < -area) {
		angle += area_two;
	}
	return angle;
}

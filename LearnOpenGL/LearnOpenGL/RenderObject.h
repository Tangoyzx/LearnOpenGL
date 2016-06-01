#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Model.h"
#include "Shader.h"
#include "Matrix.h"

class RenderObject
{
public:
	RenderObject(Model *model);
	~RenderObject();
	void Render(Shader *shader);
	Matrix *GetTransform();
private:
	Model *m_model;
	Matrix *m_transform;
};

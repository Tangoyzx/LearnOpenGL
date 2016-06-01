#pragma once

#include <GL\glew.h>
#include "Shader.h"

class PostEffectHelper
{
public:
	PostEffectHelper();
	~PostEffectHelper();
	void Render(Shader *shader);
	GLuint m_texture, m_rboId, m_fboId, m_vaoId;
private:
	void SetupFrameBuffer();
	void SetupVAO();
};
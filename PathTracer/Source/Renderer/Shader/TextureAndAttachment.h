#pragma once

#include <Renderer/SFMLOpenGL.h>

struct TextureAndAttachment
{
	GLuint m_textureHandle, m_attachment, m_target;

	TextureAndAttachment();
	TextureAndAttachment(GLuint textureHandle, GLuint attachment, GLuint target);
};


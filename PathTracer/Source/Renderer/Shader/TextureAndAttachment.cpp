#include <Renderer/Shader/TextureAndAttachment.h>

TextureAndAttachment::TextureAndAttachment()
{
}

TextureAndAttachment::TextureAndAttachment(GLuint textureHandle, GLuint attachment, GLuint target)
	: m_textureHandle(textureHandle), m_attachment(attachment), m_target(target)
{
}
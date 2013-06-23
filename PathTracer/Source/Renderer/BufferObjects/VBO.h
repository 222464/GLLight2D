#pragma once

#include <Renderer/SFMLOpenGL.h>

// Helper class for vertex buffer objects
class VBO
{
private:
	GLuint m_ID;

	GLuint m_usage;

public:
	VBO();
	~VBO();

	void Create();
	void Destroy();

	/*
		The "usage" argument can be either of the following values:

		GL_ELEMENT_ARRAY_BUFFER
		GL_ARRAY_BUFFER
		GL_UNIFORM_BUFFER
		GL_TEXTURE_BUFFER
	*/

	void Bind(GLuint usage);
	void Unbind();

	static void Unbind(GLuint usage);

	bool Created();

	GLuint GetID();
};


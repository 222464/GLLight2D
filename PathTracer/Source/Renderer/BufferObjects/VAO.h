#pragma once

#include <Renderer/SFMLOpenGL.h>

// Helper class for vertex buffer objects
class VAO
{
private:
	GLuint m_ID;

public:
	VAO();
	~VAO();

	void Create();
	void Destroy();

	void Bind();
	static void Unbind();

	bool Created();

	GLuint GetID();
};


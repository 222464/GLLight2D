#include <Renderer/SFMLOpenGL.h>

#include <iostream>

bool CheckForGLError()
{
	GLuint errorCode = glGetError();

	if(errorCode != GL_NO_ERROR)
	{
		std::cerr << gluErrorString(errorCode) << std::endl;
		abort();
		return true;
	}

	return false;
}
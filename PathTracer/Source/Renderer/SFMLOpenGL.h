#pragma once

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>

#include <string>

// Vertex attribute locations
#define ATTRIB_POSITION 0
#define ATTRIB_NORMAL 1
#define ATTRIB_TEXCOORD 2

bool CheckForGLError();

// So only runs debug function when in debug mode
#ifdef DEBUG
#define GL_ERROR_CHECK() CheckForGLError()
#else
#define GL_ERROR_CHECK()
#endif
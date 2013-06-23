#include <Renderer/BufferObjects/VBO.h>

#include <assert.h>

#include <iostream>

VBO::VBO()
	: m_ID(0)
{
}

VBO::~VBO()
{
	if(m_ID != 0)
		glDeleteBuffers(1, &m_ID);
}

void VBO::Create()
{
	assert(m_ID == 0);

	glGenBuffers(1, &m_ID);

	if(m_ID == 0)
		std::cout << "Could not create VBO!" << std::endl;
}

void VBO::Destroy()
{
	assert(m_ID != 0);

	glDeleteBuffers(1, &m_ID);

	m_ID = 0;
}

void VBO::Bind(GLuint usage)
{
	m_usage = usage;

	glBindBuffer(usage, m_ID);
}

void VBO::Unbind()
{
	glBindBuffer(m_usage, 0);
}

void VBO::Unbind(GLuint usage)
{
	glBindBuffer(usage, 0);
}

bool VBO::Created()
{
	return m_ID != 0;
}

GLuint VBO::GetID()
{
	return m_ID;
}
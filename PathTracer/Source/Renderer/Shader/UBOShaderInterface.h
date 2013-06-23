#pragma once

#include <Renderer/BufferObjects/VBO.h>

#include <Renderer/Shader/Shader.h>

#include <string>

#include <unordered_map>

class UBOShaderInterface
{
private:
	Shader* m_pShader;

	GLuint m_blockIndex;
	GLuint m_attributeLocation;
	GLuint m_bufferBindIndex;

	int m_blockSize;

	std::unordered_map<std::string, size_t> m_uniformNameToOffset;

public:
	void Create(const std::string &uniformBlockName, Shader* shader, const char** uniformNames, GLsizei numUniformNames);
	void SetUpBuffer(VBO &buffer);
	void SetBindingIndex(GLuint index);

	void BindBufferToSetIndex(VBO &buffer);
	void UnbindSetIndex();

	int GetBlockSize() const;
	Shader* GetShader() const;

	// Bind buffer you want to modify before using these
	void SetUniform(const std::string &name, GLsizei size, GLvoid* param);

	void SetUniformf(const std::string &name, float param);
	void SetUniformv2f(const std::string &name, const Vec2f &params);
	void SetUniformv3f(const std::string &name, const Vec3f &params);
	void SetUniformv4f(const std::string &name, const Vec4f &params);

	// TODO: Add more types!
};

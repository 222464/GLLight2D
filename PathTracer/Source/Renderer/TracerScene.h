#pragma once

#include <Renderer/SFMLOpenGL.h>
#include <Renderer/Material.h>
#include <Renderer/Shader/Shader.h>
#include <Renderer/BVH/BVHTree.h>
#include <Renderer/Camera.h>
#include <Renderer/Light.h>

#include <Constructs/Vec2f.h>

class TracerScene
{
private:
	Shader m_traceShader;

	GLuint m_treeBuffer;
	GLuint m_triBuffer;
	GLuint m_materialBuffer;
	GLuint m_treeTexture;
	GLuint m_triTexture;
	GLuint m_materialTexture;

	void GenerateBufferData(
		std::vector<BVHNodeBufferData> &treeBuffer,
		std::vector<TriBufferData> &triBuffer,
		std::vector<MaterialBufferData> &materialBuffer);

public:
	std::vector<Light> m_lights;
	std::vector<Material> m_materials;
	BVHTree m_tree;

	TracerScene();
	~TracerScene();

	bool Create(const std::string &shaderName);
	void GenerateSceneData();
	void Render(const Vec2f &position, const Vec2f &dims, float angle);
};


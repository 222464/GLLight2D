#pragma once

#include <Renderer/BVH/BVHNode.h>

#include <array>

struct BVHNodeBufferData
{
	Vec3f m_lowerBound;
	Vec3f m_upperBound;

	float m_geomStartIndex;
	float m_numTris;
	float m_padding1;

	float m_leftIndex;
	float m_rightIndex;
	float m_parentIndex;
};

struct MaterialBufferData
{
	Vec3f m_diffuseColor;
	float m_specularColor;
	float m_shininess;
	float m_padding1;
};

struct TriVertexBufferData
{
	Vec3f m_position;
	Vec3f m_normal;
	Vec2f m_texCoord;
	float m_materialIndex;
};

struct TriBufferData
{
	std::array<TriVertexBufferData, 3> m_data;
};

class BVHTree
{
private:
	std::unique_ptr<BVHNode> m_pRootNode;

	// Returns the index of the node in the buffer as a float
	float RecursiveCompile(BVHNode* pNode, size_t parentBufferIndex,
		std::vector<BVHNodeBufferData> &treeBuffer,
		std::vector<TriBufferData> &triBuffer);

public:
	unsigned int m_maxSplitsAfterNoTriangleReduction;
	unsigned int m_minNodeOccupants;
	unsigned int m_maxNodeOccupants;

	BVHTree();

	void Create(const AABB &rootRegion);
	void Add(Form_Triangle &triangle);

	void Compile();

	void CompileBuffers(
		std::vector<BVHNodeBufferData> &treeBuffer,
		std::vector<TriBufferData> &triBuffer);

	friend BVHNode;
};


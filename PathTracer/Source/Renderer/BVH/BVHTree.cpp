#include <Renderer/BVH/BVHTree.h>

#include <assert.h>

BVHTree::BVHTree()
	: m_maxSplitsAfterNoTriangleReduction(24), m_minNodeOccupants(3), m_maxNodeOccupants(9)
{
}

void BVHTree::Create(const AABB &rootRegion)
{
	m_pRootNode.reset(new BVHNode(this, NULL));
	m_pRootNode->m_aabb = rootRegion;
}

void BVHTree::Add(Form_Triangle &triangle)
{
	assert(m_pRootNode != NULL);

	m_pRootNode->Add(triangle, triangle.GetAABB());
}

float BVHTree::RecursiveCompile(BVHNode* pNode, size_t parentBufferIndex,
		std::vector<BVHNodeBufferData> &treeBuffer,
		std::vector<TriBufferData> &triBuffer)
{
	treeBuffer.push_back(BVHNodeBufferData());

	size_t bufferIndex = treeBuffer.size() - 1;

	treeBuffer[bufferIndex].m_geomStartIndex = static_cast<float>(triBuffer.size());
	treeBuffer[bufferIndex].m_numTris = static_cast<float>(pNode->m_occupants.size());

	assert((pNode->m_occupants.size() == 0 && pNode->m_pLeft != NULL) || (pNode->m_occupants.size() >= 0 && pNode->m_pLeft == NULL));
	
	treeBuffer[bufferIndex].m_lowerBound = pNode->m_aabb.m_lowerBound;
	treeBuffer[bufferIndex].m_upperBound = pNode->m_aabb.m_upperBound;

	for(size_t i = 0, size = pNode->m_occupants.size(); i < size; i++)
	{
		TriBufferData tri;

		for(int j = 0; j < 3; j++)
		{
			tri.m_data[j].m_position = pNode->m_occupants[i].m_points[j];
			tri.m_data[j].m_normal = pNode->m_occupants[i].m_normals[j];
			tri.m_data[j].m_texCoord = pNode->m_occupants[i].m_uv[j];
			tri.m_data[j].m_materialIndex = pNode->m_occupants[i].m_materialIndex;
		}

		triBuffer.push_back(tri);
	}

	if(pNode->m_pLeft != NULL)
	{
		assert(pNode->m_pRight != NULL);
		
		treeBuffer[bufferIndex].m_leftIndex = RecursiveCompile(pNode->m_pLeft.get(), bufferIndex,
			treeBuffer, triBuffer);
		treeBuffer[bufferIndex].m_rightIndex = RecursiveCompile(pNode->m_pRight.get(), bufferIndex,
			treeBuffer, triBuffer);

		treeBuffer[bufferIndex].m_parentIndex = static_cast<float>(parentBufferIndex);
	}
	else
	{
		treeBuffer[bufferIndex].m_leftIndex = -1.0f;
		treeBuffer[bufferIndex].m_rightIndex = -1.0f;

		treeBuffer[bufferIndex].m_parentIndex = static_cast<float>(parentBufferIndex);
	}

	return static_cast<float>(bufferIndex);
}

void BVHTree::Compile()
{
	m_pRootNode->Split(0);
}

void BVHTree::CompileBuffers(
		std::vector<BVHNodeBufferData> &treeBuffer,
		std::vector<TriBufferData> &triBuffer)
{
	RecursiveCompile(m_pRootNode.get(), 0, treeBuffer, triBuffer);
}
#pragma once

#include <Renderer/Forms/Form_Triangle.h>
#include <memory>

class BVHNode
{
private:
	class BVHTree* m_pTree;
	BVHNode* m_pParent;

	AABB m_aabb;

	std::unique_ptr<BVHNode> m_pLeft, m_pRight;

	unsigned int m_numOccupantsBelow;

	std::vector<Form_Triangle> m_occupants;

public:
	BVHNode(class BVHTree* pTree, BVHNode* pParent);

	void Add(const Form_Triangle &triangle, const AABB &triangleAABB);
	void Split(int numSplitsAfterNoTriangleReduction);

	friend class BVHTree;
};


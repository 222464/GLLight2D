#include <Renderer/BVH/BVHNode.h>

#include <Renderer/BVH/BVHTree.h>

BVHNode::BVHNode(class BVHTree* pTree, BVHNode* pParent)
	: m_pTree(pTree), m_pParent(pParent), m_numOccupantsBelow(0)
{
}

void BVHNode::Add(const Form_Triangle &triangle, const AABB &triangleAABB)
{
	m_numOccupantsBelow++;

    // Add here
    m_occupants.push_back(triangle);
}

void BVHNode::Split(int numSplitsAfterNoTriangleReduction)
{
	// Find average occupant position
    //Vec3f splitPos(0.0f, 0.0f, 0.0f);

	//for(size_t i = 0, size = m_occupants.size(); i < size; i++)
    //    splitPos += m_occupants[i].GetCenter();

	//splitPos /= static_cast<float>(m_occupants.size());

	Vec3f splitPos = m_aabb.GetCenter();

    // Pick largest axis to split along
    Vec3f halfDims = m_aabb.GetHalfDims();

    if(halfDims.x > halfDims.y)
    {
        if(halfDims.x > halfDims.z)
        {
			m_pLeft.reset(new BVHNode(m_pTree, this));

            m_pLeft->m_aabb.m_lowerBound.x = m_aabb.m_lowerBound.x;
            m_pLeft->m_aabb.m_lowerBound.y = m_aabb.m_lowerBound.y;
            m_pLeft->m_aabb.m_lowerBound.z = m_aabb.m_lowerBound.z;

            m_pLeft->m_aabb.m_upperBound.x = splitPos.x;
            m_pLeft->m_aabb.m_upperBound.y = m_aabb.m_upperBound.y;
            m_pLeft->m_aabb.m_upperBound.z = m_aabb.m_upperBound.z;

			m_pLeft->m_aabb.CalculateHalfDims();
			m_pLeft->m_aabb.CalculateCenter();

			m_pRight.reset(new BVHNode(m_pTree, this));

            m_pRight->m_aabb.m_lowerBound.x = splitPos.x;
            m_pRight->m_aabb.m_lowerBound.y = m_aabb.m_lowerBound.y;
            m_pRight->m_aabb.m_lowerBound.z = m_aabb.m_lowerBound.z;

            m_pRight->m_aabb.m_upperBound.x = m_aabb.m_upperBound.x;
            m_pRight->m_aabb.m_upperBound.y = m_aabb.m_upperBound.y;
            m_pRight->m_aabb.m_upperBound.z = m_aabb.m_upperBound.z;

			m_pRight->m_aabb.CalculateHalfDims();
			m_pRight->m_aabb.CalculateCenter();
        }
        else
        {
            m_pLeft.reset(new BVHNode(m_pTree, this));

            m_pLeft->m_aabb.m_lowerBound.x = m_aabb.m_lowerBound.x;
            m_pLeft->m_aabb.m_lowerBound.y = m_aabb.m_lowerBound.y;
            m_pLeft->m_aabb.m_lowerBound.z = m_aabb.m_lowerBound.z;

            m_pLeft->m_aabb.m_upperBound.x = m_aabb.m_upperBound.x;
            m_pLeft->m_aabb.m_upperBound.y = m_aabb.m_upperBound.y;
            m_pLeft->m_aabb.m_upperBound.z = splitPos.z;

			m_pLeft->m_aabb.CalculateHalfDims();
			m_pLeft->m_aabb.CalculateCenter();

            m_pRight.reset(new BVHNode(m_pTree, this));

            m_pRight->m_aabb.m_lowerBound.x = m_aabb.m_lowerBound.x;
            m_pRight->m_aabb.m_lowerBound.y = m_aabb.m_lowerBound.y;
            m_pRight->m_aabb.m_lowerBound.z = splitPos.z;

            m_pRight->m_aabb.m_upperBound.x = m_aabb.m_upperBound.x;
            m_pRight->m_aabb.m_upperBound.y = m_aabb.m_upperBound.y;
            m_pRight->m_aabb.m_upperBound.z = m_aabb.m_upperBound.z;

			m_pRight->m_aabb.CalculateHalfDims();
			m_pRight->m_aabb.CalculateCenter();
        }
    }
    else
    {
        if(halfDims.y > halfDims.z)
        {
            m_pLeft.reset(new BVHNode(m_pTree, this));

            m_pLeft->m_aabb.m_lowerBound.x = m_aabb.m_lowerBound.x;
            m_pLeft->m_aabb.m_lowerBound.y = m_aabb.m_lowerBound.y;
            m_pLeft->m_aabb.m_lowerBound.z = m_aabb.m_lowerBound.z;

            m_pLeft->m_aabb.m_upperBound.x = m_aabb.m_upperBound.x;
            m_pLeft->m_aabb.m_upperBound.y = splitPos.y;
            m_pLeft->m_aabb.m_upperBound.z = m_aabb.m_upperBound.z;

			m_pLeft->m_aabb.CalculateHalfDims();
			m_pLeft->m_aabb.CalculateCenter();

			m_pRight.reset(new BVHNode(m_pTree, this));

            m_pRight->m_aabb.m_lowerBound.x = m_aabb.m_lowerBound.x;
            m_pRight->m_aabb.m_lowerBound.y = splitPos.y;
            m_pRight->m_aabb.m_lowerBound.z = m_aabb.m_lowerBound.z;

            m_pRight->m_aabb.m_upperBound.x = m_aabb.m_upperBound.x;
            m_pRight->m_aabb.m_upperBound.y = m_aabb.m_upperBound.y;
            m_pRight->m_aabb.m_upperBound.z = m_aabb.m_upperBound.z;

			m_pRight->m_aabb.CalculateHalfDims();
			m_pRight->m_aabb.CalculateCenter();
        }
        else
        {
            m_pLeft.reset(new BVHNode(m_pTree, this));

            m_pLeft->m_aabb.m_lowerBound.x = m_aabb.m_lowerBound.x;
            m_pLeft->m_aabb.m_lowerBound.y = m_aabb.m_lowerBound.y;
            m_pLeft->m_aabb.m_lowerBound.z = m_aabb.m_lowerBound.z;

            m_pLeft->m_aabb.m_upperBound.x = m_aabb.m_upperBound.x;
            m_pLeft->m_aabb.m_upperBound.y = m_aabb.m_upperBound.y;
            m_pLeft->m_aabb.m_upperBound.z = splitPos.z;

			m_pLeft->m_aabb.CalculateHalfDims();
			m_pLeft->m_aabb.CalculateCenter();

			m_pRight.reset(new BVHNode(m_pTree, this));

            m_pRight->m_aabb.m_lowerBound.x = m_aabb.m_lowerBound.x;
            m_pRight->m_aabb.m_lowerBound.y = m_aabb.m_lowerBound.y;
            m_pRight->m_aabb.m_lowerBound.z = splitPos.z;

            m_pRight->m_aabb.m_upperBound.x = m_aabb.m_upperBound.x;
            m_pRight->m_aabb.m_upperBound.y = m_aabb.m_upperBound.y;
            m_pRight->m_aabb.m_upperBound.z = m_aabb.m_upperBound.z;

			m_pRight->m_aabb.CalculateHalfDims();
			m_pRight->m_aabb.CalculateCenter();
        }
    }

    // Add occupants to the new nodes if they fit
	for(size_t i = 0, size = m_occupants.size(); i < size; i++)
    {
		AABB aabb = m_occupants[i].GetAABB();

		if(m_pLeft->m_aabb.Intersects(aabb))
            m_pLeft->Add(m_occupants[i], aabb);
		if(m_pRight->m_aabb.Intersects(aabb))
            m_pRight->Add(m_occupants[i], aabb);
	}

	if(numSplitsAfterNoTriangleReduction < m_pTree->m_maxSplitsAfterNoTriangleReduction)
	{
		// Left split
		{
			// If needs split
			size_t numPointsContain = 0;

			for(size_t i = 0, size = m_pLeft->m_occupants.size(); i < size; i++)
			{
				for(size_t j = 0; j < 3; j++)
					if(m_pLeft->m_aabb.Contains(m_pLeft->m_occupants[i].m_points[j]))
						numPointsContain++;
			}

			if(numPointsContain > m_pTree->m_maxNodeOccupants)
				m_pLeft->Split(m_pLeft->m_occupants.size() == m_occupants.size() ? (numSplitsAfterNoTriangleReduction + 1) : 0);
		}

		// Right split
		{
			// If needs split
			size_t numPointsContain = 0;

			for(size_t i = 0, size = m_pRight->m_occupants.size(); i < size; i++)
			{
				for(size_t j = 0; j < 3; j++)
					if(m_pRight->m_aabb.Contains(m_pRight->m_occupants[i].m_points[j]))
						numPointsContain++;
			}

			if(numPointsContain > m_pTree->m_maxNodeOccupants)
				m_pRight->Split(m_pRight->m_occupants.size() == m_occupants.size() ? (numSplitsAfterNoTriangleReduction + 1) : 0);
		}
	}

	m_occupants.clear();
}
#include <Renderer/Model_OBJ.h>

#include <Constructs/Vec3f.h>
#include <Constructs/Vec2f.h>

#include <Utilities/UtilFuncs.h>

#include <fstream>
#include <sstream>

#include <vector>

#include <unordered_map>

#include <iostream>

#include <assert.h>

// Custom hash for custom class
size_t IndexSet::operator()(const IndexSet &set) const
{
	return static_cast<size_t>(set.vi ^ set.ti ^ set.ni);
}

bool IndexSet::operator==(const IndexSet &other) const
{
	return vi == other.vi && ti == other.ti && ni == other.ni;
}

Model_OBJ::Model_OBJ()
	: m_textureManager(Asset_Texture::Asset_Factory)
{
}

bool Model_OBJ::CreateAsset(const std::string &name)
{
	std::ifstream fromFile(name);
	
	if(!fromFile.is_open())
	{
		std::cerr << "Could not load model file " << name << std::endl;
		return false;
	}

	std::string rootName(GetRootName(name));

	std::vector<Vec3f> filePositions;
	std::vector<Vec2f> fileTexCoords;
	std::vector<Vec3f> fileNormals;

	// Hash map for linking indices to vertex array index for attributes
	std::unordered_map<IndexSet, size_t, IndexSet> indexToVertex;

	// Initial extremes
	m_aabb.m_lowerBound = Vec3f(9999.0f, 9999.0f, 9999.0f);
	m_aabb.m_upperBound = Vec3f(-9999.0f, -9999.0f, -9999.0f);

	int currentObj = -1;

	std::unordered_map<std::string, size_t> matReferences;

	while(!fromFile.eof())
	{
		// Read line header
		std::string line;
		getline(fromFile, line);

		std::stringstream ss(line);

		std::string header;
		ss >> header;

		if(header == "v")
		{
			// Add vertex
			float x, y, z;

			ss >> x >> y >> z;

			filePositions.push_back(Vec3f(x, y, z));

			// Expand AABB
			if(x < m_aabb.m_lowerBound.x)
				m_aabb.m_lowerBound.x = x;
			if(y < m_aabb.m_lowerBound.y)
				m_aabb.m_lowerBound.y = y;
			if(z < m_aabb.m_lowerBound.z)
				m_aabb.m_lowerBound.z = z;

			if(x > m_aabb.m_upperBound.x)
				m_aabb.m_upperBound.x = x;
			if(y > m_aabb.m_upperBound.y)
				m_aabb.m_upperBound.y = y;
			if(z > m_aabb.m_upperBound.z)
				m_aabb.m_upperBound.z = z;
		}
		else if(header == "vt")
		{
			// Add texture coordinate
			float s, t;

			ss >> s >> t;

			fileTexCoords.push_back(Vec2f(s, t));
		}
		else if(header == "vn")
		{
			// Add normal
			float nx, ny, nz;

			ss >> nx >> ny >> nz;

			fileNormals.push_back(Vec3f(nx, ny, nz));
		}
		else if(header == "f")
		{
			assert(m_indices.size() > 0);
			assert(currentObj == m_indices.size() - 1);

			// Add a face
			IndexSet v[3];

			ss >> v[0].vi;
			ss.ignore(1, '/');
			ss >> v[0].ti;
			ss.ignore(1, '/');
			ss >> v[0].ni;

			ss >> v[1].vi;
			ss.ignore(1, '/');
			ss >> v[1].ti;
			ss.ignore(1, '/');
			ss >> v[1].ni;

			ss >> v[2].vi;
			ss.ignore(1, '/');
			ss >> v[2].ti;
			ss.ignore(1, '/');
			ss >> v[2].ni;

			for(int i = 0; i < 3; i++)
			{
				// Search for index set 1
				std::unordered_map<IndexSet, size_t, IndexSet>::iterator it = indexToVertex.find(v[i]);

				if(it == indexToVertex.end())
				{
					// Vertex attributes do not exist, create them

					// File indicies start at 1, so convert
					size_t vertIndex = v[i].vi - 1;
					size_t texCoordIndex = v[i].ti - 1;
					size_t normalIndex = v[i].ni - 1;

					m_positions.push_back(filePositions[vertIndex]);
					m_texCoords.push_back(fileTexCoords[texCoordIndex]);
					m_normals.push_back(fileNormals[normalIndex]);

					// Index of vertex in vertex component array
					size_t realIndex = m_positions.size() - 1;

					// Add attribute set index to the map
					indexToVertex[v[i]] = realIndex;

					m_indices[currentObj].push_back(static_cast<OBJ_Index_Type>(realIndex));
				}
				else
				{
					// Index already exists, so add it
					m_indices[currentObj].push_back(static_cast<OBJ_Index_Type>(it->second));
				}
			}
		}
		else if(header == "usemtl")
		{
			// Get texture name and load it
			std::string matName;
			ss >> matName;

			// Link obj to material
			std::unordered_map<std::string, size_t>::iterator it = matReferences.find(matName);

			if(it == matReferences.end())
			{
				std::cerr << "Could not find material \"" << matName << "\"!" << std::endl;
				return false;
			}

			m_objMaterialReferences.push_back(it->second);

			// Next index in indices array
			m_indices.push_back(std::vector<OBJ_Index_Type>());
			currentObj++;
		}
		else if(header == "mtllib")
		{
			std::string libName;
			ss >> libName;

			std::ostringstream fullMaterialLibraryName;

			fullMaterialLibraryName << rootName << libName;

			if(!Material::LoadFromMTL(fullMaterialLibraryName.str(), &m_textureManager, matReferences, m_materials))
			{
				std::cerr << "- in " << name << std::endl;

				return false;
			}
		}
	}

	fromFile.close();

	m_aabb.CalculateHalfDims();
	m_aabb.CalculateCenter();

	const size_t numObjects = m_indices.size();

	// Create index VBOs
	m_numVertices.resize(numObjects);
	
	return true;
}

size_t Model_OBJ::GetNumObjects() const
{
	return m_indices.size();
}

size_t Model_OBJ::GetNumMaterials() const
{
	return m_materials.size();
}

Material* Model_OBJ::GetMaterial(size_t index)
{
	assert(index >= 0 && index < m_materials.size());

	return &m_materials[index];
}

AABB Model_OBJ::GetAABB() const
{
	return m_aabb;
}

const Vec3f &Model_OBJ::GetAABBOffsetFromModel() const
{
	return m_aabb.GetCenter();
}

size_t Model_OBJ::GetSubObjectMaterialIndex(size_t index) const
{
	return m_objMaterialReferences[index];
}

Asset* Model_OBJ::Asset_Factory()
{
	return new Model_OBJ();
}

void Model_OBJ::AddToScene(TracerScene &scene, const Matrix4x4f &transform)
{
	Matrix4x4f normalMatrix;
	transform.Inverse(normalMatrix);
	normalMatrix = normalMatrix.Transpose();

	// Create triangle forms and add them to the scene
	for(size_t i = 0; i < GetNumObjects(); i++)
	{
		// Add material to scene
		Material* pMaterial = GetMaterial(GetSubObjectMaterialIndex(i));
		scene.m_materials.push_back(*pMaterial);

		float materialIndex = static_cast<float>(scene.m_materials.size()) - 1.0f;

		for(size_t j = 0, numIndices = m_indices[i].size(); j < numIndices; j += 3)
		{
			Form_Triangle triangle;

			triangle.m_materialIndex = materialIndex;

			for(int k = 0; k < 3; k++)
			{
				triangle.m_points[k] = transform * m_positions[m_indices[i][j + k]];
				triangle.m_normals[k] = normalMatrix * m_normals[m_indices[i][j + k]];
				triangle.m_uv[k] = m_texCoords[m_indices[i][j + k]];
			}

			scene.m_tree.Add(triangle);
		}
	}
}
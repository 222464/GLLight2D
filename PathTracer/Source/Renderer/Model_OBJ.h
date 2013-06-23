#pragma once

#include <AssetManager/AssetManager.h>
#include <AssetManager/Asset.h>

#include <Renderer/Material.h>
#include <Renderer/TracerScene.h>

#include <Constructs/AABB.h>
#include <Constructs/Vec2f.h>
#include <Constructs/Vec3f.h>

#include <vector>

typedef unsigned int OBJ_Index_Type;

struct IndexSet
{
	OBJ_Index_Type vi, ti, ni;

	// Custom hash stuff - if this index set already exists, so must a vertex for it
	size_t operator()(const IndexSet &set) const;
	bool operator==(const IndexSet &other) const;
};

class Model_OBJ :
	public Asset
{
private:
	// Number of vertices for each OBJ object
	std::vector<OBJ_Index_Type> m_numVertices;
	
	std::vector<Material> m_materials;

	// Array of material indices for different OBJ objects
	std::vector<size_t> m_objMaterialReferences;

	AssetManager m_textureManager;

	AABB m_aabb;
	Vec3f m_aabbOffsetFromModel;

	std::vector<Vec3f> m_positions;
	std::vector<Vec2f> m_texCoords;
	std::vector<Vec3f> m_normals;

	std::vector<std::vector<OBJ_Index_Type>> m_indices;

public:
	Model_OBJ();
	virtual ~Model_OBJ() {}

	// Inherited from Asset
	bool CreateAsset(const std::string &name);

	size_t GetNumObjects() const;

	size_t GetNumMaterials() const;
	Material* GetMaterial(size_t index);

	AABB GetAABB() const;

	const Vec3f &GetAABBOffsetFromModel() const;

	size_t GetSubObjectMaterialIndex(size_t index) const;

	void AddToScene(TracerScene &scene, const Matrix4x4f &transform);

	// Asset factory
	static Asset* Asset_Factory();
};
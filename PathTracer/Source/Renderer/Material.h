#pragma once

#include <Constructs/Vec3f.h>
#include <AssetManager/AssetManager.h>
#include <AssetManager/Asset_Texture.h>
#include <Renderer/SFMLOpenGL.h>

// Set texture pointers to NULL to indicate it is not used
class Material
{
public:
	Vec3f m_diffuseColor;
	float m_specularColor;
	Vec3f m_emissiveColor;

	float m_shininess;

	Asset_Texture* m_pDiffuseMap;
	Asset_Texture* m_pSpecularMap;
	Asset_Texture* m_pNormalMap;
	Asset_Texture* m_pEmissiveMap;
	Asset_Texture* m_pHeightMap;

	Material();

	static bool LoadFromMTL(const std::string &fileName, AssetManager* pTextureManager, std::vector<Material> &materials);
	static bool LoadFromMTL(const std::string &fileName, AssetManager* pTextureManager, std::vector<std::string> &materialNames, std::vector<Material> &materials);
	static bool LoadFromMTL(const std::string &fileName, AssetManager* pTextureManager, std::unordered_map<std::string, size_t> &materialNamesToIndicesMap, std::vector<Material> &materials);
};
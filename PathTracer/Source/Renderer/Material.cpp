#include <Renderer/Material.h>

#include <fstream>
#include <sstream>
#include <assert.h>

Material::Material()
	: m_diffuseColor(1.0f, 1.0f, 1.0f), m_specularColor(0.0f), m_emissiveColor(0.0f, 0.0f, 0.0f), m_shininess(0.0f),
	m_pDiffuseMap(NULL), m_pSpecularMap(NULL), m_pNormalMap(NULL), m_pEmissiveMap(NULL), m_pHeightMap(NULL)
{
}

bool Material::LoadFromMTL(const std::string &fileName, AssetManager* pTextureManager, std::vector<Material> &materials)
{
	std::ifstream fromFile(fileName);

	if(!fromFile.is_open())
	{
		std::cerr << "Could not load material file " << fileName << std::endl;
		return false;
	}

	Material* pCurrent = NULL;

	while(!fromFile.eof())
	{
		// Read line header
		std::string line;
		getline(fromFile, line);

		std::stringstream ss(line);

		std::string header;
		ss >> header;

		if(header == "newmtl")
		{
			materials.push_back(Material());

			pCurrent = &materials.back();
		}
		else if(header == "Ns")
		{
			if(pCurrent == NULL)
			{
				std::cerr << "Attempted to set material parameter before defining material in \"" << fileName << "\"!" << std::endl;
				return false;
			}
			
			ss >> pCurrent->m_shininess;
		}
		else if(header == "Ka")
		{
			if(pCurrent == NULL)
			{
				std::cerr << "Attempted to set material parameter before defining material in \"" << fileName << "\"!" << std::endl;
				return false;
			}
			
			ss >> pCurrent->m_emissiveColor.x >> pCurrent->m_emissiveColor.y >> pCurrent->m_emissiveColor.z;
		}
		else if(header == "Kd")
		{
			if(pCurrent == NULL)
			{
				std::cerr << "Attempted to set material parameter before defining material in \"" << fileName << "\"!" << std::endl;
				return false;
			}
			
			ss >> pCurrent->m_diffuseColor.x >> pCurrent->m_diffuseColor.y >> pCurrent->m_diffuseColor.z;
		}
		else if(header == "Ks")
		{
			if(pCurrent == NULL)
			{
				std::cerr << "Attempted to set material parameter before defining material in \"" << fileName << "\"!" << std::endl;
				return false;
			}
			
			ss >> pCurrent->m_specularColor;
		}
		else if(header == "Ni")
		{
			if(pCurrent == NULL)
			{
				std::cerr << "Attempted to set material parameter before defining material in \"" << fileName << "\"!" << std::endl;
				return false;
			}

			ss >> pCurrent->m_shininess;
		}
		else if(header == "map_Ka")
		{
			if(pCurrent == NULL)
			{
				std::cerr << "Attempted to set material parameter before defining material in \"" << fileName << "\"!" << std::endl;
				return false;
			}
			
			std::string mapName;
			ss >> mapName;

			Asset* pAsset;

			if(!pTextureManager->GetAsset(mapName, pAsset))
			{
				std::cerr << "- in " << fileName << std::endl;

				return false;
			}

			pCurrent->m_pEmissiveMap = static_cast<Asset_Texture*>(pAsset);
		}
		else if(header == "map_Kd")
		{
			if(pCurrent == NULL)
			{
				std::cerr << "Attempted to set material parameter before defining material in \"" << fileName << "\"!" << std::endl;
				return false;
			}
			
			std::string mapName;
			ss >> mapName;

			Asset* pAsset;

			if(!pTextureManager->GetAsset(mapName, pAsset))
			{
				std::cerr << "- in " << fileName << std::endl;

				return false;
			}

			pCurrent->m_pDiffuseMap = static_cast<Asset_Texture*>(pAsset);
		}
		else if(header == "map_Ks")
		{
			if(pCurrent == NULL)
			{
				std::cerr << "Attempted to set material parameter before defining material in \"" << fileName << "\"!" << std::endl;
				return false;
			}
			
			std::string mapName;
			ss >> mapName;

			Asset* pAsset;

			if(!pTextureManager->GetAsset(mapName, pAsset))
			{
				std::cerr << "- in " << fileName << std::endl;

				return false;
			}

			pCurrent->m_pSpecularMap = static_cast<Asset_Texture*>(pAsset);
		}
		else if(header == "bump" || header == "map_Bump")
		{
			if(pCurrent == NULL)
			{
				std::cerr << "Attempted to set material parameter before defining material in \"" << fileName << "\"!" << std::endl;
				return false;
			}
			
			std::string mapName;
			ss >> mapName;

			Asset* pAsset;

			if(!pTextureManager->GetAsset(mapName, pAsset))
			{
				std::cerr << "- in " << fileName << std::endl;

				return false;
			}

			pCurrent->m_pNormalMap = static_cast<Asset_Texture*>(pAsset);
		}
		else if(header == "height" || header == "map_Height")
		{
			if(pCurrent == NULL)
			{
				std::cerr << "Attempted to set material parameter before defining material in \"" << fileName << "\"!" << std::endl;
				return false;
			}
			
			std::string mapName;
			ss >> mapName;

			Asset* pAsset;

			if(!pTextureManager->GetAsset(mapName, pAsset))
			{
				std::cerr << "- in " << fileName << std::endl;

				return false;
			}

			pCurrent->m_pHeightMap = static_cast<Asset_Texture*>(pAsset);
		}
	}

	// Set last shader
	assert(!materials.empty());

	return true;
}

bool Material::LoadFromMTL(const std::string &fileName, AssetManager* pTextureManager, std::vector<std::string> &materialNames, std::vector<Material> &materials)
{
	std::ifstream fromFile(fileName);

	if(!fromFile.is_open())
	{
		std::cerr << "Could not load material file " << fileName << std::endl;
		return false;
	}

	Material* pCurrent = NULL;

	while(!fromFile.eof())
	{
		// Read line header
		std::string line;
		getline(fromFile, line);

		std::stringstream ss(line);

		std::string header;
		ss >> header;

		if(header == "newmtl")
		{
			materials.push_back(Material());

			std::string matName;
			ss >> matName;

			materialNames.push_back(matName);

			pCurrent = &materials.back();
		}
		else if(header == "Ns")
		{
			if(pCurrent == NULL)
			{
				std::cerr << "Attempted to set material parameter before defining material in \"" << fileName << "\"!" << std::endl;
				return false;
			}
			
			ss >> pCurrent->m_shininess;
		}
		else if(header == "Ka")
		{
			if(pCurrent == NULL)
			{
				std::cerr << "Attempted to set material parameter before defining material in \"" << fileName << "\"!" << std::endl;
				return false;
			}
			
			ss >> pCurrent->m_emissiveColor.x >> pCurrent->m_emissiveColor.y >> pCurrent->m_emissiveColor.z;
		}
		else if(header == "Kd")
		{
			if(pCurrent == NULL)
			{
				std::cerr << "Attempted to set material parameter before defining material in \"" << fileName << "\"!" << std::endl;
				return false;
			}
			
			ss >> pCurrent->m_diffuseColor.x >> pCurrent->m_diffuseColor.y >> pCurrent->m_diffuseColor.z;
		}
		else if(header == "Ks")
		{
			if(pCurrent == NULL)
			{
				std::cerr << "Attempted to set material parameter before defining material in \"" << fileName << "\"!" << std::endl;
				return false;
			}
			
			ss >> pCurrent->m_specularColor;
		}
		else if(header == "map_Ka")
		{
			if(pCurrent == NULL)
			{
				std::cerr << "Attempted to set material parameter before defining material in \"" << fileName << "\"!" << std::endl;
				return false;
			}
			
			std::string mapName;
			ss >> mapName;

			Asset* pAsset;

			if(!pTextureManager->GetAsset(mapName, pAsset))
			{
				std::cerr << "- in " << fileName << std::endl;

				return false;
			}

			pCurrent->m_pEmissiveMap = static_cast<Asset_Texture*>(pAsset);
		}
		else if(header == "map_Kd")
		{
			if(pCurrent == NULL)
			{
				std::cerr << "Attempted to set material parameter before defining material in \"" << fileName << "\"!" << std::endl;
				return false;
			}
			
			std::string mapName;
			ss >> mapName;

			Asset* pAsset;

			if(!pTextureManager->GetAsset(mapName, pAsset))
			{
				std::cerr << "- in " << fileName << std::endl;

				return false;
			}

			pCurrent->m_pDiffuseMap = static_cast<Asset_Texture*>(pAsset);
		}
		else if(header == "map_Ks")
		{
			if(pCurrent == NULL)
			{
				std::cerr << "Attempted to set material parameter before defining material in \"" << fileName << "\"!" << std::endl;
				return false;
			}
			
			std::string mapName;
			ss >> mapName;

			Asset* pAsset;

			if(!pTextureManager->GetAsset(mapName, pAsset))
			{
				std::cerr << "- in " << fileName << std::endl;

				return false;
			}

			pCurrent->m_pSpecularMap = static_cast<Asset_Texture*>(pAsset);
		}
		else if(header == "bump" || header == "map_Bump")
		{
			if(pCurrent == NULL)
			{
				std::cerr << "Attempted to set material parameter before defining material in \"" << fileName << "\"!" << std::endl;
				return false;
			}
			
			std::string mapName;
			ss >> mapName;

			Asset* pAsset;

			if(!pTextureManager->GetAsset(mapName, pAsset))
			{
				std::cerr << "- in " << fileName << std::endl;

				return false;
			}

			pCurrent->m_pNormalMap = static_cast<Asset_Texture*>(pAsset);
		}
		else if(header == "height" || header == "map_Height")
		{
			if(pCurrent == NULL)
			{
				std::cerr << "Attempted to set material parameter before defining material in \"" << fileName << "\"!" << std::endl;
				return false;
			}
			
			std::string mapName;
			ss >> mapName;

			Asset* pAsset;

			if(!pTextureManager->GetAsset(mapName, pAsset))
			{
				std::cerr << "- in " << fileName << std::endl;

				return false;
			}

			pCurrent->m_pHeightMap = static_cast<Asset_Texture*>(pAsset);
		}
	}

	// Set last shader
	assert(!materials.empty());

	return true;
}

bool Material::LoadFromMTL(const std::string &fileName, AssetManager* pTextureManager, std::unordered_map<std::string, size_t> &materialNamesToIndicesMap, std::vector<Material> &materials)
{
	std::ifstream fromFile(fileName);

	if(!fromFile.is_open())
	{
		std::cerr << "Could not load material file " << fileName << std::endl;
		return false;
	}

	Material* pCurrent = NULL;

	while(!fromFile.eof())
	{
		// Read line header
		std::string line;
		getline(fromFile, line);

		std::stringstream ss(line);

		std::string header;
		ss >> header;

		if(header == "newmtl")
		{
			materials.push_back(Material());

			std::string matName;
			ss >> matName;

			materialNamesToIndicesMap[matName] = materials.size() - 1;

			pCurrent = &materials.back();
		}
		else if(header == "Ns")
		{
			if(pCurrent == NULL)
			{
				std::cerr << "Attempted to set material parameter before defining material in \"" << fileName << "\"!" << std::endl;
				return false;
			}
			
			ss >> pCurrent->m_shininess;
		}
		else if(header == "Ka")
		{
			if(pCurrent == NULL)
			{
				std::cerr << "Attempted to set material parameter before defining material in \"" << fileName << "\"!" << std::endl;
				return false;
			}
			
			ss >> pCurrent->m_emissiveColor.x >> pCurrent->m_emissiveColor.y >> pCurrent->m_emissiveColor.z;
		}
		else if(header == "Kd")
		{
			if(pCurrent == NULL)
			{
				std::cerr << "Attempted to set material parameter before defining material in \"" << fileName << "\"!" << std::endl;
				return false;
			}
			
			ss >> pCurrent->m_diffuseColor.x >> pCurrent->m_diffuseColor.y >> pCurrent->m_diffuseColor.z;
		}
		else if(header == "Ks")
		{
			if(pCurrent == NULL)
			{
				std::cerr << "Attempted to set material parameter before defining material in \"" << fileName << "\"!" << std::endl;
				return false;
			}
			
			ss >> pCurrent->m_specularColor;
		}
		else if(header == "map_Ka")
		{
			if(pCurrent == NULL)
			{
				std::cerr << "Attempted to set material parameter before defining material in \"" << fileName << "\"!" << std::endl;
				return false;
			}
			
			std::string mapName;
			ss >> mapName;

			Asset* pAsset;

			if(!pTextureManager->GetAsset(mapName, pAsset))
			{
				std::cerr << "- in " << fileName << std::endl;

				return false;
			}

			pCurrent->m_pEmissiveMap = static_cast<Asset_Texture*>(pAsset);
		}
		else if(header == "map_Kd")
		{
			if(pCurrent == NULL)
			{
				std::cerr << "Attempted to set material parameter before defining material in \"" << fileName << "\"!" << std::endl;
				return false;
			}
			
			std::string mapName;
			ss >> mapName;

			Asset* pAsset;

			if(!pTextureManager->GetAsset(mapName, pAsset))
			{
				std::cerr << "- in " << fileName << std::endl;

				return false;
			}

			pCurrent->m_pDiffuseMap = static_cast<Asset_Texture*>(pAsset);
		}
		else if(header == "map_Ks")
		{
			if(pCurrent == NULL)
			{
				std::cerr << "Attempted to set material parameter before defining material in \"" << fileName << "\"!" << std::endl;
				return false;
			}
			
			std::string mapName;
			ss >> mapName;

			Asset* pAsset;

			if(!pTextureManager->GetAsset(mapName, pAsset))
			{
				std::cerr << "- in " << fileName << std::endl;

				return false;
			}

			pCurrent->m_pSpecularMap = static_cast<Asset_Texture*>(pAsset);
		}
		else if(header == "bump" || header == "map_Bump")
		{
			if(pCurrent == NULL)
			{
				std::cerr << "Attempted to set material parameter before defining material in \"" << fileName << "\"!" << std::endl;
				return false;
			}
			
			std::string mapName;
			ss >> mapName;

			Asset* pAsset;

			if(!pTextureManager->GetAsset(mapName, pAsset))
			{
				std::cerr << "- in " << fileName << std::endl;

				return false;
			}

			pCurrent->m_pNormalMap = static_cast<Asset_Texture*>(pAsset);
		}
		else if(header == "height" || header == "map_Height")
		{
			if(pCurrent == NULL)
			{
				std::cerr << "Attempted to set material parameter before defining material in \"" << fileName << "\"!" << std::endl;
				return false;
			}
			
			std::string mapName;
			ss >> mapName;

			Asset* pAsset;

			if(!pTextureManager->GetAsset(mapName, pAsset))
			{
				std::cerr << "- in " << fileName << std::endl;

				return false;
			}

			pCurrent->m_pHeightMap = static_cast<Asset_Texture*>(pAsset);
		}
	}

	// Set last shader
	assert(!materials.empty());

	return true;
}
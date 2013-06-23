#pragma once

#include <AssetManager/Asset.h>
#include <SFML/Graphics/Texture.hpp>

class Asset_Texture :
	public Asset
{
public:
	sf::Texture m_texture;

	// Inherited from Asset
	bool CreateAsset(const std::string &name);

	static Asset* Asset_Factory()
	{
		return new Asset_Texture();
	}
};


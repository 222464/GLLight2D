#include <AssetManager/Asset_Texture.h>

bool Asset_Texture::CreateAsset(const std::string &name)
{
	return m_texture.loadFromFile(name);
}
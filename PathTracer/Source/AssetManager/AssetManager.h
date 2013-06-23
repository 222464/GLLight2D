#pragma once

#include <AssetManager/Asset.h>

#include <SFML/System/NonCopyable.hpp>

#include <unordered_map>

/* 
Asset manager requires a factory function for a particular derivative of the Asset class.
This can be provided in the ctor or later using Create(...)
Stuff returned by GetAsset functions may be casted to the derived class
*/

class AssetManager :
	public sf::NonCopyable
{
private:
	std::unordered_map<std::string, Asset*> m_assets;

	// Factory function pointer
	Asset* (*m_assetFactory)();

public:
	AssetManager();
	AssetManager(Asset* (*assetFactory)());
	~AssetManager();

	void Create(Asset* (*assetFactory)());

	bool Created();

	bool GetAsset(const std::string &name);
	bool GetAsset(const std::string &name, Asset* &pAsset);
	bool GetAsset(const std::string &name, void* pData);
	bool GetAsset(const std::string &name, Asset* &pAsset, void* pData);
	void DestroyAsset(const std::string &name);
	void ClearAssets();
};
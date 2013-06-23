#pragma once

#include <string>

#include <SFML/System/NonCopyable.hpp>

class Asset :
	public sf::NonCopyable
{
public:
	virtual ~Asset();
	virtual bool CreateAsset(const std::string &name);
	virtual bool CreateAsset(const std::string &name, void* pData);
};


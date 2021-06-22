#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <LevelTexture.hpp>

#include <memory>
#include <unordered_map>
#include <string>

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	unsigned int GetTexel(std::string name, const glm::vec2 floor) const;
	inline LevelTexture* GetLevelTexture(const std::string& name) const
	{
		const auto& iter = _wallTextures.find(name);
		if (iter != _wallTextures.end())
		{
			const auto& levelTexture = iter->second;
			return levelTexture;
		}

		return nullptr;
	}
private:
	std::unordered_map<std::string, LevelTexture*> _wallTextures;

	LevelTexture* CreateLevelTexture(std::string name, std::string filePath);
};
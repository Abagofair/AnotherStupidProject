#pragma once

#include <glm/glm.hpp>

#include <memory>
#include <string>

class LevelTexture
{
public:
	LevelTexture(
		std::string name,
		unsigned short width,
		unsigned short height,
		unsigned int* texels);

	~LevelTexture();

	std::string name;
	unsigned short width;
	unsigned short height;
	unsigned int* texels;

	glm::uvec2 GetFloorAndCeilingCoordinates(const glm::vec2& floor);
	unsigned int GetTexel(short x, short y);
};
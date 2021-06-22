#include <LevelTexture.hpp>

LevelTexture::LevelTexture(
	std::string name,
	unsigned short width,
	unsigned short height,
	unsigned int* texels) : name(name), width(width), height(height), texels(texels)
{
}

LevelTexture::~LevelTexture()
{
	delete texels;
}

glm::uvec2 LevelTexture::GetFloorAndCeilingCoordinates(const glm::vec2& floor)
{
	glm::uvec2 cell(floor.x, floor.y);

	int cellX = (int)(floor.x);
	int cellY = (int)(floor.y);

	// get the texture coordinate from the fractional part
	int tx = (int)(width * (floor.x - cell.x)) & (width - 1);
	int ty = (int)(height * (floor.y - cell.y)) & (height - 1);

	return glm::uvec2(tx, ty);
}

unsigned int LevelTexture::GetTexel(short x, short y)
{
	return texels[height * x + y];
}

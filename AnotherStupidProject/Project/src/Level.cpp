#include <Level.hpp>

bool Level::IsWallHit(short x, short y) const
{
	return _wallTextures[y][x] > 0;
}

std::string Level::GetWallTexture(short wallId) const
{
	if (wallId == 1)
		return "wall";

	return "";
}
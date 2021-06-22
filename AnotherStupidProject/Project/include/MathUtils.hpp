#pragma once

#include <glm/glm.hpp>

class MathUtils
{
public:
	static glm::vec2 RotateVec2(const glm::vec2& v, float angle)
	{
		glm::vec2 rotated;

		double oldDirX = v.x;
		rotated.x = v.x * cos(angle) - v.y * sin(angle);
		rotated.y = oldDirX * sin(angle) + v.y * cos(angle);

		return rotated;
	}
};
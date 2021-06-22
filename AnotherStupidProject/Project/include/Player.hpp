#pragma once

#include <MathUtils.hpp>

#include <InputManager.hpp>
#include <glm/glm.hpp>

#include <functional>

class Player
{
public:
	Player(
		glm::vec2 position,
		glm::vec2 forward,
		glm::vec2 right);

	void Update(float deltaTime);

	glm::vec2 position;
	glm::vec2 forward;
	glm::vec2 right;

	void MoveLeft(InputManager::InputEventParameters* params);
	void MoveRight(InputManager::InputEventParameters* params);
	void MoveForward(InputManager::InputEventParameters* params);
	void MoveBack(InputManager::InputEventParameters* params);
	void MouseRotate(InputManager::InputEventParameters* params);
private:

	float _moveSpeed = 0.05f;
};
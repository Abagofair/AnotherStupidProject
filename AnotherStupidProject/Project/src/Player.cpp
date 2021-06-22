#include <Player.hpp>

Player::Player(glm::vec2 position, glm::vec2 forward, glm::vec2 right)
	: position(position), forward(forward), right(right)
{
}

void Player::MoveLeft(InputManager::InputEventParameters* params)
{
	position.x -= right.x * _moveSpeed;
	position.y -= right.y * _moveSpeed;
}

void Player::MoveRight(InputManager::InputEventParameters* params)
{
	position.x += right.x * _moveSpeed;
	position.y += right.y * _moveSpeed;
}

void Player::MoveForward(InputManager::InputEventParameters* params)
{
	position.x += forward.x * _moveSpeed;
	position.y += forward.y * _moveSpeed;
}

void Player::MoveBack(InputManager::InputEventParameters* params)
{
	position.x -= forward.x * _moveSpeed;
	position.y -= forward.y * _moveSpeed;
}

void Player::MouseRotate(InputManager::InputEventParameters* params)
{
	const InputManager::InputEventMouseParameters* castParams = static_cast<InputManager::InputEventMouseParameters*>(params);

	double rot = -castParams->relativeMouseX * 0.01f;

	right = MathUtils::RotateVec2(right, rot);
	forward = MathUtils::RotateVec2(forward, rot);
}
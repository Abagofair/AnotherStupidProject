#include <InputManager.hpp>

InputManager* InputManager::_instance = nullptr;

InputManager* InputManager::GetInstance()
{
	if (_instance == nullptr)
	{
		_instance = new InputManager();
	}
	
	return _instance;
}
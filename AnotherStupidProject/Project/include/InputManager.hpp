#pragma once

#include <SDL.h>

#include <unordered_map>
#include <functional>

#include <queue>
#include <vector>


//not thread-safe
class InputManager {
public:
	static InputManager* GetInstance();

	struct InputEventParameters
	{
		float deltaTime;
	};

	struct InputEventMouseParameters : InputEventParameters
	{
		float relativeMouseX;
		float relativeMouseY;
	};

	struct InputEventSubscriber
	{
		unsigned int id;
		SDL_EventType eventType;
		SDL_Keycode actionkey;
		std::function<void(InputEventParameters*)> handler;
	};

	enum class KeyState 
	{
		Pressed,
		Released 
	};

	//todo:
	//subscribe to events - get event result
	// 	event result: Key pressed, event type, sdl structure
	//	Sub(SDL_MouseMotion, EventHandlerFunction(SDL_Event))

	void Subscribe(InputEventSubscriber subscriber)
	{
		if (subscriber.eventType == SDL_KEYDOWN || subscriber.eventType == SDL_KEYUP)
		{
			_keySubscribers.push_back(subscriber);
			return;
		}

		const auto& key = _subscribers.find(subscriber.eventType);
		if (key == _subscribers.end())
		{
			_subscribers[subscriber.eventType] = std::unordered_map<unsigned int, InputEventSubscriber>();
		}

		_subscribers[subscriber.eventType][subscriber.id] = subscriber;
	}

	void Consume()
	{
		//todo: this whole input handler parameter shit could be more generic
		for (const auto& keySubscriber : _keySubscribers)
		{
			const auto& keyState = _keyStates.find(keySubscriber.actionkey);
			if (keyState == _keyStates.end())
				continue;

			if (SDLEventToKeyState(keySubscriber.eventType) == keyState->second)
			{
				InputEventParameters* params = new InputEventParameters();
				params->deltaTime = 0.0f;
				keySubscriber.handler(params);
				delete params;
			}
		}

		for (const auto& event : _events)
		{
			const auto& eventTypeSubscribers = _subscribers.find(static_cast<SDL_EventType>(event.type));
			if (eventTypeSubscribers == _subscribers.end())
				continue;

			for (const auto& subscriber : eventTypeSubscribers->second)
			{
				if (event.key.keysym.sym == subscriber.second.actionkey ||
					subscriber.second.actionkey == 0)
				{
					InputEventParameters* params = CreateParameters(event);
					subscriber.second.handler(params);
					delete params;
				}
			}
		}

		_events.clear();
	}

	InputEventParameters* CreateParameters(SDL_Event event)
	{
		if (event.type == SDL_MOUSEMOTION)
		{
			InputEventMouseParameters* params = new InputEventMouseParameters();
			params->deltaTime = 0.0f;
			params->relativeMouseX = event.motion.xrel;
			params->relativeMouseY = event.motion.yrel;
			return params;
		}

		InputEventParameters* params = new InputEventParameters();
		params->deltaTime = 0.0f;
		return params;
	}

	void ReadEventQueue()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_KEYDOWN:
				case SDL_KEYUP:
				{
					_keyStates[event.key.keysym.sym] = SDLEventToKeyState(static_cast<SDL_EventType>(event.type));
					break;
				}
				case SDL_MOUSEMOTION:
				case SDL_QUIT:
				{
					_events.push_back(event);
					break;
				}
			}
		}
	}

	void ClearQueue()
	{
		//todo: handle fault
		_events.empty();
	}

private:
	InputManager()
	{
		_events = std::vector<SDL_Event>();
		_subscribers = std::unordered_map<SDL_EventType, std::unordered_map<unsigned int, InputEventSubscriber>>();
		_keySubscribers = std::vector<InputEventSubscriber>();
		_keyStates = std::unordered_map<SDL_Keycode, KeyState>();
	}

	~InputManager()
	{
		delete _instance;
	}

	KeyState SDLEventToKeyState(SDL_EventType eventType)
	{
		return eventType == SDL_KEYDOWN
			? KeyState::Pressed
			: KeyState::Released;
	}

	InputManager(const InputManager& input) = delete;
	InputManager& operator=(const InputManager&) = delete;

	static InputManager* _instance;

	//subscribe to event_type
	std::unordered_map<SDL_EventType, std::unordered_map<unsigned int, InputEventSubscriber>> _subscribers;

	std::vector<InputEventSubscriber> _keySubscribers;

	std::unordered_map<SDL_Keycode, KeyState> _keyStates;

	std::vector<SDL_Event> _events;
};
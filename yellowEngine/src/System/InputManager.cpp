#include "yellowEngine/Utility/Utils.hpp"
#include "yellowEngine/System/Display.hpp"
#include "yellowEngine/System/InputManager.hpp"


namespace yellowEngine
{
	InputManager* InputManager::__instance;


	InputManager::InputManager()
	{
		assert(__instance == nullptr);

		__instance = this;
		for (int i = 0; i < Num_Keys; i++)
		{
			_keys[i] = State_Released;
		}

		for (int i = 0; i < Num_MouseButtons; i++)
		{
			_mouseButtons[i] = State_Released;
		}
	}


	InputManager::~InputManager()
	{
	}


	bool InputManager::getKey(int keyCode)
	{
		return __instance->_keys[keyCode] == State_Pressed;
	}


	bool InputManager::getKeyDown(int keyCode)
	{
		return __instance->_keys[keyCode] == State_Down;
	}


	bool InputManager::getKeyUp(int keyCode)
	{
		return __instance->_keys[keyCode] == State_Up;
	}


	bool InputManager::getMouseButton(int button)
	{
		return __instance->_mouseButtons[button] == State_Pressed;
	}


	bool InputManager::getMouseButtonDown(int button)
	{
		return __instance->_mouseButtons[button] == State_Down;
	}


	bool InputManager::getMouseButtonUp(int button)
	{
		return __instance->_mouseButtons[button] == State_Up;
	}


	Vector2 InputManager::getMousePosition()
	{
		return __instance->_newMousePosition;
	}


	Vector2 InputManager::getDeltaMousePosition()
	{
		return __instance->_newMousePosition - __instance->_prevMousePosition;
	}


	float InputManager::getMouseScroll()
	{
		return __instance->_mouseScroll;
	}


	void InputManager::keyCallback(int keyCode, bool pressed)
	{
		_keys[keyCode] = pressed ? State_Down : State_Up;
		_changedKeys.insert(keyCode);
	}


	void InputManager::mouseButtonCallback(int buttonCode, bool pressed)
	{
		_mouseButtons[buttonCode] = pressed ? State_Down : State_Up;
		_changedMouseButtons.insert(buttonCode);
	}

	void InputManager::mouseCursorCallback(float x, float y)
	{
		_newMousePosition.x = x;
		_newMousePosition.y = Display::height - y;
	}


	void InputManager::mouseScrollCallback(float y)
	{
		_mouseScroll = y;
	}


	void InputManager::initMousePosition(float x, float y)
	{
		_prevMousePosition.x = x;
		_prevMousePosition.y = Display::height - y;

		_newMousePosition.x = x;
		_newMousePosition.y = Display::height - y;
	}


	void InputManager::update()
	{
		_mouseScroll = 0;
		for (int keyCode : _changedKeys)
		{
			if (_keys[keyCode] == State_Up)
			{
				_keys[keyCode] = State_Released;
			}
			else if (_keys[keyCode] == State_Down)
			{
				_keys[keyCode] = State_Pressed;
			}
		}
		_changedKeys.clear();

		for (int buttonCode : _changedMouseButtons)
		{
			if (_mouseButtons[buttonCode] == State_Up)
			{
				_mouseButtons[buttonCode] = State_Released;
			}
			else if (_mouseButtons[buttonCode] == State_Down)
			{
				_mouseButtons[buttonCode] = State_Pressed;
			}
		}
		_changedMouseButtons.clear();

		_prevMousePosition = _newMousePosition;
	}
}
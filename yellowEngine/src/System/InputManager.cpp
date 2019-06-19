#include "yellowEngine/System/InputManager.hpp"

namespace yellowEngine
{
	GLFWwindow* InputManager::_window;
	InputManager::State InputManager::_keys[Num_Keys];
	InputManager::State InputManager::_mouseButtons[Num_MouseButtons];
	Vector2 InputManager::_mousePosition;
	Vector2 InputManager::_deltaMousePosition;


	void InputManager::init(GLFWwindow* window)
	{
		_window = window;
		for (int i = 0; i < Num_Keys; i++)
		{
			_keys[i] = State_Released;
		}

		for (int i = 0; i < Num_MouseButtons; i++)
		{
			_mouseButtons[i] = State_Released;
		}
		double x, y;
		glfwGetCursorPos(_window, &x, &y);
		_mousePosition = Vector2((float)x, (float)y);
		_deltaMousePosition = Vector2(0, 0);
	}


	bool InputManager::getKey(int keyCode)
	{
		return _keys[keyCode] == State_Pressed;
	}


	bool InputManager::getKeyDown(int keyCode)
	{
		return _keys[keyCode] == State_Down;
	}


	bool InputManager::getKeyUp(int keyCode)
	{
		return _keys[keyCode] == State_Up;
	}


	bool InputManager::getMouseButton(int button)
	{
		return _mouseButtons[button] == State_Pressed;
	}


	bool InputManager::getMouseButtonDown(int button)
	{
		return _mouseButtons[button] == State_Down;
	}


	bool InputManager::getMouseButtonUp(int button)
	{
		return _mouseButtons[button] == State_Up;
	}


	const Vector2& InputManager::getMousePosition()
	{
		return _mousePosition;
	}


	const Vector2& InputManager::getMouseDeltaPosition()
	{
		return _deltaMousePosition;
	}


	void InputManager::update()
	{
		for (int i = 0; i < Num_Keys; i++)
		{
			int state = glfwGetKey(_window, i);
			if (state == GLFW_RELEASE)
			{
				if (_keys[i] == State_Down || _keys[i] == State_Pressed)
				{
					_keys[i] = State_Up;
				}
				else
				{
					_keys[i] = State_Released;
				}
			}
			else if (state == GLFW_PRESS)
			{
				if (_keys[i] == State_Up || _keys[i] == State_Released)
				{
					_keys[i] = State_Down;
				}
				else
				{
					_keys[i] = State_Pressed;
				}
			}
		}

		for (int i = 0; i < Num_MouseButtons; i++)
		{
			int state = glfwGetMouseButton(_window, i);
			if (state == GLFW_RELEASE)
			{
				if (_mouseButtons[i] == State_Down || _mouseButtons[i] == State_Pressed)
				{
					_mouseButtons[i] = State_Up;
				}
				else
				{
					_mouseButtons[i] = State_Released;
				}
			}
			else if (state == GLFW_PRESS)
			{
				if (_mouseButtons[i] == State_Up || _mouseButtons[i] == State_Released)
				{
					_mouseButtons[i] = State_Down;
				}
				else
				{
					_mouseButtons[i] = State_Pressed;
				}
			}
		}

		double x, y;
		glfwGetCursorPos(_window, &x, &y);
		Vector2 newMousePosition((float)x, (float)y);
		_deltaMousePosition = newMousePosition - _mousePosition;
		_mousePosition = newMousePosition;
	}
}
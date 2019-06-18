#ifndef __H_INPUTMANAGER__
#define __H_INPUTMANAGER__

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "yellowEngine/Math/Vector2.hpp"

// corrupted by static :o
namespace yellowEngine
{
	class InputManager
	{
		friend class Game;
	public:
		static void init(GLFWwindow* window);
		static bool getKey(int keyCode);
		static bool getKeyDown(int keyCode);
		static bool getKeyUp(int keyCode);

		static bool getMouseButton(int button);
		static bool getMouseButtonDown(int button);
		static bool getMouseButtonUp(int button);
		static const Vector2& getMousePosition();
		static const Vector2& getMouseDeltaPosition();

	private:
		static constexpr int Num_Keys = GLFW_KEY_LAST;
		static constexpr int Num_MouseButtons = GLFW_MOUSE_BUTTON_LAST;

		enum State
		{
			State_Released,
			State_Pressed,
			State_Up,
			State_Down
		};

		InputManager() = delete;
		~InputManager() = delete;
		
		static void update();

		static GLFWwindow* _window;
		static State _keys[Num_Keys];
		static State _mouseButtons[Num_MouseButtons];
		static Vector2 _mousePosition;
		static Vector2 _mouseDeltaPosition;
	};
}

#endif
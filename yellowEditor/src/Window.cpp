#include <GLFW/glfw3.h>

#include "Window.hpp"


namespace yellowEditor
{
	Window::Window(int width, int height, const char* name) :width(width), height(height)
	{
		handle = glfwCreateWindow(width, height, name, NULL, NULL);
	}
}
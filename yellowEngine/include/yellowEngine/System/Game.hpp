#ifndef __H_GAME__
#define __H_GAME__

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "yellowEngine/Collision/ColliderManager.hpp"

namespace yellowEngine
{
	class Game
	{
	public:
		static void createWindow(std::string name, int width, int height);
		static void init();
		static int getWidth();
		static int getHeight();
		static float getAspectRatio();
		static std::string getResourcePath(const char* fileName);
		static void run();

		// configurable properties
		static ColliderManager::BroadPhaseType broadPhaseType;

	private:
		static GLFWwindow* _window;
		static std::string _resourcePath;
		static float _width;
		static float _height;

		Game() = delete;
		~Game() = delete;
	};
}

#endif
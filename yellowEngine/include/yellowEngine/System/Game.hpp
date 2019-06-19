#ifndef __H_GAME__
#define __H_GAME__

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "yellowEngine/Collision/ColliderManager.hpp"

namespace yellowEngine
{
	class IUpdatable;

	class Game
	{
	public:
		static void createWindow(std::string name, int width, int height);
		static void init();
		static int getWidth();
		static int getHeight();
		static float getAspectRatio();
		static std::string getResourcePath(const char* fileName);
		
		static void addUpdatable(IUpdatable* target);
		static void removeUpdatable(IUpdatable* target);
		static void trimUpdatable();
		static void run();

		// configurable properties
		static ColliderManager::BroadPhaseType broadPhaseType;

	private:
		static GLFWwindow* _window;
		static std::string _resourcePath;
		static int _width;
		static int _height;
		static std::vector<IUpdatable*> _updatables;
		static int _removedCount;

		static constexpr int TrimCount = 32;

		Game() = delete;
		~Game() = delete;
	};

	class IUpdatable
	{
	public:
		IUpdatable()
		{
			Game::addUpdatable(this);
		}
		virtual ~IUpdatable()
		{
			Game::removeUpdatable(this);
		}
		virtual void update() = 0;
	};
}

#endif
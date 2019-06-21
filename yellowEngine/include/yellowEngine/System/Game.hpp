#ifndef __H_GAME__
#define __H_GAME__

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "yellowEngine/Collision/ColliderManager.hpp"

int main(void);

namespace yellowEngine
{
	class IUpdatable;
	class Game
	{
		friend int ::main(void);

	public:
		static std::string getAssetPath(const char* fileName);
		
		static void addUpdatable(IUpdatable* target);
		static void removeUpdatable(IUpdatable* target);
		static void trimUpdatable();

	private:
		static Game* _instance;

		Game(std::string name, int width, int height);
		~Game();
		Game(const Game* game) = delete;
		void init();
		void run();

		//////// configurable properties
		ColliderManager::BroadPhaseType broadPhaseType;

		GLFWwindow* _window;
		std::string _assetPath;
		std::vector<IUpdatable*> _updatables;
		int _removedCount;

	};
}

#endif
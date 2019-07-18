#ifndef __H_GAME__
#define __H_GAME__

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "yellowEngine/System/InputManager.hpp"
#include "yellowEngine/Component/Camera.hpp"
#include "yellowEngine/Collision/ColliderManager.hpp"

int main();

namespace yellowEngine
{
	class IUpdatable;
	class Game
	{
		friend int ::main();

	public:
		static void addUpdatable(IUpdatable* target);
		static void removeUpdatable(IUpdatable* target);
		static void trimUpdatable();

	private:
		static Game* __instance;

		Game(int width, int height);
		~Game();
		Game(const Game* game) = delete;
		void init();

		void start();
		void update();
		void render(Camera* camera);

		static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void glfwCursorCallback(GLFWwindow* window, double xpos, double ypos);
		static void glfwScrollCallback(GLFWwindow* window, double x, double y);

		// configurable properties
		ColliderManager::BroadPhaseType broadPhaseType;

		// managers
		InputManager* _inputManager;
		ColliderManager* _colliderManager;

		std::string _assetPath;
		std::vector<IUpdatable*> _updatables;
		int _removedCount;

		//CubeMap* _skybox;
	};
}

#endif
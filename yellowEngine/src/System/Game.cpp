#include <iostream>

#include "yellowEngine/yellowEngine.hpp"
#include "yellowEngine/System/Display.hpp"
#include "yellowEngine/System/IUpdatable.hpp"
#include "yellowEngine/System/Game.hpp"


namespace yellowEngine
{
	static const int TrimCount = 32;
	Game* Game::__instance;


	Game::Game(int width, int height)
	{
		__instance = this;

		Display::_width = width;
		Display::_height = height;
		Display::_aspectRatio = (float)width / (float)height;

		broadPhaseType = ColliderManager::BroadPhaseType_SAP;

		_assetPath = "./res/";

		IUpdatable::reportCreate = Game::addUpdatable;
		IUpdatable::reportDestroy = Game::removeUpdatable;
	}


	Game::~Game()
	{
	}


	void Game::init()
	{
		_inputManager = new InputManager();
		_colliderManager = ColliderManager::create(broadPhaseType);

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_BACK);
		glViewport(0, 0, Display::width, Display::height);

		// pre load primitives
		Model::create("./res/Mesh/cube.obj");
		Model::create("./res/Mesh/quad.obj");
		Model::create("./res/Mesh/sphere.obj");
		Model::create("./res/Mesh/cone.obj");
	}


	void Game::start()
	{
		for (int i = 0; i < _updatables.size(); i++)
		{
			if (_updatables[i]) _updatables[i]->start();
		}
	}


	void Game::update()
	{
		_colliderManager->detect();
		Animator::proceedAll();

		// maybe delay updatable creation/deletion?
		for (int i = 0; i < _updatables.size(); i++)
		{
			if (_updatables[i]) _updatables[i]->update();
		}

		// update input state at the end of frame
		_inputManager->update();
	}


	void Game::render(Camera* camera)
	{
		Technique::renderScene(camera);
		UIRenderer::renderAll();

		// render skybox
	}


	void Game::glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == -1) return;
		if (action == GLFW_REPEAT) return;
		__instance->_inputManager->keyCallback(key, action == GLFW_PRESS);
	}


	void Game::glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		if (button == -1) return;
		__instance->_inputManager->mouseButtonCallback(button, action == GLFW_PRESS);
	}


	void Game::glfwCursorCallback(GLFWwindow* window, double xpos, double ypos)
	{
		__instance->_inputManager->mouseCursorCallback((float)xpos, (float)ypos);
	}


	void Game::glfwScrollCallback(GLFWwindow* window, double x, double y)
	{
		__instance->_inputManager->mouseScrollCallback((float)y);
	}


	void Game::addUpdatable(IUpdatable* target)
	{
		__instance->_updatables.push_back(target);
	}


	// removing updatable and triming list for every request might be expansive
	// just set removed updatable as nullptr and trim them when removedCount is big enough (or when trimUpdatable is called)
	// we cannot simply change target with last element because changing excution order is not desirable
	void Game::removeUpdatable(IUpdatable* target)
	{
		int count = __instance->_updatables.size();
		int i = 0;

		for (auto it = __instance->_updatables.begin();; ++it)
		{
			i++;
			if (*it == target)
			{
				*it = nullptr;
				if (++__instance->_removedCount == TrimCount)
				{
					trimUpdatable();
				}
				return;
			}
		}
	}


	void Game::trimUpdatable()
	{
		int& removeCount = __instance->_removedCount;
		auto& updatables = __instance->_updatables;

		int trimed = 0;
		for (int i = 0; i + trimed < updatables.size(); i++)
		{
			while (updatables[i + trimed] == nullptr)
			{
				if (++trimed == removeCount) break;
			}

			if (i + trimed == updatables.size())
			{
				updatables[i] = updatables[i + trimed - 1];
			}
			else
			{
				updatables[i] = updatables[i + trimed];
			}
		}
		updatables.resize(updatables.size() - removeCount);
		removeCount = 0;
	}
}
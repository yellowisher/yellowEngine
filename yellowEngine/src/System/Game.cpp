#include <iostream>

#include "yellowEngine/yellowEngine.hpp"
#include "yellowEngine/System/Display.hpp"
#include "yellowEngine/System/IUpdatable.hpp"
#include "yellowEngine/System/Game.hpp"


namespace yellowEngine
{
	static const int TrimCount = 32;
	Game* Game::_instance;


	Game::Game(std::string name, int width, int height)
	{
		_instance = this;

		Display::_width = width;
		Display::_height = height;
		Display::_aspectRatio = (float)width / (float)height;

		_assetPath = "./res/";

		// init glfw, glad
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		_window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
		if (_window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(_window);
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return;
		}
	}


	Game::~Game()
	{
	}


	void Game::init()
	{
		InputManager::init(_window);
		ColliderManager::create(broadPhaseType);

		glEnable(GL_CULL_FACE);
	}


	void Game::run()
	{
		while (!glfwWindowShouldClose(_window))
		{
			glfwPollEvents();
			InputManager::update();
			Technique::renderScene();

			for (auto updatable : _updatables)
			{
				if (updatable) updatable->update();
			}

			glfwSwapBuffers(_window);
		}
		glfwTerminate();
	}


	std::string Game::getAssetPath(const char* fileName)
	{
		return _instance->_assetPath + fileName;
	}


	void Game::addUpdatable(IUpdatable* target)
	{
		_instance->_updatables.push_back(target);
	}


	// removing updatable and triming list for every request might be expansive
	// just set removed updatable as nullptr and trim them when removedCount is big enough (or when trimUpdatable is called)
	// we cannot simply change target with last element because changing excution order is not desirable
	void Game::removeUpdatable(IUpdatable* target)
	{
		for (auto it = _instance->_updatables.begin();; ++it)
		{
			if (*it == target)
			{
				*it = nullptr;
				if (++_instance->_removedCount == TrimCount)
				{
					trimUpdatable();
				}
			}
		}
	}


	void Game::trimUpdatable()
	{
		int& removeCount = _instance->_removedCount;
		auto& updatables = _instance->_updatables;

		int trimed = 0;
		for (int i = 0; i + trimed < updatables.size(); i++)
		{
			while (updatables[i + trimed] == nullptr)
			{
				if (++trimed == removeCount)break;
			}

			updatables[i] = updatables[i + trimed];
		}
		updatables.resize(updatables.size() - removeCount);
		removeCount = 0;
	}
}
#include <iostream>

#include "yellowEngine/yellowEngine.hpp"
#include "yellowEngine/System/Game.hpp"


namespace yellowEngine
{
	GLFWwindow* Game::_window;
	std::string Game::_resourcePath = "./res/";
	int Game::_width;
	int Game::_height;
	std::vector<IUpdatable*> Game::_updatables;
	int Game::_removedCount = 0;

	ColliderManager::BroadPhaseType Game::broadPhaseType = ColliderManager::BroadPhaseType_SAP;


	void Game::createWindow(std::string name, int width, int height)
	{
		_width = width;
		_height = height;

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

	void Game::init()
	{
		InputManager::init(_window);
		ColliderManager::create(broadPhaseType);
	}


	int Game::getWidth()
	{
		return _width;
	}


	int Game::getHeight()
	{
		return _height;
	}


	float Game::getAspectRatio()
	{
		return (float)_width / (float)_height;
	}


	std::string Game::getResourcePath(const char* fileName)
	{
		return _resourcePath + fileName;
	}


	void Game::addUpdatable(IUpdatable* target)
	{
		_updatables.push_back(target);
	}


	// removing updatable and triming list for every request might be expansive
	// just set removed updatable as nullptr and trim them when removedCount is big enough (or when trimUpdatable is called)
	// we cannot simply change target with last element because changing excution order is not desirable
	void Game::removeUpdatable(IUpdatable* target)
	{
		for (auto it = _updatables.begin();; ++it)
		{
			if (*it == target)
			{
				*it = nullptr;
				if (++_removedCount == TrimCount)
				{
					trimUpdatable();
				}
			}
		}
	}


	void Game::trimUpdatable()
	{
		int trimed = 0;
		for (int i = 0; i + trimed < _updatables.size(); i++)
		{
			while (_updatables[i + trimed] == nullptr)
			{
				if (++trimed == _removedCount)break;
			}

			_updatables[i] = _updatables[i + trimed];
		}
		_updatables.resize(_updatables.size() - _removedCount);
		_removedCount = 0;
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
}
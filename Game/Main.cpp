#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <yellowEngine/yellowEngine.hpp>

#include "Scripts/Unit.hpp"
#include "Scripts/Spawner.hpp"
#include "Scripts/GameManager.hpp"
#include "Scripts/CameraScript.hpp"

using namespace yellowEngine;

int main()
{
	//int width = 1440;
	//int height = 900;
	int width = 1920;
	int height = 1080;
	const char* name = "yellowEngine";

	// init glfw, glad
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, name, monitor, NULL);
	//GLFWwindow* window = glfwCreateWindow(width, height, name, NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// init game
	Game* game = new Game(width, height);
	//game->broadPhaseType = ColliderManager::BroadPhaseType_BVH;
	game->broadPhaseType = ColliderManager::BroadPhaseType_SAP;

	game->init();

	glfwSetKeyCallback(window, game->glfwKeyCallback);
	glfwSetMouseButtonCallback(window, game->glfwMouseButtonCallback);
	glfwSetCursorPosCallback(window, game->glfwCursorCallback);
	glfwSetScrollCallback(window, game->glfwScrollCallback);
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	game->_inputManager->initMousePosition((float)x, (float)y);

	////////// Scene

	SceneManager::loadScene("C:\\Users\\yApy\\Desktop\\Game\\Asset\\Scene\\TraceScene.yes");
	new GameManager();
	Camera::getMainCamera()->gameObject->addComponent<CameraScript>();

	////////// Scene end

	game->start();
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		game->update();
		game->render(Camera::getMainCamera());
		//ColliderManager::getInstance()->renderColliders(Camera::getMainCamera());

		glfwSwapBuffers(window);
	}
	glfwTerminate();

	return 0;
}
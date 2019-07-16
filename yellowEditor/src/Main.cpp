#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <glad/glad.h>

#include <yellowEngine/yellowEngine.hpp>

#include "Window.hpp"
#include "Gizmo.hpp"
#include "Editor.hpp"

using namespace yellowEngine;
using namespace yellowEditor;

GLubyte* gameData;
GLubyte* gameDataFlipped;
GLuint sceneTexture;

int main()
{
	if (!glfwInit()) return 1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	Window editorWindow(1440, 900, "yellowEditor");
	Window gameWindow(800, 600, "gameWindow");

	// setup editor window
	glfwMakeContextCurrent(editorWindow.handle);
	glfwSwapInterval(1); // enable vsync

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "GLAD initialization failed!" << std::endl;
		return 1;
	}
	glClearColor(255.0f / 255.0f, 225.0f / 255.0f, 144.0f / 255.0f, 1.0f);

	// create texture for scene window
	glGenTextures(1, &sceneTexture);
	glBindTexture(GL_TEXTURE_2D, sceneTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gameWindow.width, gameWindow.height, 0, GL_RGB, GL_BYTE, nullptr);

	Editor* editor = new Editor(editorWindow, gameWindow, sceneTexture);

	// setup game window
	glfwMakeContextCurrent(gameWindow.handle);
	glfwHideWindow(gameWindow.handle);

	Game* game = new Game(gameWindow.width, gameWindow.height);
	game->broadPhaseType = ColliderManager::BroadPhaseType_BVH;
	game->init();

	gameData        = new GLubyte[gameWindow.width * gameWindow.height * 3];
	gameDataFlipped = new GLubyte[gameWindow.width * gameWindow.height * 3];

	InitGizmo();

	editor->createEditorCamera();
	editor->setWelcomeScene();

	glfwMakeContextCurrent(gameWindow.handle);
	game->start();

	while (!glfwWindowShouldClose(editorWindow.handle))
	{
		// update game window first
		glfwMakeContextCurrent(gameWindow.handle);
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			game->update();
			game->render(editor->getEditorCamera());

			Transform* transform = Editor::getSelectedTransform();
			if (transform != nullptr)
			{
				Collider* collider = transform->gameObject->getComponent<Collider>();
				if (collider != nullptr)
				{
					ColliderManager::getInstance()->renderCollider(editor->getEditorCamera(), collider);
				}
			}
			//ColliderManager::getInstance()->renderColliders(editor->getEditorCamera());
			DrawGizmo();

			glfwSwapBuffers(gameWindow.handle);
		}

		glReadPixels(0, 0, gameWindow.width, gameWindow.height, GL_RGB, GL_BYTE, gameData);
		// flip y axis of scene data
		for (int y = 0; y < gameWindow.height; y++)
		{
			memcpy(
				gameDataFlipped + (y * gameWindow.width * 3),
				gameData		+ ((gameWindow.height - 1 - y) * gameWindow.width * 3),
				gameWindow.width * 3 * sizeof(GLubyte));
		}

		// then update main window
		glfwMakeContextCurrent(editorWindow.handle);
		{
			// copy data into texture
			glBindTexture(GL_TEXTURE_2D, sceneTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gameWindow.width, gameWindow.height, 0, GL_RGB, GL_BYTE, gameDataFlipped);

			glfwPollEvents();
			glClear(GL_COLOR_BUFFER_BIT);

			editor->update();
			editor->render();

			glfwSwapBuffers(editorWindow.handle);
		}
	}
	
	glfwDestroyWindow(editorWindow.handle);
	glfwTerminate();

	return 0;
}
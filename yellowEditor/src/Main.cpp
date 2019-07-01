#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <glad/glad.h>

#include <yellowEngine/yellowEngine.hpp>

#include "Window.hpp"
#include "Editor.hpp"

using namespace yellowEngine;
using namespace yellowEditor;

GLuint* gameData;
GLuint* gameDataFlipped;
GLuint sceneTexture;

int main()
{
	if (!glfwInit()) return 1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gameWindow.width, gameWindow.height, 0, GL_RGB, GL_UNSIGNED_INT, nullptr);

	Editor* editor = new Editor(editorWindow, gameWindow, sceneTexture);

	// setup game window
	glfwMakeContextCurrent(gameWindow.handle);
	glfwHideWindow(gameWindow.handle);

	Game* game = new Game(gameWindow.width, gameWindow.height);
	game->broadPhaseType = ColliderManager::BroadPhaseType_BVH;
	game->init();

	gameData        = new GLuint[gameWindow.width * gameWindow.height * 3];
	gameDataFlipped = new GLuint[gameWindow.width * gameWindow.height * 3];

#pragma region Scene
	Mesh* boxMesh = Mesh::create("Mesh/cube.obj");
	Material* boxMaterial = new Material("asdasd");

	boxMaterial->setProperty("u_Material.diffuse", Texture::create("Texture/container2.png"));
	boxMaterial->setProperty("u_Material.specular", Texture::create("Texture/container2_specular.png"));
	boxMaterial->setTechnique(Technique::getTechnique(TechniqueType_Deferred), "Shader/texture.vert", "Shader/texture.frag");

	GameObject* box = new GameObject("Box1");
	auto renderer = box->addComponent<MeshRenderer>();
	renderer->set(boxMesh, boxMaterial);

	//Model* sphere = Model::create("Mesh/sphere.obj");
	//sphere->instantiate("Sphere");

	/*GameObject* sp = sphere->instantiate("Sphere1");


	Model* model = Model::create("Mesh/nanosuit/nanosuit.obj");

	GameObject* nano = model->instantiate("Model");
	GameObject* spotLightGo = new GameObject();
	Light* sl = spotLightGo->addComponent<Light>()->setType(Light::LightType_Spot);
	sl->diffuseIntensity = 1.0f;
	sl->linear = 0.07f;
	sl->quadratic = 0.002f;
	spotLightGo->transform->setPosition(0, 10, 5);

	auto parent1 = new GameObject("Parent1");
	auto parent2 = new GameObject("Parent2");

	auto child1 = new GameObject("Child1");
	auto child2 = new GameObject("Child2");
	auto child3 = new GameObject("Child1");

	parent1->transform->addChild(child1->transform);
	parent1->transform->addChild(child2->transform);
	parent1->transform->addChild(child3->transform);*/

	GameObject* dirLight = new GameObject();
	dirLight->addComponent<Light>()->setType(Light::LightType_Dir)->diffuseIntensity = 0.8f;
	dirLight->transform->setRotation(45, 0, 0);

	//GameObject* b1 = new GameObject("Box1");
	//b1->addComponent<BoxCollider>();

	//GameObject* b2 = new GameObject("Box2");
	//b2->addComponent<BoxCollider>();

	//GameObject* b3 = new GameObject("Box3");
	//b3->addComponent<BoxCollider>();

	editor->createEditorCamera();
#pragma endregion

	while (!glfwWindowShouldClose(editorWindow.handle))
	{
		// update game window first
		glfwMakeContextCurrent(gameWindow.handle);
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			game->update();
			game->render(editor->getEditorCamera());
			ColliderManager::getInstance()->renderColliders(editor->getEditorCamera());

			glfwSwapBuffers(gameWindow.handle);
		}

		glReadPixels(0, 0, gameWindow.width, gameWindow.height, GL_RGB, GL_UNSIGNED_INT, gameData);
		// flip y axis of scene data
		for (int y = 0; y < gameWindow.height; y++)
		{
			memcpy(
				gameDataFlipped + (y * gameWindow.width * 3),
				gameData		+ ((gameWindow.height - 1 - y) * gameWindow.width * 3),
				gameWindow.width * 3 * sizeof(GLuint));
		}

		// then update main window
		glfwMakeContextCurrent(editorWindow.handle);
		{
			// copy data into texture
			glBindTexture(GL_TEXTURE_2D, sceneTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gameWindow.width, gameWindow.height, 0, GL_RGB, GL_UNSIGNED_INT, gameDataFlipped);

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
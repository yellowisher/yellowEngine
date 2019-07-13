#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <glad/glad.h>

#include <yellowEngine/yellowEngine.hpp>

#include "Window.hpp"
#include "Editor.hpp"

using namespace yellowEngine;
using namespace yellowEditor;

GLubyte* gameData;
GLubyte* gameDataFlipped;
GLuint sceneTexture;

class AnimationController : public IUpdatable, public Component
{
	BEGIN_COMPONENT(AnimationController)
	END_COMPONENT

public:
	AnimationController(GameObject* gameObject) : Component(gameObject) {}

	void update() override
	{
		auto anim = gameObject->getComponent<Animator>();

		static float f = 0;
		if (InputManager::getKeyDown(GLFW_KEY_1))
		{
			anim->gotoFrame((f += 0.5f));
		}
	}
};
COMPONENT_IMPL(AnimationController)

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

#pragma region Scene
	Material* mat = new Material("Infantry");
	mat->setProperty("u_Material.diffuse", Texture::create("./res/Mesh/RTS/blue.tga"));

	Model* m = Model::create("./res/Mesh/RTS/infantry.fbx");
	GameObject* mg = m->instantiate("infantry", mat);
	mg->transform->setScale(0.01, 0.01, 0.01);
	auto anim = mg->addComponent<Animator>();
	anim->setSpeed(0.5f);
	anim->play(m->getClip("archer_attack"));

	//Model* q = Model::create("./res/Mesh/RTS/infantry_04_attack_A.FBX");
	//auto qq = q->instantiate("Anim");

	//auto anim = mg->addComponent<Animator>();
	//auto clip = AnimationClip::create("./res/Mesh/RTS/infantry_04_attack_A.FBX", m);
	//auto clip = m->getClip(0);

	//anim->play(clip);

	//Model* model = Model::create("./res/Mesh/nanosuit/nanosuit.obj");
	//GameObject* nano = model->instantiate("Model");
	//nano->transform->setScale(0.3, 0.3, 0.3);
	//auto anim = nano->addComponent<Animator>();

	//auto clip = AnimationClip::create("./res/Animation/haha.yea");
	//anim->play(clip);

	Mesh* boxMesh = Mesh::create("./res/Mesh/cube.obj");
	GameObject* box = new GameObject("box");
	Material* boxMaterial = new Material("asdasd");

	box->addComponent<MeshRenderer>()->set(boxMesh, boxMaterial);
	box->transform->setScale(10, 0.1, 10);


	GameObject* dirLight = new GameObject();
	dirLight->addComponent<Light>()->setType(Light::LightType_Dir);
	dirLight->transform->setRotation(-45, 0, 0);

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
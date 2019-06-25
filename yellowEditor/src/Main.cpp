#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <nanogui/nanogui.h>
#include <yellowEngine/yellowEngine.hpp>
#include <iostream>

using namespace nanogui;
using namespace yellowEngine;

class CameraScript : IUpdatable
{
public:
	void update() override
	{
		static const float moveSpeed = 0.06f;
		static const float rotateSpeedX = 0.016f;
		static const float rotateSpeedY = 0.008f;
		static Vector3 rotation = Vector3(0, 0, 0);

		// translate
		Vector3 move = Vector3::zero;
		if (InputManager::getKey(GLFW_KEY_A)) move.x -= 1.0f;
		if (InputManager::getKey(GLFW_KEY_D)) move.x += 1.0f;
		if (InputManager::getKey(GLFW_KEY_W)) move.z += 1.0f;
		if (InputManager::getKey(GLFW_KEY_S)) move.z -= 1.0f;
		if (InputManager::getKey(GLFW_KEY_SPACE)) move.y += 1.0f;
		if (InputManager::getKey(GLFW_KEY_LEFT_CONTROL)) move.y -= 1.0f;

		Transform* transform = Camera::getMainCamera()->transform;
		Vector3 forward = transform->getForward();
		forward.y = 0; forward.normalize();

		Vector3 right = transform->getRight();
		right.y = 0; right.normalize();

		Vector3 up = Vector3::up;

		Vector3 movement = Vector3::zero;
		movement += forward * move.z;
		movement += right * move.x;
		movement += up * move.y;

		transform->translate(movement * moveSpeed);

		// rotate
		Vector2 rotate = InputManager::getDeltaMousePosition();
		rotation.x += -rotate.y * rotateSpeedX;
		rotation.y += -rotate.x *rotateSpeedY;

		transform->setRotation(rotation);
	}
};

class LightScript : public IUpdatable, public Component
{
public:
	LightScript(GameObject* gameObject) : Component(gameObject) {}

	void update() override
	{
		Vector3 move = Vector3::zero;
		if (InputManager::getKey(GLFW_KEY_L)) move.x -= 1.0f;
		if (InputManager::getKey(GLFW_KEY_APOSTROPHE)) move.x += 1.0f;
		if (InputManager::getKey(GLFW_KEY_P)) move.z -= 1.0f;
		if (InputManager::getKey(GLFW_KEY_SEMICOLON)) move.z += 1.0f;
		if (InputManager::getKey(GLFW_KEY_I)) move.y += 1.0f;
		if (InputManager::getKey(GLFW_KEY_K)) move.y -= 1.0f;

		transform->translate(move * 0.06f);
	}
};

class GLWindow
{
public:
	GLWindow(int width, int height, const char* name)
	{
		this->width = width;
		this->height = height;
		window = glfwCreateWindow(width, height, name, nullptr, nullptr);
	}
	~GLWindow()
	{
	}

	void bind()
	{
		glfwMakeContextCurrent(window);
	}

	GLFWwindow* window;
	int width;
	int height;
};

enum test_enum
{
	Item1 = 0,
	Item2,
	Item3
};

bool bvar = true;
int ivar = 12345678;
double dvar = 3.1415926;
float fvar = (float)dvar;
std::string strval = "A string";
test_enum enumval = Item2;
Color colval(0.5f, 0.5f, 0.7f, 1.f);

Screen *screen = nullptr;

int main()
{
	glfwInit();

	glfwSetTime(0);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLWindow* gameWindow = new GLWindow(1280, 720, "game");

	glfwWindowHint(GLFW_SAMPLES, 0);
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	GLWindow* mainWindow = new GLWindow(1440, 1080, "yellowEditor");

	mainWindow->bind();
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "GLAD initialization failed!" << std::endl;
		return -1;
	}

	glClearColor(0.2f, 0.25f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	screen = new Screen();
	screen->initialize(mainWindow->window, true);

	//int width, height;
	//glfwGetFramebufferSize(mainWindow->window, &width, &height);
	//glViewport(0, 0, width, height);
	//glfwSwapInterval(0);
	//glfwSwapBuffers(mainWindow->window);


	// copying scene
	mainWindow->bind();
	GLuint* data = new GLuint[gameWindow->width * gameWindow->height * 3];
	GLuint* flippedData = new GLuint[gameWindow->width * gameWindow->height * 3];

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gameWindow->width, gameWindow->height, 0, GL_RGB, GL_UNSIGNED_INT, nullptr);

	// Create nanogui gui
	bool enabled = true;

	Window* window = new Window(screen, "Game Scene");
	window->setLayout(new GroupLayout());

	auto imageView = new ImageView(window, texture);
	imageView->bindImage(texture);

	screen->setVisible(true);
	screen->performLayout();

	glfwSetCursorPosCallback(mainWindow->window,
							 [](GLFWwindow *, double x, double y)
	{
		screen->cursorPosCallbackEvent(x, y);
	}
	);

	glfwSetMouseButtonCallback(mainWindow->window,
							   [](GLFWwindow *, int button, int action, int modifiers)
	{
		screen->mouseButtonCallbackEvent(button, action, modifiers);
	}
	);

	glfwSetKeyCallback(mainWindow->window,
					   [](GLFWwindow *, int key, int scancode, int action, int mods)
	{
		screen->keyCallbackEvent(key, scancode, action, mods);
	}
	);

	glfwSetCharCallback(mainWindow->window,
						[](GLFWwindow *, unsigned int codepoint)
	{
		screen->charCallbackEvent(codepoint);
	}
	);

	glfwSetDropCallback(mainWindow->window,
						[](GLFWwindow *, int count, const char **filenames)
	{
		screen->dropCallbackEvent(count, filenames);
	}
	);

	glfwSetScrollCallback(mainWindow->window,
						  [](GLFWwindow *, double x, double y)
	{
		screen->scrollCallbackEvent(x, y);
	}
	);

	glfwSetFramebufferSizeCallback(mainWindow->window,
								   [](GLFWwindow *, int width, int height)
	{
		screen->resizeCallbackEvent(width, height);
	}
	);

	gameWindow->bind();
	glfwSetInputMode(gameWindow->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//glfwHideWindow(gameWindow->window);

	Game* game = new Game(gameWindow->width, gameWindow->height);
	game->init();

	glfwSetKeyCallback(gameWindow->window, game->glfwKeyCallback);
	glfwSetMouseButtonCallback(gameWindow->window, game->glfwMouseButtonCallback);
	glfwSetCursorPosCallback(gameWindow->window, game->glfwCursorCallback);
	double x, y;
	glfwGetCursorPos(gameWindow->window, &x, &y);
	game->_inputManager->initMousePosition((float)x, (float)y);

	////////// Scene
	Model* model = Model::create("Mesh/nanosuit/nanosuit.obj");

	GameObject* go = model->instantiate("nanosuit");
	go->transform->setPosition(0, 0, 0);

	GameObject* spotLightGo = new GameObject();
	Light* sl = spotLightGo->addComponent<Light>()->setType(Light::LightType_Spot);
	sl->diffuseIntensity = 1.0f;
	sl->linear = 0.07f;
	sl->quadratic = 0.002f;
	spotLightGo->addComponent<LightScript>();
	spotLightGo->transform->setPosition(0, 10, 5);

	GameObject* dirLight = new GameObject();
	dirLight->addComponent<Light>()->setType(Light::LightType_Dir)->diffuseIntensity = 0.8f;
	dirLight->transform->setRotation(45, 0, 0);

	GameObject* cameraGo = new GameObject();
	Camera* camera = cameraGo->addComponent<Camera>();
	camera->setPerspective(60.0f, 0.01f, 1000.0f);
	camera->transform->setPosition(0, 10, 5);

	new CameraScript();

	// Game loop
	while (!glfwWindowShouldClose(mainWindow->window))
	{
		gameWindow->bind();
		{
			// gameWindow
			glfwPollEvents();

			game->update();
			game->render();

			glfwSwapBuffers(gameWindow->window);
			glReadPixels(0, 0, gameWindow->width, gameWindow->height, GL_RGB, GL_UNSIGNED_INT, data);
		}
		//

		for (int y = 0; y < gameWindow->height; y++)
		{
			memcpy(flippedData + (y * gameWindow->width * 3), data + ((gameWindow->height - 1 - y) * gameWindow->width * 3), gameWindow->width * 3 * sizeof(GLuint));
		}

		mainWindow->bind();
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gameWindow->width, gameWindow->height, 0, GL_RGB, GL_UNSIGNED_INT, flippedData);
		imageView->bindImage(texture);
		{
			glfwPollEvents();

			glClearColor(0.2f, 0.25f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// Draw nanogui
			screen->drawContents();
			screen->drawWidgets();

			glfwSwapBuffers(mainWindow->window);
		}
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();

	return 0;
}
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "yellowEngine/yellowEngine.hpp"


#define GL_GLEXT_PROTOTYPES

#include <nanogui/nanogui.h>
#include <iostream>

using namespace yellowEngine;
using namespace nanogui;

#pragma region Scripts

class CameraScript : IUpdatable
{
public:
	void update() override
	{
		static const float moveSpeed = 0.06f;
		static const float rotateSpeedX = 0.16f;
		static const float rotateSpeedY = 0.08f;
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

#pragma endregion

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
	Game* game = new Game("yellowEditor", 1280, 720);
	game->init();

	GLFWwindow* window = game->_window;

	// Create a nanogui screen and pass the glfw pointer to initialize
	screen = new Screen();
	screen->initialize(window, true);

	// Create nanogui gui
	bool enabled = true;
	FormHelper *gui = new FormHelper(screen);
	nanogui::ref<Window> nanoguiWindow = gui->addWindow(Eigen::Vector2i(10, 10), "Form helper example");
	gui->addGroup("Basic types");
	gui->addVariable("bool", bvar)->setTooltip("Test tooltip.");
	gui->addVariable("string", strval);

	gui->addGroup("Validating fields");
	gui->addVariable("int", ivar)->setSpinnable(true);
	gui->addVariable("float", fvar)->setTooltip("Test.");
	gui->addVariable("double", dvar)->setSpinnable(true);

	gui->addGroup("Complex types");
	gui->addVariable("Enumeration", enumval, enabled)->setItems({ "Item 1", "Item 2", "Item 3" });
	gui->addVariable("Color", colval)
		->setFinalCallback([](const Color &c)
	{
		std::cout << "ColorPicker Final Callback: ["
			<< c.r() << ", "
			<< c.g() << ", "
			<< c.b() << ", "
			<< c.w() << "]" << std::endl;
	});

	gui->addGroup("Other widgets");
	gui->addButton("A button", []() { std::cout << "Button pressed." << std::endl; })->setTooltip("Testing a much longer tooltip, that will wrap around to new lines multiple times.");;

	screen->setVisible(true);
	screen->performLayout();
	nanoguiWindow->center();

	glfwSetCursorPosCallback(window,
							 [](GLFWwindow *, double x, double y)
	{
		screen->cursorPosCallbackEvent(x, y);
	}
	);

	glfwSetMouseButtonCallback(window,
							   [](GLFWwindow *, int button, int action, int modifiers)
	{
		screen->mouseButtonCallbackEvent(button, action, modifiers);
	}
	);

	glfwSetKeyCallback(window,
					   [](GLFWwindow *, int key, int scancode, int action, int mods)
	{
		screen->keyCallbackEvent(key, scancode, action, mods);
	}
	);

	glfwSetCharCallback(window,
						[](GLFWwindow *, unsigned int codepoint)
	{
		screen->charCallbackEvent(codepoint);
	}
	);

	glfwSetDropCallback(window,
						[](GLFWwindow *, int count, const char **filenames)
	{
		screen->dropCallbackEvent(count, filenames);
	}
	);

	glfwSetScrollCallback(window,
						  [](GLFWwindow *, double x, double y)
	{
		screen->scrollCallbackEvent(x, y);
	}
	);

	glfwSetFramebufferSizeCallback(window,
								   [](GLFWwindow *, int width, int height)
	{
		screen->resizeCallbackEvent(width, height);
	}
	);

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

	//GameObject* lightGo = new GameObject();
	//lightGo->addComponent<LightScript>();
	//Light* light = lightGo->addComponent<Light>()->setType(Light::LightType_Point);
	//light->linear = 0.007f;
	//light->quadratic = 0.0002f;
	//lightGo->transform->setPosition(0, 10, 5);

	//GameObject* dirLight = new GameObject();
	//dirLight->addComponent<Light>()->setType(Light::LightType_Dir)->diffuseIntensity = 0.6f;
	//dirLight->transform->setRotation(45, 180, 0);

	GameObject* cameraGo = new GameObject();
	Camera* camera = cameraGo->addComponent<Camera>();
	camera->setPerspective(60.0f, 0.01f, 1000.0f);
	camera->transform->setPosition(0, 10, 5);

	new CameraScript();

	while (!game->pollEvents())
	{
		glClearColor(0.2f, 0.25f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		game->pollEvents();
		game->update();
		game->render();

		screen->drawContents();
		screen->drawWidgets();
		game->swapBuffer();
	}



	return 0;
}
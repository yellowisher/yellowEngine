#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <nanogui/nanogui.h>
#include <iostream>

using namespace nanogui;

#include "yellowEngine/yellowEngine.hpp"

using namespace yellowEngine;

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

int main(int /* argc */, char ** /* argv */)
{

	glfwInit();

	glfwSetTime(0);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//glfwWindowHint(GLFW_SAMPLES, 0);
	//glfwWindowHint(GLFW_RED_BITS, 8);
	//glfwWindowHint(GLFW_GREEN_BITS, 8);
	//glfwWindowHint(GLFW_BLUE_BITS, 8);
	//glfwWindowHint(GLFW_ALPHA_BITS, 8);
	//glfwWindowHint(GLFW_STENCIL_BITS, 8);
	//glfwWindowHint(GLFW_DEPTH_BITS, 24);
	//glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	// Create a GLFWwindow object
	GLFWwindow* window = glfwCreateWindow(800, 800, "example3", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw std::runtime_error("Could not initialize GLAD!");
	glGetError(); // pull and ignore unhandled errors like GL_INVALID_ENUM

	// Create a nanogui screen and pass the glfw pointer to initialize
	screen = new Screen();
	screen->initialize(window, true);

	//int width, height;
	//glfwGetFramebufferSize(window, &width, &height);
	//glViewport(0, 0, width, height);
	//glfwSwapInterval(0);
	//glfwSwapBuffers(window);

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

	System* system = System::getInstance();
	system->setWidth(1024);
	system->setHeight(768);
	system->setResourcePath("./res/");
	
	ColliderManager* colManager = ColliderManager::create(ColliderManager::BroadPhaseType_SAP);

	Mesh* cubeMesh = Mesh::create("Mesh/cube.obj");
	Shader* colorShader = Shader::create("Shader/texture.vert", "Shader/texture.frag");
	Texture* diffuseMap = Texture::create("Texture/container2.png");
	Texture* specularMap = Texture::create("Texture/container2_specular.png");

	unsigned int lightsIndex = glGetUniformBlockIndex(colorShader->getId(), "LightBlock");
	glUniformBlockBinding(colorShader->getId(), lightsIndex, 0);

	//unsigned int cameraIndex = glGetUniformBlockIndex(colorShader->getId(), "CameraBlock");
	//glUniformBlockBinding(colorShader->getId(), cameraIndex, 1);

	GameObject* cubeGo = new GameObject("MovingCube");
	BoxCollider* boxMove = cubeGo->addComponent<BoxCollider>();
	MeshRenderer* cubeRenderer = cubeGo->addComponent<MeshRenderer>()->set(cubeMesh, colorShader);
	//boxTransform = cubeGo->transform;
	//boxTransform->setScale(2.0f, 1.0f, 4.0f);

	GameObject* cb = new GameObject("InitialStay");
	cb->addComponent<BoxCollider>();

	GameObject* cubeGo2 = new GameObject("StayCube");
	BoxCollider* boxStatic = cubeGo2->addComponent<BoxCollider>();
	MeshRenderer* cubeRenderer2 = cubeGo2->addComponent<MeshRenderer>()->set(cubeMesh, colorShader);
	cubeGo2->transform->translate(20.0f, 0, 0);

	GameObject* dl = new GameObject();
	Light* l = dl->addComponent<Light>()->setDirectional();
	l->transform->rotate(45.0f, 0, 0);

	GameObject* dirLightGo = new GameObject("dirLight");
	Shader* lightShader = Shader::create("Shader/light.vert", "Shader/light.frag");
	dirLightGo->addComponent<MeshRenderer>()->set(cubeMesh, lightShader);
	Light* light = dirLightGo->addComponent<Light>()->setPoint(1.0f, 0.14f, 0.07f);

	light->transform->rotate(45.0f, 0, 0);
	light->transform->setScale(0.2f, 0.2f, 0.2f);
	light->transform->setPosition(1.2f, 1.0f, 2.0f);

	GameObject* cameraGo = new GameObject();
	Camera* camera = cameraGo->addComponent<Camera>();
	camera->setPerspective(60.0f, 0.01f, 100.0f);
	camera->transform->setPosition(0, 0, 3.0f);
	//cameraTransform = cameraGo->transform;
	//cameraTransform->translate(0, 0, 2.0f);

	ObjectRenderer::_currentCamera = camera;

	cubeRenderer->addTexture(diffuseMap, "u_Material.diffuse");
	cubeRenderer->addTexture(specularMap, "u_Material.specular");

	cubeRenderer2->addTexture(diffuseMap, "u_Material.diffuse");
	cubeRenderer2->addTexture(specularMap, "u_Material.specular");
	colorShader->setUniform(colorShader->getUniform("u_Material.shininess"), 64.0f);

	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		glClearColor(0.2f, 0.25f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Light::updateUniformBuffer();
		//colManager->detect();
		ObjectRenderer::renderAll(camera);
		//colManager->renderColliders();

		screen->drawContents();
		screen->drawWidgets();

		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();

	return 0;
}
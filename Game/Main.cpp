#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "yellowEngine/yellowEngine.hpp"

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

int main()
{
	int width = 1280;
	int height = 720;
	const char* name = "yellowEngine";

	// init glfw, glad
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, name, NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// init game
	Game* game = new Game(width, height);
	game->init();

	glfwSetKeyCallback(window, game->glfwKeyCallback);
	glfwSetMouseButtonCallback(window, game->glfwMouseButtonCallback);
	glfwSetCursorPosCallback(window, game->glfwCursorCallback);
	double x, y;
	glfwGetCursorPos(window, &x, &y);
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
	//spotLightGo->transform->rotate(32, 57, 22);


	//GameObject* lightGo = new GameObject();
	//lightGo->addComponent<LightScript>();
	//Light* light = lightGo->addComponent<Light>()->setType(Light::LightType_Point);
	//light->linear = 0.007f;
	//light->quadratic = 0.0002f;
	//lightGo->transform->setPosition(0, 10, 5);

	//GameObject* dirLight = new GameObject();
	//dirLight->addComponent<Light>()->setType(Light::LightType_Dir)->diffuseIntensity = 0.6f;
	//dirLight->transform->setRotation(45, 180, 0);

	const char* name1 = Component::getTypeNameOf<Light>();
	const char* name2 = sl->getTypeName();

	std::cout << name1 << std::endl;
	std::cout << name2 << std::endl;
	std::cout << (name1 == name2) << std::endl;

	std::cout << Component::getTypeNameOf<BoxCollider>();

	auto rot = spotLightGo->transform->rotation;
	auto cs = Component::getComponents();
	auto ps = Component::getProperties();

	Transform* tf = spotLightGo->transform;
	Component* cp = tf;
	auto prop = Component::getProperties()["Transform"][2];
	size_t rotationOffset = prop.offset;
	Quaternion rot2 = *(Quaternion*)((size_t)cp + rotationOffset);

	GameObject* cameraGo = new GameObject();
	Camera* camera = cameraGo->addComponent<Camera>();
	camera->setPerspective(60.0f, 0.01f, 1000.0f);
	camera->transform->setPosition(0, 10, 5);

	new CameraScript();

	////////// Scene end

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		game->update();
		game->render(Camera::getMainCamera());

		glfwSwapBuffers(window);
	}
	glfwTerminate();

	return 0;
}
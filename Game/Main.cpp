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
		static const float rotateSpeedX = 0.032f;
		static const float rotateSpeedY = 0.016f;
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
	int width = 1440;
	int height = 900;
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
	game->broadPhaseType = ColliderManager::BroadPhaseType_BVH;
	game->init();

	glfwSetKeyCallback(window, game->glfwKeyCallback);
	glfwSetMouseButtonCallback(window, game->glfwMouseButtonCallback);
	glfwSetCursorPosCallback(window, game->glfwCursorCallback);
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	game->_inputManager->initMousePosition((float)x, (float)y);

	////////// Scene

	//Model* model = Model::create("./res/Mesh/nanosuit/nanosuit.obj");
	//GameObject* nano = model->instantiate("Model");
	//nano->transform->setScale(0.3, 0.3, 0.3);
	Material* boxMaterial = new Material("asdasd");
	Mesh* boxMesh = Mesh::create("./res/Mesh/cube.obj");
	Mesh* sphere = Mesh::create("./res/Mesh/sphere.obj");

	//Model* model1 = Model::create("./res/Mesh/cube.obj");
	//GameObject* nano1 = model1->instantiate("Model");
	//nano1->transform->setPosition(0, -0.5, 0);

	Material* bm = new Material("asdasqwed");
	bm->setProperty("u_Material.diffuseColor", Vector3(1.0, 0.7, 0));

	//GameObject* box2 = new GameObject("box");
	//box2->addComponent<MeshRenderer>()->set(boxMesh, bm);
	//box2->transform->setPosition(0, -1.5, 0);

	GameObject* b3 = new GameObject("box");
	b3->addComponent<MeshRenderer>()->set(boxMesh, bm);
	b3->transform->setPosition(0, -1.5, 2);

	GameObject* b4 = new GameObject("box");
	b4->addComponent<MeshRenderer>()->set(boxMesh, bm);
	b4->transform->setPosition(0, -1.5, 0.5);

	//GameObject* b5 = new GameObject("box");
	//b5->addComponent<MeshRenderer>()->set(boxMesh, bm);
	//b5->transform->setPosition(-1, -0.5, 1);

	GameObject* box = new GameObject("box");
	box->addComponent<MeshRenderer>()->set(boxMesh, boxMaterial);
	box->transform->setScale(10, 0.1, 10);
	box->transform->setPosition(0, -2, 0);

	//GameObject* dirLight = new GameObject();
	//dirLight->addComponent<Light>()->setType(Light::LightType_Point);
	//dirLight->transform->setPosition(0, 1, 0);
	//dirLight->addComponent<LightScript>();

	//GameObject* d1 = new GameObject();
	//d1->addComponent<Light>()->setType(Light::LightType_Spot);
	//d1->transform->setPosition(0, -0.5, 3);
	//d1->addComponent<LightScript>();

	GameObject* d2 = new GameObject();
	auto dlight = d2->addComponent<Light>()->setType(Light::LightType_Dir);
	//dlight->diffuseIntensity = 0.5;
	d2->transform->setPosition(0, 0, 3);
	d2->transform->setRotation(-30, 30, 0);

	GameObject* cameraGo = new GameObject();
	Camera* camera = cameraGo->addComponent<Camera>();
	camera->setPerspective(60.0f, 0.01f, 1000.0f);
	camera->transform->setPosition(0, 0, 3);

	new CameraScript();

	////////// Scene end

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glViewport(0, 0, Display::width, Display::height);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		game->update();
		game->render(Camera::getMainCamera());

		glfwSwapBuffers(window);
	}
	glfwTerminate();

	return 0;
}

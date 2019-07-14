#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "yellowEngine/yellowEngine.hpp"

using namespace yellowEngine;

class CameraScript : public Component, public IUpdatable
{
public:
	CameraScript(GameObject* gameObject) : Component(gameObject) {}

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

class LightScript : public Component, public IUpdatable
{
public:
	LightScript(GameObject* gameObject) : Component(gameObject) {}

	void update() override
	{
		if (InputManager::getKeyDown(GLFW_KEY_1))
		{
			Model* m = Model::create("./res/Mesh/cube.obj");
			//Model* m = Model::create("./res/Mesh/nanosuit/nanosuit.obj");
			m->instantiate("qwe");
		}


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

class AnimationController : public IUpdatable, public Component
{
public:
	AnimationController(GameObject* gameObject) : Component(gameObject) {}

	void update() override
	{
		auto anim = gameObject->getComponent<Animator>();

		static int t = 0;
		if (InputManager::getKeyDown(GLFW_KEY_1))
		{
			Model* m = Model::create("./res/Mesh/RTS/infantry.fbx");
			anim->play(m->getClip(t++));
		}
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
	//Model* m = Model::create("./res/Mesh/RTS/TT_RTS_Character_customizable.FBX");
	//Material* mat = new Material("Infantry");
	//mat->setProperty("u_Material.diffuse", Texture::create("./res/Mesh/RTS/blue.tga"));

	//Model* m = Model::create("./res/Mesh/RTS/infantry.fbx");
	//GameObject* mg = m->instantiate("infantry", mat);
	//mg->transform->setScale(0.01, 0.01, 0.01);
	//auto anim = mg->addComponent<Animator>();
	//anim->setSpeed(0.5f);
	//mg->addComponent<AnimationController>();

	//mg->addComponent<AnimationController>();
	//mg->transform->setScale(10,10,10);
	//mg->transform->setRotation(-90, 0, 0);
	//mg->transform->setPosition(0, -1, 0);
	Material* boxMaterial = new Material("asdasd");

	auto que = Model::create("./res/Mesh/quad.obj");
	que->instantiate("tt");

	//Model* model = Model::create("./res/Mesh/nanosuit/nanosuit.obj");
	//GameObject* nano = model->instantiate("Model");
	//nano->transform->setScale(0.3, 0.3, 0.3);

	Mesh* boxMesh = Mesh::create("./res/Mesh/cube.obj");
	//Mesh* sphere = Mesh::create("./res/Mesh/sphere.obj");

	//Model* model1 = Model::create("./res/Mesh/cube.obj");
	//GameObject* nano1 = model1->instantiate("Model");
	//nano1->transform->setPosition(0, -0.5, 0);

	//Model* model2 = Model::create("./res/Mesh/base.fbx");
	//GameObject* nano2 = model2->instantiate("Model");
	//nano2->transform->setScale(0.3, 0.3, 0.3);

	//Material* bm = new Material("asdasqwed");
	//bm->setProperty("u_Material.diffuseColor", Vector3(1.0, 0.7, 0));

	Material* wm = new Material("QWE");

	//GameObject* box2 = new GameObject("box");
	//box2->addComponent<MeshRenderer>()->set(boxMesh, bm);

	GameObject* b3 = new GameObject("box");
	b3->addComponent<MeshRenderer>()->set(boxMesh, wm);
	b3->transform->setPosition(0, -1, 0);
	b3->transform->setScale(10, 1, 10);

	//GameObject* b4 = new GameObject("box");
	//b4->addComponent<MeshRenderer>()->set(boxMesh, bm);
	//b4->transform->setPosition(0, -1.5, 0.5);

	//GameObject* b5 = new GameObject("box");
	//b5->addComponent<MeshRenderer>()->set(boxMesh, bm);
	//b5->transform->setPosition(-1, -0.5, 1);

	//GameObject* box = new GameObject("box");
	//box->addComponent<MeshRenderer>()->set(boxMesh, boxMaterial);
	//box->transform->setScale(10, 0.1, 10);
	//box->transform->setPosition(0, -2, 0);

	//GameObject* dirLight = new GameObject();
	//dirLight->addComponent<Light>()->setType(Light::LightType_Point);
	//dirLight->transform->setPosition(0, 1, 0);
	//dirLight->addComponent<LightScript>();

	//GameObject* d1 = new GameObject();
	//d1->addComponent<Light>()->setType(Light::LightType_Spot);
	//d1->transform->setPosition(0, -0.5, 3);
	//d1->addComponent<LightScript>();

	Material* texmat = new Material("tm");
	texmat->setProperty("u_Material.diffuse", Texture::create("./res/Texture/folder.png"));

	GameObject* d2 = new GameObject();
	auto dlight = d2->addComponent<Light>()->setType(Light::LightType_Dir);
	d2->transform->setPosition(0, 0, 3);
	d2->transform->setRotation(-30, 30, 0);
	d2->addComponent<LightScript>();

	GameObject* cameraGo = new GameObject();
	Camera* camera = cameraGo->addComponent<Camera>();
	camera->setPerspective(60.0f, 0.01f, 1000.0f);
	camera->transform->setPosition(0, 0, 3);
	cameraGo->addComponent<CameraScript>();

	////////// Scene end

	int f = 0;
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (++f == 30)
		{
			Model* m1 = Model::create("./res/Mesh/hqad.obj");
			Model* m = Model::create("./res/Mesh/heu.obj");
			//m->instantiate("qweqwe");

			Model::create("./res/Mesh/quad.obj")->instantiate("qweqwe", texmat);
		}

		game->update();
		game->render(Camera::getMainCamera());

		glfwSwapBuffers(window);
	}
	glfwTerminate();

	return 0;
}

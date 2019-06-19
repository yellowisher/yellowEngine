#include "yellowEngine/yellowEngine.hpp"

using namespace yellowEngine;

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
		if (InputManager::getKey(GLFW_KEY_A				)) move.x -= 1.0f;
		if (InputManager::getKey(GLFW_KEY_D				)) move.x += 1.0f;
		if (InputManager::getKey(GLFW_KEY_W				)) move.z -= 1.0f;
		if (InputManager::getKey(GLFW_KEY_S				)) move.z += 1.0f;
		if (InputManager::getKey(GLFW_KEY_SPACE			)) move.y += 1.0f;
		if (InputManager::getKey(GLFW_KEY_LEFT_CONTROL	)) move.y -= 1.0f;

		Transform* transform = Camera::getMainCamera()->transform;
		Vector3 forward = transform->getForward();
		forward.y = 0; forward.normalize();

		Vector3 right = transform->getRight();
		right.y = 0; right.normalize();

		Vector3 up = Vector3::up;

		Vector3 movement = Vector3::zero;
		movement += forward * move.z;
		movement += right	* move.x;
		movement += up		* move.y;

		transform->translate(movement * moveSpeed);

		// rotate
		Vector2 rotate = InputManager::getDeltaMousePosition();
		rotation.x += -rotate.y * rotateSpeedX;
		rotation.y += -rotate.x *rotateSpeedY;

		transform->setRotation(rotation);
	}
};

int main(void)
{
	Game::createWindow("yellowEngine", 1280, 720);
	Game::init();
	
	////////// Scene
	Model* model = Model::create("Mesh/nanosuit/nanosuit.obj");

	GameObject* go = model->instantiate("nanosuit");

	GameObject* lightGo = new GameObject();
	Light* light = lightGo->addComponent<Light>()->setType(Light::LightType_Dir);
	lightGo->transform->rotate(45, 135.0f, 0);

	GameObject* cameraGo = new GameObject();
	Camera* camera = cameraGo->addComponent<Camera>();
	camera->setPerspective(60.0f, 0.01f, 1000.0f);

	new CameraScript();

	////////// Scene end

	Game::run();

	return 0;
}
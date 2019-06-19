using namespace std;

#include <iostream>

#include "yellowEngine/yellowEngine.hpp"
using namespace yellowEngine;

Transform* cameraTransform;
Transform* boxTransform;

float lastX = 1024.0f / 2.0f;
float lastY = 768.0f / 2.0f;
bool firstMouse = true;

float _yaw = 0;
float _pitch = 0;

float speed = 1.0f;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = lastX - (float)xpos;
	float yoffset = lastY - (float)ypos;

	lastX = (float)xpos;
	lastY = (float)ypos;

	_yaw += xoffset * 0.08f;
	_pitch += yoffset * 0.16f;

	cameraTransform->setRotation(_pitch, _yaw, 0);
}

GameObject* g = nullptr;
bool prevPressed = false;
bool pressed = false;

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Vector3 forward = cameraTransform->getForward();
		forward.y = 0;
		forward.normalize();
		cameraTransform->translate(forward * -0.06f*speed);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Vector3 forward = cameraTransform->getForward();
		forward.y = 0;
		forward.normalize();
		cameraTransform->translate(forward * 0.06f*speed);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cameraTransform->translate(cameraTransform->getRight() * -0.06f*speed);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cameraTransform->translate(cameraTransform->getRight() * 0.06f*speed);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		cameraTransform->translate(0, -0.06f*speed, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		cameraTransform->translate(0, 0.06f*speed, 0);
	}

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		boxTransform->translate(boxTransform->getForward()*-0.06f);
	}
	if (glfwGetKey(window, GLFW_KEY_SEMICOLON) == GLFW_PRESS)
	{
		boxTransform->translate(boxTransform->getForward()*0.06f);
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
	{
		boxTransform->translate(boxTransform->getRight()*-0.06f);
	}
	if (glfwGetKey(window, GLFW_KEY_APOSTROPHE) == GLFW_PRESS)
	{
		boxTransform->translate(boxTransform->getRight()*0.06f);
	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
	{
		boxTransform->rotate(0, 3.0f, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS)
	{
		boxTransform->rotate(0, -3.0f, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
	{
		Vector3 scale = boxTransform->scale;
		scale.x += 0.01f;
		scale.y += 0.01f;
		scale.z += 0.01f;
		boxTransform->setScale(scale);
	}
	if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
	{
		Vector3 scale = boxTransform->scale;
		scale.x -= 0.01f;
		scale.y -= 0.01f;
		scale.z -= 0.01f;
		boxTransform->setScale(scale);
	}

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		pressed = true;
	}
	else
	{
		pressed = false;
	}
}

int main(void)
{
	Game::createWindow("yellowEngine", 1280, 720);
	Game::init();

	Model* model = Model::create("Mesh/nanosuit/nanosuit.obj");

	GameObject* go = model->instantiate("nanosuit");

	GameObject* lightGo = new GameObject();
	Light* light = lightGo->addComponent<Light>()->setType(Light::LightType_Dir);
	lightGo->transform->rotate(45, 135.0f, 0);

	GameObject* cameraGo = new GameObject();
	Camera* camera = cameraGo->addComponent<Camera>();
	camera->setPerspective(60.0f, 0.01f, 1000.0f);

	cameraTransform = cameraGo->transform;
	cameraTransform->translate(0, 0, 10);

	Game::run();

	return 0;
}
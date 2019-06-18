using namespace std;

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
	System* system = System::getInstance();
	system->setWidth(1024);
	system->setHeight(768);
	system->setResourcePath("./res/");

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1024, 768, "yellow engine", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	InputManager::init(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//glEnable(GL_CULL_FACE);

	//glfwSetKeyCallback(window, _keyCallback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Shader* textureShader = Shader::create("Shader/texture.vert", "Shader/texture.frag");
	//unsigned int lightsIndex = glGetUniformBlockIndex(textureShader->getId(), "LightBlock");
	//glUniformBlockBinding(textureShader->getId(), lightsIndex, 0);

	//Model* model = Model::create("Mesh/92-dog_anim/dog final.blend");
	//Model* model = Model::create("Mesh/BaseMesh_Anim.fbx");
	Model* model = Model::create("Mesh/nanosuit/nanosuit.obj");

	GameObject* go = model->instantiate("nanosuit");
	//go->transform->setScale(0.01f, 0.01f, 0.01f);
	//go->transform->rotate(270, 0, 0);
	boxTransform = go->transform;

	GameObject* dl = new GameObject();
	Light* light = dl->addComponent<Light>()->setType(Light::LightType_Dir);
	dl->transform->rotate(45, 135.0f, 0);

	GameObject* cameraGo = new GameObject();
	Camera* camera = cameraGo->addComponent<Camera>();
	camera->setPerspective(60.0f, 0.01f, 1000.0f);
	//camera->setPerspective(60.0f, 0.01f, 10000.0f);

	cameraTransform = cameraGo->transform;
	cameraTransform->translate(0, 0, 40);

	ObjectRenderer::_currentCamera = camera;

	//auto clip0 = model->getClips().begin()->second;
	//auto clip1 = (++model->getClips().begin())->second;
	//auto anim = go->addComponent<Animator>();
	//anim->play(clip1);

	int width = System::getInstance()->getWidth();
	int height = System::getInstance()->getHeight();

	Framebuffer* gbuffer = new Framebuffer(4, System::getInstance()->getWidth(), System::getInstance()->getHeight(), GL_RGB, GL_FLOAT, GL_RGB32F);
	Shader* geometryShader = Shader::create("Shader/deferred_geometry.vert", "Shader/deferred_geometry.frag");

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glEnable(GL_DEPTH_TEST);

		glClearColor(0.1, 0.1, 0.1, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ObjectRenderer::renderAll(camera);

		// geometry pass
		//gbuffer->bindForWriting();
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//ObjectRenderer::renderAll(camera, geometryShader);

		//// lighting pass
		//gbuffer->unbind();
		//gbuffer->bindForReading();
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//gbuffer->setBufferToRead(0);
		//glBlitFramebuffer(
		//	0, 0, width, height,
		//	0, 0, width / 2, height / 2,
		//	GL_COLOR_BUFFER_BIT, GL_LINEAR
		//);

		//gbuffer->setBufferToRead(1);
		//glBlitFramebuffer(
		//	0, 0, width, height,
		//	width / 2, 0, width, height / 2,
		//	GL_COLOR_BUFFER_BIT, GL_LINEAR
		//);

		//gbuffer->setBufferToRead(2);
		//glBlitFramebuffer(
		//	0, 0, width, height,
		//	0, height / 2, width / 2, height,
		//	GL_COLOR_BUFFER_BIT, GL_LINEAR
		//);

		//gbuffer->setBufferToRead(3);
		//glBlitFramebuffer(
		//	0, 0, width, height,
		//	width / 2, height / 2, width, height,
		//	GL_COLOR_BUFFER_BIT, GL_LINEAR
		//);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
using namespace std;

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include "ColliderManagerr.hpp"
#include "Matrix.hpp"
#include "GameObject.hpp"
#include "Transform.hpp"
#include "Quaternion.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "MeshRenderer.hpp"
#include "ShaderProgram.hpp"
#include "Matrix.hpp"
#include "Camera.hpp"
#include "System.hpp"
#include "Light.hpp"
#include "BoxCollider.hpp"
#include "SphereCollider.hpp"

#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "lib/glew32.lib")
#pragma comment(lib, "lib/glfw3.lib")

Transform* cameraTransform;
Transform* boxTransform;

float lastX = 1024.0f / 2.0f;
float lastY = 768.0f / 2.0f;
bool firstMouse = true;

float _yaw = 0;
float _pitch = 0;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = lastX - xpos;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	_yaw += xoffset * 0.08f;
	_pitch += yoffset * 0.16f;

	cameraTransform->setRotation(_pitch, _yaw, 0);
}

GameObject* g = nullptr;

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Vector3 forward = cameraTransform->getForward();
		forward.y = 0;
		forward.normalize();
		cameraTransform->translate(forward * -0.06f);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Vector3 forward = cameraTransform->getForward();
		forward.y = 0;
		forward.normalize();
		cameraTransform->translate(forward * 0.06f);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cameraTransform->translate(cameraTransform->getRight() * -0.06f);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cameraTransform->translate(cameraTransform->getRight() * 0.06f);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		cameraTransform->translate(0, -0.06f, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		cameraTransform->translate(0, 0.06f, 0);
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

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		if (g == nullptr)
		{
			g = new GameObject("New");
			g->addComponent<BoxCollider>();
		}
		else
		{
			delete(g);
			g = nullptr;
		}
	}
}


int main(void)
{
	System* system = System::getInstance();
	system->setWidth(1024);
	system->setHeight(768);

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

	if (glewInit() != GLEW_OK)
	{
		cout << "Failed to initialize GLEW" << endl;
		glfwTerminate();
		return -1;
	}
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	ColliderManager* colManager = new ColliderManager(ColliderManager::BroadPhaseType_SAP);

	Mesh* cubeMesh = Mesh::create("../yellowEngine/cube.obj");
	ShaderProgram* colorShader = ShaderProgram::create("../yellowEngine/texture.vert", "../yellowEngine/texture.frag");
	Texture* diffuseMap = Texture::create("../yellowEngine/container2.png");
	Texture* specularMap = Texture::create("../yellowEngine/container2_specular.png");

	unsigned int lightsIndex = glGetUniformBlockIndex(colorShader->getId(), "LightBlock");
	glUniformBlockBinding(colorShader->getId(), lightsIndex, 0);

	//unsigned int cameraIndex = glGetUniformBlockIndex(colorShader->getId(), "CameraBlock");
	//glUniformBlockBinding(colorShader->getId(), cameraIndex, 1);

	GameObject* cubeGo = new GameObject("MovingCube");
	BoxCollider* boxMove = cubeGo->addComponent<BoxCollider>();
	MeshRenderer* cubeRenderer = cubeGo->addComponent<MeshRenderer>()->set(cubeMesh, colorShader);
	boxTransform = cubeGo->transform;
	boxTransform->setScale(2.0f, 1.0f, 4.0f);
	//boxTransform->rotate(45.0f, 0, 90.0f);

	GameObject* cb = new GameObject("InitialStay");
	cb->addComponent<BoxCollider>();

	GameObject* cubeGo2 = new GameObject("StayCube");
	BoxCollider* boxStatic = cubeGo2->addComponent<BoxCollider>();
	MeshRenderer* cubeRenderer2 = cubeGo2->addComponent<MeshRenderer>()->set(cubeMesh, colorShader);
	cubeGo2->transform->translate(5.0f, 0, 0);

	GameObject* dl = new GameObject();
	Light* l = dl->addComponent<Light>()->setDirectional();
	l->transform->rotate(45.0f, 0, 0);

	GameObject* dirLightGo = new GameObject("dirLight");
	ShaderProgram* lightShader = ShaderProgram::create("../yellowEngine/light.vert", "../yellowEngine/light.frag");
	dirLightGo->addComponent<MeshRenderer>()->set(cubeMesh, lightShader);
	Light* light = dirLightGo->addComponent<Light>()->setPoint(1.0f, 0.14f, 0.07f);

	light->transform->rotate(45.0f, 0, 0);
	light->transform->setScale(0.2f, 0.2f, 0.2f);
	light->transform->setPosition(1.2f, 1.0f, 2.0f);

	GameObject* cameraGo = new GameObject();
	Camera* camera = cameraGo->addComponent<Camera>();
	camera->setPerspective(60.0f, 0.01f, 100.0f);
	cameraTransform = cameraGo->transform;
	cameraTransform->translate(0, 0, 2.0f);

	ObjectRenderer::_currentCamera = camera;

	cubeRenderer->addTexture(diffuseMap, "u_Material.diffuse");
	cubeRenderer->addTexture(specularMap, "u_Material.specular");

	cubeRenderer2->addTexture(diffuseMap, "u_Material.diffuse");
	cubeRenderer2->addTexture(specularMap, "u_Material.specular");
	colorShader->setUniform(colorShader->getUniform("u_Material.shininess"), 64.0f);


	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		colManager->detect();
		Light::updateUniformBuffer();
		ObjectRenderer::renderAll(camera);

		colManager->renderColliders();


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
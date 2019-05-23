using namespace std;

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

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

#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "lib/glew32.lib")
#pragma comment(lib, "lib/glfw3.lib")

Transform* cameraTransform;
Transform* lightTransform;

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

	_yaw += xoffset * 0.00002f;
	_pitch += yoffset * 0.00004f;

	cameraTransform->setRotation(0, _yaw, _pitch);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)cameraTransform->translate(cameraTransform->getForward() * -0.06f);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)cameraTransform->translate(cameraTransform->getForward() * 0.06f);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)cameraTransform->translate(cameraTransform->getRight() * -0.06f);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)cameraTransform->translate(cameraTransform->getRight() * 0.06f);

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)lightTransform->translate(0, 0.06f, 0);
	if (glfwGetKey(window, GLFW_KEY_SEMICOLON) == GLFW_PRESS)lightTransform->translate(0, -0.06f, 0);
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)lightTransform->translate(-0.06f, 0, 0);
	if (glfwGetKey(window, GLFW_KEY_APOSTROPHE) == GLFW_PRESS)lightTransform->translate(0.06f, 0, 0);
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)lightTransform->translate(0, 0, -0.06f);
	if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS)lightTransform->translate(0, 0, 0.06f);
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

	GLFWwindow* window = glfwCreateWindow(1024, 768, "Yellow engine", NULL, NULL);
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


	//ShaderProgram* shader = ShaderProgram::create("../YellowEngine/texture.vs", "../YellowEngine/texture.ps");
	//Mesh* mesh = Mesh::create("../YellowEngine/c.obj");
	//Texture* texture = Texture::create("../YellowEngine/wall.jpg");

	//GameObject* parent = new GameObject("GameObject");
	//parent->addComponent<MeshRenderer>();
	//MeshRenderer* meshRenderer0 = parent->getComponent<MeshRenderer>();
	//meshRenderer0->set(mesh, shader);
	//meshRenderer0->setTexture(texture);

	//GameObject* child = new GameObject("GameObject");
	//child->addComponent<MeshRenderer>();
	//MeshRenderer* meshRenderer1 = child->getComponent<MeshRenderer>();
	//meshRenderer1->set(mesh, shader);
	//meshRenderer1->setTexture(texture);

	//GameObject* cameraObject = new GameObject();
	//cameraObject->addComponent<Camera>();
	//Camera* camera = cameraObject->getComponent<Camera>();
	//camera->setPerspective(90.0f, 0.01f, 100.0f);
	//cameraTransform = cameraObject->transform;

	//int t = 0;
	//parent->transform->translate(Vector3(0, 0, -3.0f));
	//parent->transform->setScale(Vector3(2.0f, 4.0f, 1.0f));

	//child->transform->translate(Vector3(2.0f, 0, -3.0f));
	//child->transform->setScale(Vector3(0.7f, 0.7f, 0.7f));
	//child->transform->rotate(Vector3(0, 0, 45.0f));

	Mesh* cubeMesh = Mesh::create("../YellowEngine/cube.obj");
	Texture* diffuseMap = Texture::create("../YellowEngine/container2.png");
	Texture* specularMap = Texture::create("../YellowEngine/container2_specular.png");

	GameObject* cubeGo = new GameObject("Cube");
	ShaderProgram* colorShader = ShaderProgram::create("../YellowEngine/texture.vert", "../YellowEngine/texture.frag");
	cubeGo->addComponent<MeshRenderer>()->set(cubeMesh, colorShader);

	GameObject* lightGo = new GameObject("Light");
	ShaderProgram* lightShader = ShaderProgram::create("../YellowEngine/light.vert", "../YellowEngine/light.frag");
	lightGo->addComponent<MeshRenderer>()->set(cubeMesh, lightShader);

	lightTransform = lightGo->transform;
	lightGo->transform->setScale(0.2f, 0.2f, 0.2f);
	lightGo->transform->setPosition(1.2f, 1.0f, 2.0f);

	GameObject* cameraGo = new GameObject();
	Camera* camera = cameraGo->addComponent<Camera>();
	cameraTransform = cameraGo->transform;
	cameraTransform->translate(0, 0, 2.0f);

	Renderer::_currentCamera = camera;

	colorShader->use();
	colorShader->setUniform(colorShader->getUniform("u_Material.diffuse"), 0);
	colorShader->setUniform(colorShader->getUniform("u_Material.specular"), 1);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		colorShader->use();
		colorShader->setUniform(colorShader->getUniform("u_Light.position"), lightTransform->getWorldPosition());

		colorShader->setUniform(colorShader->getUniform("u_Light.ambient"), Vector3(0.1f, 0.1f, 0.1f));
		colorShader->setUniform(colorShader->getUniform("u_Light.diffuse"), Vector3(0.5f, 0.5f, 0.5f));
		colorShader->setUniform(colorShader->getUniform("u_Light.specular"), Vector3(1.0f, 1.0f, 1.0f));

		colorShader->setUniform(colorShader->getUniform("u_Material.shininess"), 32.0f);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap->_id);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap->_id);

		Renderer::renderAll(camera);


		//Renderer::renderAll(camera);





		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
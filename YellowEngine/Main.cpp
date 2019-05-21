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

float xsum = 0;
float ysum = 0;

bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	float xoffset = 1024.0f / 2.0f - xpos;
	float yoffset = 768.0f / 2.0f - ypos;

	xsum += xoffset;
	ysum += yoffset;

	cameraTransform->setRotation(0, xsum*0.00002f, ysum*0.00004f);
	glfwSetCursorPos(window, 1024.0f / 2.0f, 768.0f / 2.0f);
	//cameraTransform->rotate(0, xoffset*0.00002f, yoffset*0.00004f);
}


void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)cameraTransform->translate(cameraTransform->getForward() * -0.03f);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)cameraTransform->translate(cameraTransform->getForward() * 0.03f);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)cameraTransform->translate(cameraTransform->getRight() * -0.03f);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)cameraTransform->translate(cameraTransform->getRight() * 0.03f);
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
	glfwSetCursorPosCallback(window, mouse_callback);

	ShaderProgram* shader = ShaderProgram::create("../YellowEngine/texture.vs", "../YellowEngine/texture.ps");
	Mesh* mesh = Mesh::create("../YellowEngine/c.obj");
	Texture* texture = Texture::create("../YellowEngine/wall.jpg");

	GameObject* parent = new GameObject("GameObject");
	parent->addComponent<MeshRenderer>();
	MeshRenderer* meshRenderer0 = parent->getComponent<MeshRenderer>();
	meshRenderer0->set(mesh, shader);
	meshRenderer0->setTexture(texture);

	GameObject* child = new GameObject("GameObject");
	child->addComponent<MeshRenderer>();
	MeshRenderer* meshRenderer1 = child->getComponent<MeshRenderer>();
	meshRenderer1->set(mesh, shader);
	meshRenderer1->setTexture(texture);

	GameObject* cameraObject = new GameObject();
	cameraObject->addComponent<Camera>();
	Camera* camera = cameraObject->getComponent<Camera>();
	camera->setPerspective(90.0f, 0.01f, 100.0f);
	cameraTransform = cameraObject->transform;

	//Matrix projection = Matrix::createOrthographic(2.0f, 2.0f, -1.0f, 100.0f);
	//Matrix projection = Matrix::createPerspective(90.0f, 1024.0f / 768.0f, 0.01f, 100.0f);

	int t = 0;
	parent->transform->translate(Vector3(0, 0, -3.0f));
	parent->transform->setScale(Vector3(2.0f, 4.0f, 1.0f));

	child->transform->translate(Vector3(2.0f, 0, -3.0f));
	child->transform->setScale(Vector3(0.7f, 0.7f, 0.7f));
	child->transform->rotate(Vector3(0, 0, 45.0f));

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		//if (++t < 100)
		//{
		//	parent->transform->rotate(Vector3(0.0002f, 0, 0));
		//	child->transform->rotate(Vector3(0.00001f, 0, 0.00003f));
		//}
		//else if (t == 100)
		//{
		//	parent->transform->addChild(child->transform);
		//}
		//else if (t < 300)
		//{
		//	parent->transform->rotate(Vector3(0, 0, 0.0003f));
		//	child->transform->rotate(Vector3(0, 0.0002f, 0));
		//}
		//else
		//{
		//	parent->transform->removeChild(child->transform);
		//	parent->transform->rotate(Vector3(0.0002f, 0, 0));
		//	child->transform->rotate(Vector3(0.00001f, 0, 0.00003f));
		//}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		Renderer::renderAll(camera);





		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
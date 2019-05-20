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

#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "lib/glew32.lib")
#pragma comment(lib, "lib/glfw3.lib")

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main(void)
{
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

	Matrix view;
	//Matrix projection = Matrix::createOrthographic(2.0f, 2.0f, -1.0f, 100.0f);
	Matrix projection = Matrix::createPerspective(90.0f, 1024.0f / 768.0f, 0.01f, 100.0f);

	int modelHandle = shader->getUniformHandle("model");
	int viewHandle = shader->getUniformHandle("view");
	int projectionHandle = shader->getUniformHandle("projection");

	shader->setUniform(viewHandle, view);
	shader->setUniform(projectionHandle, projection);

	int t = 0;
	parent->transform->translate(Vector3(0, 0, -3.0f));
	parent->transform->setScale(Vector3(2.0f, 4.0f, 1.0f));

	child->transform->translate(Vector3(2.0f, 0, -3.0f));
	child->transform->setScale(Vector3(0.7f, 0.7f, 0.7f));
	child->transform->rotate(Vector3(0, 0, 45.0f));

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		if (++t < 100)
		{
			parent->transform->rotate(Vector3(0.0002f, 0, 0));
			child->transform->rotate(Vector3(0.00001f, 0, 0.00003f));
		}
		else if (t == 100)
		{
			parent->transform->addChild(child->transform);
			cout << "Attach" << endl;
		}
		else if (t < 200)
		{
			parent->transform->rotate(Vector3(0, 0, 0.0003f));
			child->transform->rotate(Vector3(0, 0.0002f, 0));
		}
		else
		{
			parent->transform->removeChild(child->transform);
			parent->transform->rotate(Vector3(0.0002f, 0, 0));
			child->transform->rotate(Vector3(0.00001f, 0, 0.00003f));
		}


		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		meshRenderer0->render();
		meshRenderer1->render();





		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
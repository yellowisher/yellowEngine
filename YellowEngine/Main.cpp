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

	GameObject* gameObject = new GameObject("GameObject");
	gameObject->addComponent<MeshRenderer>();
	MeshRenderer* meshRenderer = gameObject->getComponent<MeshRenderer>();
	meshRenderer->set(mesh, shader);
	meshRenderer->setTexture(texture);

	Matrix view;
	//Matrix projection = Matrix::createOrthographic(2.0f, 2.0f, -1.0f, 100.0f);
	Matrix projection = Matrix::createPerspective(90.0f, 1024.0f / 768.0f, 0.01f, 100.0f);

	int modelHandle = shader->getUniformHandle("model");
	int viewHandle = shader->getUniformHandle("view");
	int projectionHandle = shader->getUniformHandle("projection");

	shader->setUniform(viewHandle, view);
	shader->setUniform(projectionHandle, projection);

	int t = 0;
	gameObject->transform->translate(Vector3(0, 0, -2.0f));

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);



		t++;
		gameObject->transform->rotate(Vector3(0, 0.02f, 0.03f));

		Matrix m = gameObject->transform->getMatrix();
		shader->setUniform(modelHandle, gameObject->transform->getMatrix());



		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		meshRenderer->render();






		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
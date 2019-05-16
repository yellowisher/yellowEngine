using namespace std;

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include "Matrix.hpp"
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
	glDisable(GL_CULL_FACE);

	ShaderProgram* shader = ShaderProgram::create("../YellowEngine/texture.vs", "../YellowEngine/texture.ps");
	Mesh* mesh = Mesh::create("../YellowEngine/c.obj");
	Texture* texture = Texture::create("../YellowEngine/wall.jpg");
	MeshRenderer renderer(mesh, shader);
	renderer.setTexture(texture);

	//Matrix model;
	mat4 model = mat4(1.0f);

	//Matrix projection = Matrix::identity;
	//Matrix projection = Matrix::createPerspective(45.0f, (float)1024 / (float)768, 0.1f, 100.0f);
	//Matrix projection = Matrix::createOrthographic((float)1024, (float)768, 0.1f, 100.0f);

	float w = 1024;
	float h = 768;

	mat4 view = mat4(1.0f);
	mat4 projection = ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 100.0f);

	//mat4 projection = perspective(radians(45.0f), (float)1024 / (float)768, 0.1f, 100.0f);

	shader->use();

	int modelHandle = shader->getUniformHandle("model");
	int viewHandle = shader->getUniformHandle("view");
	int projectionHandle = shader->getUniformHandle("projection");

	glUniformMatrix4fv(modelHandle, 1, GL_FALSE, value_ptr(model));
	glUniformMatrix4fv(viewHandle, 1, GL_FALSE, value_ptr(view));
	glUniformMatrix4fv(projectionHandle, 1, GL_FALSE, value_ptr(projection));

	int t = 0;

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		float a = 0.015f*t;
		float angle = 10.0f + 0.005f*t++;

		//model = Quaternion(Vector3(0, a, angle)).toMatrix();
		//model = rotate(model, radians(angle), vec3(1.0f, 0.3f, 0.5f));
		glUniformMatrix4fv(modelHandle, 1, GL_FALSE, value_ptr(model));


		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer.render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
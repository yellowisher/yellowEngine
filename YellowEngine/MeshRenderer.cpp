#include <GL/glew.h>

#include "MeshRenderer.hpp"
#include "GameObject.hpp"

#include <iostream>

MeshRenderer::MeshRenderer(GameObject* gameObject) :Renderer(gameObject)
{
	_texture = nullptr;
}


MeshRenderer::~MeshRenderer()
{
}


void MeshRenderer::set(Mesh* mesh, ShaderProgram* shader)
{
	_mesh = mesh;
	_shader = shader;
	_binding = VertexLayoutBinding::create(mesh, shader);

	if (_binding == nullptr)
	{
		std::cout << "Mesh-Shader binding failed" << endl;
	}

	_modelUniformHandle = _shader->getUniformHandle("model");
}


void MeshRenderer::render()
{
	if (_texture)_texture->use();

	_shader->use();
	_shader->setUniform(_modelUniformHandle, getGameObject()->transform->getMatrix());
	glBindVertexArray(_binding->getVertexArrayHandle());
	glDrawElements(GL_TRIANGLES, _mesh->getVertexCount(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(NULL);
}


void MeshRenderer::setTexture(Texture* texture)
{
	_texture = texture;
}
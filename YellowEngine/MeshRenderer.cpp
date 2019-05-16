#include <GL/glew.h>

#include "MeshRenderer.hpp"

#include <iostream>

MeshRenderer::MeshRenderer(Mesh* mesh, ShaderProgram* shader) :_mesh(mesh), _shader(shader)
{
	_texture = nullptr;
	_binding = VertexLayoutBinding::create(mesh, shader);

	if (_binding == nullptr)
	{
		std::cout << "Mesh-Shader binding failed" << endl;
	}
}


MeshRenderer::~MeshRenderer()
{

}


void MeshRenderer::render()
{
	if (_texture)_texture->use();

	_shader->use();
	glBindVertexArray(_binding->getVertexArrayHandle());
	glDrawElements(GL_TRIANGLES, _mesh->getElementCount(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(NULL);
}


void MeshRenderer::setTexture(Texture* texture)
{
	_texture = texture;
}
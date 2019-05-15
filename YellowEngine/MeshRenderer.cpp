#include <GL/glew.h>

#include "MeshRenderer.hpp"

MeshRenderer::MeshRenderer(Mesh* mesh)
{
	_mesh = mesh;
	_texture = nullptr;
}


MeshRenderer::~MeshRenderer()
{

}


void MeshRenderer::render()
{
	if (_texture)_texture->use();
	glBindVertexArray(_mesh->getVertexArrayHandle());
	glDrawElements(GL_TRIANGLES, _mesh->getElementCount(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(NULL);
}


void MeshRenderer::setTexture(Texture* texture)
{
	_texture = texture;
}
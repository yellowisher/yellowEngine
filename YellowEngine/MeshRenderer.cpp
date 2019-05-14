#include <GL/glew.h>

#include "MeshRenderer.hpp"

MeshRenderer::MeshRenderer(Mesh* mesh)
{
	this->mesh = mesh;
	texture = nullptr;
}


MeshRenderer::~MeshRenderer()
{

}


void MeshRenderer::render()
{
	if (texture)texture->use();
	glBindVertexArray(mesh->getVertexArrayHandle());
	glDrawElements(GL_TRIANGLES, mesh->getElementCount(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(NULL);
}


void MeshRenderer::setTexture(Texture* texture)
{
	this->texture = texture;
}
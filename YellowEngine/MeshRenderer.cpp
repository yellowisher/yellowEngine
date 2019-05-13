#include <GL/glew.h>

#include "MeshRenderer.hpp"

MeshRenderer::MeshRenderer(Mesh* mesh)
{
	this->mesh = mesh;
}


MeshRenderer::~MeshRenderer()
{

}


void MeshRenderer::render()
{
	glBindVertexArray(mesh->getVertexArrayHandle());
	glDrawElements(GL_TRIANGLES, mesh->getElementCount(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(NULL);
}
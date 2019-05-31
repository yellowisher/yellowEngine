#include <GL/glew.h>

#include "MeshRenderer.hpp"
#include "GameObject.hpp"

#include <iostream>

MeshRenderer::MeshRenderer(GameObject* gameObject) :ObjectRenderer(gameObject)
{
}


MeshRenderer::~MeshRenderer()
{
}


MeshRenderer* MeshRenderer::set(Mesh* mesh, ShaderProgram* shader)
{
	_mesh = mesh;
	_shader = shader;
	_binding = VertexLayoutBinding::create(mesh, shader);

	if (_binding == nullptr)
	{
		std::cout << "Mesh-Shader binding failed" << endl;
		return nullptr;
	}
	return this;
}


void MeshRenderer::_render()
{
	// update automatic uniforms (Model, ProjectionView, etc...)
	_shader->use();
	_shader->updateUniforms(gameObject);

	// active all textures
	for (int i = 0; i < _textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		_textures[i]->use();
	}

	glBindVertexArray(_binding->getVertexArrayHandle());
	glDrawElements(GL_TRIANGLES, _mesh->getVertexCount(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(NULL);
}


void MeshRenderer::addTexture(Texture* texture, const char* usage)
{
	_shader->setUniform(_shader->getUniform(usage), (int)_textures.size());
	_textures.push_back(texture);
}
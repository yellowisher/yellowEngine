#include <glad/glad.h>

#include "yellowEngine/System/GameObject.hpp"
#include "yellowEngine/Component/MeshRenderer.hpp"


namespace yellowEngine
{
	MeshRenderer::MeshRenderer(GameObject* gameObject) :ObjectRenderer(gameObject)
	{
	}


	MeshRenderer::~MeshRenderer()
	{
	}


	MeshRenderer* MeshRenderer::set(Mesh* mesh, Shader* shader)
	{
		_mesh = mesh;
		_shader = shader;
		return this;
	}


	void MeshRenderer::_render()
	{
		// update automatic uniforms (Model, ProjectionView, etc...)
		_shader->use();
		_shader->updateUniforms(gameObject);

		// active all textures
		for (size_t i = 0; i < _textures.size(); i++)
		{
			glActiveTexture((GLenum)(GL_TEXTURE0 + i));
			_textures[i]->use();
		}

		glBindVertexArray(_mesh->getVertexArrayHandle());
		glDrawElements(GL_TRIANGLES, _mesh->getVertexCount(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(NULL);
	}


	void MeshRenderer::addTexture(Texture* texture, const char* usage)
	{
		_shader->setUniform(_shader->getUniform(usage), (int)_textures.size());
		_textures.push_back(texture);
	}
}
#include "yellowEngine/Component/SkinnedMeshRenderer.hpp"

namespace yellowEngine
{
	SkinnedMeshRenderer::SkinnedMeshRenderer(GameObject* gameObject) :MeshRenderer(gameObject)
	{
	}


	SkinnedMeshRenderer::~SkinnedMeshRenderer()
	{
	}


	SkinnedMeshRenderer* SkinnedMeshRenderer::set(Mesh* mesh, Shader* shader)
	{
		_mesh = mesh;
		_shader = shader;

		return this;
	}


	void SkinnedMeshRenderer::_render()
	{
		_shader->use();
		_shader->updateUniforms(gameObject);

		for (size_t i = 0; i < _textures.size(); i++)
		{
			glActiveTexture((GLenum)(GL_TEXTURE0 + i));
			_textures[i]->use();
		}

		glDrawElements(GL_TRIANGLES, _mesh->getVertexCount(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(NULL);
	}
}
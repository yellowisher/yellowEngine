#include <glad/glad.h>

#include "yellowEngine/Component/GameObject.hpp"
#include "yellowEngine/Component/MeshRenderer.hpp"


namespace yellowEngine
{
	MeshRenderer::MeshRenderer(GameObject* gameObject) :ObjectRenderer(gameObject)
	{
		_mesh = nullptr;
	}


	MeshRenderer::~MeshRenderer()
	{
	}


	MeshRenderer* MeshRenderer::set(Mesh* mesh, const Material& material)
	{
		_mesh = mesh;
		_material = material;
		_material.init(gameObject, mesh);
		return this;
	}


	void MeshRenderer::_render(Shader* shader)
	{
		if (_mesh == nullptr)return;

		// if shader is not given; render with its own material
		if (shader == nullptr) {
			_material.bind();
			glDrawElements(GL_TRIANGLES, _mesh->getVertexCount(), GL_UNSIGNED_INT, 0);
			_material.unbind();
		}
		else {
			shader->bind();
			shader->updateUniforms(gameObject);
			auto binding = VertexLayoutBinding::create(_mesh, shader);
			binding->bind();
			glDrawElements(GL_TRIANGLES, _mesh->getVertexCount(), GL_UNSIGNED_INT, 0);
			binding->unbind();
			shader->unbind();
		}
	}
}
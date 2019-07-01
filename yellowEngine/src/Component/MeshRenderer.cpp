#include <glad/glad.h>

#include "yellowEngine/Component/GameObject.hpp"
#include "yellowEngine/Component/MeshRenderer.hpp"


namespace yellowEngine
{
	COMPONENT_IMPL(MeshRenderer)


	MeshRenderer::MeshRenderer(GameObject* gameObject) :ObjectRenderer(gameObject)
	{
		_mesh = nullptr;
	}


	MeshRenderer::~MeshRenderer()
	{
		if (_material->_technique != nullptr)
		{
			_material->_technique->removeRenderer(this);
		}
	}


	void MeshRenderer::onValueChanged()
	{
		if (_prevTechnique != nullptr)
		{
			_prevTechnique->removeRenderer(this);
		}
		set(_mesh, _material);
	}


	MeshRenderer* MeshRenderer::set(Mesh* mesh, Material* material)
	{
		_mesh = mesh;
		_material = material;
		if (_material->_technique != nullptr)
		{
			_material->_technique->addRenderer(this);
		}
		_prevTechnique = _material->_technique;
		return this;
	}


	void MeshRenderer::_render(const char* vsPath, const char* fsPath)
	{
		if (_mesh == nullptr) return;

		_material->bind(this, vsPath, fsPath);
		glDrawElements(GL_TRIANGLES, _mesh->getVertexCount(), GL_UNSIGNED_INT, 0);
		//_material.unbind();
	}
}
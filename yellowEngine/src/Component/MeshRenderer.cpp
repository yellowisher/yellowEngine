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
		if (_material._technique != nullptr)
		{
			_material._technique->removeRenderer(this);
		}
	}


	MeshRenderer* MeshRenderer::set(Mesh* mesh, const Material& material)
	{
		_mesh = mesh;
		_material = material;
		_material.attachTo(gameObject, mesh);
		if (_material._technique != nullptr)
		{
			_material._technique->addRenderer(this);
		}
		return this;
	}


	void MeshRenderer::_render(const char* vsPath, const char* fsPath)
	{
		if (_mesh == nullptr) return;

		_material.bind(vsPath, fsPath);
		glDrawElements(GL_TRIANGLES, _mesh->getVertexCount(), GL_UNSIGNED_INT, 0);
		//_material.unbind();
	}
}
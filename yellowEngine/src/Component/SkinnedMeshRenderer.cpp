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

		return this;
	}


	void SkinnedMeshRenderer::_render()
	{
	}
}
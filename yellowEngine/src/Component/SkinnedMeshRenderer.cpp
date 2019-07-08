#include "yellowEngine/Component/SkinnedMeshRenderer.hpp"

namespace yellowEngine
{
	COMPONENT_IMPL(SkinnedMeshRenderer)


	SkinnedMeshRenderer::SkinnedMeshRenderer(GameObject* gameObject) :MeshRenderer(gameObject)
	{
	}


	SkinnedMeshRenderer::~SkinnedMeshRenderer()
	{
	}


	SkinnedMeshRenderer* SkinnedMeshRenderer::set(Mesh* mesh, Material* material, std::vector<std::pair<Transform*, Matrix>> joints, Transform* modelRoot)
	{
		MeshRenderer::set(mesh, material);

		_modelRoot = modelRoot;
		_joints = joints;
		return this;
	}


	// TODO: premultiply global matrix so no more get local matrix?
	void SkinnedMeshRenderer::_render(const char* vsPath, const char* fsPath)
	{
		if (_mesh == nullptr) return;

		Shader* shader = _material->bind(this, vsPath, fsPath);

		if (vsPath == nullptr)
		{
			// update joint matrices
			for (int i = 0; i < _joints.size(); i++)
			{
				Matrix matrix;
				Transform* cursor = _joints[i].first;
				while (cursor != _modelRoot)
				{
					matrix = cursor->getLocalMatrix() * matrix;
					cursor = cursor->getParent();
				}
				shader->setUniform("u_Joints[0]", matrix * _joints[i].second, i);
			}
		}
		glDrawElements(GL_TRIANGLES, _mesh->getVertexCount(), GL_UNSIGNED_INT, 0);
	}
}
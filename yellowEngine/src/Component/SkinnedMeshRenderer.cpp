#include "yellowEngine/Component/SkinnedMeshRenderer.hpp"

namespace yellowEngine
{
	SkinnedMeshRenderer::SkinnedMeshRenderer(GameObject* gameObject) :MeshRenderer(gameObject)
	{
	}


	SkinnedMeshRenderer::~SkinnedMeshRenderer()
	{
	}


	SkinnedMeshRenderer* SkinnedMeshRenderer::set(Mesh* mesh, const Material& material, std::vector<std::pair<Transform*, Matrix>> joints, Transform* modelRoot)
	{
		_mesh = mesh;
		_material = material;
		_material.init(gameObject, mesh);
			
		_modelRoot = modelRoot;
		_jointUniform = _material.getShader()->getUniform("u_Joints[0]");
		_joints = joints;
		return this;
	}


	// TODO: premultiply global matrix so no more get local matrix?
	void SkinnedMeshRenderer::_render(Shader* shader)
	{
		if (_mesh == nullptr)return;

		// if shader is not given; render with its own material
		if (shader == nullptr)
		{
			_material.bind();

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
				_material.getShader()->setUniform(_jointUniform, matrix * _joints[i].second, i);
			}
			glDrawElements(GL_TRIANGLES, _mesh->getVertexCount(), GL_UNSIGNED_INT, 0);
			_material.unbind();
		}
		else
		{
			_material.bind(shader);
			glDrawElements(GL_TRIANGLES, _mesh->getVertexCount(), GL_UNSIGNED_INT, 0);
			_material.unbind();
		}
	}
}
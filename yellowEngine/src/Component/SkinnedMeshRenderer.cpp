#include "yellowEngine/Component/SkinnedMeshRenderer.hpp"

namespace yellowEngine
{
	SkinnedMeshRenderer::SkinnedMeshRenderer(GameObject* gameObject) :MeshRenderer(gameObject)
	{
	}


	SkinnedMeshRenderer::~SkinnedMeshRenderer()
	{
	}


	SkinnedMeshRenderer* SkinnedMeshRenderer::set(Mesh* mesh, const Material& material, std::vector<Transform*> jointTransforms)
	{
		_mesh = mesh;
		_material = material;
		_material.init(gameObject, mesh);
			
		_jointUniform = _material.getShader()->getUniform("u_Joints[0]");
		_jointTransforms = jointTransforms;
		return this;
	}


	void SkinnedMeshRenderer::_render(Shader* shader)
	{
		// if shader is not given; render with its own material
		if (shader == nullptr)
		{
			_material.bind();

			// update joint matrices
			for (int i = 0; i < _jointTransforms.size(); i++)
			{
				_material.getShader()->setUniform(_jointUniform, _jointTransforms[i]->getMatrix(), i);
			}
			glDrawElements(GL_TRIANGLES, _mesh->getVertexCount(), GL_UNSIGNED_INT, 0);
			_material.unbind();
		}
		else
		{
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
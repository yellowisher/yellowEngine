#include <queue>
#include <cassert>

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


	SkinnedMeshRenderer* SkinnedMeshRenderer::set(
		Mesh* mesh, Material* material, 
		std::vector<std::pair<Transform*, Matrix>> joints, 
		std::map<std::string, int> jointIndices,
		Transform* modelRoot)
	{
		MeshRenderer::set(mesh, material);

		_modelRoot = modelRoot;
		_joints = joints;
		_jointIndices = jointIndices;
		return this;
	}


	// TODO: premultiply global matrix so no more get local matrix?
	void SkinnedMeshRenderer::_render(const char* vsPath, const char* fsPath)
	{
		if (_mesh == nullptr) return;

		Shader* shader = _material->bind(this, vsPath, fsPath);

		shader->setUniform("u_Model", _modelRoot->getMatrix());
		shader->setUniform("u_Skinning", true);
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
		glDrawElements(GL_TRIANGLES, _mesh->getVertexCount(), GL_UNSIGNED_INT, 0);
		shader->setUniform("u_Skinning", false);
	}


	void SkinnedMeshRenderer::clone(Component* target)
	{
		Component::clone(target);

		SkinnedMeshRenderer* targetSMR = dynamic_cast<SkinnedMeshRenderer*>(target);
		
		Transform* root = targetSMR->transform;
		while (root->gameObject->getName() != _modelRoot->gameObject->getName())
		{
			root = root->getParent();
		}

		targetSMR->_modelRoot = root;

		// FIXME:
		// at this point, I don't come up with any other idea of 
		// re-link joints without changing joint storeage structure
		// so with belief of all joints name are different (which is naive)
		// link them with their name

		targetSMR->_jointIndices = _jointIndices;

		std::queue<Transform*> visitQueue;
		visitQueue.push(root);
		int leftCount = _jointIndices.size();

		targetSMR->_joints.resize(_joints.size());
		while (!visitQueue.empty())
		{
			Transform* visit = visitQueue.front();
			visitQueue.pop();

			std::string jName = visit->gameObject->getName();
			auto jit = _jointIndices.find(jName);
			if (jit != _jointIndices.end())
			{
				targetSMR->_joints[jit->second] = { visit, _joints[jit->second].second };

				if (--leftCount == 0)
				{
					break;
				}
			}

			for (auto child : visit->getChildren())
			{
				visitQueue.push(child);
			}
		}

		assert(leftCount == 0);
	}
}
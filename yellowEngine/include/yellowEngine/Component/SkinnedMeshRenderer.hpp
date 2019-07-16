#ifndef __H_SKINNEDMESHRENDERER__
#define __H_SKINNEDMESHRENDERER__

#include <vector>

#include "yellowEngine/Component/MeshRenderer.hpp"

namespace yellowEngine
{
	class SkinnedMeshRenderer : public MeshRenderer
	{
		BEGIN_COMPONENT(SkinnedMeshRenderer)
			PROPERTY(SkinnedMeshRenderer, Mesh, _meshPath, "Mesh")
			PROPERTY(SkinnedMeshRenderer, Material, _materialPath, "Material")
		END_COMPONENT

	public:
		SkinnedMeshRenderer(GameObject* gameObject);
		~SkinnedMeshRenderer();

		virtual void onValueChanged() override;

		SkinnedMeshRenderer* set(
			Mesh* mesh, Material* material,
			std::vector<std::pair<Transform*, Matrix>> joints,
			std::map<std::string, int> jointIndices,
			Transform* modelRoot);
		void _render(const char* vsPath = nullptr, const char* fsPath = nullptr) override;

	private:
		// because SkinnedMeshRenderer holds corresponding joint transforms and its offset matrix,
		// cloning the component is not just memcpying members
		// so SkinnedMeshRenderer needs its own version of clone
		virtual void clone(Component* target) override;

		Transform* _modelRoot;
		//const Uniform* _jointUniform;
		std::vector<std::pair<Transform*, Matrix>> _joints;
		std::map<std::string, int> _jointIndices;
	};
}

#endif
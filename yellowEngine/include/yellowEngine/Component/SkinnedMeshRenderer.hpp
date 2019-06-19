#ifndef __H_SKINNEDMESHRENDERER__
#define __H_SKINNEDMESHRENDERER__

#include <vector>

#include "yellowEngine/Component/MeshRenderer.hpp"

namespace yellowEngine
{
	class SkinnedMeshRenderer : public MeshRenderer
	{
	public:
		SkinnedMeshRenderer(GameObject* gameObject);
		~SkinnedMeshRenderer();

		SkinnedMeshRenderer* set(
			Mesh* mesh, const Material& material,
			std::vector<std::pair<Transform*, Matrix>> joints, Transform* modelRoot);
		void _render(const char* vsPath = nullptr, const char* fsPath = nullptr) override;

	private:
		Transform* _modelRoot;
		const Uniform* _jointUniform;
		std::vector<std::pair<Transform*, Matrix>> _joints;
	};
}

#endif
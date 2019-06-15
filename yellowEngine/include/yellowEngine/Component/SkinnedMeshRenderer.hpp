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

		SkinnedMeshRenderer* set(Mesh* mesh, const Material& material, std::vector<Transform*> jointTransforms);
		void _render(Shader* shader) override;

	private:
		const Uniform* _jointUniform;
		std::vector<Transform*> _jointTransforms;
	};
}

#endif
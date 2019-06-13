#ifndef __H_SKINNEDMESHRENDERER__
#define __H_SKINNEDMESHRENDERER__

#include "yellowEngine/Component/MeshRenderer.hpp"

namespace yellowEngine
{
	class SkinnedMeshRenderer : public MeshRenderer
	{
	public:
		SkinnedMeshRenderer(GameObject* gameObject);
		~SkinnedMeshRenderer();

		SkinnedMeshRenderer* set(Mesh* mesh, Shader* shader);
		void _render() override;

	private:

	};
}

#endif
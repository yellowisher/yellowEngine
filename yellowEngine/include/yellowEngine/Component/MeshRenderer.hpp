#ifndef __H_MESHRENDERER__
#define __H_MESHRENDERER__

#include <vector>

#include "yellowEngine/Component/Component.hpp"
#include "yellowEngine/Component/ObjectRenderer.hpp"
#include "yellowEngine/Rendering/Mesh.hpp"
#include "yellowEngine/Rendering/Shader.hpp"
#include "yellowEngine/Rendering/Material.hpp"


namespace yellowEngine
{
	class MeshRenderer : public ObjectRenderer
	{
	public:
		MeshRenderer(GameObject* gameObject);
		virtual ~MeshRenderer();

		MeshRenderer* set(Mesh* mesh, const Material& material);
		virtual void _render() override;

	protected:
		Mesh* _mesh;
		Material _material;
	};
}

#endif
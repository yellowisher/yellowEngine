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
		BEGIN_COMPONENT(MeshRenderer)
			PROPERTY(MeshRenderer, Mesh, _mesh, "Mesh")
			PROPERTY(MeshRenderer, Material, _material, "Material")
		END_COMPONENT

	public:
		MeshRenderer(GameObject* gameObject);
		virtual ~MeshRenderer();
		virtual void onValueChanged() override;

		MeshRenderer* set(Mesh* mesh, const Material& material);
		virtual void _render(const char* vsPath = nullptr, const char* fsPath = nullptr) override;

	protected:
		Mesh* _mesh;
		Material _material;
		Technique* _prevTechnique;
	};
}

#endif
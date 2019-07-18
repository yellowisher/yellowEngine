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
		BEGIN_COMPONENT_NO_CTOR(MeshRenderer)
			PROPERTY(MeshRenderer, Mesh, _meshPath, "Mesh")
			PROPERTY(MeshRenderer, Material, _materialPath, "Material")
		END_COMPONENT

	public:
		MeshRenderer(GameObject* gameObject);
		virtual ~MeshRenderer();
		virtual void onValueChanged() override;

		Mesh* getMesh() { return _mesh; }
		Material* getMaterial() { return _material; }
		MeshRenderer* set(Mesh* mesh, Material* material = nullptr);
		virtual void _render(const char* vsPath = nullptr, const char* fsPath = nullptr) override;

	protected:
		std::string _meshPath;
		std::string _materialPath;

		Mesh* _mesh;
		Material* _material;
		Technique* _prevTechnique;
	};
}

#endif
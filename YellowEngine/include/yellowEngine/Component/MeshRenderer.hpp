#ifndef __H_MESHRENDERER__
#define __H_MESHRENDERER__

#include <vector>

#include "yellowEngine/Component/Component.hpp"
#include "yellowEngine/Component/ObjectRenderer.hpp"
#include "yellowEngine/Rendering/Mesh.hpp"
#include "yellowEngine/Rendering/ShaderProgram.hpp"
#include "yellowEngine/Rendering/VertexLayoutBinding.hpp"
#include "yellowEngine/Rendering/Texture.hpp"

namespace yellowEngine
{
	class MeshRenderer : public ObjectRenderer
	{
	public:
		MeshRenderer(GameObject* gameObject);
		~MeshRenderer();

		MeshRenderer* set(Mesh* mesh, ShaderProgram* shader);
		void _render() override;
		void addTexture(Texture* texture, const char* usage);

	private:
		Mesh* _mesh;
		ShaderProgram* _shader;
		VertexLayoutBinding* _binding;
		std::vector<Texture*> _textures;
	};
}

#endif
#ifndef __H_MATERIAL__
#define __H_MATERIAL__

#include <vector>

#include "yellowEngine/Component/GameObject.hpp"
#include "yellowEngine/Rendering/Mesh.hpp"
#include "yellowEngine/Rendering/Shader.hpp"
#include "yellowEngine/Rendering/Texture.hpp"
#include "yellowEngine/Rendering/VertexLayoutBinding.hpp"

// TODO: create property

namespace yellowEngine
{
	class Material
	{
	public:
		Material() {};
		Material(Shader* shader);
		~Material();
		Material* init(GameObject* gameObject, Mesh* mesh);
		void addTexture(Texture* texture, const char* usage);
		void bind();
		void unbind();
		Shader* getShader();

	private:
		GameObject* _gameObject;
		Shader* _shader;
		std::vector<std::pair<std::string, Texture*>> _textures;
		VertexLayoutBinding* _binding;
	};
}

#endif
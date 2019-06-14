#ifndef __H_MATERIAL__
#define __H_MATERIAL__

#include <vector>

#include "yellowEngine/Rendering/Shader.hpp"
#include "yellowEngine/Rendering/Texture.hpp"

namespace yellowEngine
{
	class Material
	{
	public:
		Material();
		~Material();
		void addTexture(Texture* texture, const char* usage);

	private:
		Shader* _shader;
		std::vector<Texture*> _textures;
	};
}

#endif
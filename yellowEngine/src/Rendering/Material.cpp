#include "yellowEngine/Rendering/Material.hpp"

namespace yellowEngine
{
	Material::Material()
	{
	}


	Material::~Material()
	{
	}


	void Material::addTexture(Texture * texture, const char * usage)
	{
		_shader->setUniform(_shader->getUniform(usage), (int)_textures.size());
		_textures.push_back(texture);
	}
}
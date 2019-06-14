#include "yellowEngine/Rendering/Material.hpp"

namespace yellowEngine
{
	Material::Material(Shader* shader) :_shader(shader)
	{
	}


	Material::~Material()
	{
	}


	Material* Material::init(GameObject* gameObject, Mesh* mesh)
	{
		_gameObject = gameObject;
		_binding = VertexLayoutBinding::create(mesh, _shader);

		for (size_t i = 0; i < _textures.size(); i++)
		{
			_shader->setUniform(_shader->getUniform(_textures[i].first), (int)i);
		}
		return this;
	}


	void Material::addTexture(Texture* texture, const char* usage)
	{
		_textures.push_back({ usage,texture });
	}


	void Material::bind()
	{
		_shader->bind();
		_shader->updateUniforms(_gameObject);

		for (size_t i = 0; i < _textures.size(); i++)
		{
			glActiveTexture((GLenum)(GL_TEXTURE0 + i));
			_textures[i].second->bind();
		}

		_binding->bind();
	}


	void Material::unbind()
	{
		_shader->unbind();
		_binding->unbind();
	}


	Shader* Material::getShader()
	{
		return _shader;
	}
}
#include "yellowEngine/Rendering/Material.hpp"

namespace yellowEngine
{
	Material::Material(Shader* shader) :_shader(shader)
	{
		for (auto uniformPair : _shader->getUniforms())
		{
			if (uniformPair.second.type == GL_SAMPLER_1D ||
				uniformPair.second.type == GL_SAMPLER_2D ||
				uniformPair.second.type == GL_SAMPLER_3D)continue;
			size_t commaPos = uniformPair.first.find('.');
			if (uniformPair.first.substr(0, commaPos) == "u_Material")
			{
				Property prop;
				std::string name = uniformPair.first.substr(commaPos + 1);
				prop.uniform = _shader->getUniform(uniformPair.first);
				_properties.insert({ name, prop });
			}
		}
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

		// update auto binding uniforms (like model matrix)
		_shader->updateUniforms(_gameObject);

		for (auto prop : _properties)
		{
			switch (prop.second.uniform->type)
			{
				case GL_INT:
					_shader->setUniform(prop.second.uniform, prop.second.intValue);
					break;
				case GL_FLOAT:
					_shader->setUniform(prop.second.uniform, prop.second.floatValue);
					break;
				case GL_FLOAT_VEC2:
					_shader->setUniform(prop.second.uniform, prop.second.vector2Value);
					break;
				case GL_FLOAT_VEC3:
					_shader->setUniform(prop.second.uniform, prop.second.vector3Value);
					break;
				case GL_FLOAT_VEC4:
					_shader->setUniform(prop.second.uniform, prop.second.vector4Value);
					break;
				case GL_FLOAT_MAT4:
					_shader->setUniform(prop.second.uniform, prop.second.matrixValue);
					break;
				default:
					continue;
			}
		}

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


	void Material::setProperty(const char* name, int value)
	{
		_properties[name].intValue = value;
	}


	void Material::setProperty(const char* name, float value)
	{
		_properties[name].floatValue = value;
	}


	void Material::setProperty(const char* name, Vector2 value)
	{
		_properties[name].vector2Value = value;
	}


	void Material::setProperty(const char* name, Vector3 value)
	{
		_properties[name].vector3Value = value;
	}


	void Material::setProperty(const char* name, Vector4 value)
	{
		_properties[name].vector4Value = value;
	}


	void Material::setProperty(const char* name, Matrix value)
	{
		_properties[name].matrixValue = value;
	}
}
#include "yellowEngine/Rendering/Material.hpp"

namespace yellowEngine
{
	Material::Material()
	{
		_shader = nullptr;
	}

	Material::Material(Shader* shader)
	{
		_shader = shader;
	}


	Material::~Material()
	{
	}


	Material* Material::init(GameObject* gameObject, Mesh* mesh)
	{
		_gameObject = gameObject;
		_mesh = mesh;
		return this;
	}


	void Material::addTexture(Texture* texture, const char* usage)
	{
		_textures.insert({ usage, texture });
	}


	void Material::bind(Shader* shader)
	{
		if (shader == nullptr)
		{
			shader = _shader;
		}

		shader->bind();

		// update auto binding uniforms (like model matrix)
		shader->updateUniforms(_gameObject);

		/*for (auto uniformPair : shader->getUniforms())
		{
			switch (uniformPair.second.type)
			{
				case GL_INT:
					shader->setUniform(uniformPair.first, _properties[uniformPair.second.name].intValue);
					break;
				case GL_FLOAT:
					shader->setUniform(uniformPair.first, _properties[uniformPair.second.name].floatValue);
					break;
				case GL_FLOAT_VEC2:
					shader->setUniform(uniformPair.first, _properties[uniformPair.second.name].vector2Value);
					break;
				case GL_FLOAT_VEC3:
					shader->setUniform(uniformPair.first, _properties[uniformPair.second.name].vector3Value);
					break;
				case GL_FLOAT_VEC4:
					shader->setUniform(uniformPair.first, _properties[uniformPair.second.name].vector4Value);
					break;
				case GL_FLOAT_MAT4:
					shader->setUniform(uniformPair.first, _properties[uniformPair.second.name].matrixValue);
					break;
				default:
					continue;
			}
		}*/

		const std::vector<std::string>& names = shader->getTextureUnits();
		for (int i = 0; i < names.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			_textures[names[i]]->bind();
		}

		VertexLayoutBinding::create(_mesh, shader)->bind();
	}


	void Material::unbind()
	{
		_shader->unbind();
		VertexLayoutBinding::unbind();
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
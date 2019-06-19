#include "yellowEngine/Rendering/VertexLayoutBinding.hpp"
#include "yellowEngine/Rendering/Material.hpp"


namespace yellowEngine
{
	Material::Material()
	{
		_technique = nullptr;
	}


	Material::~Material()
	{
	}


	void Material::setTechnique(Technique* technique, const char* defaultVsPath, const char* defaultFsPath)
	{
		_technique = technique;
		_defaultVsPath = defaultVsPath;
		_defaultFsPath = defaultFsPath;
	}


	Material* Material::attachTo(GameObject* gameObject, Mesh* mesh)
	{
		_gameObject = gameObject;
		_mesh = mesh;
		return this;
	}


	void Material::addTexture(Texture* texture, const char* usage)
	{
		_textures.insert({ usage, texture });
	}


	void Material::bind(const char* vsPath, const char* fsPath)
	{
		if (vsPath == nullptr) vsPath = _defaultVsPath;
		if (fsPath == nullptr) fsPath = _defaultFsPath;

		Shader* shader = Shader::create(vsPath, fsPath);

		// update auto binding uniforms (like model matrix)
		shader->updateUniforms(_gameObject);

		for (auto uniformPair : shader->getUniforms())
		{
			auto it = _properties.find(uniformPair.second.name);
			if (it == _properties.end())continue;

			switch (uniformPair.second.type)
			{
				case GL_INT:
					shader->setUniform(uniformPair.first, it->second.intValue);
					break;
				case GL_FLOAT:
					shader->setUniform(uniformPair.first, it->second.floatValue);
					break;
				case GL_FLOAT_VEC2:
					shader->setUniform(uniformPair.first, it->second.vector2Value);
					break;
				case GL_FLOAT_VEC3:
					shader->setUniform(uniformPair.first, it->second.vector3Value);
					break;
				case GL_FLOAT_VEC4:
					shader->setUniform(uniformPair.first, it->second.vector4Value);
					break;
				case GL_FLOAT_MAT4:
					shader->setUniform(uniformPair.first, it->second.matrixValue);
					break;
				default:
					continue;
			}
		}

		const std::vector<std::string>& names = shader->getTextureUnits();
		for (int i = 0; i < names.size(); i++)
		{
			auto it = _textures.find(names[i]);
			if (it == _textures.end()) continue;

			glActiveTexture(GL_TEXTURE0 + i);
			it->second->bind();
		}

		VertexLayoutBinding::create(_mesh, shader)->bind();
	}


	void Material::unbind()
	{
		VertexLayoutBinding::unbind();
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
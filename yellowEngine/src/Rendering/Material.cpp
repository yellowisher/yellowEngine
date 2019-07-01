#include <iostream>
#include <fstream>
#include <json/json.h>

#include "yellowEngine/Component/MeshRenderer.hpp"
#include "yellowEngine/Rendering/VertexLayoutBinding.hpp"
#include "yellowEngine/Rendering/Material.hpp"


namespace yellowEngine
{
	enum Primitive
	{
		Primitive_Float,
		Primitive_Texture,
		Primitive_Color
	};

	static std::map<std::string, Primitive> primitiveToString = {
		{"float", Primitive_Float},
		{"texture", Primitive_Texture},
		{"color", Primitive_Color}
	};

	std::map<std::string, Material*> Material::_materialCache;

	Material* Material::create(const char* path)
	{
		auto it = _materialCache.find(path);
		if (it != _materialCache.end())
		{
			return it->second;
		}

		std::ifstream document(path, std::ifstream::binary);

		Json::Reader reader;
		Json::Value json;
		if (!reader.parse(document, json))
		{
			std::cout << "Material json parseing failed" << std::endl;
			return nullptr;
		}

		Material* material = new Material(path);
		for (auto propJson : json["properties"])
		{
			auto it = primitiveToString.find(propJson["type"].asString());
			if (it != primitiveToString.end())
			{
				std::string name = propJson["name"].asString();
				switch (it->second)
				{
					case Primitive_Float:
					{
						material->setProperty(name.c_str(), propJson["value"].asFloat());
						break;
					}
					case Primitive_Texture:
					{
						std::string path = propJson["value"].asString();
						material->setProperty(name.c_str(), Texture::create(path.c_str()));
						break;
					}
					case Primitive_Color:
					{
						Vector3 value;
						value.x = propJson["value"]["r"].asFloat();
						value.y = propJson["value"]["g"].asFloat();
						value.z = propJson["value"]["b"].asFloat();
						material->setProperty(name.c_str(), value);
						break;
					}
				}
			}
		}
		return material;
	}


	Material::Material(const char* path)
	{
		_technique = nullptr;
		_materialCache.insert({ path, this });

		// set default values
		setProperty("u_Material.color", Vector3(1.0f, 1.0f, 1.0f));
		setProperty("u_Material.diffuse", Texture::create("Texture/default_diffuse.png"));
		setProperty("u_Material.specular", Texture::create("Texture/default_specular.png"));
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


	void Material::bind(MeshRenderer* meshRenderer, const char* vsPath, const char* fsPath)
	{
		if (vsPath == nullptr) vsPath = _defaultVsPath;
		if (fsPath == nullptr) fsPath = _defaultFsPath;

		Shader* shader = Shader::create(vsPath, fsPath);

		// update auto binding uniforms (like model matrix)
		shader->updateUniforms(meshRenderer->gameObject);

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

		VertexLayoutBinding::create(meshRenderer->getMesh(), shader)->bind();
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


	void Material::setProperty(const char* name, Texture* texture)
	{
		_textures[name] = texture;
	}
}
#include <iostream>
#include <fstream>
#include <json/json.h>

#include "yellowEngine/Utility/Utils.hpp"
#include "yellowEngine/Component/MeshRenderer.hpp"
#include "yellowEngine/Rendering/VertexLayoutBinding.hpp"
#include "yellowEngine/Rendering/Material.hpp"


// TODO: rework material serialization method; not bunch of enum
namespace yellowEngine
{
	std::map<std::string, Material*> Material::_materialCache;
	std::map<std::string, Material::Primitive> Material::stringToPrimitive = {
		{"float", Material::Primitive_Float},
		{"texture", Material::Primitive_Texture},
		{"color", Material::Primitive_Color}
	};

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
			auto it = stringToPrimitive.find(propJson["type"].asString());
			if (it != stringToPrimitive.end())
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

	void Material::saveAsFile(Material* material, const char* path)
	{
		Json::Value json;

		// create default material
		if (material == nullptr)
		{
			material = new Material("default");
		}

		for (auto& nameTexturePair : material->_textures)
		{
			Json::Value propJson;
			propJson["name"] = nameTexturePair.first;
			propJson["type"] = "string";
			propJson["value"] = nameTexturePair.second->getName();
			json["properties"].append(propJson);
		}

		for (auto& namePropPair : material->_properties)
		{
			Json::Value propJson;
			propJson["name"] = namePropPair.first;
			propJson["type"] = namePropPair.second.type;
			auto primitive = stringToPrimitive[namePropPair.second.type];
			switch (primitive)
			{
				case Primitive_Float:
					propJson["value"] = namePropPair.second.floatValue;
					break;
				case Primitive_Color:
					propJson["value"]["r"] = namePropPair.second.colorValue.x;
					propJson["value"]["g"] = namePropPair.second.colorValue.y;
					propJson["value"]["b"] = namePropPair.second.colorValue.z;
					break;
			}
			json["properties"].append(propJson);
		}

		std::ofstream ofs(path, std::ifstream::binary);
		auto writer = Json::StyledStreamWriter();
		writer.write(ofs, json);
	}


	Material::Material(const char* path)
	{
		_path = path;
		setTechnique(Technique::getTechnique(TechniqueType_Deferred), "./res/Shader/default.vert", "./res/Shader/default.frag");
		_materialCache.insert({ path, this });

		// set default values
		setProperty("u_Material.diffuseColor", Vector3(1.0f, 1.0f, 1.0f));
		setProperty("u_Material.specularColor", Vector3(1.0f, 1.0f, 1.0f));
		setProperty("u_Material.diffuse", Texture::create("./res/Texture/default_diffuse.png"));
		setProperty("u_Material.specular", Texture::create("./res/Texture/default_specular.png"));
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


	Shader* Material::bind(MeshRenderer* meshRenderer, const char* vsPath, const char* fsPath)
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
				case GL_FLOAT_VEC3:
					shader->setUniform(uniformPair.first, it->second.colorValue);
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

		return shader;
	}


	void Material::unbind()
	{
		VertexLayoutBinding::unbind();
	}

	std::string Material::getPath()
	{
		return _path;
	}


	void Material::setProperty(const char* name, int value)
	{
		_properties[name].intValue = value;
		_properties[name].type = "int";
	}


	void Material::setProperty(const char* name, float value)
	{
		_properties[name].floatValue = value;
		_properties[name].type = "float";
	}


	void Material::setProperty(const char* name, Vector3 value)
	{
		_properties[name].colorValue = value;
		_properties[name].type = "color";
	}


	void Material::setProperty(const char* name, Texture* texture)
	{
		_textures[name] = texture;
	}
}
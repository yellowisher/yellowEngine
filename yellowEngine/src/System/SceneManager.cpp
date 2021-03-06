#include <iostream>
#include <fstream>
#include <map>
#include <json/json.h>

#include "yellowEngine/System/Game.hpp"
#include "yellowEngine/System/SceneManager.hpp"


namespace yellowEngine
{
	enum Primitive
	{
		Primitive_Bool,
		Primitive_Int,
		Primitive_Float,
		Primitive_String
	};

	struct DataType
	{
		std::string name;
		Primitive primitive;
	};

	static std::map<std::string, std::vector<DataType>> dataTypes = {
		{"bool",	 {{"value", Primitive_Bool}}},
		{"int",		 {{"value", Primitive_Int}}},
		{"float",	 {{"value", Primitive_Float}}},
		{"Vector2",  {{"x", Primitive_Float}, {"y",Primitive_Float}}},
		{"Vector3",  {{"x", Primitive_Float}, {"y",Primitive_Float}, {"z",Primitive_Float}}},
		{"Vector4",  {{"x", Primitive_Float}, {"y",Primitive_Float}, {"z",Primitive_Float}, {"w",Primitive_Float}}},
		{"Mesh",	 {{"path", Primitive_String}}},
		{"Material", {{"path", Primitive_String}}}
	};

	// reading functions
	static void addComponents(GameObject* gameObject, Json::Value& compsJson);
	static void addChildren(GameObject* parent, Json::Value& childrenJson);

	// writing functions
	static void fillComponents(GameObject* gameObject, Json::Value& goJson);
	static void fillChildren(Transform* parent, Json::Value& goJson);


	void SceneManager::clearScene()
	{
		delete(Transform::Root->gameObject);
		Transform::Root = nullptr;
		Transform::Root = (new GameObject("Root"))->transform;
	}


	void SceneManager::loadScene(const char* path)
	{
		std::ifstream document(path, std::ifstream::binary);

		Json::Reader reader;
		Json::Value root;
		if (!reader.parse(document, root))
		{
			std::cout << "Json parseing failed" << std::endl;
			return;
		}

		// clear scene first
		clearScene();

		for (auto goJson : root["gameObjects"])
		{
			GameObject* gameObject = new GameObject(goJson["name"].asString().c_str());

			addComponents(gameObject, goJson["components"]);
			addChildren(gameObject, goJson["children"]);
		}
	}


	void addComponents(GameObject* gameObject, Json::Value& compsJson)
	{
		for (auto compJson : compsJson)
		{
			std::string compName = compJson["script"].asString();
			Component* component;
			if (compName == "Transform")
			{
				component = gameObject->transform;
			}
			else
			{
				component = gameObject->addComponent(compName);
			}

			auto properties = Component::getProperties()[compName];
			for (auto propJson : compJson["properties"])
			{
				std::string propName = propJson["name"].asString();
				for (auto it = properties.begin(); it != properties.end(); ++it)
				{
					if (it->name == propName)
					{
						auto itt = dataTypes.find(propJson["type"].asString());
						if (itt != dataTypes.end())
						{
							auto valueTypes = itt->second;
							size_t cursor = (size_t)component + it->offset;

							for (auto valueType : valueTypes)
							{
								switch (valueType.primitive)
								{
									case Primitive_Bool:
									{
										bool value = propJson["value"][valueType.name].asBool();
										*(bool*)(cursor) = value;
										cursor += sizeof(bool);
										break;
									}
									case Primitive_Int:
									{
										int value = propJson["value"][valueType.name].asInt();
										*(int*)(cursor) = value;
										cursor += sizeof(int);
										break;
									}
									case Primitive_Float:
									{
										float value = propJson["value"][valueType.name].asFloat();
										*(float*)(cursor) = value;
										cursor += sizeof(float);
										break;
									}
									case Primitive_String:
									{
										std::string value = propJson["value"][valueType.name].asString();
										*(std::string*)(cursor) = value;
										cursor += sizeof(std::string);
										break;
									}
									default:
										break;
								}
							}
							break;
						}
					}
				}
			}

			component->onValueChanged();
		}
	}


	void addChildren(GameObject* parent, Json::Value& childrenJson)
	{
		for (auto childJson : childrenJson)
		{
			GameObject* child = new GameObject(childJson["name"].asString().c_str());

			parent->transform->addChild(child->transform);
			addComponents(child, childJson["components"]);
			addChildren(child, childJson["children"]);
		}
	}


	void SceneManager::saveScene(const char* path, Transform* ignore)
	{
		Json::Value gosJson;

		for (auto transform : Transform::Root->getChildren())
		{
			if (transform == ignore) continue;

			Json::Value goJson;
			goJson["name"] = transform->gameObject->getName();
			fillComponents(transform->gameObject, goJson);
			fillChildren(transform, goJson);
			gosJson.append(goJson);
		}
		Json::Value root;
		root["gameObjects"] = gosJson;

		std::ofstream ofs(path, std::ifstream::binary);
		auto writer = Json::StyledStreamWriter();
		writer.write(ofs, root);
	}


	void fillComponents(GameObject* gameObject, Json::Value& goJson)
	{
		Json::Value compsJson;
		for (auto component : gameObject->getComponents())
		{
			Json::Value compJson;
			compJson["script"] = component->getTypeName();
			
			Json::Value propsJson;
			auto properties = Component::getProperties()[component->getTypeName()];

			for (auto prop : properties)
			{
				auto it = dataTypes.find(prop.type);
				std::vector<DataType>* valueTypes = nullptr;

				Json::Value propJson;
				Json::Value valueJson;

				if (it == dataTypes.end())
				{
					const auto& enums = Component::getEnums()[component->getTypeName()];
					auto it = enums.find(prop.type);
					if (it != enums.end())
					{
						valueTypes = &dataTypes["int"];
						propJson["type"] = "int";
					}
				}
				else
				{
					valueTypes = &dataTypes[prop.type];
					propJson["type"] = prop.type;
				}

				if (valueTypes != nullptr)
				{
					propJson["name"] = prop.name;

					size_t cursor = (size_t)component + prop.offset;
					for (auto valueType : *valueTypes)
					{
						switch (valueType.primitive)
						{
							case Primitive_Bool:
							{
								valueJson[valueType.name] = Json::Value(*(bool*)(cursor));
								cursor += sizeof(bool);
								break;
							}
							case Primitive_Int:
							{
								valueJson[valueType.name] = Json::Value(*(int*)(cursor));
								cursor += sizeof(int);
								break;
							}
							case Primitive_Float:
							{
								valueJson[valueType.name] = Json::Value(*(float*)(cursor));
								cursor += sizeof(int);
								break;
							}
							case Primitive_String:
								valueJson[valueType.name] = Json::Value(*(std::string*)(cursor));
								cursor += sizeof(int);
								break;
							default:
								break;
						}
					}

					propJson["value"] = valueJson;
					propsJson.append(propJson);
				}
			}
			
			compJson["properties"] = propsJson;
			compsJson.append(compJson);
		}
		goJson["components"] = compsJson;
	}


	void fillChildren(Transform* parent, Json::Value& goJson)
	{
		Json::Value childrenJson;
		for (auto child : parent->getChildren())
		{
			Json::Value childJson;
			childJson["name"] = child->gameObject->getName();
			fillComponents(child->gameObject, childJson);
			fillChildren(child, childJson);
			childrenJson.append(childJson);
		}

		goJson["children"] = childrenJson;
	}
}